#include"mcts.h"

/*
double regularized_reward(const int makespan, vector<vector<int>> ctime, vector<vector<int>> real_ctime){
    int deviation = 0;
    for(int i=0; i<real_ctime.size(); ++i){
        for(int j=0; j<real_ctime[i].size(); ++j){
            if(real_ctime[i][j]>0){
                deviation += max(0, real_ctime[i][j]-ctime[i][j]);
            }
        }
    }
    return static_cast<double>(deviation) / static_cast<double>(makespan);
}

double average_deviation(const vector<vector<int>> ctime, const vector<vector<int>> real_ctime_front, const vector<vector<int>> real_ctime_root){
    int deviation = 0;
    int count = 0;
    for(int i=0; i<real_ctime_front.size(); ++i){
        for(int j=0; j<real_ctime_front[i].size();++j){
            // if((real_ctime_root[i][j]<0) && (real_ctime_front[i][j]>0)){
            if(real_ctime_front[i][j]>0){
                deviation += max(0, real_ctime_front[i][j]-ctime[i][j]);
                count += 1;
            }
            // else if((ctime[i][j]>0)){
            //     deviation += max(0, real_ctime_front[i][j]-ctime[i][j]);
            //     count+=1;
            // }
        }
    }
    if(count==0){return 0;}
    // return (static_cast<double>(deviation) / static_cast<double>(count) / static_cast<double>(count) / 50.);
    return (static_cast<double>(deviation) / static_cast<double>(count) / static_cast<double>(count));
}

int deviation(const vector<vector<int>>& ctime, const vector<vector<int>>& real_ctime){
    int dev = 0;
    for(int i=0; i<ctime.size(); ++i){
        for(int k=0; k<ctime[i].size(); ++k){
            if(real_ctime[i][k]>0){
                dev += max(0, real_ctime[i][k]-ctime[i][k]);
            }
        }
    }
    return dev;
}

double deviation2(const int horizon, const vector<vector<int>> real_ctime_front, const vector<vector<int>> &root_completion_table, const vector<vector<int>> &root_real_completion_table){
// double deviation2(const int horizon, const vector<vector<int>> real_ctime_front, DFSP& state_root){
    int dev = 0;
    int count = 0;
    for(int i=0; i<root_completion_table.size(); ++i){
        for(int k=0; k<root_completion_table[i].size(); ++k){
            if(root_real_completion_table[i][k]<0){
                if(real_ctime_front[i][k]>0){
                    dev += max(0, real_ctime_front[i][k]-root_completion_table[i][k]);
                    count += 1;
                }
            }
        }
    }
    return static_cast<double>(dev)/static_cast<double>(count)/static_cast<double>(count);
}

// double penalty1(const int horizon, DFSP& state_front, DFSP& state_root){
double penalty1(const int horizon, const vector<vector<int>> &process_time, const vector<vector<int>> &completion_table, const vector<vector<int>> &front_real_completion_table, const vector<vector<int>> &front_tasks_remain, const vector<array<int, 3>> &front_dispatch){
    int dev = 0;
    int count = 0;
    for(int i=0; i<completion_table.size(); ++i){
        for(int k=0; k<completion_table[i].size(); ++k){
            int foo = completion_table[i][k]-process_time[i][k];
            if(front_real_completion_table[i][k]>0){
                dev += max(0, front_real_completion_table[i][k]-completion_table[i][k]);
                ++count;
            }
            else if(foo<=horizon){
                if(front_tasks_remain[i][k]>0){
                    dev += max(0, (horizon - foo));
                    ++count;
                }
                else{
                    dev += max(0, (front_dispatch[i][1] - foo));
                    ++count;
                }
            }
        }
    }
    // return static_cast<double>(dev)/static_cast<double>(state_front.makespan)/50.0;
    return static_cast<double>(dev)/static_cast<double>(count)/static_cast<double>(count);
}

double penalty2(const int horizon, const int mkspan, const vector<vector<int>>& real_ctime_front, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<vector<int>>& task_remain, vector<array<int,3>>& real_patch){
    int dev = 0;
    // int count = 0;
    for(int i=0; i<ctime.size(); ++i){
        for(int k=0; k<ctime[i].size(); ++k){
            if(real_ctime_front[i][k]>0){
                dev += max(0, real_ctime_front[i][k]-ctime[i][k]);
            }
            else if(ctime[i][k]-ptime[i][k] <= horizon){
                if(task_remain[i][k]>0){
                    dev += max(0, (horizon - ctime[i][k] + ptime[i][k]));
                    continue;
                }
                else{
                    dev += max(0, (real_patch[i][1]- ctime[i][k] + ptime[i][k]));
                }
            }
        }
    }
    // return static_cast<double>(dev)/static_cast<double>(mkspan)/100.0;
    // return static_cast<double>(dev)/static_cast<double>(mkspan);
    return static_cast<double>(dev)/10000.0;
}

double penalty3(const int horizon, const int num_of_job, const vector<vector<int>>& real_ctime_front, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<vector<int>>& task_remain, vector<array<int,3>>& real_patch){
    int dev = 0;
    // int count = 0;
    for(int i=0; i<ctime.size(); ++i){
        for(int k=0; k<ctime[i].size(); ++k){
            if(real_ctime_front[i][k]>0){
                // dev += max(0, real_ctime_front[i][k]-ctime[i][k]);
                if(real_ctime_front[i][k]-ctime[i][k]>0){
                    dev += 1;
                }
            }
            else if(ctime[i][k]-ptime[i][k] <= horizon){
                if(task_remain[i][k]>0){
                    // dev += max(0, (horizon - ctime[i][k] + ptime[i][k]));
                    if((horizon - ctime[i][k] + ptime[i][k])>0){
                        dev+=1;
                    }
                    continue;
                }
                else{
                    // dev += max(0, (real_patch[i][1]- ctime[i][k] + ptime[i][k]));
                    if((real_patch[i][1]- ctime[i][k] + ptime[i][k])>0){
                        dev+=1;
                    }
                }
            }
        }
    }
    int stages = ctime[0].size();
    return static_cast<double>(dev)/static_cast<double>(num_of_job)/static_cast<double>(stages);
}

double penalty4(const int horizon, const int mkspan, const vector<vector<int>>& real_ctime_front, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<vector<int>>& task_remain, vector<array<int,3>>& real_patch){
    int dev = 0;
    // int count = 0;
    for(int i=0; i<ctime.size(); ++i){
        for(int k=0; k<ctime[i].size(); ++k){
            if(real_ctime_front[i][k]>0){
                // dev += max(0, real_ctime_front[i][k]-ctime[i][k]);
                dev += pow(real_ctime_front[i][k]-ctime[i][k],2);
            }
            else if(ctime[i][k]-ptime[i][k] <= horizon){
                if(task_remain[i][k]>0){
                    // dev += max(0, (horizon - ctime[i][k] + ptime[i][k]));
                    dev += pow(horizon-ctime[i][k]+ptime[i][k],2);
                    continue;
                }
                else{
                    // dev += max(0, (real_patch[i][1]- ctime[i][k] + ptime[i][k]));
                    dev += pow(real_patch[i][1]-ctime[i][k]+ptime[i][k],2);
                }
            }
        }
    }
    // return static_cast<double>(dev)/static_cast<double>(mkspan)/50.0;
    // return static_cast<double>(dev)/static_cast<double>(mkspan);
    return static_cast<double>(dev)/pow(static_cast<double>(mkspan), 2.0);
}

double penalty5(const int horizon, const int mkspan, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<vector<int>>& task_remain, vector<array<int,3>>& real_patch){
    int dev = 0;
    // int count = 0;
    for(int i=0; i<ctime.size(); ++i){
        for(int k=0; k<ctime[i].size(); ++k){
            int temp = ctime[i][k]-ptime[i][k];
            if(temp<=horizon){
                if(task_remain[i][k]>0){
                    dev += (horizon - temp);
                }
                else{
                    dev += abs(real_patch[i][1]-temp);
                }
            }
        }
    }
    // return static_cast<double>(dev)/static_cast<double>(mkspan);
    // return static_cast<double>(dev)/static_cast<double>(mkspan)/100.0;
    return static_cast<double>(dev)/10000.0;
    // return static_cast<double>(dev);
    // return static_cast<double>(dev)/pow(static_cast<double>(mkspan), 2.0);
}

double penalty6(const int num_job, const int horizon, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<vector<int>>& real_ctime_root, const vector<vector<int>>& task_remain, vector<array<int,3>>& real_patch){
    int dev = 0;
    // int count = 0;
    for(int i=0; i<ctime.size(); ++i){
        for(int k=0; k<ctime[i].size(); ++k){
            if(real_ctime_root[i][k]<0){
                int temp = ctime[i][k] - ptime[i][k];
                if(temp<=horizon){
                    if(task_remain[i][k]>0){
                        dev += (horizon - temp);
                    }
                    else if(real_patch[i][0]==k){
                        dev += abs(real_patch[i][1] - temp);
                    }
                }
            }
        }
    }
    // return static_cast<double>(dev)/5000.0;
    return static_cast<double>(dev)/static_cast<double>(num_job)/100.0;
}

double effective_productivity(const int num_job, const int horizon, const vector<vector<int>>& ptime, const vector<vector<int>>& real_ctime, vector<array<int,3>>& real_patch){
    double rat = 0;
    unsigned waiting_time;
    for(int i=0; i<ptime.size(); ++i){
        int k = ptime[i].size();
        int k_prime = k;
        while((k>=0) && (real_ctime[i][k]<0)){
            --k;
            }
        if((k==0)&&(real_ctime[i][0]<0)){
            if(real_patch[i][0]>-1){
                waiting_time += real_patch[i][1];
            }
            else{
                waiting_time += horizon;
            }
        }
        else if((k==0)&& (real_ctime[i][0]>0)){
            waiting_time += (real_ctime[i][0] - ptime[i][0]);
            if(real_patch[i][0]<0){
                waiting_time += (horizon - real_ctime[i][0]);
            }
            else{
                waiting_time += (real_patch[i][1] - real_ctime[i][0]);
            }
        }
        else{
            int temp = 0;
            for(int l = 0; l<=k; ++l){
                temp += ptime[i][l];
            }
            waiting_time += (horizon - temp);
            if((k<k_prime)){
                if(real_patch[i][0]<0){
                    waiting_time += (horizon - real_ctime[i][k]);
                }
                else{
                    waiting_time += (real_patch[i][1] - real_ctime[i][k]);
                }
            }
        }
    }
    return static_cast<double>(waiting_time) / static_cast<double>(num_job) / static_cast<double>(horizon) / static_cast<double>(horizon);
}
*/

