#ifndef WORKSHOP_MODEL
#define WORKSHOP_MODEL

// #include<cstdlib>
// #include<future>
// #include<thread>
#include"funcs.h"
#include"workshop_funcs.h"

class PFSP{
private:
    int num_machine = 0;
    int num_job = 0;
    vector<vector<int>> process_time;
    list<int> solution;
    int makespan = 0;
    int optimal_span = 0;

public:
    PFSP(const int jobs, const int machines, const vector<vector<int>> ProcessTime){
        this->num_machine = machines;
        this->num_job = jobs;
        this->process_time = ProcessTime;
    }
    PFSP(const int jobs, const int machines, const vector<vector<int>> ProcessTime, const int opti_span){
        this->num_machine = machines;
        this->num_job = jobs;
        this->process_time = ProcessTime;
        this->optimal_span = opti_span;
    }

    // list<int> solve(const char*);
    void solve_neh();
    list<int> random_solution(const unsigned seed);
    list<int> NEH();
    void set_solution(list<int>);
    vector<vector<int>> table_with_makespan(const int, const int, const list<int>&);
    // void rper(int a=0);
    int span_of_solution(const list<int>& solu);

    int get_num_job(){return this->num_job;}
    int get_num_machine(){return this->num_machine;}
    vector<vector<int>> get_process_time(){return this->process_time;}
    int get_makespan(){return this->makespan;}
    list<int> get_solution(){return this->solution;}
    int get_optimal_span(){return this->optimal_span;}
};



class HFSP {
public:
    int num_of_stage;  //流水线长度
    int num_of_job;  //工件数量
    vector<int> machines_per_stage;  //每个阶段机器数
    vector<vector<int>> process_time;  //加工时间矩阵
    list<int> coded_solution;  //工件进入流水线顺序
    list<vector<int>> solution;  //调度时间表
 
    vector<vector<int>> completion_table;  //各个工件在各个stage的完成时间
    int makespan = 0;
// public:
    HFSP(const int stages, const int jobs, const vector<vector<int>>& ProcessTime, const vector<int>& NumMachines){
        num_of_stage = stages;
        num_of_job = jobs;
        process_time = ProcessTime;
        completion_table = vector<vector<int>>(jobs, vector<int>(stages, 0));
        if (NumMachines.size() == stages){
            machines_per_stage = NumMachines;
        }
        else if(NumMachines.size() == 1){
            for(int i=0; i<stages; ++i){
                machines_per_stage.emplace_back(NumMachines[0]);
            }
        }
        else{
            cerr<<"Input Error!"<<endl;
            exit(EXIT_FAILURE);
        }
    }
    void rper(int a=0);  //打印实例信息
    void solve(const char*);  //求解
    list<vector<int>> NEH();  //NEH 算法
    list<vector<int>> RITM();  //RITM 算法
    //模拟退火算法
    list<vector<int>> sa(float temperature, const float anneal_rate, const int max_iter, const int iter_size, const int stop_crtn);
    void solve_sa(float temperature, const float anneal_rate, const int max_iter, const int iter_size, const int stop_crtn);
    
    //给实例设定一个解
    void set_solution(list<int>);

    int get_num_job(){return num_of_job;};
    int get_num_stage(){return num_of_stage;};
    int get_makespan(){return makespan;};
    vector<int> get_machines_per_stage(){return machines_per_stage;};
    list<vector<int>> get_solution(){return solution;};
    list<int> get_coded_solution(){return coded_solution;}
    vector<vector<int>> get_completion_table(){return completion_table;};
    vector<vector<int>> get_process_time(){return process_time;};
};


#endif