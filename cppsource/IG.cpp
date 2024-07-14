#include"IG.h"

DFSP1 IG_resched_time(DFSP1& state0, const vector<vector<int>>& process_time, const double temperature, const int d, const double time_budget, const unsigned seed){
    auto start = chrono::system_clock::now();

    mt19937 gen{seed};
    uniform_real_distribution<double> dist01(0,1);

    const int m = state0.get_num_stage();
    vector<list<int>> q = (state0.get_q());
    vector<list<int>> resched_list(state0.get_reschedlist());
    int span = state0.expected_static_span_outq(process_time, q);
    q = IterImprov_insert(q, state0, process_time, resched_list, span, gen());
    vector<list<int>> q_best(q);
    int span_best = span;


    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;

    while(diff.count()<time_budget){
        vector<list<int>> q_temp(q);
        vector<list<int>> reinsert_lists = Destruction(q_temp, resched_list, d, gen());
        int span_temp = Construction(q_temp, reinsert_lists, state0, process_time, gen());
        q_temp = IterImprov_insert(q_temp, state0, process_time, resched_list, span_temp, gen());
        if(span_temp<span){
            q = q_temp;
            span = span_temp;
            if(span < span_best){
                q_best = q;
                span_best = span;
            }
        }
        else{
            double exponent = static_cast<double>(span - span_temp) / temperature;
            double prob = exp(exponent);
            double a = dist01(gen);
            if(a<prob){
                span = span_temp;
                q = q_temp;
            }
        }
        end = chrono::system_clock::now();
        diff = end-start;
    }
    return DFSP1(state0, q_best);
}

vector<list<int>> IterImprov_insert(vector<list<int>> q, DFSP1& state, const vector<vector<int>>& process_time, const vector<list<int>>& resched_list, int& span, const unsigned seed){
    mt19937 gen{seed};
    const int numj = state.get_num_job();
    bool improve = true;
    int span_temp1 = span;
    const int stages = state.get_num_stage();
    vector<list<int>> q_temp(q);
    // cout<<"@@@@@@";
    while(improve){
        // cout<<"#######";
        improve = false;
        for(int foo=0; foo<numj; ++foo){
            int span_temp2 = 9999999;
            for(int stageind=stages-1; stageind>=0; --stageind){
                // cout<<"!!!!!!!";
                if(!resched_list[stageind].empty()){
                    list<int>& l = q_temp[stageind];
                    int len = l.size();
                    // choose one job
                    int pos = gen()%len;
                    auto it = l.begin();
                    for(int i=0;i<pos;++i){
                        ++it;
                    }
                    int tempjobind = *it;
                    l.erase(it);

                    // insert
                    it = l.begin();
                    int len1 = l.size();
                    l.insert(it, tempjobind);
                    --it;
                    span_temp2 = state.expected_static_span_outq(process_time, q_temp);
                    // int span_temp3 = span_temp2;
                    int best_pos = 0;
                    int temp_pos = 0;
                    // cout<<"[[[[[[]]]]]]";
                    for(int i=0; i<len1; ++i){
                        it = l.erase(it);
                        ++it;
                        ++temp_pos;
                        l.insert(it, tempjobind);
                        --it;
                        int span_temp3 = state.expected_static_span_outq(process_time, q_temp);
                        if(span_temp3<span_temp2){
                            best_pos = temp_pos;
                            span_temp2 = span_temp3;
                        }
                    }
                    it = l.end(); --it; l.erase(it);
                    auto it1 = l.begin();
                    for(int i=0; i<best_pos; ++i){++it1;}
                    l.insert(it1, tempjobind);
                }
            }
            if(span_temp2<span_temp1){
                q = q_temp;
                span_temp1 = span_temp2;
                span = span_temp2;
                improve = true;
            }
        }
    }
    return q;
}


