#include "tree.h"

/*
// NodeFSP NodeFSP::generate_child(int horizon){
//     if(this->state.turn==1){
//         return NodeFSP(this->state.resched());
//     }
//     // else if(this->state.turn==1){
//     else{
//         return NodeFSP(this->state.simul_run(horizon));
//     }
//     // else{
//     //     cerr<<"Function generate_child in class NodeFSP sees error"<<endl;
//     // }
// }


void NodeFSP::add_child(const DFSP& ins){
    NodeFSP child = NodeFSP(ins);
    child.parent = this;
    this->children.emplace_back(child);
    // auto it = children.end();
    // (*it).parent = this;
}
void NodeFSP::kidnap_child(NodeFSP& child){
    child.parent = this;
    this->visits += child.visits;
    this->children.emplace_back(child);
}

void node_merge(NodeFSP& node1, NodeFSP& node2){
    for(auto it2=node2.children.begin(); it2!=node2.children.end(); ++it2){
        auto it_find = find(node1.children.begin(), node1.children.end(), *it2);
        if(it_find == node1.children.end()){
            node1.kidnap_child(*it2);
        }
        else{
            node1.visits += it2->visits;
            it_find->visits += it2->visits;
            it_find->reward = ((it_find->reward)*double(it_find->visits) + (it2->reward)*double(it2->visits))/double(it_find->visits);
        }
    }
    return;
}

// NodeFSP node_merge_beta(NodeFSP node1, NodeFSP node2){
//     if(node1.fully_expanded || node2.fully_expanded){node1.fully_expanded = true;}
//     for(auto it2 = node2.children.begin(); it2!=node2.children.end(); ++it2){
//         auto it_find = find(node1.children.begin(), node1.children.end(), *it2);
//         if(it_find == node1.children.end()){
//             node1.kidnap_child(*it2);
//         }
//         else{
//             it_find->visits += it2->visits;
//         }
//     }
// }

// int node_merge(NodeFSP* node1, NodeFSP* node2){
// // int tree_emerge(NodeFSP node1, NodeFSP node2){
//     if(node1->fully_expanded || node2->fully_expanded) node1->fully_expanded = true;
//     for(auto it2=node2->children.begin(); it2!=node2->children.end(); ++it2){
//         auto it_find = find(node1->children.begin(), node1->children.end(), *it2);
//         if(it_find == node1->children.end()){
//             node1->kidnap_child(*it2);
//         }
//         else{
//             it_find->visits += it2->visits;
//             it_find->reward = ((it_find->reward)*double(it_find->visits) + (it2->reward)*double(it2->visits))/double(it_find->visits);
//         }
//     }
//     return 0;
// }

// NodeFSP* list_merge(list<NodeFSP>& l){
void list_merge(list<NodeFSP>& l){
    unsigned len = l.size();

    while(len>3){
        unsigned pairs = len/2;
        vector<thread> threads;
        auto it1 = l.begin(); auto it2 = l.begin(); ++it2;
        for(unsigned i=0; i<pairs; ++i){
        // for(unsigned i=0; i<1; ++i){
            // thread temp(node_merge, ref(*it1), ref(*it2));
            threads.emplace_back(thread(node_merge, ref(*it1), ref(*it2)));
            // futures[i] = async(launch::async, node_merge, *it1, *it2);
            ++it1; ++it1;
            ++it2; ++it2;
        }
        for(auto &i:threads){
            i.join();
        }
        auto it = l.begin();
        while(it!=l.end()){
            ++it;
            it = l.erase(it);
        }
        len = l.size();
    }
    if(len==3){
        auto it1 = l.begin(); auto it2 = l.begin(); ++it2;
        node_merge(*it1, *it2);
        it2 = l.erase(it2);
        node_merge(*it1, *it2);
        l.erase(it2);
        // return l.front();
        // return &(*it1);
        return;
    }
    if(len==2){
        auto it1 = l.begin(); auto it2 = l.begin(); ++it2;
        node_merge(*it1, *it2);
        l.erase(it2);
        // return l.front();
        // return &(*it1);
        return;
    }
    if(len==1){
        // return l.front();
        return ;
    }
}

void list_merge_single(list<NodeFSP>& l){
    int len = l.size();
    if(len==1){return;}
    auto it1 = l.begin();
    auto it2 = l.begin();
    ++it2;
    while(it2!=l.end()){
        node_merge(*it1, *it2);
        it2 = l.erase(it2);
    }
    return;
}
*/

void NodeFSP1::add_child(const DFSP1& ins){
    NodeFSP1 child = NodeFSP1(ins);
    child.parent = this;
    this->children.emplace_back(child);
}

void NodeFSP1::kidnap_child(NodeFSP1& child){
    child.parent = this;
    this->visits += child.visits;
    this->children.emplace_back(child);
}

//融合两个节点 node1 和 node2，将两者的奖励值根据访问次数加权平均，并将子节点列表合并
void node_merge1(NodeFSP1& node1, NodeFSP1& node2){
    for(auto it2=node2.children.begin(); it2!=node2.children.end(); ++it2){
        auto it_find = find(node1.children.begin(), node1.children.end(), *it2);
        if(it_find == node1.children.end()){
            node1.kidnap_child(*it2);
        }
        else{
            node1.visits += it2->visits;
            it_find->visits += it2->visits;
            it_find->reward = ((it_find->reward)*double(it_find->visits) + (it2->reward)*double(it2->visits))/double(it_find->visits);
        }
    }
    return;
}

//将列表中的节点多线程两两合并，重复直至仅剩一个节点
void list_merge1(list<NodeFSP1>& l){
    unsigned len = l.size();

    while(len>3){
        unsigned pairs = len/2;
        vector<thread> threads;
        auto it1 = l.begin(); auto it2 = l.begin(); ++it2;
        for(unsigned i=0; i<pairs; ++i){
            threads.emplace_back(thread(node_merge1, ref(*it1), ref(*it2)));
            ++it1; ++it1;
            ++it2; ++it2;
        }
        for(auto &i:threads){
            i.join();
        }
        auto it = l.begin();
        while(it!=l.end()){
            ++it;
            it = l.erase(it);
        }
        len = l.size();
    }
    if(len==3){
        auto it1 = l.begin(); auto it2 = l.begin(); ++it2;
        node_merge1(*it1, *it2);
        it2 = l.erase(it2);
        node_merge1(*it1, *it2);
        l.erase(it2);
        return;
    }
    if(len==2){
        auto it1 = l.begin(); auto it2 = l.begin(); ++it2;
        node_merge1(*it1, *it2);
        l.erase(it2);
        return;
    }
    if(len==1){
        return ;
    }
}

//从列表中的第二个节点开始，依次将其融合到第一个节点中。
void list_merge_single1(list<NodeFSP1>& l){
    int len = l.size();
    if(len==1){return;}
    auto it1 = l.begin();
    auto it2 = l.begin();
    ++it2;
    while(it2!=l.end()){
        node_merge1(*it1, *it2);
        it2 = l.erase(it2);
    }
    return;
}