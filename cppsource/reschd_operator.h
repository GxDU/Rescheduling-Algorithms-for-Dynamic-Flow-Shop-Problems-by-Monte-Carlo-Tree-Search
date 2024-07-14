#ifndef MY_OPERATORS
#define MY_OPERATORS

#include<list>
#include<vector>
#include<algorithm>
#include<random>
#include"funcs.h"
#include"workshop_funcs.h"

/*
void random_rschd(vector<list<int>>& ques, const map<int,vector<int>>& r_list, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<int>& mac_per_stage);
void random_resched(vector<list<int>>& ques, const vector<list<int>>& r_list, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<int>& mac_per_stage, unsigned seed);
void random_resched1(vector<list<int>>& ques, const vector<list<int>>& r_list, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<int>& mac_per_stage, unsigned seed);

void random_resched_insert(vector<list<int>>& ques, const vector<list<int>>& r_list, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<int>& mac_per_stage, unsigned seed);
*/

//一下为对单个工件队列做重调度的函数，q 为被调整顺序的工件队列，stage 为流水线阶段的指标，ptime 为加工时间矩阵， ctime 为原始生产计划的完成时间表
void operator_insert_front(const int job_ind, list<int>& q, const int position);  //将工件 job_ind 插入到队列 q 的第 position个位置
void operator_insert_back(const int job_ind, list<int>& q, const int position);  //将工件 job_ind 插入到队列 q 的倒数第 position 个位置
void operator_spt(list<int>& q, const int stage, const vector<vector<int>>& ptime);  //按照最短时间优先对队列 q 进行排序
void operator_lpt(list<int>& q, const int stage, const vector<vector<int>>& ptime);  //按照最长时间优先对队列 q 进行排序
void operator_srpt(list<int>& q, const int stage, const vector<vector<int>>& ptime);  //根据每个工件在后续机器上的加工总时长进行升序排序
void operator_ect(list<int>& q, const int stage, const vector<vector<int>>& ctime);  //按照原计划的最早完成时间对工件进行排序
void operator_est(list<int>& q, const int stage, const vector<vector<int>>& ctime, const vector<vector<int>>& ptime);  //按照原生产计划的最早开始加工时间对工件进行排序
void operator_neh(list<int>& q, const int stage, const vector<vector<int>>& ptime); //按照 neh 等构造型启发式规则进行排序，下同
void operator_neh1(list<int>& q, const int stage, const vector<vector<int>>& ptime);
void operator_nehnm(list<int>& q, const int stage, const vector<vector<int>>& ptime);
void operator_nehnm1(list<int>& q, const int stage, const vector<vector<int>>& ptime);
void operator_nehkk(list<int>& q, const int stage, const vector<vector<int>>& ptime);
void operator_ritm(list<int>& q, const int stage, const vector<vector<int>>& ptime, const vector<int>& m_per_state);
// void operator_maxju(list<int>& q, const int stage, const vector<vector<int>>& ptime, const vector<int>& m_per_stage)

//以下为三种 recusive insert 规则
void local_ri_neh(const int current_time, int stage_ind, const int num_stage, const vector<int>& m_per_stage, const vector<vector<int>>& ptime, vector<list<int>>& ques, vector<vector<array<int, 4>>> mstate, vector<array<int, 3>> dispatch, const vector<vector<vector<float>>>& br_p, const vector<vector<vector<float>>>& eff_p);
void local_ri_end(const int stage_ind, const int num_stage, const vector<vector<int>>& ptime, vector<list<int>>& ques);
void local_ri_oddeven(const int stage_ind, const int num_stage, const vector<vector<int>>& ptime, vector<list<int>>& ques);

//随机的全局重调度算子
void random_global_resched(vector<list<int>>& ques, const int current_time, const int num_stage, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<int>& m_per_stage, const vector<vector<array<int, 4>>>& mstate, const vector<array<int, 3>>& dispatch, const vector<vector<vector<float>>>& br_p, const vector<vector<vector<float>>>& eff_p, unsigned seed);

#endif