#include "SA.h"

DFSP1 FTA_resched_time(DFSP1 state0, const vector<vector<int>>& process_time_all, const double init_temperature, const double time_budget, const unsigned seed){
    const int n = state0.get_num_job();
    const int m = state0.get_num_stage();
    vector<vector<int>> ptime(n, vector<int>(m, 0));
    for(int tempind=0; tempind<n; ++tempind){
        ptime[tempind] = process_time_all[tempind];
    }
 
    mt19937 gen{seed};
    vector<list<int>> resched_list(state0.get_reschedlist());
    const int stages = state0.get_num_stage();

    vector<list<int>> init_solu(state0.get_q());
    vector<list<int>> history_best_solu(init_solu);
    int history_best_span = state0.expected_static_span_outq(process_time_all, init_solu);

    vector<list<int>> current_solu(init_solu);
    int current_span = history_best_span;
    vector<list<int>> temp_solu(init_solu);
    int temp_span = history_best_span;

    uniform_real_distribution<double> dist01(0,1);
    auto start = chrono::system_clock::now();
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    while(diff.count()<time_budget){
        mt19937 gen1{gen()};
        double span_before_search = history_best_span;
        temp_solu = SA_resched_local_insert(current_solu, resched_list, gen1());
        temp_span = state0.expected_static_span_outq(process_time_all, temp_solu);
        if((temp_span<current_span)||(temp_span==current_span)){
            current_solu = temp_solu;
            current_span = temp_span;
        }
        else{
            double exponent = static_cast<double>(current_span-temp_span) / init_temperature;
            double prob = exp(exponent);
            double a = dist01(gen1);
            if(a<prob){
                current_solu = temp_solu;
                current_span = temp_span;
            }
        }
        if(current_span<history_best_span){
            history_best_solu = current_solu;
            history_best_span = current_span;
        }
        end = chrono::system_clock::now();
        diff = end-start;
    }
    return DFSP1(state0, history_best_solu);
}

DFSP1 FTA1_resched_time(DFSP1 state0, const vector<vector<int>>& process_time_all, const double init_temperature, const double time_budget, const unsigned seed){
    auto start = chrono::system_clock::now();

    const int n = state0.get_num_job();
    const int m = state0.get_num_stage();
    vector<vector<int>> ptime(n, vector<int>(m, 0));
    for(int tempind=0; tempind<n; ++tempind){
        ptime[tempind] = process_time_all[tempind];
    }
 
    mt19937 gen{seed};
    vector<list<int>> resched_list(state0.get_reschedlist());
    const int stages = state0.get_num_stage();

    vector<int> resched_stages;
    for(int i=0; i<stages; ++i){
        if(resched_list[i].size()>0){
            resched_stages.emplace_back(i);
        }
    }
    const int num_resched_stages = resched_stages.size();

    vector<list<int>> init_solu(state0.get_q());
    vector<list<int>> history_best_solu(init_solu);
    int history_best_span = state0.expected_static_span_outq(process_time_all, init_solu);

    vector<list<int>> current_solu(init_solu);
    int current_span = history_best_span;
    vector<list<int>> temp_solu(init_solu);
    int temp_span = history_best_span;

    uniform_real_distribution<double> dist01(0,1);
    // auto start = chrono::system_clock::now();
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;
    while(diff.count()<time_budget){
        mt19937 gen1{gen()};
        double span_before_search = history_best_span;

        int which_stage = gen1()%num_resched_stages;
        int stageind = resched_stages[which_stage];
        // temp_solu = SA_resched_local_insert(current_solu, resched_list, gen1());
        temp_solu = SA_resched_local_insert_stage(current_solu, stageind, gen1());
        temp_span = state0.expected_static_span_outq(process_time_all, temp_solu);
        if((temp_span<current_span)||(temp_span==current_span)){
            current_solu = temp_solu;
            current_span = temp_span;
        }
        else{
            double exponent = static_cast<double>(current_span-temp_span) / init_temperature;
            double prob = exp(exponent);
            double a = dist01(gen1);
            if(a<prob){
                current_solu = temp_solu;
                current_span = temp_span;
            }
        }
        if(current_span<history_best_span){
            history_best_solu = current_solu;
            history_best_span = current_span;
        }
        end = chrono::system_clock::now();
        diff = end-start;
    }
    return DFSP1(state0, history_best_solu);
}

vector<list<int>> SA_resched_local_insert(const vector<list<int>>& current_solu, const vector<list<int>>& resched_list, unsigned seed){
    mt19937 gen{seed};
    // uniform_real_distribution<float> dist(0,1);
    vector<list<int>> sampled_solu(current_solu);
    const int stages = resched_list.size();
    for(int stageind=0; stageind<stages; ++stageind){
        if(!resched_list[stageind].empty()){
            list<int>& l = sampled_solu[stageind];
            uniform_int_distribution<int> unint(0, l.size()-1);
            int pos1 = unint(gen); int pos2 = unint(gen);
            while(pos1 == pos2){
                pos2 = unint(gen);
            }
            auto it1 = begin(l);
            for(int foo=0; foo<pos1; ++foo){++it1;}
            int temp = *it1;
            l.erase(it1);
            auto it2 = begin(l);
            for(int foo=0; foo<pos2; ++foo){++it2;}
            l.insert(it2, temp);
        }
    }
    return sampled_solu;
}

vector<list<int>> SA_resched_local_insert_stage(const vector<list<int>>& current_solu, const int stageind, unsigned seed){
    mt19937 gen{seed};
    vector<list<int>> sampled_solu(current_solu);
    list<int>& l = sampled_solu[stageind];
    uniform_int_distribution<int> unint(0, l.size()-1);
    int pos1 = unint(gen); int pos2 = unint(gen);
    while(pos1 == pos2){
        pos2 = unint(gen);
    }
    auto it1 = begin(l);
    for(int foo=0; foo<pos1; ++foo){++it1;}
    int temp = *it1;
    l.erase(it1);
    auto it2 = begin(l);
    for(int foo=0; foo<pos2; ++foo){++it2;}
    l.insert(it2, temp);

    return sampled_solu;
}