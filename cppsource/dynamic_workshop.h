#ifndef DWSP_MODEL
#define DWSP_MODEL

// #include<set>
// #include<array>
// #include<queue>

#include "funcs.h"
#include "workshop.h"
#include "reschd_operator.h"

const int batchsize = 5;

bool termi(const queue<int>& arriv, const vector<vector<int>>& real_ctime);
bool termi(const vector<list<int>> ques, const vector<vector<array<int,4>>>& mstate);
bool termi(const int stage_ind, const vector<list<int>> ques, const vector<vector<array<int,4>>>& mstate);

// void workshop_evolve_1step(const int &num_stage, int &t, vector<vector<array<int, 4>>> &mstate, vector<vector<int>> &tasks, vector<array<int,3>> &real_patch, vector<list<int>> &resched_list, vector<list<int>> &que, mt19937 &gen1, mt19937 &gen2, uniform_real_distribution<float> &dist01, const vector<vector<int>> &process_time, const vector<vector<vector<float>>> &eff_prob, const vector<vector<vector<float>>> &br_prob);

class DFSP1{
private:
    int num_of_stage; //流水线长度
    int num_of_job; //车间中工件数量
    int current_time; //车间当前时间
    vector<int> machines_per_stage; //流水线每个阶段的机器数量
    int makespan; //对应静态流水车间问题的生产完成时间

    vector<vector<array<int,4>>> machine_state; //储存每台机器的状态信息
    // each element looks like [M(t), A(t), E(t), job_index]，分别为机器的忙碌状态指示函数、健康状态指示函数、效率水平指示函数和正在处理的工作指标

    vector<vector<int>> task_remain; //任务剩余矩阵
    vector<array<int,3>> real_dispatch; // 储存每个任务的 [所指派的机器指标，真实开始加工时间，原计划的期望完成时间]
    
    vector<vector<int>> real_completion_time; //储存每个工作在流水线每个阶段的实际加工完成时间
    vector<list<int>> Q; //记录流水线每个阶段前正在等待被加工的工作队列信息
    
    vector<vector<vector<float>>> br_prob; //储存每个stage中每台机器的损坏与修复转移概率
    vector<vector<vector<float>>> eff_prob; //效率水平转移概率

    queue<int> arrive_times;//每批新工作的到达时间

    vector<list<int>> reschedule_list; //储存需要重调度的信息{stage_index: [disruption_types, job_ind, disruption_types, job_ind...]}
    // disruption types 0: new job, 1: machine breakdown, 2:task delay and processing on this stage is delayed,
    // 3: task delay and the job arrive this stage with with delayment

    int turn; //0: env turn, 1: reschd turn; -1 termination condition, -2 at horizon

public:
    DFSP1(){
        this->num_of_job = 0;
        this->num_of_stage = 0;
        this->current_time = 0;
    }
    // 从一个静态流水车间实例来创建动态流水车间实例，不考虑新工作的到达
    DFSP1(const HFSP& ins, const vector<vector<vector<float>>>& param_br_prob, const vector<vector<vector<float>>>& param_eff_prob){
        this->num_of_job = ins.num_of_job;
        this->num_of_stage = ins.num_of_stage;
        this->current_time = 0;
        this->machines_per_stage = ins.machines_per_stage;
        this->makespan = ins.makespan;
        vector<vector<array<int, 4>>> mstate(num_of_stage);
        for(int i=0; i<num_of_stage; i++){
            for(int j=0; j<this->machines_per_stage[i]; j++){
                mstate[i].emplace_back(array<int,4>{0,1,1,-1});
            }
        }
        this->machine_state = mstate;
        this->real_dispatch = vector<array<int,3>>(this->num_of_job, array<int,3>{-1,-1,-1});
        this->real_completion_time = vector<vector<int>>(this->num_of_job, vector<int>(this->num_of_stage, -1));
        this->Q = vector<list<int>>(this->num_of_stage);
        Q[0] = ins.coded_solution;
        this->reschedule_list = vector<list<int>>(this->num_of_stage, list<int>());
        
        this->br_prob = param_br_prob;
        this->eff_prob = param_eff_prob;
        this->arrive_times = queue<int>();
        this->turn = 0;
    } 

