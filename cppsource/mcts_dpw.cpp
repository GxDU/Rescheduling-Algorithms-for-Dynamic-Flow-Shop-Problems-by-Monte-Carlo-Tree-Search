#include "mcts_dpw.h"

// NodeFSP UCT_with_PW_LS(NodeFSP root, const double time_budget, const double scalar, const float k, const float alpha, const float ratio, const int horizon, const unsigned seed, const int repeat){
//     mt19937 gen{seed};
//     auto start = chrono::system_clock::now();
//     auto end = chrono::system_clock::now();
//     chrono::duration<double> diff = end-start;

//     while(diff.count()<time_budget){
//         NodeFSP* front = &root;
//         while((front->state.get_turn() >= 0) && (front->visits > 0)){
//             front = PW_LS(*front, scalar, horizon, k, alpha, ratio, gen());
//         }
//         double r = SimDefault_horizon_fsp(front->state, horizon, root, gen(), repeat);
//         BackProp_fsp(*front, r);
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

// NodeFSP UCT_with_DPW(NodeFSP root, const double time_budget, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const float ratio, const int horizon, const unsigned seed, const int repeat){
//     mt19937 gen{seed};
//     auto start = chrono::system_clock::now();
//     auto end = chrono::system_clock::now();
//     chrono::duration<double> diff = end-start;

//     while(diff.count()<time_budget){
//         NodeFSP* front = &root;
//         while((front->state.get_turn() >= 0) && (front->visits > 0)){
//             front = DPW(*front, scalar, horizon, k1, alpha1, k2, alpha2, ratio, gen());
//         }
//         double r = SimDefault_horizon_fsp(front->state, horizon, root, gen(), repeat);
//         BackProp_fsp(*front, r);
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

// NodeFSP UCT_with_PW(NodeFSP root, const double time_budget, const double scalar, const float k, const float alpha, const int horizon, const unsigned seed, const int repeat){
//     mt19937 gen{seed};
//     auto start = chrono::system_clock::now();
//     auto end = chrono::system_clock::now();
//     chrono::duration<double> diff = end-start;

//     while(diff.count()<time_budget){
//         NodeFSP* front = &root;
//         while((front->state.get_turn() >= 0) && (front->visits > 0)){
//             front = PW(*front, scalar, horizon, k, alpha, gen());
//         }
//         double r = SimDefault_horizon_fsp(front->state, horizon, root, gen(), repeat);
//         BackProp_fsp(*front, r);
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

// NodeFSP* PW_LS(NodeFSP& parent, const double scalar, const int horizon, const float k, const float alpha, const float ratio, const unsigned seed){
//     // float k = 20;
//     // float alpha = 0.2;
//     mt19937 gen{seed};
//     NodeFSP* child1;
//     if(parent.children.size()==0){
//         parent.add_child(parent.state.resched1(gen()));
//         // parent.add_child(parent.state.resched1_plus_insert(gen()));
//         child1 = &(parent.children.back());
//     }
//     else if((!parent.fully_expanded) && (parent.children.size()<(unsigned)(k*pow(static_cast<float>(parent.visits), alpha)))){
//         // parent.children.sort(better_node);
//         if(parent.visits%20 == 10){
//             parent.children.sort(better_node);
//         }
//         child1 = Get_Resched_LS(parent, gen(), scalar, ratio);
//         // child1 = Get_Resched(parent, gen(), scalar);
//     }
//     else{
//         child1 = Epsilon_Greedy_UCT(parent, scalar, gen());
//     }

//     //Expand child1
//     NodeFSP newnode = NodeFSP(child1->state.simul_run(horizon, gen(), gen(), gen()));
//     auto it = find(child1->children.begin(), child1->children.end(), newnode);
//     if(it == child1->children.end()){
//         child1->add_child(newnode.state);
//         return &(child1->children.back());
//     }
//     else{
//         return &(*it);
//     }
// }

// NodeFSP* PW(NodeFSP& parent, const double scalar, const int horizon, const float k, const float alpha, const unsigned seed){
//     // float k = 20;
//     // float alpha = 0.2;
//     mt19937 gen{seed};
//     NodeFSP* child1;
//     if(parent.children.size()==0){
//         parent.add_child(parent.state.resched1(gen()));
//         child1 = &(parent.children.back());
//     }
//     else if((!parent.fully_expanded) && (parent.children.size()<(unsigned)(k*pow(static_cast<float>(parent.visits), alpha)))){
//         child1 = Get_Resched(parent, gen(), scalar);
//     }
//     else{
//         child1 = Epsilon_Greedy_UCT(parent, scalar, gen());
//     }

//     //Expand child1
//     NodeFSP newnode = NodeFSP(child1->state.simul_run(horizon, gen(), gen(), gen()));
//     auto it = find(child1->children.begin(), child1->children.end(), newnode);
//     if(it == child1->children.end()){
//         child1->add_child(newnode.state);
//         return &(child1->children.back());
//     }
//     else{
//         return &(*it);
//     }
// }

