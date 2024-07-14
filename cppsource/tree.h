#ifndef MY_TREE
#define MY_TREE

#include"dynamic_workshop.h"

/*
class NodeFSP{
public:
// private:
    unsigned visits;
    double reward;
    DFSP state;
    list<NodeFSP> children;
    NodeFSP* parent;
    unsigned exp_num_of_children;
    bool fully_expanded;
    double old_reward = 0;
    bool reward_changed = false;

// public:
    NodeFSP(){
        this->visits = -1;
        this->reward = 0.0;
        this->state = DFSP();
        this->children = list<NodeFSP>();
        this->parent = nullptr;
        this->exp_num_of_children = 0;
        this->fully_expanded = false;
    }

    NodeFSP(const DFSP& ins){
        this->visits = 0;
        this->reward = 0.0;
        this->state = ins;
        this->children = list<NodeFSP>();
        this->parent = nullptr;
        this->exp_num_of_children = 0;
        this->fully_expanded = false;
    }
    NodeFSP(const DFSP* ins, NodeFSP* p){
        this->visits = 0;
        this->reward = 0.0;
        this->state = *ins;
        this->children = list<NodeFSP>();
        this->parent = p;
        this->exp_num_of_children = 0;
        this->fully_expanded = false;
    }

    // NodeFSP generate_child(int horizon);

    void add_child(const DFSP& ins);
    void kidnap_child(NodeFSP& child);

    bool operator == (const NodeFSP& other){
        return (this->state == other.state);
    }
};

void node_merge(NodeFSP& node1, NodeFSP& node2);

// NodeFSP* list_merge(list<NodeFSP>& l);
void list_merge(list<NodeFSP>& l);
void list_merge_single(list<NodeFSP>& l);

inline bool better_node(NodeFSP& n1, NodeFSP& n2){
    return n1.reward < n2.reward;
}
*/

class NodeFSP1{
public:
    unsigned visits;  //节点访问次数
    double reward;  //节点奖励值
    DFSP1 state;  //对应的车间状态
    list<NodeFSP1> children;  //子节点列表
    NodeFSP1* parent;  //指向亲节点的指针
    unsigned exp_num_of_children;  //期望子节点数量（用于限制节点展开的尝试次数）
    bool fully_expanded;  //节点是否完全展开的指示变量
    double old_reward = 0;  //上次反向传播更新前的旧奖励值（用于修改了反向传播规则的 mcts_newbp 算法）
    bool reward_changed = false;  //用于修改了反向传播规则的 mcts_newbp 算法，如果一个重调度节点的奖励值被一个新的最优子节点刷新，则记为true

    //创建一个空节点
    NodeFSP1(){
        this->visits = -1;
        this->reward = 0.0;
        this->state = DFSP1();
        this->children = list<NodeFSP1>();
        this->parent = nullptr;
        this->exp_num_of_children = 0;
        this->fully_expanded = false;
    }
    //创建一个状态为 ins 的节点
    NodeFSP1(const DFSP1& ins){
        this->visits = 0;
        this->reward = 0.0;
        this->state = ins;
        this->children = list<NodeFSP1>();
        this->parent = nullptr;
        this->exp_num_of_children = 0;
        this->fully_expanded = false;
    }
    //创建一个父节点为 p 的节点
    NodeFSP1(const DFSP1* ins, NodeFSP1* p){
        this->visits = 0;
        this->reward = 0.0;
        this->state = *ins;
        this->children = list<NodeFSP1>();
        this->parent = p;
        this->exp_num_of_children = 0;
        this->fully_expanded = false;
    }

    //状态为 ins 的车间状态加入到子节点列表中
    void add_child(const DFSP1& ins);

    //将另一个节点的子节点抢过来（用于并行化的 MCTS)
    void kidnap_child(NodeFSP1& child);

    //判断两个节点是否相同
    bool operator == (const NodeFSP1& other){
        return (this->state == other.state);
    }
};

// 根据奖励值的高低比较两个节点
inline bool better_node1(NodeFSP1& n1, NodeFSP1& n2){
    return n1.reward < n2.reward;
}

//下述函数均用于并行化的 MCTS 算法
//融合两个节点，（用于并行化的 MCTS)
void node_merge1(NodeFSP1& node1, NodeFSP1& node2);

//融合列表中的所有节点（并行版）
void list_merge1(list<NodeFSP1>& l);

//融合列表中的所有节点（单线程版）
void list_merge_single1(list<NodeFSP1>& l);

#endif