    // Considering new job. 从一个静态流水车间实例创建动态流水车间实例
    DFSP1(const HFSP& ins, const vector<vector<vector<float>>>& param_br_prob, const vector<vector<vector<float>>>& param_eff_prob, const vector<int>& arv_times){
        this->num_of_job = ins.num_of_job;
        this->num_of_stage = ins.num_of_stage;
        this->current_time = 0;
        this->machines_per_stage = ins.machines_per_stage;
        this->makespan = ins.makespan;

        vector<vector<array<int, 4>>> mstate(num_of_stage);
        for(int i=0; i<num_of_stage; i++){
            for(int j=0; j<this->machines_per_stage[i]; j++){
                mstate[i].emplace_back(array<int,4>{0,1,1,-1});
            }
        }
        this->machine_state = mstate;

        this->task_remain = vector<vector<int>> (this->num_of_job, vector<int>(this->num_of_stage, 1));
        this->real_dispatch = vector<array<int,3>>(this->num_of_job, array<int,3>{-1,-1,-1});
        this->real_completion_time = vector<vector<int>>(this->num_of_job, vector<int>(this->num_of_stage, -1));
        this->Q = vector<list<int>>(this->num_of_stage);
        Q[0] = ins.coded_solution;
        this->reschedule_list = vector<list<int>>(this->num_of_stage, list<int>());

        this->br_prob = param_br_prob;
        this->eff_prob = param_eff_prob;
        for(auto ele:arv_times){
            this->arrive_times.push(ele);
        }
        this->turn = 0;
    } 

    //直接输入问题规模和初始解创建动态流水车间实例，不考虑新工作。
    DFSP1(const int num_job, const int num_stage, const vector<int> m_per_stage, const int span, const list<int>& solu_seq, const vector<vector<vector<float>>>& param_br_prob, const vector<vector<vector<float>>>& param_eff_prob){
        this->num_of_job = num_job;
        this->num_of_stage = num_stage;
        this->current_time = 0;
        this->machines_per_stage = m_per_stage;
        this->makespan = span;

        vector<vector<array<int, 4>>> mstate(num_stage);
        for(int i=0; i<num_stage; i++){
            for(int j=0; j<m_per_stage[i]; j++){
                mstate[i].emplace_back(array<int,4>{0,1,1,-1});
            }
        }
        this->machine_state = mstate;

        this->task_remain = vector<vector<int>> (num_job, vector<int>(num_stage, 1));
        this->real_dispatch = vector<array<int,3>>(num_job, array<int,3>{-1,-1,-1});
        this->real_completion_time = vector<vector<int>>(num_job, vector<int>(num_stage, -1));
        this->Q = vector<list<int>>(num_stage);
        Q[0] = solu_seq;
        this->reschedule_list = vector<list<int>>(num_stage, list<int>());

        this->br_prob = param_br_prob;
        this->eff_prob = param_eff_prob;
        this->turn = 0;       
    }

    //在模拟车间生产的过程中动态事件发生后，根据车间当前状态变量创建动态流水车间状态实例
    DFSP1(const DFSP1* ins, int param_t, int param_num_job, vector<vector<array<int,4>>>& param_machine_state,
    vector<array<int,3>> &param_real_dispatch, vector<vector<int>>& param_task_remain, vector<vector<int>> param_real_completion_time,
    vector<list<int>>param_Q, queue<int> param_arrive_times, vector<list<int>> param_reschedule_list, int param_turn){
        this->current_time = param_t;
        this->num_of_job = param_num_job;
        this->num_of_stage = ins->num_of_stage;
        this->machines_per_stage = ins->machines_per_stage;
        // this->process_time= ins->process_time;
        // this->exp_process_time = ins->exp_process_time;
        // this->completion_table = ins->completion_table;
        this->makespan = ins->makespan;

        this->machine_state = param_machine_state;
        this->real_dispatch = param_real_dispatch;
        this->task_remain = param_task_remain;
        this->real_completion_time = param_real_completion_time;
        this->Q = param_Q;

        this->arrive_times = param_arrive_times;
        this->reschedule_list = param_reschedule_list;

        this->br_prob = ins->br_prob;
        this->eff_prob = ins->eff_prob;
        this->turn = param_turn;
    }