/*
NodeFSP UCT_horizon_fsp(NodeFSP &root, const double scalar, const int budget, const int horizon, const unsigned seed, const int repeat){
    mt19937 gen{seed};
    int count = 0;
    while(count < budget){
        ++count;
        NodeFSP* front = SimTree_horizon_fsp(root, scalar, horizon, gen());
        double r = SimDefault_horizon_fsp(front->state, horizon, root, gen(), repeat);
        BackProp_fsp(*front, r);
    }
    NodeFSP bestnode = root.children.front();
    double bestscore = root.children.front().reward;
    for(auto &c:root.children){
        if(c.reward<bestscore){
            bestnode = c;
        }
    }
    return bestnode;
}

NodeFSP UCT_horizon_time_fsp(NodeFSP &root, const double scalar, const double time_budget, const int horizon, const unsigned seed, const int repeat){
    mt19937 gen{seed};
    auto start = chrono::system_clock::now();
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    while(diff.count()<time_budget){
        NodeFSP* front = SimTree_horizon_fsp(root, scalar, horizon, gen());
        double r = SimDefault_horizon_fsp(front->state, horizon, root, gen(), repeat);
        BackProp_fsp(*front, r);
        end = chrono::system_clock::now();
        diff = end-start;
    }
    NodeFSP bestnode = root.children.front();
    double bestscore = root.children.front().reward;
    for(auto &c:root.children){
        if(c.reward<bestscore){
            bestnode = c;
        }
    }
    return bestnode;   
}

NodeFSP parallel_UCT_horizon_fsp(NodeFSP &root, const double scalar, const double time_slot, const int budget, const int workers, const int horizon, const unsigned seed, const int repeat){
    int num_child = root.children.size();
    mt19937 gen{seed};
    list<future<NodeFSP>> future_slave_roots;
    list<NodeFSP> slave_nodes;
    while(root.visits < (unsigned)(budget*(num_child+1))){
        future_slave_roots.clear();
        slave_nodes.clear();
        for(int i=0; i<workers; ++i){
            future_slave_roots.emplace_back(async(launch::async, slave_UCT_fsp, root, scalar, time_slot, workers, horizon, gen(), repeat));
        }
        for(auto &i:future_slave_roots){
            slave_nodes.emplace_back(i.get());
        }
        list_merge(slave_nodes);
        future_slave_roots.clear();
        root = slave_nodes.front();
        for(auto &i:root.children){
            i.parent = &root;
            i.children.clear();
            i.fully_expanded = false;
        }
        num_child = root.children.size();
    }
    return MostVisitedChild_fsp(root);
}

NodeFSP parallel_UCT_horizon_fsp_time(NodeFSP &root, const double scalar, const double time_slot, const double time_budget, const int workers, const int horizon, const unsigned seed, const int repeat){
    mt19937 gen{seed};
    list<future<NodeFSP>> future_slave_roots;
    list<NodeFSP> slave_nodes;
    auto start = chrono::system_clock::now();
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int count = 0;
    while(diff.count()<time_budget){
        future_slave_roots.clear();
        slave_nodes.clear();
        for(int i=0; i<workers; ++i){
            future_slave_roots.emplace_back(async(launch::async, slave_UCT_fsp, root, scalar, time_slot, workers, horizon, gen(), repeat));
        }
        for(auto &i:future_slave_roots){
            slave_nodes.emplace_back(i.get());
        }
        list_merge(slave_nodes);
        future_slave_roots.clear();
        root = slave_nodes.front();
        for(auto &i:root.children){
            i.parent = &root;
            i.children.clear();
            i.fully_expanded = false;
        }
        end = chrono::system_clock::now();
        diff = end - start;
    }
    return BestChild_fsp(root, scalar);
}

NodeFSP slave_UCT_fsp(NodeFSP root, const double scalar, const double time_slot, const int workers, const int horizon, const unsigned seed, const int repeat){
    root.visits /= (unsigned)(workers);
    root.visits += root.children.size();
    for(auto &c:root.children){
        c.visits = c.visits / (unsigned)(workers) + 1;
        c.parent = &root;
    }
    mt19937 gen{seed};
    uniform_real_distribution<double> dist(0, 1);
    auto start = chrono::system_clock::now();
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int count = 0;
    while(diff.count() < time_slot){
        NodeFSP* front = SimTree_horizon_fsp(root, scalar, horizon, gen());
        double r = SimDefault_horizon_fsp(front->state, horizon, root, gen(), repeat);
        ++count;
        BackProp_fsp(*front, r);
        end = chrono::system_clock::now();
        diff = end-start;
    }
    return root;
}

NodeFSP* SimTree_horizon_fsp(NodeFSP& root, const double scalar, const int horizon, const unsigned seed){
    mt19937 gen{seed};
    if(root.children.empty()){
        Expand_fsp(root, horizon, gen());
        return &(root.children.back());
    }
    else{
        uniform_real_distribution<float> dist01(0,1);
        NodeFSP* cur = &root;
        int cur_turn = cur->state.get_turn();
        while((!cur->children.empty()) && (cur_turn!=-1) && (cur_turn!=-2)){
            float a = dist01(gen);
            if(a<0.5){
                if(cur_turn = 1){
                    cur = BestChild_fsp_pointer(*cur, scalar);
                }
                else{
                    int len = cur->children.size();
                    int choice = (int)gen()%len;
                    auto it = cur->children.begin();
                    for(int foo=0;foo<choice; ++foo){
                        ++it;
                    }
                    cur = &(*it);
                }
            }
            else{
                if(cur->fully_expanded){
                    if(cur_turn = 1){
                        cur = BestChild_fsp_pointer(*cur, scalar);
                    }
                    else{
                        int len = cur->children.size();
                        int choice = (int)gen()%len;
                        auto it = cur->children.begin();
                        for(int foo=0;foo<choice; ++foo){
                            ++it;
                        }
                        cur = &(*it);
                    }
                }
                else{
                    if(Expand_fsp(*cur, horizon, gen())){
                        return &(cur->children.back());
                    }
                    else{
                        if(cur_turn = 1){
                            cur = BestChild_fsp_pointer(*cur, scalar);
                        }
                        else{
                            int len = cur->children.size();
                            int choice = (int)gen()%len;
                            auto it = cur->children.begin();
                            for(int foo=0;foo<choice; ++foo){
                                ++it;
                            }
                            cur = &(*it);
                        }
                    }
                }
            }
            cur_turn = cur->state.get_turn();
        }
        if((cur_turn==-1) || (cur_turn==-2)){
            return cur;
        }
        else{
            Expand_fsp(*cur, horizon, gen());
            return &(cur->children.back());
        }
    }
}

double SimDefault_horizon_fsp(DFSP state, const int horizon, NodeFSP& root, const unsigned seed, const int repeat){
    // if((state.turn==-1) || (state.turn==-2)){
    int turn = state.get_turn();
    if((turn==-1) || (turn==-2)){
        // return deviation2(horizon, state.real_completion_time, root.state);
        return deviation2(horizon, state.get_real_completion_table(), root.state.get_completion_table(), root.state.get_real_completion_table());
        // return average_deviation(state.completion_table, state.real_completion_time, root.state.real_completion_time);
        // return penalty1(horizon, state, root.state);
        // return penalty2(horizon, state.makespan, state.real_completion_time, state.process_time, state.completion_table, state.task_remain, state.real_dispatch);
        // r = 8.8;
        // return penalty3(horizon, state.num_of_job, state.real_completion_time, state.process_time, state.completion_table, state.task_remain, state.real_dispatch);
        // return penalty4(horizon, state.makespan, state.real_completion_time, state.process_time, state.completion_table, state.task_remain, state.real_dispatch);
        // return penalty5(horizon, state.makespan, state.process_time, state.completion_table, state.task_remain, state.real_dispatch);
        // return penalty6(state.num_of_job, horizon, state.process_time, state.completion_table, root.state.real_completion_time, state.task_remain, state.real_dispatch);
        // return effective_productivity(state.num_of_job, horizon, state.process_time, state.real_completion_time, state.real_dispatch);
        // return static_cast<double>state.current_time/static_cast<double>state.makespan;
        // return static_cast<double>state.current_time;
        return static_cast<double>(state.get_time());
    }
    else{
        mt19937 gen{seed};
        DFSP state_front = state.rollout(horizon, gen(), gen(), gen());
        return state_front.multi_simu_makespan(repeat, gen());
        // return static_cast<double>state_front.expected_span();
        // return (static_cast<double>state_front.current_time / static_cast<double>(state_front.makespan));
        // return static_cast<double>state_front.current_time;

        // double re = 0.0;
        // for(int i=0; i<repeat; ++i){
        //     // DFSP state_front = state.rollout(horizon, gen(), gen(), gen());
        //     re += static_cast<double>state.rollout(horizon, gen(), gen(), gen()).get_time();
        //     // re += static_cast<double>state_front.current_time;
        // }
        // return re/static_cast<double>repeat;

        // double re = 0.0;
        // for(int i=0; i<10; ++i){
        //     DFSP state_front = state.rollout(horizon, gen(),gen(),gen());
        //     // re += penalty2(horizon, state_front.makespan, state_front.real_completion_time, state_front.process_time, state_front.completion_table, state_front.task_remain, state_front.real_dispatch);
        //     // re += penalty5(horizon, state_front.makespan, state_front.process_time, state_front.completion_table, state_front.task_remain, state_front.real_dispatch);
        //     // re += penalty6(state.num_of_job, horizon, state.process_time, state.completion_table, root.state.real_completion_time, state.task_remain, state.real_dispatch);
        //     // re += effective_productivity(state_front.num_of_job, horizon, state_front.process_time, state_front.real_completion_time, state_front.real_dispatch);
        //     re += (static_cast<double>state_front.current_time / static_cast<double>(state_front.makespan));
        // }
        // return re/10.0;

        // DFSP state_front = state.rollout(horizon, gen(), gen(), gen());
        // vector<vector<int>> real_ctime = state.rollout_real_ctime(horizon);
        // return penalty1(horizon, state_front, root.state);
        // return penalty2(horizon, state_front.makespan, state_front.real_completion_time, state_front.process_time, state_front.completion_table, state_front.task_remain, state_front.real_dispatch);
        // return penalty3(horizon, state_front.num_of_job, state_front.real_completion_time, state_front.process_time, state_front.completion_table, state_front.task_remain, state_front.real_dispatch);
        // return average_deviation(state_front.completion_table, state_front.real_completion_time, root.state.real_completion_time);
        // return deviation2(horizon, state.real_completion_time, root.state);
        // return penalty4(horizon, state_front.makespan, state_front.real_completion_time, state_front.process_time, state_front.completion_table, state_front.task_remain, state_front.real_dispatch);
        // return penalty5(horizon, state_front.makespan, state_front.process_time, state_front.completion_table, state_front.task_remain, state_front.real_dispatch);
    }
}

void BackProp_fsp(NodeFSP& front, const double reward){
    NodeFSP* cur = &front;
    while(cur != nullptr){
        cur->visits += 1;
        cur->reward += ((reward - cur->reward)/static_cast<double>(cur->visits));
        cur = cur->parent;
    }
    return ;
}

NodeFSP BestChild_fsp(NodeFSP& root, const double scalar){
    NodeFSP bestnode;
    double bestscore;
    int foo = 0;
    for(auto &c:root.children){
        // double explore = sqrt(2.0*log(static_cast<double>(root.visits))/static_cast<double>(c.visits));
        double explore = sqrt(log(static_cast<double>(root.visits))/static_cast<double>(c.visits));
        double score = c.reward - explore*scalar;
        if(foo==0){
            bestnode = c;
            bestscore = score;
        }
        else if(score<bestscore){
            bestnode = c;
            bestscore = score;
        }
        ++foo;
    }
    return bestnode; 
}

NodeFSP MostVisitedChild_fsp(NodeFSP& root){
    vector<NodeFSP> bestnodes;
    unsigned visits = root.children.front().visits;
    for(auto &c:root.children){
        if(c.visits>visits){
            bestnodes.clear();
            bestnodes.emplace_back(c);
            visits = c.visits;
        }
        else if(c.visits == visits){
            bestnodes.emplace_back(c);
        }
    }
    if(bestnodes.size()==1){
        return bestnodes.front();
    }
    else if(bestnodes.size()>1){
        NodeFSP bestnode;
        double bestscore;
        int foo = 0;
        for(auto &c:bestnodes){
            // double score = c.reward;
            if(foo==0){
                bestnode = c;
                bestscore = c.reward;
            }
            else if(c.reward<bestscore){
                bestnode = c;
                bestscore = c.reward;
            }
            ++foo;
        }
        return bestnode;
    }
    else{
        cerr<<"No best child found!"<<endl;
        return root;
    }
}

NodeFSP* BestChild_fsp_pointer(NodeFSP& par, const double scalar){
    NodeFSP* bestpoint = nullptr;
    double bestscore;
    int foo = 0;
    for(auto &c:par.children){
        // double explore = sqrt(2.0*log(static_cast<double>(par.visits))/static_cast<double>(c.visits));
        double explore = sqrt(log(static_cast<double>(par.visits))/static_cast<double>(c.visits));
        double score = c.reward - explore*scalar;
        if(foo==0){
            bestpoint = &c;
            bestscore = score;
        }
        else if(score<bestscore){
            bestpoint = &c;
            bestscore = score;
        }
        ++foo;
    }
    return bestpoint; 
}

bool Expand_fsp(NodeFSP& node, const int horizon, const unsigned seed){
    mt19937 gen{seed};
    int node_turn = node.state.get_turn();
    if(node.children.empty()){
        // if(node.state.turn==1){
        if(node_turn==1){
            // node.add_child(node.state.resched(gen()));
            node.add_child(node.state.resched1(gen()));
            // node.add_child(node.state.global_resched(gen()));
            
            // node.add_child(node.state.resched1_plus_insert(gen()));
        }
        // else if(node.state.turn==0){
        else if(node_turn==0){
            node.add_child(node.state.simul_run(horizon, gen(), gen(), gen()));
        }
        return true;
    }
    else{
        int shots = 0;
        unsigned lenchildren = node.children.size();
        int numsup = (int)(1.5*(float)(node.exp_num_of_children + 2));
        // if(node.state.turn==1){
        if(node_turn==1){
            // NodeFSP new_node = NodeFSP(node.state.resched(gen()));
            NodeFSP new_node = NodeFSP(node.state.resched1(gen()));
            // NodeFSP new_node = NodeFSP(node.state.global_resched(gen()));

            // NodeFSP new_node = NodeFSP(node.state.resched1_plus_insert(gen()));
            shots += 1;

            auto it = find(node.children.begin(), node.children.end(), new_node);
            while((it!=node.children.end()) && shots<numsup){
                // new_node = NodeFSP(node.state.resched(gen()));
                new_node = NodeFSP(node.state.resched1(gen()));
                // new_node = NodeFSP(node.state.global_resched(gen()));

                // new_node = NodeFSP(node.state.resched1_plus_insert(gen()));
                it = find(node.children.begin(), node.children.end(), new_node);
                shots += 1;
            }
            if(shots >= numsup){
                node.fully_expanded = true;
                return false;
            }
            else if(shots == 1){
                node.exp_num_of_children = lenchildren + 1;
                node.add_child(new_node.state);
                return true;
            }
            node.exp_num_of_children = (unsigned)((float)shots*(float)lenchildren/(float)(shots-1)) + 1; 
            node.add_child(new_node.state);

            return true;
        }
        else{
            NodeFSP new_node = NodeFSP(node.state.simul_run(horizon, gen(), gen(), gen()));
            shots += 1;

            auto it = find(node.children.begin(), node.children.end(), new_node);
            while((it!=node.children.end()) && shots<numsup){
                new_node = NodeFSP(node.state.simul_run(horizon, gen(), gen(), gen()));
                it = find(node.children.begin(), node.children.end(), new_node);
                shots += 1;
            }
            if(shots >= numsup){
                node.fully_expanded = true;
                return false;
            }
            else if(shots == 1){
                node.exp_num_of_children = lenchildren + 1;
                node.add_child(new_node.state);
                return true;
            }
            node.exp_num_of_children = (unsigned)((float)shots*(float)lenchildren/(float)(shots-1)) + 1; 
            node.add_child(new_node.state);
            return true;
        }
        return false;
    }
}
*/


