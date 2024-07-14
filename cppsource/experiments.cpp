#include "headers.h"
#include "experiments.h"

/*
int tard(const vector<vector<int>>& ctime, const vector<vector<int>>& real_ctime){
    int tard = 0;
    for(int i=0; i<ctime.size(); ++i){
        tard += max(0, real_ctime[i].back()-ctime[i].back());
    }
    return tard;
}

void test_rollout(DFSP state, int predict_lenth){
    random_device rd;
    mt19937 gen{rd()};
    DFSP state1 = state.env_run(gen(), gen(), gen());
    DFSP state2;
    DFSP state3;
    int horizon;
    vector<double> out;
    int count = 0;
    // while(state1.turn != -1){
    while(state1.get_turn() != -1){
        // horizon = state1.current_time + predict_lenth;
        horizon = state1.get_time() + predict_lenth;
        state2 = state1.rollout(horizon, gen(), gen(), gen());
        state = state1.resched(gen());
        state2 = state.rollout(horizon, gen(), gen(), gen());
        out.emplace_back(1.0);
        
        state1 = state.env_run(gen(), gen(), gen());
        count += 1;
    }
    for(auto &i:out){
        cout<<i<<"__";
    }
    cout<<endl;
}


vector<double> exp_fsp_mcts(DFSP state, const int budget, const int workers, const int predict_lenth, const double scalar, const double time_slot, const int repeat){
    random_device rd;
    mt19937 gen{rd()};
    DFSP state1 = state.env_run(gen(), gen(), gen());
    NodeFSP current_node = NodeFSP(state1);
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_node.state.get_turn() != -1){
    while(current_node.state.get_turn() != -1){
        // int horizon = current_node.state.get_time() + predict_lenth;
        int horizon = current_node.state.get_time() + predict_lenth;
        NodeFSP newnode = parallel_UCT_horizon_fsp(current_node, scalar, time_slot, budget, workers, horizon, gen(), repeat);
        current_node = NodeFSP(newnode.state.env_run(gen(), gen(), gen()));
        count += 1;
    }


    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_node.state.completion_table, current_node.state.real_completion_time);
    int dev = tard(current_node.state.get_completion_table(), current_node.state.get_real_completion_table());
    // int makespan = current_node.state.get_time();
    int makespan = current_node.state.get_time();
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return return_val;
}

vector<double> exp_fsp_mcts_time(DFSP state, const double time_budget, const int workers, const int predict_lenth, const double scalar, const double time_slot, const int repeat){
    random_device rd;
    mt19937 gen{rd()};
    DFSP state1 = state.env_run(gen(), gen(), gen());
    NodeFSP current_node = NodeFSP(state1);
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_node.state.get_turn() != -1){
    while(current_node.state.get_turn() != -1){
        // int horizon = current_node.state.get_time() + predict_lenth;
        int horizon = current_node.state.get_time() + predict_lenth;
        NodeFSP newnode = parallel_UCT_horizon_fsp_time(current_node, scalar, time_slot, time_budget, workers, horizon, gen(), repeat);
        current_node = NodeFSP(newnode.state.env_run(gen(), gen(), gen()));
        count += 1;
    }

    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_node.state.completion_table, current_node.state.real_completion_time);
    int dev = tard(current_node.state.get_completion_table(), current_node.state.get_real_completion_table());
    // int makespan = current_node.state.get_time();
    int makespan = current_node.state.get_time();
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return return_val;
}

vector<double> exp_fsp_rs(DFSP state){
    random_device rd;
    mt19937 gen{rd()};
    DFSP current_state = state.env_run(gen(), gen(), gen());
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.get_turn()!=-1){
    while(current_state.get_turn()!=-1){
        DFSP state1 = current_state.resched_rightshift();
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return return_val;
}

vector<double> exp_fsp_spt(DFSP state){
    random_device rd;
    mt19937 gen{rd()};
    DFSP current_state = state.env_run(gen(), gen(), gen());
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP state1 = current_state.resched_spt();
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = deviation(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return return_val;
}

vector<double> exp_fsp_srpt(DFSP state){
    random_device rd;
    mt19937 gen{rd()};
    DFSP current_state = state.env_run(gen(), gen(), gen());
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP state1 = current_state.resched_srpt();
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return return_val;
}

vector<double> exp_fsp_lpt(DFSP state){
    random_device rd;
    mt19937 gen{rd()};
    DFSP current_state = state.env_run(gen(), gen(), gen());
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP state1 = current_state.resched_lpt();
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = deviation(current_state.completion_table, current_state.real_completion_time);
    // int dev = tard(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return return_val;
}

vector<double> exp_fsp_ect(DFSP state){
    // cerr<<"1111"<<endl;
    random_device rd;
    mt19937 gen{rd()};
    DFSP current_state = state.env_run(gen(), gen(), gen());
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP state1 = current_state.resched_ect();
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return return_val;
}

vector<double> exp_fsp_est(DFSP state){
    random_device rd;
    mt19937 gen{rd()};
    DFSP current_state = state.env_run(gen(), gen(), gen());
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP state1 = current_state.resched_est();
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return return_val;
}


vector<double> exp_fsp_random(DFSP state){
    random_device rd;
    mt19937 gen{rd()};
    DFSP current_state = state.env_run(gen(), gen(), gen());
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP state1 = current_state.resched(gen());
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return return_val;
}

vector<double> exp_fsp_ritm(DFSP state){
    random_device rd;
    mt19937 gen{rd()};
    DFSP current_state = state.env_run(gen(), gen(), gen());
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP state1 = current_state.resched_ritm();
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return return_val;
}

vector<double> exp_fsp_neh(DFSP state){
    random_device rd;
    mt19937 gen{rd()};
    DFSP current_state = state.env_run(gen(), gen(), gen());
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP state1 = current_state.resched_neh();
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return return_val;
}

vector<double> exp_fsp_rs_with_seed(DFSP state, const unsigned seed, const int index){
    mt19937 gen{seed};
    DFSP current_state = state.env_run(gen(), gen(), gen());
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP state1 = current_state.resched_rightshift();
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp_spt_with_seed(DFSP state, const unsigned seed, const int index){
    mt19937 gen{seed};
    DFSP current_state = state.env_run(gen(), gen(), gen());
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP state1 = current_state.resched_spt();
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp_lpt_with_seed(DFSP state, const unsigned seed, const int index){
    mt19937 gen{seed};
    DFSP current_state = state.env_run(gen(), gen(), gen());
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_state.get_turn()!=-1){
        DFSP state1 = current_state.resched_lpt();
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = deviation(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp_srpt_with_seed(DFSP state, const unsigned seed, const int index){
    mt19937 gen{seed};
    DFSP current_state = state.env_run(gen(), gen(), gen());
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_state.get_turn()!=-1){
        DFSP state1 = current_state.resched_srpt();
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp_ect_with_seed(DFSP state, const unsigned seed, const int index){
    mt19937 gen{seed};
    DFSP current_state = state.env_run(gen(), gen(), gen());
    int count = 0;
    auto start = chrono::system_clock::now();
    // cerr<<"222222"<<endl;
    while(current_state.get_turn()!=-1){
        DFSP state1 = current_state.resched_ect();
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp_est_with_seed(DFSP state, const unsigned seed, const int index){
    mt19937 gen{seed};
    DFSP current_state = state.env_run(gen(), gen(), gen());
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_state.get_turn()!=-1){
        DFSP state1 = current_state.resched_est();
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp_neh_with_seed(DFSP state, const unsigned seed, const int index){
    mt19937 gen{seed};
    DFSP current_state = state.env_run(gen(), gen(), gen());
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_state.get_turn()!=-1){
        DFSP state1 = current_state.resched_neh();
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp_ritm_with_seed(DFSP state, const unsigned seed, const int index){
    mt19937 gen{seed};
    DFSP current_state = state.env_run(gen(), gen(), gen());
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_state.get_turn()!=-1){
        DFSP state1 = current_state.resched_ritm();
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();

    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp_random_with_seed(DFSP state, const unsigned seed, const int index){
    mt19937 gen{seed};
    random_device rd;
    mt19937 gen1{rd()};
    DFSP current_state = state.env_run(gen(), gen(), gen());
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_state.get_turn()!=-1){
        DFSP state1 = current_state.resched(gen1());
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

// vector<double> exp_fsp_mcts(DFSP state, const int budget, const int workers, const int predict_lenth, const double scalar, const double time_slot){
vector<double> exp_fsp_mcts_with_seed(DFSP state, const int budget, const int workers, const int predict_lenth, const double scalar, const double time_slot, const unsigned seed, const int index, const int repeat){
    // NodeFSP node0 = NodeFSP(state);
    random_device rd;
    mt19937 gen{seed};
    mt19937 gen1{rd()};
    DFSP state1 = state.env_run(gen(), gen(), gen());
    NodeFSP current_node = NodeFSP(state1);
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_node.state.get_turn() != -1 && count<200){
    while(current_node.state.get_turn() != -1){
        int horizon = current_node.state.get_time() + predict_lenth;
        // cout<<"start tree seach"<<endl;
        NodeFSP newnode = parallel_UCT_horizon_fsp(current_node, scalar, time_slot, budget, workers, horizon, gen1(), repeat);
        // cout<<"end tree seach"<<endl;
        // newnode = BestChild_fsp(newnode, scalar);
        current_node = NodeFSP(newnode.state.env_run(gen(), gen(), gen()));
        // cout<<current_node.state.get_turn()<<"@@@@@@"<<endl;
        count += 1;
    }


    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_node.state.completion_table, current_node.state.real_completion_time);
    int dev = tard(current_node.state.get_completion_table(), current_node.state.get_real_completion_table());
    // int makespan = current_node.state.get_time();
    int makespan = current_node.state.get_time();

    // return: deviation, makespan, times of rescheduling, average time
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return return_val;
}

// vector<double> exp_fsp_mcts_time(DFSP state, const double time_budget, const int workers, const int predict_lenth, const double scalar, const double time_slot){
vector<double> exp_fsp_mcts_time_with_seed(DFSP state, const double time_budget, const int workers, const int predict_lenth, const double scalar, const double time_slot, const unsigned seed, const int index, const int repeat){
    random_device rd;
    mt19937 gen{seed};
    mt19937 gen1{seed};
    // NodeFSP node0 = NodeFSP(state);
    DFSP state1 = state.env_run(gen(), gen(), gen());
    NodeFSP current_node = NodeFSP(state1);
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_node.state.get_turn() != -1 && count<200){
    while(current_node.state.get_turn() != -1){
        // cerr<<"========="<<count<<endl;
        int horizon = current_node.state.get_time() + predict_lenth;
        NodeFSP newnode = parallel_UCT_horizon_fsp_time(current_node, scalar, time_slot, time_budget, workers, horizon, gen1(), repeat);
        current_node = NodeFSP(newnode.state.env_run(gen(), gen(), gen()));
        count += 1;
    }

    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_node.state.completion_table, current_node.state.real_completion_time);
    int dev = tard(current_node.state.get_completion_table(), current_node.state.get_real_completion_table());
    // int makespan = current_node.state.get_time();
    int makespan = current_node.state.get_time();

    // return: deviation, makespan, times of rescheduling, average time
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp_single_mcts_with_seed(DFSP state, const double time_budget, const int predict_lenth, const double scalar, const unsigned seed, const int index, const int repeat){
    mt19937 gen(seed);
    mt19937 gen1(seed);
    DFSP state1 = state.env_run(gen(), gen(), gen());
    NodeFSP current_node = NodeFSP(state1);
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_node.state.get_turn() != -1){
        int horizon = current_node.state.get_time() + predict_lenth;
        // NodeFSP newnode = parallel_UCT_horizon_fsp_time(current_node, scalar, time_slot, time_budget, workers, horizon, gen1());
        NodeFSP newnode = UCT_horizon_time_fsp(current_node, scalar, time_budget, horizon, gen1(), repeat);
        current_node = NodeFSP(newnode.state.env_run(gen(), gen(), gen()));
        count += 1;
    }

    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_node.state.completion_table, current_node.state.real_completion_time);
    int dev = tard(current_node.state.get_completion_table(), current_node.state.get_real_completion_table());
    // int makespan = current_node.state.get_time();
    int makespan = current_node.state.get_time();

    // return: deviation, makespan, times of rescheduling, average time
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp_pwmcts_with_seed(DFSP state, const double time_budget, const int predict_lenth, const double scalar, const unsigned seed, const int index, const float k, const float alpha, const int repeat){
    mt19937 gen(seed);
    mt19937 gen1(seed);
    DFSP state1 = state.env_run(gen(), gen(), gen());
    NodeFSP current_node = NodeFSP(state1);
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_node.state.get_turn() != -1){
        int horizon = current_node.state.get_time() + predict_lenth;
        NodeFSP newnode = UCT_with_PW(current_node, time_budget, scalar, k, alpha, horizon, gen1(), repeat);
        current_node = NodeFSP(newnode.state.env_run(gen(), gen(), gen()));
        count += 1;
    }

    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_node.state.completion_table, current_node.state.real_completion_time);
    int dev = tard(current_node.state.get_completion_table(), current_node.state.get_real_completion_table());
    // int makespan = current_node.state.get_time();
    int makespan = current_node.state.get_time();

    // return: deviation, makespan, times of rescheduling, average time
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp_dpwmcts_with_seed(DFSP state, const double time_budget, const int predict_lenth, const double scalar, const unsigned seed, const int index, const float k1, const float alpha1, const float k2, const float alpha2, const float ratio, const int repeat){
    mt19937 gen(seed);
    mt19937 gen1(seed);
    DFSP state1 = state.env_run(gen(), gen(), gen());
    NodeFSP current_node = NodeFSP(state1);
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_node.state.get_turn() != -1){
        int horizon = current_node.state.get_time() + predict_lenth;
        NodeFSP newnode = UCT_with_DPW(current_node, time_budget, scalar, k1, alpha1, k2, alpha2, ratio, horizon, gen1(), repeat);
        current_node = NodeFSP(newnode.state.env_run(gen(), gen(), gen()));
        count += 1;
    }

    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_node.state.completion_table, current_node.state.real_completion_time);
    int dev = tard(current_node.state.get_completion_table(), current_node.state.get_real_completion_table());
    // int makespan = current_node.state.get_time();
    int makespan = current_node.state.get_time();

    // return: deviation, makespan, times of rescheduling, average time
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp_lsmcts_with_seed(DFSP state, const double time_budget, const int predict_lenth, const double scalar, const unsigned seed, const int index, const float k, const float alpha, const float ratio, const int repeat){
    mt19937 gen(seed);
    mt19937 gen1(seed);
    DFSP state1 = state.env_run(gen(), gen(), gen());
    NodeFSP current_node = NodeFSP(state1);
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_node.state.get_turn() != -1){
        int horizon = current_node.state.get_time() + predict_lenth;
        NodeFSP newnode = UCT_with_PW_LS(current_node, time_budget, scalar, k, alpha, ratio, horizon, gen1(), repeat);
        current_node = NodeFSP(newnode.state.env_run(gen(), gen(), gen()));
        count += 1;
    }

    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // double timeused = diff.count();
    // int dev = tard(current_node.state.completion_table, current_node.state.real_completion_time);
    int dev = tard(current_node.state.get_completion_table(), current_node.state.get_real_completion_table());
    // int makespan = current_node.state.get_time();
    int makespan = current_node.state.get_time();

    // return: deviation, makespan, times of rescheduling, average time
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}


vector<double> exp_fsp_new_single_mcts_with_seed(DFSP state, const double time_budget, const int predict_lenth, const double scalar, const unsigned seed, const int index, const int repeat){
    mt19937 gen(seed);
    mt19937 gen1(seed);
    DFSP state1 = state.env_run(gen(), gen(), gen());
    NodeFSP current_node = NodeFSP(state1);
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_node.state.get_turn() != -1){
        int horizon = current_node.state.get_time() + predict_lenth;
        // NodeFSP newnode = UCT_horizon_time_fsp(current_node, scalar, time_budget, horizon, gen1(), repeat);
        NodeFSP newnode = new_UCT_horizon_time_fsp(current_node, scalar, time_budget, horizon, gen1(), repeat);
        current_node = NodeFSP(newnode.state.env_run(gen(), gen(), gen()));
        count += 1;
    }

    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_node.state.completion_table, current_node.state.real_completion_time);
    int dev = tard(current_node.state.get_completion_table(), current_node.state.get_real_completion_table());
    // int makespan = current_node.state.get_time();
    int makespan = current_node.state.get_time();

    // return: deviation, makespan, times of rescheduling, average time
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}


vector<double> exp_sa_resched(DFSP state, const float temperature, const float anneal_rate, const double time_budget, const int iter_size, const int stop_crtn, const unsigned seed, const int index, const bool shuffle){
    mt19937 gen{seed};
    mt19937 gen1{seed};
    // cout<<"begin"<<endl;
    DFSP current_state = state.env_run(gen(), gen(), gen());
    // cout<<"begin2"<<endl;
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_state.get_turn()!=-1){
        // cout<<count<<endl;
        DFSP state1 = SA_resched(temperature, anneal_rate, time_budget, iter_size, stop_crtn, gen1(), current_state, shuffle);
        // cout<<"@@@"<<endl;
        current_state = state1.env_run(gen(), gen(), gen());
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    // int dev = tard(current_state.completion_table, current_state.real_completion_time);
    int dev = tard(current_state.get_completion_table(), current_state.get_real_completion_table());
    // int makespan = current_state.current_time;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(dev));
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;

    return return_val;
}

*/