// NodeFSP* DPW(NodeFSP& parent, const double scalar, const int horizon, const float k1, const float alpha1, const float k2, const float alpha2, const float ratio, const unsigned seed){
//     mt19937 gen{seed};
//     uniform_real_distribution<float> dist01;
//     NodeFSP* child1;
//     if(parent.children.size()==0){
//         parent.add_child(parent.state.resched1(gen()));
//         child1 = &(parent.children.back());
//     }
//     else if((!parent.fully_expanded) && (parent.children.size()<(unsigned)(k1*pow(static_cast<float>(parent.visits), alpha1)))){
//         // child1 = Get_Resched(parent, gen(), scalar);
//         parent.children.sort(better_node);
//         child1 = Get_Resched_LS(parent, gen(), scalar, ratio);
 
//     }
//     else{
//         child1 = Epsilon_Greedy_UCT(parent, scalar, gen());
//     }

//     //Expand child1
//     if(child1->children.size()==0){
//         NodeFSP newnode = NodeFSP(child1->state.simul_run(horizon, gen(),gen(), gen()));
//         child1->add_child(newnode.state);
//         return &(child1->children.back());
//     }
//     if((child1->children.size()>0) && (!child1->fully_expanded) && (child1->children.size()<(unsigned)(k2*pow(static_cast<float>(child1->visits), alpha2)))){
//         // NodeFSP newnode = NodeFSP(child1->state.simul_run(horizon, gen(), gen(), gen()));
//         // auto it = find(child1->children.begin(), child1->children.end(), newnode);
//         // if(it==child1->children.end()){
//         //     child1->add_child(newnode.state);
//         //     return &(child1->children.back());
//         // }
//         // else{
//         //     return &(*it);
//         // }
//         NodeFSP newnode;
//         for(int k=0; k<20; ++k){
//             newnode = NodeFSP(child1->state.simul_run(horizon, gen(), gen(), gen()));
//             auto it = find(child1->children.begin(), child1->children.end(), newnode);
//             if(it==child1->children.end()){
//                 child1->add_child(newnode.state);
//                 return &(child1->children.back());
//             }
//         }
//         child1->fully_expanded = true;
//     }
//     float rand = dist01(gen);
//     bool foo = false;
//     float temp = 0;
//     for(auto &c:child1->children){
//         temp += static_cast<float>(c.visits) / static_cast<float>(child1->visits);
//         if(rand <= temp){
//             return &c;
//         }
//     }
//     return &(child1->children.back());
//     // else{
//     //     float rand = dist01(gen);
//     //     bool foo = false;
//     //     float temp = 0;
//     //     for(auto &c:child1->children){
//     //         temp += static_cast<float>(c.visits) / static_cast<float>(child1->visits);
//     //         if(rand <= temp){
//     //             return &c;
//     //         }
//     //     }
//     //     return &(child1->children.back());
//     // }
// }

// NodeFSP* Get_Resched(NodeFSP& parent, const unsigned seed, const double scalar){
//     mt19937 gen{seed};
//     mt19937 gen1{gen()};
//     uniform_real_distribution<float> dist01(0, 1);
//     // float u = dist01(gen);
//     bool new_action_found = false;
//     if(parent.visits<100){
//         NodeFSP new_node = NodeFSP(parent.state.resched1(gen()));
//         auto it = find(parent.children.begin(), parent.children.end(), new_node);
//         if(it==parent.children.end()){
//             parent.add_child(new_node.state);
//             return &(parent.children.back());
//         }
//         else{
//             return &(*it);
//         }
//     }
//     int num_child = parent.children.size();
//     for(int k=0; k<20; ++k){
//         // int child_ind = gen()%num_child;
//         // auto child_iter = parent.children.begin();
//     //     for(int foo=0; foo<child_ind; ++foo){++child_iter;}
//     //     vector<list<int>> que_copy((*child_iter).state.Q);
//     //     for(int i=0; i<parent.state.num_of_stage; ++i){
//     //         if(parent.state.reschedule_list[i].size()>1){
//     //             int len = que_copy[i].size();
//     //             int ind = gen()%len;
//     //             auto job_iter1 = que_copy[i].begin();
//     //             for(int foo=0; foo<ind; ++foo){++job_iter1;}
//     //             int job_ind = *job_iter1;
//     //             que_copy[i].erase(job_iter1);
//     //             int insert_pos = gen()%len;
//     //             while(insert_pos==ind){
//     //                 insert_pos = gen()%len;
//     //             }
//     //             job_iter1 = que_copy[i].begin();
//     //             for(int foo=0; foo<insert_pos; ++foo){++job_iter1;}
//     //             que_copy[i].insert(job_iter1, job_ind);
//     //         }
//     //     }
//     //     NodeFSP new_node = NodeFSP(DFSP(&(parent.state), que_copy));

//     //     // NodeFSP new_node = NodeFSP(parent.state.resched_rand_insert(gen()));
//     //     auto it = find(parent.children.begin(), parent.children.end(), new_node);
//     //     if(it==parent.children.end()){
//     //         parent.add_child(new_node.state);
//     //         return &(parent.children.back());
//     //     }