vector<list<int>> Destruction(vector<list<int>>& q, const vector<list<int>>& resched_list, const int d, const unsigned seed){
    mt19937 gen{seed};
    const int stages = resched_list.size();
    vector<list<int>> reinsert_lists(stages, list<int>());
    for(int stageind=0; stageind<stages; ++stageind){
        if(!resched_list[stageind].empty()){
            int len = q[stageind].size();
            int sample_num = min(d, len-1);
            for(int foo=0; foo<sample_num; ++foo){
                int pos = gen()%len;
                auto it = q[stageind].begin();
                for(int i=0; i<pos; ++i){++it;}
                reinsert_lists[stageind].emplace_back(*it);
                q[stageind].erase(it);
                --len;
            }
        }
    }
    return reinsert_lists;
}

int Construction(vector<list<int>>& q, vector<list<int>>& reinsert_lists, DFSP1& state, const vector<vector<int>>& ptime, const unsigned seed){
    const int stages = state.get_num_stage();
    int span_all = 0;
    vector<list<int>> q_temp(q);
    for(int stageind=stages-1; stageind>=0; --stageind){
        while(reinsert_lists[stageind].size()>0){
            list<int>& l = reinsert_lists[stageind];
            list<int>& ql = q_temp[stageind];
            int len = ql.size();
            int jobind = l.front();
            auto it1 = ql.begin();
            ql.insert(it1, jobind);
            --it1;
            int span_best = state.expected_static_span_outq(ptime, q_temp);
            int pos_best = 0;
            int pos_temp = 0;

            for(int foo=0; foo<len; ++foo){
                ++pos_temp;
                it1 = ql.erase(it1);
                ++it1;
                ql.insert(it1, jobind);
                --it1;
                int span_temp = state.expected_static_span_outq(ptime, q_temp);
                if(span_temp<span_best){
                    pos_best = pos_temp;
                    span_best = span_temp;
                }
            }
            it1 = ql.end(); --it1; ql.erase(it1);
            span_all = span_best;
            auto it2 = ql.begin();
            auto it3 = q[stageind].begin();
            for(int foo=0; foo<pos_best; ++foo){
                ++it2;
                ++it3;
            }
            ql.insert(it2, jobind);
            q[stageind].insert(it3, jobind);
            l.pop_front();
        }
    }
    return span_all;
}


DFSP1 IG1_resched_time(DFSP1& state0, const vector<vector<int>>& process_time, const double temperature, const int d, const double time_budget, const unsigned seed){
    auto start = chrono::system_clock::now();

    mt19937 gen{seed};
    uniform_real_distribution<double> dist01(0,1);

    const int m = state0.get_num_stage();
    vector<list<int>> q = (state0.get_q());
    vector<list<int>> resched_list(state0.get_reschedlist());

    vector<int> resched_stages;
    for(int i=0; i<resched_list.size(); ++i){
        if(resched_list[i].size()>0){
            resched_stages.emplace_back(i);
        }
    }
    const int num_resched_stage = resched_stages.size();

    int span = state0.expected_static_span_outq(process_time, q);
    q = IterImprov_insert1(q, state0, process_time, resched_stages, span, gen());
    vector<list<int>> q_best(q);
    int span_best = span;

    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end-start;

    while(diff.count()<time_budget){
        vector<list<int>> q_temp(q);
        int which_stage = gen()%num_resched_stage;
        int stageind = resched_stages[which_stage];
        list<int> reinsert_list = Destruction1(q_temp[stageind], d, gen());
        int span_temp = Construction1(q_temp, stageind, reinsert_list, state0, process_time, gen());
        q_temp = IterImprov_insert1(q_temp, state0, process_time, resched_stages, span_temp, gen());
        if(span_temp<span){
            q = q_temp;
            span = span_temp;
            if(span < span_best){
                q_best = q;
                span_best = span;
            }
        }
        else{
            double exponent = static_cast<double>(span - span_temp) / temperature;
            double prob = exp(exponent);
            double a = dist01(gen);
            if(a<prob){
                span = span_temp;
                q = q_temp;
            }
        }
        end = chrono::system_clock::now();
        diff = end-start;
    }
    return DFSP1(state0, q_best);
}