vector<double> exp_fsp1_rs_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index){
    mt19937 gen{seed};
    DFSP1 current_state = state.env_run(gen(), gen(), gen(), process_time);
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP1 state1 = current_state.resched_rightshift();
        current_state = state1.env_run(gen(), gen(), gen(), process_time);
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp1_spt_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index){
    mt19937 gen{seed};
    DFSP1 current_state = state.env_run(gen(), gen(), gen(), process_time);
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_state.get_turn()!=-1){
        DFSP1 state1 = current_state.resched_spt(process_time);
        current_state = state1.env_run(gen(), gen(), gen(), process_time);
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp1_lpt_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index){
    mt19937 gen{seed};
    DFSP1 current_state = state.env_run(gen(), gen(), gen(), process_time);
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_state.get_turn()!=-1){
        DFSP1 state1 = current_state.resched_lpt(process_time);
        current_state = state1.env_run(gen(), gen(), gen(), process_time);
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp1_srpt_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index){
    mt19937 gen{seed};
    DFSP1 current_state = state.env_run(gen(), gen(), gen(), process_time);
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP1 state1 = current_state.resched_srpt(process_time);
        current_state = state1.env_run(gen(), gen(), gen(), process_time);
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp1_ritm_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index){
    mt19937 gen{seed};
    DFSP1 current_state = state.env_run(gen(), gen(), gen(), process_time);
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP1 state1 = current_state.resched_ritm(process_time);
        current_state = state1.env_run(gen(), gen(), gen(), process_time);
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp1_neh_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index){
    mt19937 gen{seed};
    DFSP1 current_state = state.env_run(gen(), gen(), gen(), process_time);
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP1 state1 = current_state.resched_neh(process_time);
        current_state = state1.env_run(gen(), gen(), gen(), process_time);
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp1_neh1_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index){
    mt19937 gen{seed};
    DFSP1 current_state = state.env_run(gen(), gen(), gen(), process_time);
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP1 state1 = current_state.resched_neh1(process_time);
        current_state = state1.env_run(gen(), gen(), gen(), process_time);
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp1_nehnm_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index){
    mt19937 gen{seed};
    DFSP1 current_state = state.env_run(gen(), gen(), gen(), process_time);
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP1 state1 = current_state.resched_nehnm(process_time);
        current_state = state1.env_run(gen(), gen(), gen(), process_time);
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp1_nehnm1_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index){
    mt19937 gen{seed};
    DFSP1 current_state = state.env_run(gen(), gen(), gen(), process_time);
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP1 state1 = current_state.resched_nehnm1(process_time);
        current_state = state1.env_run(gen(), gen(), gen(), process_time);
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp1_nehkk_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index){
    mt19937 gen{seed};
    DFSP1 current_state = state.env_run(gen(), gen(), gen(), process_time);
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP1 state1 = current_state.resched_nehkk(process_time);
        current_state = state1.env_run(gen(), gen(), gen(), process_time);
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp1_random_with_seed(DFSP1 state, const vector<vector<int>> process_time, const unsigned seed, const int index){
    mt19937 gen{seed};
    mt19937 gen1{seed};
    gen1();
    mt19937 gen2{gen1()};
    DFSP1 current_state = state.env_run(gen(), gen(), gen(), process_time);
    int count = 0;
    auto start = chrono::system_clock::now();
    // while(current_state.turn!=-1){
    while(current_state.get_turn()!=-1){
        DFSP1 state1 = current_state.resched_random(gen2(), process_time);
        current_state = state1.env_run(gen(), gen(), gen(), process_time);
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}


vector<double> exp_fsp1_uct_time_with_seed(DFSP1 state, const vector<vector<int>> process_time, const double time_budget, const int predict_lenth, const double scalar, const unsigned seed, const int index, const int repeat, const int reward_version, const int return_version){
    mt19937 gen(seed);
    mt19937 gen1(seed);
    DFSP1 state1 = state.env_run(gen(), gen(), gen(), process_time);
    NodeFSP1 current_node = NodeFSP1(state1);
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_node.state.get_turn() != -1){
        int horizon = current_node.state.get_time() + predict_lenth;
        // NodeFSP1 newnode = parallel_UCT_horizon_fsp_time(current_node, scalar, time_slot, time_budget, workers, horizon, gen1());
        NodeFSP1 newnode = UCT_horizon_time_fsp1(process_time, current_node, scalar, time_budget, horizon, gen1(), repeat, reward_version, return_version);
        current_node = NodeFSP1(newnode.state.env_run(gen(), gen(), gen(), process_time));
        count += 1;
    }

    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_node.state.get_time();

    // return: deviation, makespan, times of rescheduling, average time
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back(static_cast<double>(index));
    return return_val;
}

vector<double> exp_fsp1_uct_iter_with_seed(DFSP1 state, const vector<vector<int>> process_time, const int iter_budget, const int predict_lenth, const double scalar, const unsigned seed, const int index, const int repeat, const int reward_version, const int return_version){
    mt19937 gen(seed);
    mt19937 gen1(seed);
    DFSP1 state1 = state.env_run(gen(), gen(), gen(), process_time);
    NodeFSP1 current_node = NodeFSP1(state1);
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_node.state.get_turn() != -1){
        int horizon = current_node.state.get_time() + predict_lenth;
        // NodeFSP1 newnode = parallel_UCT_horizon_fsp_time(current_node, scalar, time_slot, time_budget, workers, horizon, gen1());
        NodeFSP1 newnode = UCT_horizon_fsp1(process_time, current_node, scalar, iter_budget, horizon, gen1(), repeat, reward_version, return_version);
        current_node = NodeFSP1(newnode.state.env_run(gen(), gen(), gen(), process_time));
        count += 1;
    }

    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_node.state.get_time();

    // return: deviation, makespan, times of rescheduling, average time
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back(static_cast<double>(index));
    return return_val;   
}

