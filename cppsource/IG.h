#ifndef MY_IG
#define MY_IG

#include "headers.h"
#include "funcs.h"
#include "workshop.h"
#include "dynamic_workshop.h"

//基于 IG 算法的重调度方法，每次迭代过程中对所有需要考虑重调度的队列进行解构、重构和局部改进
DFSP1 IG_resched_time(DFSP1& state0, const vector<vector<int>>& process_time, const double temperature, const int d, const double time_budget, const unsigned seed);
vector<list<int>> IterImprov_insert(vector<list<int>> q, DFSP1& state, const vector<vector<int>>& process_time, const vector<list<int>>& resched_list, int& span, const unsigned seed);
vector<list<int>> Destruction(vector<list<int>>& q, const vector<list<int>>& resched_list, const int d, const unsigned seed);
int Construction(vector<list<int>>& q, vector<list<int>>& reinsert_lists, DFSP1& state, const vector<vector<int>>& ptime, const unsigned seed);

//每次迭代过程中随机地对一个需要考虑重调度的队列进行解构、重构和局部改进
DFSP1 IG1_resched_time(DFSP1& state0, const vector<vector<int>>& process_time, const double temperature, const int d, const double time_budget, const unsigned seed);
vector<list<int>> IterImprov_insert1(vector<list<int>> q, DFSP1& state, const vector<vector<int>>& process_time, const vector<int>& resched_stages, int& span, const unsigned seed);
list<int> Destruction1(list<int>& l, const int d, const unsigned seed);
int Construction1(vector<list<int>>& q, const int stageind, list<int>& reinsert_list, DFSP1& state, const vector<vector<int>>& ptime, const unsigned seed);

#endif