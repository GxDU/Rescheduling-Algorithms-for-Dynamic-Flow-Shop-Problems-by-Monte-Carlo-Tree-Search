#ifndef WORKSHOP_FUNCS
#define WORKSHOP_FUNCS

// #include<string>
// #include<memory>
// #include<assert.h>

// #include</home/liulab/anaconda3/envs/py311/lib/python3.11/site-packages/pybind11/include/pybind11/pybind11.h>
// #include</home/liulab/anaconda3/envs/py311/lib/python3.11/site-packages/pybind11/include/pybind11/stl.h>
//// #include<pybind11.h>
//// #include<stl.h>
#include"funcs.h"
namespace py = pybind11;

//生成一个长度为 jobs 的随机工作序列
list<int> random_solu_seq(const int jobs);

//对于加工时间矩阵为 process_time 的 pfsp，计算解 solu 对应的makespan
int calculate_span_pfsp(const list<int>& solu, const vector<vector<int>>& process_time);

// NEH algorithm
void ta_acc(const int machines, const int index, const int k, const int size_arr, const vector<vector<int>>& process_time, list<int>& partial_solution, int (&f)[], int (&q)[], int(&e)[]);
list<int> NEH_func(const vector<vector<int>>& process_time);
void sub_NEH(const int machines, const int index, const int k, const vector<vector<int>>& process_time, list<int>& partial_solution);
int best_index_neh(const int row, const int col, const int (&f)[], const int (&q)[]);

//NEH1 algorithm
list<int> NEH1_func(const vector<vector<int>>& process_time);
void sub_NEH1(const int machines, const int index, const int k, const vector<vector<int>>& process_time, list<int>& partial_solution, const vector<int>& a_value, const vector<int>& b_value, int& a_solu, int& b_solu);
int best_index_neh1(const int row, const int col, const int (&f)[], const int (&q)[], const int tie_brk, int& makespan);

// NEHNM
vector<vector<int>> sort_NM(const int n, const int m, const vector<vector<int>>& process_time); 
list<int> NEHNM_func(const vector<vector<int>>& process_time);

// NEHNM1
vector<vector<int>> sort_NM1(const int n, const int m, const vector<vector<int>>& process_time, vector<int>& total_ptime, vector<int>& a_value, vector<int>& b_value);
list<int> NEHNM1_func(const vector<vector<int>>& process_time);

//NEHKK1
list<int> NEHKK_func(const vector<vector<int>>& process_time);
void sub_NEHKK(const int machines, const int index, const int k, const vector<vector<int>>& process_time, list<int>&  partial_solution, const vector<int>& a_value, const vector<int>& b_value);
int best_index_nehkk(const int row, const int col, const int (&f)[], const int (&q)[], const int tie_brk);

//固定 seq 中的工件顺序不变，将 inds 中的工件以 NEH 最优插入的方式插入到 seq 序列之后
list<int> partial_fixed_NEH(const vector<vector<int>>& process_time, list<int> seq, const vector<int>& inds);
int best_index_partial_neh(const int num_fixed, const int row, const int col, const int (&f)[], const int (&q)[]);
void sub_partial_fixed_NEH_ta(const int num_fixed, const int machines, const int index, const int k, const vector<vector<int>>& process_time, list<int>& seq, list<int>& partial_solution, const vector<int>& inds);

//
// Some fundamental fuctions in HFSP
//
// 在有平行机器的 HFSP 中，利用 First available machine 规则（FAM）解码工作序列
// decoded solution (job_index, stage_index, machine_index, start_time)
list<vector<int>> decode_FAM(const list<int>& coded_solu, const vector<int>& machines_per_stage, const vector<vector<int>>& process_time);

// 在有平行机器的 HFSP 中，利用 First available machine 规则（FAM）计算工作序列对应的生产完成时间
int HFSP_makespan_FAM(const list<int>& coded_solu, const vector<int>& machines_per_stage, const vector<vector<int>>& process_time);

//对每个剩余工件 j 根据现在流水线中每个 stage 的最早可用时间计算其闲置时间
int idle_time(const vector<int>& process_time, const vector<int>& earliest_avai_times);

//HFSP 的 RITM 算法
//返回解码后的解
list<vector<int>> Algo_RITM(const int jobs, const int stages, const vector<int>& machines_per_stage, const vector<vector<int>>& process_time, const float a1=0.5, const float a2=1.2);
//返回工件加工顺序作为解
vector<int> Algo_RITM_coded(const int jobs, const int stages, const vector<int>& machines_per_stage, const vector<vector<int>>& process_time, const float a1=0.5, const float a2=1.2);
//指派工件
int assign_job(const int job_ind, int& pos, list<vector<int>>& solution, const vector<int>& process_time, vector<int>& completime, vector<vector<int>>& avaitime, vector<int>& earliest_avaitime, vector<int>& m_index);

// 结合 FAM 规则求解 HFSP 的 SA 算法
list<int> SA_slave_HFSP(float temperature, const float anneal_rate, const int max_iter, const int iter_size, const int stop_crtn, const vector<int>& machines_perstage, const vector<vector<int>>& process_time, const int initsolu);
// SA 的邻域搜索算子，包括 SWAP，INSERT，REVERSE
list<int> SA_sample_neighbor(list<int>, int);

#endif