//         float u = dist01(gen1);
//         // if(u<0.45){
//         //     //randomly pick one child from parent.children
//         //     int child_ind = gen()%num_child;
//         //     auto child_iter = parent.children.begin();
//         //     for(int foo=0; foo<child_ind; ++foo){++child_iter;}
//         //     vector<list<int>> que_copy((*child_iter).state.Q);
//         //     for(int i=0; i<parent.state.num_of_stage; ++i){
//         //         if(parent.state.reschedule_list[i].size()>1){
//         //             int len = que_copy[i].size();
//         //             int ind = gen()%len;
//         //             auto job_iter1 = que_copy[i].begin();
//         //             for(int foo=0; foo<ind; ++foo){++job_iter1;}
//         //             int job_ind = *job_iter1;
//         //             que_copy[i].erase(job_iter1);
//         //             int insert_pos = gen()%len;
//         //             while(insert_pos==ind){
//         //                 insert_pos = gen()%len;
//         //             }
//         //             job_iter1 = que_copy[i].begin();
//         //             for(int foo=0; foo<insert_pos; ++foo){++job_iter1;}
//         //             que_copy[i].insert(job_iter1, job_ind);
//         //         }
//         //     }
//         //     NodeFSP new_node = NodeFSP(DFSP(&(parent.state), que_copy));

//         //     // NodeFSP new_node = NodeFSP(parent.state.resched_rand_insert(gen()));
//         //     auto it = find(parent.children.begin(), parent.children.end(), new_node);
//         //     if(it==parent.children.end()){
//         //         parent.add_child(new_node.state);
//         //         return &(parent.children.back());
//         //     }
//         // }
//         // else if(u<0.9){
//         //     int child_ind = gen()%num_child;
//         //     auto child_iter = parent.children.begin();
//         //     for(int foo=0; foo<child_ind; ++foo){++child_iter;}
//         //     vector<list<int>> que_copy((*child_iter).state.Q);
//         //     for(int i=0; i<parent.state.num_of_stage; ++i){
//         //         if(parent.state.reschedule_list[i].size()>1){
//         //             int len = que_copy[i].size();
//         //             int ind1 = gen()%len;
//         //             int ind2 = gen()%len;
//         //             while(ind2==ind1){ind2 = gen()%len;}
//         //             auto job_iter1 = que_copy[i].begin();
//         //             auto job_iter2 = que_copy[i].begin();
//         //             for(int foo=0; foo<ind1; ++foo){++job_iter1;}
//         //             for(int foo=0; foo<ind2; ++foo){++job_iter2;}
//         //             int temp = *job_iter1;
//         //             *job_iter1 = *job_iter2;
//         //             *job_iter2 = temp;
//         //         }
//         //     }
//         //     NodeFSP new_node = NodeFSP(DFSP(&(parent.state), que_copy));

//         //     // NodeFSP new_node = NodeFSP(parent.state.resched_rand_insert(gen()));
//         //     auto it = find(parent.children.begin(), parent.children.end(), new_node);
//         //     if(it==parent.children.end()){
//         //         parent.add_child(new_node.state);
//         //         return &(parent.children.back());
//         //     }
//         // }
//         if(u<0.8){
//             //randomly pick one child from parent.children
//             int child_ind = gen()%num_child;
//             auto child_iter = parent.children.begin();
//             for(int foo=0; foo<child_ind; ++foo){++child_iter;}
//             // vector<list<int>> que_copy((*child_iter).state.Q);
//             vector<list<int>> que_copy((*child_iter).state.get_q());
//             int num_of_stage = parent.state.get_num_stage();
//             vector<list<int>> resched_list(parent.state.get_reschedlist());
//             // for(int i=0; i<parent.state.num_of_stage; ++i){
//             for(int i=0; i<num_of_stage; ++i){
//                 // if(parent.state.reschedule_list[i].size()>1){
//                 if(resched_list[i].size()>1){
//                     int len = que_copy[i].size();
//                     int ind = gen()%len;
//                     auto job_iter1 = que_copy[i].begin();
//                     for(int foo=0; foo<ind; ++foo){++job_iter1;}
//                     int job_ind = *job_iter1;
//                     que_copy[i].erase(job_iter1);
//                     int insert_pos = gen()%len;
//                     while(insert_pos==ind){
//                         insert_pos = gen()%len;
//                     }
//                     job_iter1 = que_copy[i].begin();
//                     for(int foo=0; foo<insert_pos; ++foo){++job_iter1;}
//                     que_copy[i].insert(job_iter1, job_ind);
//                 }
//             }
//             NodeFSP new_node = NodeFSP(DFSP(&(parent.state), que_copy));

//             // NodeFSP new_node = NodeFSP(parent.state.resched_rand_insert(gen()));
//             auto it = find(parent.children.begin(), parent.children.end(), new_node);
//             if(it==parent.children.end()){
//                 parent.add_child(new_node.state);
//                 return &(parent.children.back());
//             }
//         }
//         else{
//             NodeFSP new_node = NodeFSP(parent.state.resched1(gen()));
//             auto it = find(parent.children.begin(), parent.children.end(), new_node);
//             if(it==parent.children.end()){
//                 parent.add_child(new_node.state);
//                 return &(parent.children.back());
//             }
//         }
//     }
//     parent.fully_expanded = true;
//     return Epsilon_Greedy_UCT(parent, scalar, gen());
// }

