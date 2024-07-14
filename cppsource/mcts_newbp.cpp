#include"mcts_newbp.h"

// void BackProp_fsp_new(NodeFSP& front, const double reward){
//     // firstly update the reward value of front
//     NodeFSP* cur = &front;
//     cur->visits += 1;
//     cur->reward += ((reward-cur->reward)/static_cast<double>(cur->visits));

//     // recursively update the reward value of parents 
//     NodeFSP* cur_parent = cur->parent;
//     int parent_turn = cur_parent->state.get_turn();
//     while(cur_parent != nullptr){
//         cur->visits += 1;
//         if((parent_turn==1)||(cur_parent->visits<1)){
//             if(cur->reward < cur_parent->reward){
//                 cur_parent->old_reward = cur_parent->reward;
//                 cur_parent->reward_changed = true;
//                 cur_parent->reward = cur->reward;
//                 cur_parent->visits += 1;
//             }
//             else{
//                 cur_parent->visits += 1;
//             }
//             cur->reward_changed = false;
//         }
//         else{
//             if(cur->reward_changed){
//                 cur->reward_changed = false;
//                 cur_parent->reward_changed = true;
//                 cur_parent->visits += 1;
//                 cur_parent->old_reward = cur_parent->reward;
//                 // cur_parent->reward += ((cur->reward - cur->old_reward)*static_cast<double>(cur->visits) + cur->old_reward - cur_parent->old_reward)/static_cast<double>(cur_parent->visits);
//                 cur_parent->reward += ((cur->reward - cur->old_reward)*static_cast<double>(cur->visits-1)*0.5 + cur->reward - cur_parent->old_reward)/static_cast<double>(cur_parent->visits);
//             }
//             else{
//                 cur_parent->visits += 1;
//                 cur_parent->reward += ((cur->reward - cur_parent->reward)/static_cast<double>(cur_parent->visits));
//             }
//         }
//         cur = cur_parent;
//         cur_parent = cur->parent;
//     }
//     return ;
// }


void BackProp_fsp1_new(NodeFSP1& front, const double reward, const double alpha){
    // firstly update the reward value of front
    NodeFSP1* cur = &front;
    cur->visits += 1;
    cur->reward += ((reward-cur->reward)/static_cast<double>(cur->visits));

    // recursively update the reward value of parents 
    NodeFSP1* cur_parent = cur->parent;
    int parent_turn = cur_parent->state.get_turn();
    while(cur_parent != nullptr){
        cur->visits += 1;
        if((parent_turn==1)||(cur_parent->visits<1)){
            if(cur->reward < cur_parent->reward){
                cur_parent->old_reward = cur_parent->reward;
                cur_parent->reward_changed = true;
                // cur_parent->reward = cur->reward;
                cur_parent->reward -= (cur_parent->reward - cur->reward)*alpha;
                cur_parent->visits += 1;
            }
            else{
                cur_parent->visits += 1;
            }
            cur->reward_changed = false;
        }
        else{
            if(cur->reward_changed){
                cur->reward_changed = false;
                cur_parent->reward_changed = true;
                cur_parent->visits += 1;
                cur_parent->old_reward = cur_parent->reward;
                // cur_parent->reward += ((cur->reward - cur->old_reward)*static_cast<double>(cur->visits) + cur->old_reward - cur_parent->old_reward)/static_cast<double>(cur_parent->visits);
                // cur_parent->reward += ((cur->reward - cur->old_reward)*static_cast<double>(cur->visits-1)*0.5 + cur->reward - cur_parent->old_reward)/static_cast<double>(cur_parent->visits);
                cur_parent->reward += ((cur->reward - cur->old_reward)*static_cast<double>(cur->visits-1)*alpha + cur->reward - cur_parent->old_reward)/static_cast<double>(cur_parent->visits);
            }
            else{
                cur_parent->visits += 1;
                cur_parent->reward += ((cur->reward - cur_parent->reward)/static_cast<double>(cur_parent->visits));
            }
        }
        cur = cur_parent;
        cur_parent = cur->parent;
    }
    return ;
}


// NodeFSP new_UCT_horizon_time_fsp(NodeFSP &root, const double scalar, const double time_budget, const int horizon, const unsigned seed, const int repeat){
//     mt19937 gen{seed};
//     auto start = chrono::system_clock::now();
//     auto end = chrono::system_clock::now();
//     chrono::duration<double> diff = end-start;
//     while(diff.count()<time_budget){
//         NodeFSP* front = SimTree_horizon_fsp(root, scalar, horizon, gen());
//         double r = SimDefault_horizon_fsp(front->state, horizon, root, gen(), repeat);
//         BackProp_fsp_new(*front, r);
//         end = chrono::system_clock::now();
//         diff = end-start;
//     }
//     NodeFSP bestnode = root.children.front();
//     double bestscore = root.children.front().reward;
//     for(auto &c:root.children){
//         if(c.reward<bestscore){
//             bestnode = c;
//         }
//     }
//     return bestnode;   
// }

NodeFSP1 new_UCT_horizon_iter_fsp1(const vector<vector<int>>& ptime, NodeFSP1 &root, const double scalar, const int iter_budget, const int horizon, const unsigned seed, const int repeat, const int reward_version, const int return_version, const double alpha){
    mt19937 gen{seed};
    int count = 0;
    while(count < iter_budget){
        NodeFSP1* front = SimTree_horizon_fsp1(ptime, root, scalar, horizon, gen());
        double r = SimDefault_horizon_fsp1(ptime, front->state, horizon, root, gen(), reward_version, repeat);
        BackProp_fsp1_new(*front, r, alpha);
        ++count;
    }
    NodeFSP1 bestnode = root.children.front();
    double bestscore = root.children.front().reward;
    for(auto &c:root.children){
        if(c.reward<bestscore){
            bestnode = c;
        }
    }
    return bestnode;
    if(return_version==0){
        NodeFSP1 bestnode = root.children.front();
        double bestscore = root.children.front().reward;
        for(auto &c:root.children){
            if(c.reward<bestscore){
                bestnode = c;
            }
        }
        return bestnode;
    }
    else if(return_version==1){
        return BestChild_fsp1(root, scalar);
    }
    else{
        return MostVisitedChild_fsp1(root);
    }
}