vector<double> exp_fsp1_uctnewbp_iter_with_seed(DFSP1 state, const vector<vector<int>> process_time, const int iter_budget, const int predict_lenth, const double scalar, const unsigned seed, const int index, const int repeat, const int reward_version, const int return_version, const double alpha){
    mt19937 gen(seed);
    mt19937 gen1(seed);
    DFSP1 state1 = state.env_run(gen(), gen(), gen(), process_time);
    NodeFSP1 current_node = NodeFSP1(state1);
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_node.state.get_turn() != -1){
        int horizon = current_node.state.get_time() + predict_lenth;
        // NodeFSP1 newnode = parallel_UCT_horizon_fsp_time(current_node, scalar, time_slot, time_budget, workers, horizon, gen1());
        // NodeFSP1 newnode = UCT_horizon_fsp1(process_time, current_node, scalar, iter_budget, horizon, gen1(), repeat, reward_version, return_version);
        NodeFSP1 newnode = new_UCT_horizon_iter_fsp1(process_time, current_node, scalar, iter_budget, horizon, gen1(), repeat, reward_version, return_version, alpha);
        current_node = NodeFSP1(newnode.state.env_run(gen(), gen(), gen(), process_time));
        count += 1;
    }

    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_node.state.get_time();

    // return: deviation, makespan, times of rescheduling, average time
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back(static_cast<double>(index));
    return return_val;
}

