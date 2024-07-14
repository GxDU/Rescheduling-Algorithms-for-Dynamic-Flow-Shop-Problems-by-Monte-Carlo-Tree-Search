#ifndef MY_MCTS_DPW
#define MY_MCTS_DPW

#include "headers.h"
#include "tree.h"
#include "mcts.h"


// NodeFSP UCT_with_PW_LS(NodeFSP root, const double time_budget, const double scalar, const float k, const float alpha, const float ratio, const int horizon, const unsigned seed, const int repeat);

// NodeFSP UCT_with_PW(NodeFSP root, const double time_budget, const double scalar, const float k, const float alpha, const int horizon, const unsigned seed, const int repeat);

// NodeFSP UCT_with_DPW(NodeFSP root, const double time_budget, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const float ratio, const int horizon, const unsigned seed, const int repeat);

// NodeFSP MostVisitedChild_fsp(NodeFSP& root);

// NodeFSP* PW_LS(NodeFSP& parent, const double scalar, const int horizon, const float k, const float alpha, const float ratio, const unsigned seed);

// NodeFSP* PW(NodeFSP& parent, const double scalar, const int horizon, const float k, const float alpha, const unsigned seed);

// NodeFSP* DPW(NodeFSP& parent, const double scalar, const int horizon, const float k1, const float alpha1, const float k2, const float alpha2, const float ratio, const unsigned seed);

// NodeFSP* Get_Resched(NodeFSP& parent, const unsigned seed, const double scalar);

// NodeFSP* Get_Resched_LS(NodeFSP& parent, const unsigned seed, const double scalar, const float ratio);

// NodeFSP* Epsilon_Greedy_UCT(NodeFSP& parent, const double scalar, const unsigned seed);


// NodeFSP UCT1_with_DPW1_time(NodeFSP root, const double time_budget, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const float ratio, const int horizon, const unsigned seed, const int repeat);

// UCT1_with_DPW 在对重调度节点局部搜索中，对此节点排名比例为前 ratio 部分的子节点中随机选取一个，对其工作的队列进行随机插入的局部搜索
// horizon为对未来预测的时长范围，本工作中未考虑滚动时域，在实验中取999999
// root 为等待重调度的节点，ptime为任务加工时间矩阵，budget参数为算法停止条件，return_version 为结果返回策略，见论文
//其余参数的含义见论文中结合 DPW 的 MCTS 算法部分
//算法终止时间为迭代次数
NodeFSP1 UCT1_with_DPW_iter(NodeFSP1 root, const vector<vector<int>>& ptime, const int iter_budget, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const float ratio, const int horizon, const unsigned seed, const int return_version);
//算法终止条件为时间
NodeFSP1 UCT1_with_DPW_time(NodeFSP1 root, const vector<vector<int>>& ptime, const double time_budget, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const float ratio, const int horizon, const unsigned seed, const int return_version);
NodeFSP1* DPW1(NodeFSP1& parent, const vector<vector<int>>& ptime, const double scalar, const int horizon, const float k1, const float alpha1, const float k2, const float alpha2, const float ratio, const unsigned seed);
NodeFSP1* Epsilon_Greedy_UCT1(NodeFSP1& parent, const double scalar, const unsigned seed);
NodeFSP1* Get_Resched_LS1(NodeFSP1& parent, const vector<vector<int>>& ptime, const unsigned seed, const double scalar, const float ratio);
void pw_resched_local_insert(vector<list<int>>& sampled_solu, const vector<list<int>>& resched_list, unsigned seed);

// UCT1_with_DPW2 在 DPW 的局部搜索中，对重调度节点进行局部搜索时，直接对此重调度节点的工作队列进行局部搜索，故没有ratio参数
NodeFSP1 UCT1_with_DPW2_time(NodeFSP1 root, const vector<vector<int>>& ptime, const double time_budget, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const int horizon, const unsigned seed, const int return_version);
NodeFSP1 UCT1_with_DPW2_iter(NodeFSP1 root, const vector<vector<int>>& ptime, const int iter_budget, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const int horizon, const unsigned seed, const int return_version);
NodeFSP1* DPW2(NodeFSP1& parent, const vector<vector<int>>& ptime, const double scalar, const int horizon, const float k1, const float alpha1, const float k2, const float alpha2, const unsigned seed);
NodeFSP1* Get_Resched_LS2(NodeFSP1& parent, const vector<vector<int>>& ptime, const unsigned seed, const double scalar);





#endif