// NodeFSP* Get_Resched_LS(NodeFSP& parent, const unsigned seed, const double scalar, const float ratio){
//     mt19937 gen{seed};
//     mt19937 gen1{gen()};
//     uniform_real_distribution<float> dist01(0, 1);
//     // float u = dist01(gen);
//     bool new_action_found = false;
//     // if(parent.visits<100){ 
//     if(parent.visits<50){ 
//         NodeFSP new_node = NodeFSP(parent.state.resched1(gen()));
//         auto it = find(parent.children.begin(), parent.children.end(), new_node);
//         if(it==parent.children.end()){
//             parent.add_child(new_node.state);
//             return &(parent.children.back());
//         }
//         else{
//             return &(*it);
//         }
//     }
//     int num_child = parent.children.size();
//     // int choose_range = min(num_child, 10);
//     int choose_range = static_cast<int>(static_cast<float>(num_child) * ratio + 0.5) + 1;
//     choose_range = min(num_child, choose_range);
//     for(int k=0; k<20; ++k){
//         // int child_ind = gen()%num_child;
//         // auto child_iter = parent.children.begin();
//     //     for(int foo=0; foo<child_ind; ++foo){++child_iter;}
//     //     vector<list<int>> que_copy((*child_iter).state.Q);
//     //     for(int i=0; i<parent.state.num_of_stage; ++i){
//     //         if(parent.state.reschedule_list[i].size()>1){
//     //             int len = que_copy[i].size();
//     //             int ind = gen()%len;
//     //             auto job_iter1 = que_copy[i].begin();
//     //             for(int foo=0; foo<ind; ++foo){++job_iter1;}
//     //             int job_ind = *job_iter1;
//     //             que_copy[i].erase(job_iter1);
//     //             int insert_pos = gen()%len;
//     //             while(insert_pos==ind){
//     //                 insert_pos = gen()%len;
//     //             }
//     //             job_iter1 = que_copy[i].begin();
//     //             for(int foo=0; foo<insert_pos; ++foo){++job_iter1;}
//     //             que_copy[i].insert(job_iter1, job_ind);
//     //         }
//     //     }
//     //     NodeFSP new_node = NodeFSP(DFSP(&(parent.state), que_copy));

//     //     // NodeFSP new_node = NodeFSP(parent.state.resched_rand_insert(gen()));
//     //     auto it = find(parent.children.begin(), parent.children.end(), new_node);
//     //     if(it==parent.children.end()){
//     //         parent.add_child(new_node.state);
//     //         return &(parent.children.back());
//     //     }

//         float u = dist01(gen1);
//         // randomly choose one child from the choose range of parent.children 
//         // and then randomly insert one job into another position.
//         if(u<0.45){
//             //randomly pick one child from parent.children
//             // int child_ind = gen()%num_child;
//             int child_ind = gen()%choose_range;
//             auto child_iter = parent.children.begin();
//             for(int foo=0; foo<child_ind; ++foo){++child_iter;}
//             // vector<list<int>> que_copy((*child_iter).state.Q);
//             vector<list<int>> que_copy((*child_iter).state.get_q());
//             int num_of_stage = parent.state.get_num_stage();
//             vector<list<int>> resched_list(parent.state.get_reschedlist());
//             // for(int i=0; i<parent.state.num_of_stage; ++i){
//             for(int i=0; i<num_of_stage; ++i){
//                 // if(parent.state.reschedule_list[i].size()>1){
//                 if(resched_list[i].size()>1){
//                     int len = que_copy[i].size();
//                     int ind = gen()%len;
//                     auto job_iter1 = que_copy[i].begin();
//                     for(int foo=0; foo<ind; ++foo){++job_iter1;}
//                     int job_ind = *job_iter1;
//                     que_copy[i].erase(job_iter1);
//                     int insert_pos = gen()%len;
//                     while(insert_pos==ind){
//                         insert_pos = gen()%len;
//                     }
//                     job_iter1 = que_copy[i].begin();
//                     for(int foo=0; foo<insert_pos; ++foo){++job_iter1;}
//                     que_copy[i].insert(job_iter1, job_ind);
//                 }
//             }
//             NodeFSP new_node = NodeFSP(DFSP(&(parent.state), que_copy));