vector<list<int>> IterImprov_insert1(vector<list<int>> q, DFSP1& state, const vector<vector<int>>& process_time, const vector<int>& resched_stages, int& span, const unsigned seed){
    mt19937 gen{seed};
    const int numj = state.get_num_job();
    bool improve = true;
    int span_temp1 = span;
    const int stages = state.get_num_stage();
    vector<list<int>> q_temp(q);

    const int num_resched_stage = resched_stages.size();
    
    while(improve){
        // cout<<"#######";
        improve = false;
        for(int foo=0; foo<numj; ++foo){
            int which_stage = gen()%num_resched_stage;
            int stageind = resched_stages[which_stage];
            int span_temp2 = 9999999;

            list<int>& l = q_temp[stageind];
            int len = l.size();
            // choose one job
            int pos = gen()%len;
            auto it = l.begin();
            for(int i=0;i<pos;++i){
                ++it;
            }
            int tempjobind = *it;
            l.erase(it);

            // insert
            it = l.begin();
            int len1 = l.size();
            l.insert(it, tempjobind);
            --it;
            span_temp2 = state.expected_static_span_outq(process_time, q_temp);
            int best_pos = 0;
            int temp_pos = 0;
            for(int i=0; i<len1; ++i){
                it = l.erase(it);
                ++it;
                ++temp_pos;
                l.insert(it, tempjobind);
                --it;
                int span_temp3 = state.expected_static_span_outq(process_time, q_temp);
                if(span_temp3<span_temp2){
                    best_pos = temp_pos;
                    span_temp2 = span_temp3;
                }
            }
            it = l.end(); --it; l.erase(it);
            auto it1 = l.begin();
            for(int i=0; i<best_pos; ++i){++it1;}
            l.insert(it1, tempjobind);
  
            if(span_temp2<span_temp1){
                q[stageind] = q_temp[stageind];
                span_temp1 = span_temp2;
                span = span_temp2;
                improve = true;
            }
        }
    }
    return q;
}

list<int> Destruction1(list<int>& l, const int d, const unsigned seed){
    mt19937 gen{seed};
    int len = l.size();
    int sample_num = min(d, len-1);
    list<int> reinsert_list;
    for(int foo=0; foo<sample_num; ++foo){
        int pos = gen()%len;
        auto it = l.begin();
        for(int i=0; i<pos; ++i){++it;}
        reinsert_list.emplace_back(*it);
        l.erase(it);
        --len;
    }

    return reinsert_list;
}

int Construction1(vector<list<int>>& q, const int stageind, list<int>& reinsert_list, DFSP1& state, const vector<vector<int>>& ptime, const unsigned seed){
    const int stages = state.get_num_stage();
    int span_all = 0;
    vector<list<int>> q_temp(q);

    while(reinsert_list.size()>0){
        list<int>& ql = q_temp[stageind];
        int len = ql.size();
        int jobind = reinsert_list.front();
        auto it1 = ql.begin();
        ql.insert(it1, jobind);
        --it1;
        int span_best = state.expected_static_span_outq(ptime, q_temp);
        int pos_best = 0;
        int pos_temp = 0;

        for(int foo=0; foo<len; ++foo){
            ++pos_temp;
            it1 = ql.erase(it1);
            ++it1;
            ql.insert(it1, jobind);
            --it1;
            int span_temp = state.expected_static_span_outq(ptime, q_temp);
            if(span_temp<span_best){
                pos_best = pos_temp;
                span_best = span_temp;
            }
        }
        it1 = ql.end(); --it1; ql.erase(it1);
        
        span_all = span_best;
        auto it2 = ql.begin();
        auto it3 = q[stageind].begin();
        for(int foo=0; foo<pos_best; ++foo){
            ++it2;
            ++it3;
        }
        ql.insert(it2, jobind);
        q[stageind].insert(it3, jobind);
        reinsert_list.pop_front();
    }
    return span_all;
}