    //将车间状态 *ins 的工作队列调整为 param_Q 之后，创建调度完成车间状态
    DFSP1(const DFSP1* ins, const vector<list<int>>& param_Q){
        this->current_time = ins->current_time;
        this->num_of_job = ins->num_of_job;
        this->num_of_stage = ins->num_of_stage;
        this->machines_per_stage = ins->machines_per_stage;
        // this->process_time= ins->process_time;
        // this->exp_process_time = ins->exp_process_time;
        // this->completion_table = ins->completion_table;
        this->makespan = ins->makespan;

        this->machine_state = ins->machine_state;
        this->real_dispatch = ins->real_dispatch;
        this->task_remain = ins->task_remain;
        this->real_completion_time = ins->real_completion_time;
        this->Q = param_Q;

        this->arrive_times = ins->arrive_times;
        this->reschedule_list = vector<list<int>>(this->num_of_stage, list<int>());

        this->br_prob = ins->br_prob;
        this->eff_prob = ins->eff_prob;
        this->turn = 0;
    }
    
    //上一个函数的重载
    DFSP1(DFSP1& ins, const vector<list<int>>& param_Q){
        this->current_time = ins.get_time();
        this->num_of_job = ins.get_num_job();
        this->num_of_stage = ins.get_num_stage();
        this->machines_per_stage = ins.get_mac_per_stage();
        // this->process_time= ins.get_process_time();
        // this->exp_process_time = ins.get_exp_process_time();
        // this->completion_table = ins.get_completion_table();
        this->makespan = ins.get_span();

        this->machine_state = ins.get_machine_state();
        this->real_dispatch = ins.get_real_dispatch();
        this->task_remain = ins.get_task_remain();
        this->real_completion_time = ins.get_real_completion_table();
        this->Q = param_Q;

        this->arrive_times = ins.get_arrive_times();
        this->reschedule_list = vector<list<int>>(this->num_of_stage, list<int>());

        this->br_prob = ins.get_brprob();
        this->eff_prob = ins.get_effprob();
        this->turn = 0;
    }

    //模拟车间运行状态，产生下一个需要重调度的场景，三个随机数种子分别控制不同随机数的产生    
    DFSP1 env_run(unsigned seed1, unsigned seed2, unsigned seed3, const vector<vector<int>>& process_time);

    //模拟车间运行状态, 用于 MCTS 中展开场景树生成未来重调度节点
    DFSP1 simul_run(const int horizon, unsigned seed1, unsigned seed2, unsigned seed3, const vector<vector<int>>& process_time);

    //模拟车间运行直到预测时间窗的末端，中间若产生动态扰动一律以右移规则处理。
    DFSP1 rollout(const int horizon, unsigned seed1, unsigned seed2, unsigned seed3, const vector<vector<int>>& process_time);

    //从当前的车间状态出发，进行 simulation_times 次运行模拟，求取平均生产完成时间。
    double multi_simu_makespan(const int simulation_times, unsigned seed, const vector<vector<int>>& process_time);

    //从当前的车间状态出发，若将工作队列调整为 out_q 后，进行多次运行模拟，求取平均生产完成时间。
    double multi_simu_makespan_q(const int simulation_times, unsigned seed, const vector<vector<int>>& process_time, const vector<list<int>>& out_q);