vector<double> exp_fsp1_dpw_time_with_seed(DFSP1 state, const vector<vector<int>> process_time, const double time_budget, const int predict_lenth, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const float ratio, const unsigned seed, const int index, const int return_version){
    mt19937 gen(seed);
    mt19937 gen1(seed);
    DFSP1 state1 = state.env_run(gen(), gen(), gen(), process_time);
    NodeFSP1 current_node = NodeFSP1(state1);
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_node.state.get_turn() != -1){
        int horizon = current_node.state.get_time() + predict_lenth;
        NodeFSP1 newnode = UCT1_with_DPW_time(current_node, process_time, time_budget, scalar, k1, alpha1, k2, alpha2, ratio, horizon, gen1(), return_version);
        current_node = NodeFSP1(newnode.state.env_run(gen(), gen(), gen(), process_time));
        count += 1;
    }

    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_node.state.get_time();

    // return: deviation, makespan, times of rescheduling, average time
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back(static_cast<double>(index));
    return return_val;
}

vector<double> exp_fsp1_dpw_iter_with_seed(DFSP1 state, const vector<vector<int>> process_time, const int iter_budget, const int predict_lenth, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const float ratio, const unsigned seed, const int index, const int return_version){
    mt19937 gen(seed);
    mt19937 gen1(seed);
    DFSP1 state1 = state.env_run(gen(), gen(), gen(), process_time);
    NodeFSP1 current_node = NodeFSP1(state1);
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_node.state.get_turn() != -1){
        int horizon = current_node.state.get_time() + predict_lenth;
        NodeFSP1 newnode = UCT1_with_DPW_iter(current_node, process_time, iter_budget, scalar, k1, alpha1, k2, alpha2, ratio, horizon, gen1(), return_version);
        current_node = NodeFSP1(newnode.state.env_run(gen(), gen(), gen(), process_time));
        count += 1;
    }

    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_node.state.get_time();

    // return: deviation, makespan, times of rescheduling, average time
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back(static_cast<double>(index));
    return return_val;   
}