//             // NodeFSP new_node = NodeFSP(parent.state.resched_rand_insert(gen()));
//             auto it = find(parent.children.begin(), parent.children.end(), new_node);
//             if(it==parent.children.end()){
//                 parent.add_child(new_node.state);
//                 return &(parent.children.back());
//             }
//         }
//         // randomly choose one child from the choose range of parent.children 
//         // and then randomly swap positions of two jobs.
//         else if(u<0.9){
//             // int child_ind = gen()%num_child;
//             int child_ind = gen()%choose_range;
//             auto child_iter = parent.children.begin();
//             for(int foo=0; foo<child_ind; ++foo){++child_iter;}
//             // vector<list<int>> que_copy((*child_iter).state.Q);
//             vector<list<int>> que_copy((*child_iter).state.get_q());
//             int num_of_stage = parent.state.get_num_stage();
//             vector<list<int>> resched_list(parent.state.get_reschedlist());
//             // for(int i=0; i<parent.state.num_of_stage; ++i){
//             for(int i=0; i<num_of_stage; ++i){
//                 // if(parent.state.reschedule_list[i].size()>1){
//                 if(resched_list[i].size()>1){
//                     int len = que_copy[i].size();
//                     int ind1 = gen()%len;
//                     int ind2 = gen()%len;
//                     while(ind2==ind1){ind2 = gen()%len;}
//                     auto job_iter1 = que_copy[i].begin();
//                     auto job_iter2 = que_copy[i].begin();
//                     for(int foo=0; foo<ind1; ++foo){++job_iter1;}
//                     for(int foo=0; foo<ind2; ++foo){++job_iter2;}
//                     int temp = *job_iter1;
//                     *job_iter1 = *job_iter2;
//                     *job_iter2 = temp;
//                 }
//             }
//             NodeFSP new_node = NodeFSP(DFSP(&(parent.state), que_copy));

//             // NodeFSP new_node = NodeFSP(parent.state.resched_rand_insert(gen()));
//             auto it = find(parent.children.begin(), parent.children.end(), new_node);
//             if(it==parent.children.end()){
//                 parent.add_child(new_node.state);
//                 return &(parent.children.back());
//             }
//         }
//         // if(u<0.8){
//         //     //randomly pick one child from parent.children
//         //     // int child_ind = gen()%num_child;
//         //     int child_ind = gen()%choose_range;
//         //     auto child_iter = parent.children.begin();
//         //     for(int foo=0; foo<child_ind; ++foo){++child_iter;}
//         //     vector<list<int>> que_copy((*child_iter).state.Q);
//         //     for(int i=0; i<parent.state.num_of_stage; ++i){
//         //         if(parent.state.reschedule_list[i].size()>1){
//         //             int len = que_copy[i].size();
//         //             int ind = gen()%len;
//         //             auto job_iter1 = que_copy[i].begin();
//         //             for(int foo=0; foo<ind; ++foo){++job_iter1;}
//         //             int job_ind = *job_iter1;
//         //             que_copy[i].erase(job_iter1);
//         //             int insert_pos = gen()%len;
//         //             while(insert_pos==ind){
//         //                 insert_pos = gen()%len;
//         //             }
//         //             job_iter1 = que_copy[i].begin();
//         //             for(int foo=0; foo<insert_pos; ++foo){++job_iter1;}
//         //             que_copy[i].insert(job_iter1, job_ind);
//         //         }
//         //     }
//         //     NodeFSP new_node = NodeFSP(DFSP(&(parent.state), que_copy));

//         //     // NodeFSP new_node = NodeFSP(parent.state.resched_rand_insert(gen()));
//         //     auto it = find(parent.children.begin(), parent.children.end(), new_node);
//         //     if(it==parent.children.end()){
//         //         parent.add_child(new_node.state);
//         //         return &(parent.children.back());
//         //     }
//         // }
//         else{
//             // NodeFSP new_node = NodeFSP(parent.state.resched1(gen()));
//             NodeFSP new_node = NodeFSP(parent.state.resched1_plus_insert(gen()));
//             auto it = find(parent.children.begin(), parent.children.end(), new_node);
//             if(it==parent.children.end()){
//                 parent.add_child(new_node.state);
//                 return &(parent.children.back());
//             }
//         }
//     }
//     parent.fully_expanded = true;
//     return Epsilon_Greedy_UCT(parent, scalar, gen());
// }

// NodeFSP* Epsilon_Greedy_UCT(NodeFSP& parent, const double scalar, const unsigned seed){
//     mt19937 gen{seed};
//     uniform_real_distribution<float> dist01(0,1);
//     float u = dist01(gen);
//     if(u<0.1){
//         int len = parent.children.size();
//         auto it = parent.children.begin();
//         int ind = gen()%len;
//         for(int k=0; k<ind; ++k){++it;}
//         return &(*it);
//     }
//     else{
//         return BestChild_fsp_pointer(parent, scalar);
//     }
// }

NodeFSP1 UCT1_with_DPW_iter(NodeFSP1 root, const vector<vector<int>>& ptime, const int iter_budget, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const float ratio, const int horizon, const unsigned seed, const int return_version){
    mt19937 gen{seed};
    int count = 0;
    while(count<iter_budget){
        ++count;
        NodeFSP1* front = &root;
        while((front->state.get_turn() >= 0) && (front->visits > 0)){
            // front = DPW1(*front, scalar, horizon, k1, alpha1, k2, alpha2, ratio, gen());
            front = DPW1(*front, ptime, scalar, horizon, k1, alpha1, k2, alpha2, ratio, gen());
        }
        // double r = SimDefault_horizon_fsp1(front->state, horizon, root, gen(), repeat);
        double r = SimDefault_horizon_fsp1(ptime, front->state, horizon, root, gen(), 0, 10);
        BackProp_fsp1(*front, r);
    }
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
    else{
        return MostVisitedChild_fsp1(root);
    }
}