    //下面三个函数，利用机器状态转移的概率对每个工作期望加工时间进行计算，并以此计算出期望生产完成时间（效果并不好）
    float expected_span(const vector<vector<int>>& process_time, const vector<vector<vector<float>>>& exp_process_time);
    int expected_static_span(const vector<vector<int>>& process_time);
    int expected_static_span_outq(const vector<vector<int>>& process_time, const vector<list<int>>& q);

    //几种基于调度规则和构造型启发式规则的局部重调度算子
    DFSP1 resched_spt(const vector<vector<int>>& process_time);//最短时间有限
    DFSP1 resched_srpt(const vector<vector<int>>& process_time);//剩余最短时间优先（即在剩余机器上的加工时间之和）
    DFSP1 resched_lpt(const vector<vector<int>>& process_time);//最长时间优先
    DFSP1 resched_rightshift();//右移

    //DFSP1 resched_est(); //按照静态流水车间生产时间表的最早开始时间优先 
    //DFSP1 resched_ect(); //按照静态流水车间生产时间表的最早完成时间优先
    //上述两个算子在单机器的流水线中没有意义，与右移规则相同，在柔性流水车间中或有用

    //六种构造型启发式规则
    DFSP1 resched_ritm(const vector<vector<int>>& process_time);
    DFSP1 resched_neh(const vector<vector<int>>& process_time);
    DFSP1 resched_neh1(const vector<vector<int>>& process_time);
    DFSP1 resched_nehnm(const vector<vector<int>>& process_time);
    DFSP1 resched_nehnm1(const vector<vector<int>>& process_time);
    DFSP1 resched_nehkk(const vector<vector<int>>& process_time);

    //随机从上述规则中选取一个
    DFSP1 resched_random(const unsigned seed, const vector<vector<int>>& process_time);

    //用于 MCTS 过程中的随机重调度算子，对每一个需要重调度的工作队列，从 rs, spt, ritm, neh, neh1, nehnm, nehnm1, nehkk中随机选取一个
    DFSP1 resched_random_uct(const unsigned seed, const vector<vector<int>>& process_time);

    //在上面的随机重调度函数基础上，额外引入了随机插入的邻域搜索算子
    DFSP1 resched_rand_insert(const unsigned seed);

    //全局重调度
    // DFSP1 global_resched(const unsigned seed, const vector<vector<int>>& process_time);

    //判断此时的车间状态是否属于生产完成状态
    bool is_terminal();

    //判断两个车间状态是否相同的布尔函数
    bool operator == (const DFSP1 &other){
        return (this->current_time==other.current_time) && compare_vva4(this->machine_state, other.machine_state) && compare_vv(this->task_remain, other.task_remain) && compare_vl(this->Q, other.Q) && compare_va3(this->real_dispatch, other.real_dispatch);
    }
    
    //返回对应的静态流水车间的生产完成时间
    int get_span(){
        return this->makespan;
    }

    //下列为返回车间各种状态信息的函数
    vector<vector<array<int,4>>> get_machine_state(){
        return this->machine_state;
    }
    vector<vector<int>> get_task_remain(){
        return this->task_remain;
    }
    vector<array<int,3>> get_real_dispatch(){
        return this->real_dispatch;
    }
    queue<int> get_arrive_times(){
        return this->arrive_times;
    }
    int get_time(){
        return this->current_time;
    }
    int get_turn(){
        return this->turn;
    }
    int get_num_job(){
        return this->num_of_job;
    }
    int get_num_stage(){
        return this->num_of_stage;
    }
    vector<int> get_mac_per_stage(){
        return this->machines_per_stage;

    }
    vector<list<int>> get_reschedlist(){
        return this->reschedule_list;
    }
    vector<list<int>> get_q(){
        return this->Q;
    }
    vector<vector<int>> get_real_completion_table(){
        return this->real_completion_time;
    }
    vector<vector<vector<float>>> get_brprob(){
        return this->br_prob;
    }
    vector<vector<vector<float>>> get_effprob(){
        return this->eff_prob;
    }
};

#endif