vector<double> exp_fsp1_dpw2_time_with_seed(DFSP1 state, const vector<vector<int>> process_time, const double time_budget, const int predict_lenth, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const unsigned seed, const int index, const int return_version){
    mt19937 gen(seed);
    mt19937 gen1(seed);
    DFSP1 state1 = state.env_run(gen(), gen(), gen(), process_time);
    NodeFSP1 current_node = NodeFSP1(state1);
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_node.state.get_turn() != -1){
        int horizon = current_node.state.get_time() + predict_lenth;
        // NodeFSP1 newnode = UCT1_with_DPW_time(current_node, process_time, time_budget, scalar, k1, alpha1, k2, alpha2, ratio, horizon, gen1(), return_version);
        NodeFSP1 newnode = UCT1_with_DPW2_time(current_node, process_time, time_budget, scalar, k1, alpha1, k2, alpha2, horizon, gen1(), return_version);
        current_node = NodeFSP1(newnode.state.env_run(gen(), gen(), gen(), process_time));
        count += 1;
    }

    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_node.state.get_time();

    // return: deviation, makespan, times of rescheduling, average time
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back(static_cast<double>(index));
    return return_val;
}

vector<double> exp_fsp1_dpw2_iter_with_seed(DFSP1 state, const vector<vector<int>> process_time, const int iter_budget, const int predict_lenth, const double scalar, const float k1, const float alpha1, const float k2, const float alpha2, const unsigned seed, const int index, const int return_version){
    mt19937 gen(seed);
    mt19937 gen1(seed);
    DFSP1 state1 = state.env_run(gen(), gen(), gen(), process_time);
    NodeFSP1 current_node = NodeFSP1(state1);
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_node.state.get_turn() != -1){
        int horizon = current_node.state.get_time() + predict_lenth;
        // NodeFSP1 newnode = UCT1_with_DPW_iter(current_node, process_time, iter_budget, scalar, k1, alpha1, k2, alpha2, ratio, horizon, gen1(), return_version);
        NodeFSP1 newnode = UCT1_with_DPW2_iter(current_node, process_time, iter_budget, scalar, k1, alpha1, k2, alpha2, horizon, gen1(), return_version);
        current_node = NodeFSP1(newnode.state.env_run(gen(), gen(), gen(), process_time));
        count += 1;
    }

    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_node.state.get_time();

    // return: deviation, makespan, times of rescheduling, average time
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back(static_cast<double>(index));
    return return_val;   
}