NodeFSP1 UCT1_with_DPW_time(NodeFSP1 root, const vector<vector<int>>& ptime, const double time_budget, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const float ratio, const int horizon, const unsigned seed, const int return_version){
    mt19937 gen{seed};
    auto start = chrono::system_clock::now();
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;

    while(diff.count()<time_budget){
        NodeFSP1* front = &root;
        while((front->state.get_turn() >= 0) && (front->visits > 0)){
            // front = DPW1(*front, scalar, horizon, k1, alpha1, k2, alpha2, ratio, gen());
            front = DPW1(*front, ptime, scalar, horizon, k1, alpha1, k2, alpha2, ratio, gen());
        }
        // double r = SimDefault_horizon_fsp1(front->state, horizon, root, gen(), repeat);
        double r = SimDefault_horizon_fsp1(ptime, front->state, horizon, root, gen(), 0, 10);
        BackProp_fsp1(*front, r);
        end = chrono::system_clock::now();
        diff = end-start;
    }
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
    // else if(return_version==1){
    //     return BestChild_fsp1(root, scalar);
    // }
    else{
        return MostVisitedChild_fsp1(root);
    }
}

NodeFSP1* DPW1(NodeFSP1& parent, const vector<vector<int>>& ptime, const double scalar, const int horizon, const float k1, const float alpha1, const float k2, const float alpha2, const float ratio, const unsigned seed){
    mt19937 gen{seed};
    uniform_real_distribution<float> dist01;
    NodeFSP1* child1;
    if(parent.children.size()==0){
        // parent.add_child(parent.state.resched1(gen()));
        parent.add_child(parent.state.resched_random_uct(gen(), ptime));
        child1 = &(parent.children.back());
    }
    else if((!parent.fully_expanded) && (parent.children.size()<(unsigned)(k1*pow(static_cast<float>(parent.visits), alpha1)))){
        // parent.children.sort(better_node1);
        if(parent.visits%10 == 9){
            parent.children.sort(better_node1);
        }
        // child1 = Get_Resched_LS1(parent, gen(), scalar, ratio);
        child1 = Get_Resched_LS1(parent, ptime, gen(), scalar, ratio);
 
    }
    else{
        child1 = Epsilon_Greedy_UCT1(parent, scalar, gen());
    }

    //Expand child1
    if(child1->children.size()==0){
        NodeFSP1 newnode = NodeFSP1(child1->state.simul_run(horizon, gen(),gen(), gen(), ptime));
        child1->add_child(newnode.state);
        return &(child1->children.back());
    }
    if((child1->children.size()>0) && (!child1->fully_expanded) && (child1->children.size()<(unsigned)(k2*pow(static_cast<float>(child1->visits), alpha2)))){
        NodeFSP1 newnode;
        // for(int k=0; k<20; ++k){
        for(int k=0; k<10; ++k){
            newnode = NodeFSP1(child1->state.simul_run(horizon, gen(), gen(), gen(), ptime));
            auto it = find(child1->children.begin(), child1->children.end(), newnode);
            if(it==child1->children.end()){
                child1->add_child(newnode.state);
                return &(child1->children.back());
            }
        }
        child1->fully_expanded = true;
    }
    float rand = dist01(gen);
    bool foo = false;
    float temp = 0;
    for(auto &c:child1->children){
        temp += static_cast<float>(c.visits) / static_cast<float>(child1->visits);
        if(rand <= temp){
            return &c;
        }
    }
    return &(child1->children.back());
    // else{
    //     float rand = dist01(gen);
    //     bool foo = false;
    //     float temp = 0;
    //     for(auto &c:child1->children){
    //         temp += static_cast<float>(c.visits) / static_cast<float>(child1->visits);
    //         if(rand <= temp){
    //             return &c;
    //         }
    //     }
    //     return &(child1->children.back());
    // }
}

NodeFSP1* Epsilon_Greedy_UCT1(NodeFSP1& parent, const double scalar, const unsigned seed){
    mt19937 gen{seed};
    uniform_real_distribution<float> dist01(0,1);
    float u = dist01(gen);
    // if(u<0.1){
    if(u<0.05){
        int len = parent.children.size();
        auto it = parent.children.begin();
        int ind = gen()%len;
        for(int k=0; k<ind; ++k){++it;}
        return &(*it);
    }
    else{
        // return BestChild_fsp_pointer(parent, scalar);
        return BestChild_fsp_pointer1(parent, scalar);
    }
}

