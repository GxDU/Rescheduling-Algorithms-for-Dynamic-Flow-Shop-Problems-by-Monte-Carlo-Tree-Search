#ifndef MY_EXPERIMENTS
#define MY_EXPERIMENTS

#include"headers.h"
#include"tree.h"
#include"mcts.h"
#include"mcts_dpw.h"
#include"mcts_newbp.h"
#include"dynamic_workshop.h"
#include"SA.h"
#include"IG.h"

/*
// int tard(const vector<vector<int>>& ctime, const vector<vector<int>>& real_ctime);

// void test_rollout(DFSP state, int predict_lenth);

// vector<double> exp_fsp_rs(DFSP state);

// vector<double> exp_fsp_mcts(DFSP state, const int budget, const int workers, const int predict_lenth, const double scalar, const double time_slot, const int repeat);

// vector<double> exp_fsp_mcts_time(DFSP state, const double time_budget, const int workers, const int predict_lenth, const double scalar, const double time_slot, const int repeat);

// vector<double> exp_fsp_spt(DFSP state);

// vector<double> exp_fsp_srpt(DFSP state);

// vector<double> exp_fsp_lpt(DFSP state);

// vector<double> exp_fsp_ect(DFSP state);

// vector<double> exp_fsp_est(DFSP state);

// vector<double> exp_fsp_ritm(DFSP state);

// vector<double> exp_fsp_neh(DFSP state);

// vector<double> exp_fsp_random(DFSP state);

// vector<double> exp_fsp_rs_with_seed(DFSP state, const unsigned seed, const int index);

// vector<double> exp_fsp_spt_with_seed(DFSP state, const unsigned seed, const int index);

// vector<double> exp_fsp_lpt_with_seed(DFSP state, const unsigned seed, const int index);

// vector<double> exp_fsp_srpt_with_seed(DFSP state, const unsigned seed, const int index);

// vector<double> exp_fsp_ect_with_seed(DFSP state, const unsigned seed, const int index);

// vector<double> exp_fsp_est_with_seed(DFSP state, const unsigned seed, const int index);

// vector<double> exp_fsp_ritm_with_seed(DFSP state, const unsigned seed, const int index);

// vector<double> exp_fsp_neh_with_seed(DFSP state, const unsigned seed, const int index);

// vector<double> exp_fsp_random_with_seed(DFSP state, const unsigned seed, const int index);

// vector<double> exp_fsp_mcts_with_seed(DFSP state, const int budget, const int workers, const int predict_lenth, const double scalar, const double time_slot, const unsigned seed, const int index, const int repeat);

// vector<double> exp_fsp_mcts_time_with_seed(DFSP state, const double time_budget, const int workers, const int predict_lenth, const double scalar, const double time_slot, const unsigned seed, const int index, const int repeat);

// vector<double> exp_fsp_single_mcts_with_seed(DFSP state, const double time_budget, const int predict_lenth, const double scalar, const unsigned seed, const int index, const int repeat);

// vector<double> exp_fsp_pwmcts_with_seed(DFSP state, const double time_budget, const int predict_lenth, const double scalar, const unsigned seed, const int index, const float k, const float alpha, const int repeat);

// vector<double> exp_fsp_dpwmcts_with_seed(DFSP state, const double time_budget, const int predict_lenth, const double scalar, const unsigned seed, const int index, const float k1, const float alpha1, const float k2, const float alpha2, const float ratio, const int repeat);

// vector<double> exp_fsp_lsmcts_with_seed(DFSP state, const double time_budget, const int predict_lenth, const double scalar, const unsigned seed, const int index, const float k, const float alpha, const float ratio, const int repeat);

// vector<double> exp_fsp_new_single_mcts_with_seed(DFSP state, const double time_budget, const int predict_lenth, const double scalar, const unsigned seed, const int index, const int repeat);

// vector<double> exp_sa_resched(DFSP state, const float temperature, const float anneal_rate, const double time_budget, const int iter_size, const int stop_crtn, const unsigned seed, const int index, const bool shuffle);
*/

//参数含义： 
    // state: 车间初始状态， process_time: 加工时间矩阵，seed: 随机种子， index: 线程指标

// 以下函数返回四元组（生产完成时间，算法总运行时间，重调度次数，线程指标）

vector<double> exp_fsp1_rs_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index);

vector<double> exp_fsp1_spt_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index);

vector<double> exp_fsp1_lpt_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index);

vector<double> exp_fsp1_srpt_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index);

vector<double> exp_fsp1_ritm_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index);

vector<double> exp_fsp1_neh_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index);
vector<double> exp_fsp1_neh1_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index);
vector<double> exp_fsp1_nehnm_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index);
vector<double> exp_fsp1_nehnm1_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index);
vector<double> exp_fsp1_nehkk_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index);


vector<double> exp_fsp1_random_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index);


vector<double> exp_fsp1_uct_time_with_seed(DFSP1 state, const vector<vector<int>> process_time, const double time_budget, const int predict_lenth, const double scalar, const unsigned seed, const int index, const int repeat, const int reward_version, const int return_version);
vector<double> exp_fsp1_uct_iter_with_seed(DFSP1 state, const vector<vector<int>> process_time, const int iter_budget, const int predict_lenth, const double scalar, const unsigned seed, const int index, const int repeat, const int reward_version, const int return_version);
vector<double> exp_fsp1_uctnewbp_iter_with_seed(DFSP1 state, const vector<vector<int>> process_time, const int iter_budget, const int predict_lenth, const double scalar, const unsigned seed, const int index, const int repeat, const int reward_version, const int return_version, const double alpha);


vector<double> exp_fsp1_dpw_time_with_seed(DFSP1 state, const vector<vector<int>> process_time, const double time_budget, const int predict_lenth, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const float ratio, const unsigned seed, const int index, const int return_version);
vector<double> exp_fsp1_dpw_iter_with_seed(DFSP1 state, const vector<vector<int>> process_time, const int iter_budget, const int predict_lenth, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const float ratio, const unsigned seed, const int index, const int return_version);

vector<double> exp_fsp1_dpw2_time_with_seed(DFSP1 state, const vector<vector<int>> process_time, const double time_budget, const int predict_lenth, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const unsigned seed, const int index, const int return_version);
vector<double> exp_fsp1_dpw2_iter_with_seed(DFSP1 state, const vector<vector<int>> process_time, const int iter_budget, const int predict_lenth, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const unsigned seed, const int index, const int return_version);


vector<double> exp_fsp1_fta_time_with_seed(DFSP1 state, const vector<vector<int>> process_time, const double init_t, const double time_budget, const unsigned seed, const int index);
vector<double> exp_fsp1_fta1_time_with_seed(DFSP1 state, const vector<vector<int>> process_time, const double init_t, const double time_budget, const unsigned seed, const int index);

vector<double> exp_fsp1_ig_time_with_seed(DFSP1 state, const vector<vector<int>> process_time, const double temperature, const int d, const double time_budget, const unsigned seed, const int index);
vector<double> exp_fsp1_ig1_time_with_seed(DFSP1 state, const vector<vector<int>> process_time, const double temperature, const int d, const double time_budget, const unsigned seed, const int index);

#endif