vector<double> exp_fsp1_fta_time_with_seed(DFSP1 state, const vector<vector<int>> process_time, const double init_t, const double time_budget, const unsigned seed, const int index){
    mt19937 gen{seed};
    mt19937 gen1{seed};
    // cout<<"begin"<<endl;
    DFSP1 current_state = state.env_run(gen(), gen(), gen(), process_time);
    // cout<<"begin2"<<endl;
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_state.get_turn()!=-1){
        // cout<<count<<endl;
        // DFSP state1 = SA_resched(temperature, anneal_rate, time_budget, iter_size, stop_crtn, gen1(), current_state, shuffle);
        DFSP1 state1 = FTA_resched_time(current_state, process_time, init_t, time_budget, gen1());
        // cout<<"@@@"<<endl;
        current_state = state1.env_run(gen(), gen(), gen(), process_time);
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp1_fta1_time_with_seed(DFSP1 state, const vector<vector<int>> process_time, const double init_t, const double time_budget, const unsigned seed, const int index){
    mt19937 gen{seed};
    mt19937 gen1{seed};
    DFSP1 current_state = state.env_run(gen(), gen(), gen(), process_time);
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_state.get_turn()!=-1){
        DFSP1 state1 = FTA1_resched_time(current_state, process_time, init_t, time_budget, gen1());
        current_state = state1.env_run(gen(), gen(), gen(), process_time);
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}



vector<double> exp_fsp1_ig_time_with_seed(DFSP1 state, const vector<vector<int>> process_time, const double temperature, const int d, const double time_budget, const unsigned seed, const int index){
    mt19937 gen{seed};
    mt19937 gen1{seed};
    // cout<<"begin"<<endl;
    DFSP1 current_state = state.env_run(gen(), gen(), gen(), process_time);
    // cout<<"begin2"<<endl;
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_state.get_turn()!=-1){
        // cout<<count<<endl;
        // DFSP1 state1 = FTA_resched_time(current_state, process_time, init_t, time_budget, gen1());
        DFSP1 state1 = IG_resched_time(current_state, process_time, temperature, d, time_budget, gen1());
        // cout<<"@@@"<<endl;
        current_state = state1.env_run(gen(), gen(), gen(), process_time);
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}

vector<double> exp_fsp1_ig1_time_with_seed(DFSP1 state, const vector<vector<int>> process_time, const double temperature, const int d, const double time_budget, const unsigned seed, const int index){
    mt19937 gen{seed};
    mt19937 gen1{seed};
    DFSP1 current_state = state.env_run(gen(), gen(), gen(), process_time);
    int count = 0;
    auto start = chrono::system_clock::now();
    while(current_state.get_turn()!=-1){
        DFSP1 state1 = IG1_resched_time(current_state, process_time, temperature, d, time_budget, gen1());
        current_state = state1.env_run(gen(), gen(), gen(), process_time);
        count += 1;
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    int makespan = current_state.get_time();
    
    vector<double> return_val;
    return_val.emplace_back(static_cast<double>(makespan));
    return_val.emplace_back(diff.count());
    return_val.emplace_back(static_cast<double>(count));
    return_val.emplace_back((double)index);
    return return_val;
}