NodeFSP1* Get_Resched_LS1(NodeFSP1& parent, const vector<vector<int>>& ptime, const unsigned seed, const double scalar, const float ratio){
    mt19937 gen{seed};
    mt19937 gen1{gen()};
    uniform_real_distribution<float> dist01(0, 1);
    // float u = dist01(gen);
    bool new_action_found = false;
    if(parent.visits<100){ 
        NodeFSP1 new_node = NodeFSP1(parent.state.resched_random_uct(gen(), ptime));
        auto it = find(parent.children.begin(), parent.children.end(), new_node);
        if(it==parent.children.end()){
            parent.add_child(new_node.state);
            return &(parent.children.back());
        }
        else{
            return &(*it);
        }
    }
    int num_child = parent.children.size();
    int choose_range = static_cast<int>(static_cast<float>(num_child) * ratio + 0.5) + 1;
    choose_range = min(num_child, choose_range);
    for(int k=0; k<5; ++k){
        float u = dist01(gen1);
        // randomly choose one child from the choose range of parent.children 
        // and then randomly insert one job into another position.
        // if(u<0.45){
        if(u<0.9){
            //randomly pick one child from parent.children
            int child_ind = gen()%choose_range;
            auto child_iter = parent.children.begin();
            for(int foo=0; foo<child_ind; ++foo){++child_iter;}
            vector<list<int>> que_copy((*child_iter).state.get_q());
            int num_of_stage = parent.state.get_num_stage();
            vector<list<int>> resched_list(parent.state.get_reschedlist());

            for(int stageind=0; stageind<num_of_stage; ++stageind){
                if(!resched_list[stageind].empty()){
                    list<int>& l = que_copy[stageind];
                    int lenthofl = l.size();
                    int pos1 = gen()%lenthofl; int pos2 = gen()%lenthofl;
                    auto it1 = begin(l);
                    for(int foo=0; foo<pos1; ++foo){++it1;}
                    int temp = *it1;
                    l.erase(it1);
                    auto it2 = begin(l);
                    for(int foo=0; foo<pos2; ++foo){++it2;}
                    l.insert(it2, temp);
                }
            }

            NodeFSP1 new_node = NodeFSP1(DFSP1(&(parent.state), que_copy));

            auto it = find(parent.children.begin(), parent.children.end(), new_node);
            if(it==parent.children.end()){
                parent.add_child(new_node.state);
                return &(parent.children.back());
            }
        }
        else{
            NodeFSP1 new_node = NodeFSP1(parent.state.resched_random_uct(gen(), ptime));
            auto it = find(parent.children.begin(), parent.children.end(), new_node);
            if(it==parent.children.end()){
                parent.add_child(new_node.state);
                return &(parent.children.back());
            }
        }
    }
    parent.fully_expanded = true;
    return Epsilon_Greedy_UCT1(parent, scalar, gen());
}

void pw_resched_local_insert(vector<list<int>>& sampled_solu, const vector<list<int>>& resched_list, unsigned seed){
    mt19937 gen{seed};
    const int stages = resched_list.size();
    for(int stageind=0; stageind<stages; ++stageind){
        if(!resched_list[stageind].empty()){
            list<int>& l = sampled_solu[stageind];
            uniform_int_distribution<int> unint(0, l.size()-1);
            int pos1 = unint(gen); int pos2 = unint(gen);
            // while(pos1 == pos2){
                // pos2 = unint(gen);
            // }
            auto it1 = begin(l);
            for(int foo=0; foo<pos1; ++foo){++it1;}
            int temp = *it1;
            l.erase(it1);
            auto it2 = begin(l);
            for(int foo=0; foo<pos2; ++foo){++it2;}
            l.insert(it2, temp);
        }
    }
    return ;
}

NodeFSP1 UCT1_with_DPW2_time(NodeFSP1 root, const vector<vector<int>>& ptime, const double time_budget, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const int horizon, const unsigned seed, const int return_version){
    mt19937 gen{seed};
    auto start = chrono::system_clock::now();
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;

    while(diff.count()<time_budget){
        NodeFSP1* front = &root;
        while((front->state.get_turn() >= 0) && (front->visits > 0)){
            // front = DPW1(*front, scalar, horizon, k1, alpha1, k2, alpha2, ratio, gen());
            front = DPW2(*front, ptime, scalar, horizon, k1, alpha1, k2, alpha2, gen());
        }
        // double r = SimDefault_horizon_fsp1(front->state, horizon, root, gen(), repeat);
        double r = SimDefault_horizon_fsp1(ptime, front->state, horizon, root, gen(), 0, 10);
        BackProp_fsp1(*front, r);
        end = chrono::system_clock::now();
        diff = end-start;
    }
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
    else{
        return MostVisitedChild_fsp1(root);
    }
}

NodeFSP1 UCT1_with_DPW2_iter(NodeFSP1 root, const vector<vector<int>>& ptime, const int iter_budget, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const int horizon, const unsigned seed, const int return_version){
    mt19937 gen{seed};

    int count = 0;
    while(count < iter_budget){
        count++;
        NodeFSP1* front = &root;
        while((front->state.get_turn() >= 0) && (front->visits > 0)){
            // front = DPW1(*front, scalar, horizon, k1, alpha1, k2, alpha2, ratio, gen());
            front = DPW2(*front, ptime, scalar, horizon, k1, alpha1, k2, alpha2, gen());
        }
        // double r = SimDefault_horizon_fsp1(front->state, horizon, root, gen(), repeat);
        double r = SimDefault_horizon_fsp1(ptime, front->state, horizon, root, gen(), 0, 10);
        BackProp_fsp1(*front, r);
    }
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
    else{
        return MostVisitedChild_fsp1(root);
    }
}

