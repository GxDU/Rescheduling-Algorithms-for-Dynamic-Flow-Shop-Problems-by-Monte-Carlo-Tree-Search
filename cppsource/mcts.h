#ifndef MY_MCTS
#define MY_MCTS

#include"headers.h"
#include"tree.h"

/*
double regularized_reward(const int makespan, const vector<vector<int>> ctime, const vector<vector<int>> real_ctime);
double average_deviation(const vector<vector<int>> ctime, const vector<vector<int>> real_ctime_front, const vector<vector<int>> real_ctime_root);
int deviation(const vector<vector<int>>& ctime, const vector<vector<int>>& real_ctime);
// double deviation2(const int horizon, const vector<vector<int>> real_ctime_front, const DFSP& state_root);
double deviation2(const int horizon, const vector<vector<int>> real_ctime_front, const vector<vector<int>> &root_completion_table, const vector<vector<int>> &root_real_completion_table);
// double penalty1(const int horizon, const DFSP& state_front, const DFSP& state_root);
double penalty1(const int horizon, DFSP& state_front, DFSP& state_root);
double penalty2(const int horizon, const int mkspan, const vector<vector<int>>& real_ctime_front, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<vector<int>>& taks_remain, vector<array<int,3>>& real_patch);
double penalty3(const int horizon, const int num_of_job, const vector<vector<int>>& real_ctime_front, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<vector<int>>& taks_remain, vector<array<int,3>>& real_patch);
double penalty4(const int horizon, const int mkspan, const vector<vector<int>>& real_ctime_front, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<vector<int>>& task_remain, vector<array<int,3>>& real_patch);
double penalty5(const int horizon, const int mkspan, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<vector<int>>& task_remain, vector<array<int,3>>& real_patch);
double penalty6(const int num_job, const int horizon, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<vector<int>>& real_ctime_root, const vector<vector<int>>& task_remain, vector<array<int,3>>& real_patch);
double effective_productivity(const int num_job, const int horizon, const vector<vector<int>>& ptime, const vector<vector<int>>& real_ctime, vector<array<int,3>>& real_patch);
*/

/*
// parameters are: root, scalar, time slot, search budget, workers, horizon.
NodeFSP UCT_horizon_fsp(NodeFSP &root, const double scalar, const int budget, const int horizon, const unsigned seed, const int repeat);
NodeFSP UCT_horizon_time_fsp(NodeFSP &root, const double scalar, const double time_budget, const int horizon, const unsigned seed, const int repeat);
NodeFSP parallel_UCT_horizon_fsp(NodeFSP&, const double, const double, const int, const int, const int, const unsigned, const int repeat);
NodeFSP parallel_UCT_horizon_fsp_time(NodeFSP &root, const double scalar, const double time_slot, const double time_budget, const int workers, const int horizon, const unsigned seed, const int repeat);
// parameters: root, scalar, time slot, workers, horizon
NodeFSP slave_UCT_fsp(NodeFSP, const double, const double, const int, const int, const unsigned, const int);
// parameters: current node, scalar, horizon
NodeFSP* SimTree_horizon_fsp(NodeFSP&, const double, const int, const unsigned);
// parameters: state, horizon
// double SimDefault_horizon_fsp(NodeFSP, const int, const NodeFSP&);
// double SimDefault_horizon_fsp(DFSP, const int, const NodeFSP&, const unsigned, const int);
double SimDefault_horizon_fsp(DFSP, const int, NodeFSP&, const unsigned, const int);
// parameters: current node, reward
void BackProp_fsp(NodeFSP&, const double);
// parameters: node, scalar
NodeFSP BestChild_fsp(NodeFSP&, const double);
NodeFSP MostVisitedChild_fsp(NodeFSP&);
// NodeFSP* BestChild_fsp_pointer(NodeFSP*, const double);
NodeFSP* BestChild_fsp_pointer(NodeFSP&, const double);
// parameters: node, horizon, returns true is it finds a new child node, else it returns false.
// NodeFSP Expand_fsp(NodeFSP*, const int);
bool Expand_fsp(NodeFSP&, const int, const unsigned);
*/

//限制迭代次数为 budget 次
NodeFSP1 UCT_horizon_fsp1(const vector<vector<int>>& ptime, NodeFSP1 &root, const double scalar, const int budget, const int horizon, const unsigned seed, const int repeat, const int reward_version, const int return_version);
//限制搜索时长为 time_budget
NodeFSP1 UCT_horizon_time_fsp1(const vector<vector<int>>& ptime, NodeFSP1 &root, const double scalar, const double time_budget, const int horizon, const unsigned seed, const int repeat, const int reward_version, const int return_version);
//参数含义：
//ptime: 每个工件在流水线每个阶段的加工时间矩阵
//root: 代表需要重调度的车间状态的节点
//scalar: 上置信界系数
//budget: MCTS 算法的终止条件
//horizon: 对未来的预测时间窗终点, 在实验中采取非常大的数值以保证预测到车间生产完成的状态
//seed: 随机数种子
//repeat: 当reward_version=1 时, 对生产完成时间的评估是通过 repeat 次随机rollout 并取平均得到的
//reward_version : 0 在评估生产完成时间时不考虑动态事件, 1 多次仿真求取平均生产完成时间, 2 利用转移概率计算期望生产完成时间
//return_version: 0 返回平均生产完成时间最短的子节点, 1 返回上置信界修正后生产完成时间最短的子节点, 2 返回最多次访问的子节点

//MCTS 算法中的选择过程, parameters: ptime, root, scalar, horizon, seed
NodeFSP1* SimTree_horizon_fsp1(const vector<vector<int>>& ptime, NodeFSP1& root, const double scalar, const int horizon, const unsigned seed);

//MCTS 算法中的仿真过程, 采用基于右移规则的 rollout, 评估车间状态 state 的生产完成时间
// double SimDefault_horizon_fsp1(const vector<vector<int>>&, DFSP1, const int, NodeFSP1&, const unsigned, const int, const int);
double SimDefault_horizon_fsp1(const vector<vector<int>>& ptime, DFSP1 state, const int horizon, NodeFSP1& root, const unsigned seed, const int reward_version, const int repeat);

//MCTS 算法中的反向传播过程
void BackProp_fsp1(NodeFSP1& front, const double reward);


//搜索并返回节点 root 的子节点中UCB最高的子节点的copy
NodeFSP1 BestChild_fsp1(NodeFSP1& root, const double scalar);

//搜索并返回被最多次访问的子节点的copy
NodeFSP1 MostVisitedChild_fsp1(NodeFSP1&);

//搜索并返回节点 node 的子节点中 UCB 最高子节点的地址
NodeFSP1* BestChild_fsp_pointer1(NodeFSP1&, const double);

//MCTS 中的节点展开函数
// parameters: processing times, node, horizon, returns true is it finds a new child node, else it returns false.
bool Expand_fsp1(const vector<vector<int>>&, NodeFSP1&, const int, const unsigned);


#endif