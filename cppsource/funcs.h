#ifndef MY_FUNCS
#define MY_FUNCS

#include"headers.h"

//计算每个任务在机器损坏和低效率状态运行影响下的期望加工时间
vector<vector<vector<float>>> calculate_exp_time(const vector<vector<int>>& p_time, const int num_job, const int num_stage, const vector<int>& m_per_stage, const vector<vector<vector<float>>>& br_p, const vector<vector<vector<float>>>& effi_p);

//利用期望加工时间计算出期望 makespan (算出来的结果与蒙特卡洛采样得到的结果有差，尚未明确为何)
float exp_span(const int current_time, const int stage_ind, const int num_stage, const vector<int>& m_per_stage, const vector<vector<int>>& p_time, const vector<vector<vector<float>>>& exp_p_time, vector<vector<array<int,4>>> mstate, vector<list<int>> ques, vector<array<int,3>> dispatch, const vector<vector<vector<float>>>& br_p, const vector<vector<vector<float>>>& eff_p);

//根据当前状态信息，在不考虑随机动态事件的干扰下计算生产完成时间。
int partial_span_static(const int current_time, const int stage_ind, const int num_stage, const vector<int>& m_per_stage, const vector<vector<int>>& p_time, vector<vector<array<int,4>>> mstate, vector<list<int>> ques, vector<array<int,3>> dispatch, const vector<vector<vector<float>>>& br_p, const vector<vector<vector<float>>>& eff_p);

//打印函数
void print_1d_vector(const vector<int>& );
void print_1d_list(const list<int>&);
void print_2d_vector(const vector<vector<int>>&);
void print_list_of_vec(const list<vector<int>>&);
void print_list_of_vec_1line(const list<vector<int>>&);
void print_string(const string& str);

//根据数组 or 向量的最后一个元素的大小来判断两个数组 or 向量的大小
inline bool greater_iarray2(const array<int,2>& a, const array<int,2>& b){
    return a[1] > b[1];
}

inline bool less_iarray2(const array<int,2>& a, const array<int,2>& b){
    return a[1] < b[1];
}

inline bool greater_ivec(const vector<int>& a, const vector<int>& b){
    return a.back() > b.back();
}
inline bool less_ivec(const vector<int>& a, const vector<int>& b){
    return a.back() < b.back();
}
inline bool greatereq_ivec(const vector<int>& a, const vector<int>& b){
    // return a.back() >= b.back();
    return (a.back() > b.back()) || (a.back() == b.back());
}
inline bool lesseq_ivec(const vector<int>& a, const vector<int>& b){
    return a.back() <= b.back();
}

inline bool greater_fvec(const vector<float>& a, const vector<float>& b){
    return a.back() > b.back();
}
inline bool less_fvec(const vector<float>& a, const vector<float>& b){
    return a.back() < b.back();
}
inline bool greatereq_fvec(const vector<float>& a, const vector<float>& b){
    return a.back() >= b.back();
}
inline bool lesseq_fvec(const vector<float>& a, const vector<float>& b){
    return a.back() <= b.back();
}


//比较几种特殊结构的数据是否相同的布尔函数
bool compare_vv(const vector<vector<int>>& a, const vector<vector<int>>& b);
bool compare_vl(const vector<list<int>>& a, const vector<list<int>>& b);
bool compare_va3(const vector<array<int,3>> &a, const vector<array<int, 3>> &b);
bool compare_vvv(const vector<vector<vector<int>>>& a, const vector<vector<vector<int>>>& b);
bool compare_vva4(const vector<vector<array<int,4>>>& a, const vector<vector<array<int,4>>>& b);
bool compare_mia3(const map<int,array<int,3>> &a, const map<int, array<int,3>> &b);
bool compare_mia2(const map<int,array<int,2>> &a, const map<int, array<int,2>> &b);

//矩阵求和函数（两种不同数值类型的重载）
unsigned sum_matrix(const vector<vector<int>> mat);
unsigned sum_matrix(const vector<vector<unsigned>> mat);

//两种轮盘赌选择函数，一种按照整数数组和分量的比例选择，第二种按照输入的随机向量的概率选择
int roulette_from_ivec(const vector<int>& ivec);
int roulette_from_pvec(const vector<float>& fvec);

//交换整数列表 q 中 i1 和 i2 位置的两个元素
void swap_list(list<int>& q, const int i1, const int i2);

//将 min(i1,i2) 位置的元素插入到 max(i1,i2) 位置的后面
void forward_insert_list(list<int> &q, const int i1, const int i2);
//将 max(i1,i2) 位置的元素插入到 min(i1,i2) 位置的前面
void backward_insert_list(list<int> &q, const int i1, const int i2);

//随机生成一个长度为 num 的置换列表
list<int> rand_range_list(const int num, const unsigned seed);

//随机打乱一个列表
void shuffle_list(list<int>& a, const unsigned seed);


#endif