NodeFSP1 UCT_horizon_fsp1(const vector<vector<int>>& ptime, NodeFSP1 &root, const double scalar, const int budget, const int horizon, const unsigned seed, const int repeat, const int reward_version, const int return_version){
    mt19937 gen{seed};
    int count = 0;
    while(count < budget){
        ++count;
        NodeFSP1* front = SimTree_horizon_fsp1(ptime, root, scalar, horizon, gen());
        double r = SimDefault_horizon_fsp1(ptime, front->state, horizon, root, gen(), reward_version, repeat);
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
    else if(return_version==1){
        return BestChild_fsp1(root, scalar);
    }
    else{
        return MostVisitedChild_fsp1(root);
    }
}

NodeFSP1 UCT_horizon_time_fsp1(const vector<vector<int>>& ptime, NodeFSP1 &root, const double scalar, const double time_budget, const int horizon, const unsigned seed, const int repeat, const int reward_version, const int return_version){
    mt19937 gen{seed};
    auto start = chrono::system_clock::now();
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    while(diff.count()<time_budget){
        NodeFSP1* front = SimTree_horizon_fsp1(ptime, root, scalar, horizon, gen());
        double r = SimDefault_horizon_fsp1(ptime, front->state, horizon, root, gen(), reward_version, repeat);
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
    else if(return_version==1){
        return BestChild_fsp1(root, scalar);
    }
    else{
        return MostVisitedChild_fsp1(root);
    }
}


NodeFSP1* SimTree_horizon_fsp1(const vector<vector<int>>& ptime, NodeFSP1& root, const double scalar, const int horizon, const unsigned seed){
    mt19937 gen{seed};
    if(root.children.empty()){
        Expand_fsp1(ptime, root, horizon, gen());
        return &(root.children.back());
    }
    else{
        uniform_real_distribution<float> dist01(0,1);
        NodeFSP1* cur = &root;
        int cur_turn = cur->state.get_turn();
        while((!cur->children.empty()) && (cur_turn!=-1) && (cur_turn!=-2)){
            float a = dist01(gen);
            if(a<0.5){
                if(cur_turn = 1){
                    cur = BestChild_fsp_pointer1(*cur, scalar);
                }
                else{
                    int len = cur->children.size();
                    int choice = (int)gen()%len;
                    auto it = cur->children.begin();
                    for(int foo=0;foo<choice; ++foo){
                        ++it;
                    }
                    cur = &(*it);
                }
            }
            else{
                if(cur->fully_expanded){
                    if(cur_turn = 1){
                        cur = BestChild_fsp_pointer1(*cur, scalar);
                    }
                    else{
                        int len = cur->children.size();
                        int choice = (int)gen()%len;
                        auto it = cur->children.begin();
                        for(int foo=0;foo<choice; ++foo){
                            ++it;
                        }
                        cur = &(*it);
                    }
                }
                else{
                    if(Expand_fsp1(ptime, *cur, horizon, gen())){
                        return &(cur->children.back());
                    }
                    else{
                        if(cur_turn = 1){
                            cur = BestChild_fsp_pointer1(*cur, scalar);
                        }
                        else{
                            int len = cur->children.size();
                            int choice = (int)gen()%len;
                            auto it = cur->children.begin();
                            for(int foo=0;foo<choice; ++foo){
                                ++it;
                            }
                            cur = &(*it);
                        }
                    }
                }
            }
            cur_turn = cur->state.get_turn();
        }
        if((cur_turn==-1) || (cur_turn==-2)){
            return cur;
        }
        else{
            Expand_fsp1(ptime, *cur, horizon, gen());
            return &(cur->children.back());
        }
    }
}

double SimDefault_horizon_fsp1(const vector<vector<int>>& ptime, DFSP1 state, const int horizon, NodeFSP1& root, const unsigned seed, const int reward_version, const int repeat){
    int turn = state.get_turn();
    if((turn==-1) || (turn==-2)){
        return static_cast<double>(state.get_time());
    }
    else{
        if(reward_version==1){
        // if(reward_version==-10){
            mt19937 gen{seed};
            return state.multi_simu_makespan(repeat, gen(), ptime);
        }
        // else if(reward_version==0){
        else{
            return state.expected_static_span(ptime);
        }
    }
}

void BackProp_fsp1(NodeFSP1& front, const double reward){
    NodeFSP1* cur = &front;
    while(cur != nullptr){
        cur->visits += 1;
        cur->reward += ((reward - cur->reward)/static_cast<double>(cur->visits));
        cur = cur->parent;
    }
    return ;
}


NodeFSP1 BestChild_fsp1(NodeFSP1& root, const double scalar){
    NodeFSP1 bestnode;
    double bestscore;
    int foo = 0;
    for(auto &c:root.children){
        // double explore = sqrt(2.0*log(static_cast<double>(root.visits))/static_cast<double>(c.visits));
        double explore = sqrt(log(static_cast<double>(root.visits))/static_cast<double>(c.visits));
        double score = c.reward - explore*scalar;
        if(foo==0){
            bestnode = c;
            bestscore = score;
        }
        else if(score<bestscore){
            bestnode = c;
            bestscore = score;
        }
        ++foo;
    }
    return bestnode; 
}

NodeFSP1 MostVisitedChild_fsp1(NodeFSP1& root){
    vector<NodeFSP1> bestnodes;
    unsigned visits = root.children.front().visits;
    for(auto &c:root.children){
        if(c.visits>visits){
            bestnodes.clear();
            bestnodes.emplace_back(c);
            visits = c.visits;
        }
        else if(c.visits == visits){
            bestnodes.emplace_back(c);
        }
    }
    if(bestnodes.size()==1){
        return bestnodes.front();
    }
    else if(bestnodes.size()>1){
        NodeFSP1 bestnode;
        double bestscore;
        int foo = 0;
        for(auto &c:bestnodes){
            // double score = c.reward;
            if(foo==0){
                bestnode = c;
                bestscore = c.reward;
            }
            else if(c.reward<bestscore){
                bestnode = c;
                bestscore = c.reward;
            }
            ++foo;
        }
        return bestnode;
    }
    else{
        cerr<<"No best child found!"<<endl;
        return root;
    }
}

NodeFSP1* BestChild_fsp_pointer1(NodeFSP1& par, const double scalar){
    NodeFSP1* bestpoint = nullptr;
    double bestscore;
    int foo = 0;
    for(auto &c:par.children){
        // double explore = sqrt(2.0*log(static_cast<double>(par.visits))/static_cast<double>(c.visits));
        double explore = sqrt(log(static_cast<double>(par.visits))/static_cast<double>(c.visits));
        double score = c.reward - explore*scalar;
        if(foo==0){
            bestpoint = &c;
            bestscore = score;
        }
        else if(score<bestscore){
            bestpoint = &c;
            bestscore = score;
        }
        ++foo;
    }
    return bestpoint;
}

bool Expand_fsp1(const vector<vector<int>>& ptime, NodeFSP1& node, const int horizon, const unsigned seed){
    mt19937 gen{seed};
    int node_turn = node.state.get_turn();
    if(node.children.empty()){
        // if(node.state.turn==1){
        if(node_turn==1){
            node.add_child(node.state.resched_random_uct(gen(), ptime));
            // node.add_child(node.state.resched_random1_uct(gen(), ptime));
        }
        else if(node_turn==0){
            node.add_child(node.state.simul_run(horizon, gen(), gen(), gen(), ptime));
        }
        return true;
    }
    else{
        int shots = 0;
        unsigned lenchildren = node.children.size();
        // int numsup = static_cast<int>(1.5*static_cast<float>(node.exp_num_of_children + 2));
        int numsup = static_cast<int>(node.exp_num_of_children + 5);
        // if(node.state.turn==1){
        if(node_turn==1){
            NodeFSP1 new_node = NodeFSP1(node.state.resched_random_uct(gen(), ptime));
            shots += 1;

            auto it = find(node.children.begin(), node.children.end(), new_node);
            while((it!=node.children.end()) && shots<numsup){
                new_node = NodeFSP1(node.state.resched_random_uct(gen(), ptime));
                it = find(node.children.begin(), node.children.end(), new_node);
                shots += 1;
            }
            if(shots >= numsup){
                node.fully_expanded = true;
                return false;
            }
            else if(shots == 1){
                node.exp_num_of_children = lenchildren + 1;
                node.add_child(new_node.state);
                return true;
            }
            // node.exp_num_of_children = (unsigned)((float)shots*(float)lenchildren/(float)(shots-1)) + 1; 
            node.exp_num_of_children = static_cast<unsigned>(static_cast<float>(shots*lenchildren)/static_cast<float>(shots-1)) + 1; 
            node.add_child(new_node.state);

            return true;
        }
        else{
            NodeFSP1 new_node = NodeFSP1(node.state.simul_run(horizon, gen(), gen(), gen(), ptime));
            shots += 1;

            auto it = find(node.children.begin(), node.children.end(), new_node);
            while((it!=node.children.end()) && shots<numsup){
                new_node = NodeFSP1(node.state.simul_run(horizon, gen(), gen(), gen(), ptime));
                it = find(node.children.begin(), node.children.end(), new_node);
                shots += 1;
            }
            if(shots >= numsup){
                node.fully_expanded = true;
                return false;
            }
            else if(shots == 1){
                node.exp_num_of_children = lenchildren + 1;
                node.add_child(new_node.state);
                return true;
            }
            node.exp_num_of_children = static_cast<unsigned>(static_cast<float>(shots*lenchildren)/static_cast<float>(shots-1)) + 1; 
            node.add_child(new_node.state);
            return true;
        }
        return false;
    }
}
