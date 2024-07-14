#ifndef MY_SA
#define MY_SA

#include "headers.h"
#include "funcs.h"
#include "workshop.h"
#include "dynamic_workshop.h"

 
// 基于固定温度算法的局部重调度算法，每次迭代过程中对所有需要重调度的队列进行邻域搜索
DFSP1 FTA_resched_time(DFSP1 state0, const vector<vector<int>>& process_time_all, const double init_temperature, const double time_budget, const unsigned seed);
//parameters: 
// state0: 需要重调度的车间状态
// process_time_all: 工件的加工时间矩阵
// init_temperature: 算法中的温度
// time_budget: 算法搜索时长
// seed: 随机数种子


// 基于固定温度算法的局部重调度算法，每次迭代过程中只对某个需要重调度的队列进行邻域搜索
DFSP1 FTA1_resched_time(DFSP1 state0, const vector<vector<int>>& process_time_all, const double init_temperature, const double time_budget, const unsigned seed);

//邻域搜索算子，对resched_list中储存的队列都进行随机插入的邻域搜索，返回新的队列信息
vector<list<int>> SA_resched_local_insert(const vector<list<int>>& current_solu, const vector<list<int>>& resched_list, unsigned seed);

//邻域搜索算子，对指定的第 stageind 个工件队列进行随机插入的邻域搜索，并返回新的队列信息
vector<list<int>> SA_resched_local_insert_stage(const vector<list<int>>& current_solu, const int stageind, unsigned seed);

#endif