#ifndef MY_MCTS_BP
#define MY_MCTS_BP

#include"headers.h"
#include"tree.h"
#include"mcts.h"

// void BackProp_fsp_new(NodeFSP&, const double);

void BackProp_fsp1_new(NodeFSP1&, const double, const double);

// NodeFSP new_UCT_horizon_time_fsp(NodeFSP &root, const double scalar, const double time_budget, const int horizon, const unsigned seed, const int repeat);

NodeFSP1 new_UCT_horizon_iter_fsp1(const vector<vector<int>>& ptime, NodeFSP1 &root, const double scalar, const int iter_budget, const int horizon, const unsigned seed, const int repeat, const int reward_version, const int return_version, const double alpha);

#endif