NodeFSP1* DPW2(NodeFSP1& parent, const vector<vector<int>>& ptime, const double scalar, const int horizon, const float k1, const float alpha1, const float k2, const float alpha2, const unsigned seed){
    mt19937 gen{seed};
    uniform_real_distribution<float> dist01;
    NodeFSP1* child1;
    int parent_turn=parent.state.get_turn();
    if(parent.children.empty()){
        // if(parent.state.turn==1){
        if(parent_turn==1){
            parent.add_child(parent.state.resched_random_uct(gen(), ptime));
            child1 = &(parent.children.back());
        }
        // else(parent_turn==0){
        else{
            parent.add_child(parent.state.simul_run(horizon, gen(), gen(), gen(), ptime));
            child1 = &(parent.children.back());
        }
        return child1;
    }
    else{
        if(parent_turn==1){
            if((!parent.fully_expanded) && (parent.children.size()<(unsigned)(k1*pow(static_cast<float>(parent.visits), alpha1)))){
                child1 = Get_Resched_LS2(parent, ptime, gen(), scalar);
            }
            else{
                child1 = Epsilon_Greedy_UCT1(parent, scalar, gen());
            }
            return child1;
        }
        else{
            if(parent.children.size()==0){
                NodeFSP1 newnode = NodeFSP1(parent.state.simul_run(horizon, gen(),gen(), gen(), ptime));
                parent.add_child(newnode.state);
                return &(parent.children.back());
            }
            else if((parent.children.size()>0) && (!parent.fully_expanded) && (parent.children.size()<(unsigned)(k2*pow(static_cast<float>(parent.visits), alpha2)))){
                NodeFSP1 newnode;
                // for(int k=0; k<20; ++k){
                for(int k=0; k<10; ++k){
                    newnode = NodeFSP1(parent.state.simul_run(horizon, gen(), gen(), gen(), ptime));
                    auto it = find(parent.children.begin(), parent.children.end(), newnode);
                    if(it==parent.children.end()){
                        parent.add_child(newnode.state);
                        return &(parent.children.back());
                    }
                }
                parent.fully_expanded = true;

                // int numc = parent.children.size();
                // int pos = gen()%numc;
                // auto itpos = parent.children.begin();
                // for(int foo=0; foo<pos; ++foo){
                //     ++itpos;
                // }
                // return &(*itpos);
            }           
            int numc = parent.children.size();
            int pos = gen()%numc;
            auto itpos = parent.children.begin();
            for(int foo=0; foo<pos; ++foo){
                ++itpos;
            }
            return &(*itpos);
        }
    }

}

NodeFSP1* Get_Resched_LS2(NodeFSP1& parent, const vector<vector<int>>& ptime, const unsigned seed, const double scalar){
    mt19937 gen{seed};
    mt19937 gen1{gen()};
    uniform_real_distribution<float> dist01(0, 1);
    // float u = dist01(gen);
    bool new_action_found = false;
    // if(parent.visits<100){ 
    // if(parent.visits<50){ 
    if(parent.visits<100){ 
        // NodeFSP1 new_node = NodeFSP1(parent.state.resched1(gen()));
        NodeFSP1 new_node = NodeFSP1(parent.state.resched_random_uct(gen(), ptime));
        auto it = find(parent.children.begin(), parent.children.end(), new_node);
        if(it==parent.children.end()){
            parent.add_child(new_node.state);
            return &(parent.children.back());
        }
        else{
            return &(*it);
        }
    }
    const int num_of_stage = parent.state.get_num_stage();
    for(int k=0; k<10; ++k){
        float u = dist01(gen1);
        // randomly choose one child from the choose range of parent.children 
        // and then randomly insert one job into another position.
        if(u<0.9){
            //randomly pick one child from parent.children
            // int child_ind = gen()%num_child;
            // vector<list<int>> que_copy((*child_iter).state.get_q());
            vector<list<int>> que_copy(parent.state.get_q());
            vector<list<int>> resched_list(parent.state.get_reschedlist());
            for(int stageind=0; stageind<num_of_stage; ++stageind){
                if(!resched_list[stageind].empty()){
                    list<int>& l = que_copy[stageind];
                    const int lenth = l.size();
                    // int pos1 = unint(gen); int pos2 = unint(gen);
                    int pos1 = gen1()%lenth;
                    int pos2 = gen1()%lenth;

                    auto it1 = begin(l);
                    for(int foo=0; foo<pos1; ++foo){++it1;}
                    int temp = *it1;
                    l.erase(it1);
                    auto it2 = begin(l);
                    for(int foo=0; foo<pos2; ++foo){++it2;}
                    l.insert(it2, temp);
                }
            }

            NodeFSP1 new_node = NodeFSP1(DFSP1(&(parent.state), que_copy));

            auto it = find(parent.children.begin(), parent.children.end(), new_node);
            if(it==parent.children.end()){
                parent.add_child(new_node.state);
                return &(parent.children.back());
            }
        }
        else{
            // NodeFSP1 new_node = NodeFSP1(parent.state.resched1_plus_insert(gen()));
            NodeFSP1 new_node = NodeFSP1(parent.state.resched_random_uct(gen(), ptime));
            auto it = find(parent.children.begin(), parent.children.end(), new_node);
            if(it==parent.children.end()){
                parent.add_child(new_node.state);
                return &(parent.children.back());
            }
        }
    }
    parent.fully_expanded = true;
    return Epsilon_Greedy_UCT1(parent, scalar, gen());
}

