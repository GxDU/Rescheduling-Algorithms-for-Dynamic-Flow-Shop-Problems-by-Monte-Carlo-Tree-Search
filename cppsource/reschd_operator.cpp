#include"reschd_operator.h"

/*
void random_rschd(vector<list<int>>& ques, const map<int,vector<int>>& r_list, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<int>& mac_per_state){
    random_device rd;
    mt19937 gen{rd()};
    uniform_real_distribution<float> dist01(0,1);
    // if(r_list.empty()){return;}
    for(auto &ele:r_list){
        int stage_ind = ele.first;
        int len_q = ques[stage_ind].size();
        int len_resched = ele.second.size();

        if(len_q==2){
            float a = dist01(gen);
            if(a<0.5){
                auto it = ques[stage_ind].begin();
                int temp = ques[stage_ind].front();
                *it = ques[stage_ind].back();
                ++it;
                *it = temp;
            }
        }
        else{
            int i = 0;
            int j = 1;
            while(i<len_resched){
                float a = dist01(gen);
                int job_ind = ele.second[j];
                if(ele.second[i]==0){
                    if(a<0.15){
                        int pos = min(5, len_q);
                        pos = int(gen()) % pos;
                        operator_insert_back(job_ind, ques[stage_ind], pos);
                    }
                    else if(a<0.3){operator_spt(ques[stage_ind], stage_ind, ptime);}
                    else if(a<0.45){operator_lpt(ques[stage_ind], stage_ind, ptime);}
                    else if(a<0.6){operator_srpt(ques[stage_ind], stage_ind, ptime);}
                    else if(a<0.75){operator_neh(ques[stage_ind], stage_ind, ptime);}
                    else if(a<0.9){operator_ritm(ques[stage_ind], stage_ind, ptime, mac_per_state);}
                }
                else if(ele.second[i]==1){
                    // 1 insert front, 2 spt, 3 lpt, 4srpt, 5 ect, 6 est, 7 neh, 8 ritm 
                    if(a<0.2){
                        int pos = min(5, len_q);
                        pos = int(gen()) % pos;
                        operator_insert_front(job_ind, ques[stage_ind], pos);
                    }
                    else if(a<0.3){operator_spt(ques[stage_ind], stage_ind, ptime);}
                    else if(a<0.4){operator_lpt(ques[stage_ind], stage_ind, ptime);}
                    else if(a<0.5){operator_srpt(ques[stage_ind], stage_ind, ptime);}
                    else if(a<0.6){operator_ect(ques[stage_ind],stage_ind, ctime);}
                    else if(a<0.7){operator_est(ques[stage_ind], stage_ind, ctime, ptime);}
                    else if(a<0.8){operator_neh(ques[stage_ind], stage_ind, ptime);}
                    else if(a<0.9){operator_ritm(ques[stage_ind], stage_ind, ptime, mac_per_state);}
                }
                else if(ele.second[i]==2){
                    if(a<0.125){operator_spt(ques[stage_ind], stage_ind, ptime);}
                    else if(a<0.25){operator_lpt(ques[stage_ind], stage_ind, ptime);}
                    else if(a<0.375){operator_srpt(ques[stage_ind], stage_ind, ptime);}
                    else if(a<0.5){operator_ect(ques[stage_ind], stage_ind, ctime);}
                    else if(a<0.625){operator_est(ques[stage_ind], stage_ind, ctime, ptime);}
                    else if(a<0.75){operator_neh(ques[stage_ind], stage_ind, ptime);}
                    else if(a<0.875){operator_ritm(ques[stage_ind], stage_ind, ptime, mac_per_state);}
                }
                else if(ele.second[i]==3){
                    if(a<0.2){
                        int pos = min(5, len_q);
                        pos = int(gen()) % pos;
                        operator_insert_back(job_ind, ques[stage_ind], pos);
                    }
                    else if(a<0.3){operator_spt(ques[stage_ind], stage_ind, ptime);}
                    else if(a<0.4){operator_lpt(ques[stage_ind], stage_ind, ptime);}
                    else if(a<0.5){operator_srpt(ques[stage_ind], stage_ind, ptime);}
                    else if(a<0.6){operator_ect(ques[stage_ind],stage_ind, ctime);}
                    else if(a<0.7){operator_est(ques[stage_ind], stage_ind, ctime, ptime);}
                    else if(a<0.8){operator_neh(ques[stage_ind], stage_ind, ptime);}
                    else if(a<0.9){operator_ritm(ques[stage_ind], stage_ind, ptime, mac_per_state);}
                }
                i+=2; j+=2;
            }
        }
    }
}

void random_resched(vector<list<int>>& ques, const vector<list<int>>& r_list, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<int>& mac_per_stage, unsigned seed){
    mt19937 gen{seed};
    uniform_real_distribution<float> dist01(0,1);
    for(int stageind=0; stageind<mac_per_stage.size(); ++stageind){
        if(!r_list[stageind].empty()){
            int len_q = ques[stageind].size();
            int len_resched = r_list[stageind].size();
            if(len_q == 2){
                float a = dist01(gen);
                if(a<0.5){
                    auto it = ques[stageind].begin();
                    int temp = ques[stageind].front();
                    *it = ques[stageind].back();
                    ++it;
                    *it = temp;
                }
            }
            else{
                auto it1 = r_list[stageind].begin();
                auto it2 = r_list[stageind].begin();
                ++it2;
                while(it1!=r_list[stageind].end()){
                    float a = dist01(gen);
                    int job_ind = *it2;
                    if((*it1)==0){
                        if(a<0.15){
                            int pos = min(5, len_q);
                            pos = int(gen()) % pos;
                            operator_insert_back(job_ind, ques[stageind], pos);
                        }
                        else if(a<0.3){operator_spt(ques[stageind], stageind, ptime);}
                        else if(a<0.45){operator_lpt(ques[stageind], stageind, ptime);}
                        else if(a<0.6){operator_srpt(ques[stageind], stageind, ptime);}
                        else if(a<0.75){operator_neh(ques[stageind], stageind, ptime);}
                        else if(a<0.9){operator_ritm(ques[stageind], stageind, ptime, mac_per_stage);}
                    }
                    else if((*it1)==1){
                        // 1 insert front, 2 spt, 3 lpt, 4srpt, 5 ect, 6 est, 7 neh, 8 ritm 
                        if(a<0.166){operator_ritm(ques[stageind], stageind, ptime, mac_per_stage);}
                        else if(a<0.332){operator_neh(ques[stageind], stageind, ptime);}
                        else if(a<0.498){operator_spt(ques[stageind], stageind, ptime);}
                        else if(a<0.664){operator_ect(ques[stageind],stageind, ctime);}
                        else if(a<0.84){operator_est(ques[stageind], stageind, ctime, ptime);}
                                               
                        // if(a<0.2){operator_ritm(ques[stageind], stageind, ptime, mac_per_stage);}
                        // // if(a<0.2){
                        // //     int pos = min(5, len_q);
                        // //     pos = int(gen()) % pos;
                        // //     operator_insert_front(job_ind, ques[stageind], pos);
                        // // }
                        // else if(a<0.4){operator_spt(ques[stageind], stageind, ptime);}
                        // else if(a<0.6){operator_ect(ques[stageind],stageind, ctime);}
                        // else if(a<0.8){operator_est(ques[stageind], stageind, ctime, ptime);}
                        // // else if(a<0.75){operator_neh(ques[stageind], stageind, ptime);}
                        // // else if(a<0.8){
                        // //     int swap1 = int(gen()) % len_q;
                        // //     int swap2 = int(gen()) % len_q;
                        // //     while(swap1==swap2){
                        // //         swap2 = int(gen()) % len_q;
                        // //     }
                        // //     // swap_list(ques[stageind], min(swap1, swap2), max(swap1, swap2));
                        // //     swap_list(ques[stageind], swap1, swap2);
                        // // }
                        // // else if(a<0.875){operator_ritm(ques[stageind], stageind, ptime, mac_per_stage);}

                        // if(a<0.2){
                        //     int pos = min(5, len_q);
                        //     pos = int(gen()) % pos;
                        //     operator_insert_front(job_ind, ques[stageind], pos);
                        // }
                        // else if(a<0.3){operator_spt(ques[stageind], stageind, ptime);}
                        // else if(a<0.4){operator_lpt(ques[stageind], stageind, ptime);}
                        // else if(a<0.5){operator_srpt(ques[stageind], stageind, ptime);}
                        // else if(a<0.6){operator_ect(ques[stageind],stageind, ctime);}
                        // else if(a<0.7){operator_est(ques[stageind], stageind, ctime, ptime);}
                        // else if(a<0.8){operator_neh(ques[stageind], stageind, ptime);}
                        // // else if(a<0.8){
                        // //     int swap1 = int(gen()) % len_q;
                        // //     int swap2 = int(gen()) % len_q;
                        // //     while(swap1==swap2){
                        // //         swap2 = int(gen()) % len_q;
                        // //     }
                        // //     // swap_list(ques[stageind], min(swap1, swap2), max(swap1, swap2));
                        // //     swap_list(ques[stageind], swap1, swap2);
                        // // }
                        // else if(a<0.9){operator_ritm(ques[stageind], stageind, ptime, mac_per_stage);}
                    }
                    else if((*it1)==2){
                        if(a<0.125){operator_spt(ques[stageind], stageind, ptime);}
                        else if(a<0.25){operator_lpt(ques[stageind], stageind, ptime);}
                        else if(a<0.375){operator_srpt(ques[stageind], stageind, ptime);}
                        else if(a<0.5){operator_ect(ques[stageind], stageind, ctime);}
                        else if(a<0.625){operator_est(ques[stageind], stageind, ctime, ptime);}
                        else if(a<0.75){operator_neh(ques[stageind], stageind, ptime);}
                        else if(a<0.875){operator_ritm(ques[stageind], stageind, ptime, mac_per_stage);}
                    }
                    else if((*it1)==3){
                        if(a<0.2){
                            int pos = min(5, len_q);
                            pos = int(gen()) % pos;
                            operator_insert_back(job_ind, ques[stageind], pos);
                        }
                        else if(a<0.3){operator_spt(ques[stageind], stageind, ptime);}
                        else if(a<0.4){operator_lpt(ques[stageind], stageind, ptime);}
                        else if(a<0.5){operator_srpt(ques[stageind], stageind, ptime);}
                        else if(a<0.6){operator_ect(ques[stageind],stageind, ctime);}
                        else if(a<0.7){operator_est(ques[stageind], stageind, ctime, ptime);}
                        else if(a<0.8){operator_neh(ques[stageind], stageind, ptime);}
                        else if(a<0.9){operator_ritm(ques[stageind], stageind, ptime, mac_per_stage);}
                    }
                    ++it1; ++it1;
                    ++it2; ++it2;
                }
            }
        }
        else{continue;}
    }
}

void random_resched1(vector<list<int>>& ques, const vector<list<int>>& r_list, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<int>& mac_per_stage, unsigned seed){
    mt19937 gen{seed};
    uniform_real_distribution<float> dist01(0,1);
    for(int stageind=0; stageind<mac_per_stage.size(); ++stageind){
        if(!r_list[stageind].empty()){
            int len_q = ques[stageind].size();
            int len_resched = r_list[stageind].size();
            if(len_q == 2){
                float a = dist01(gen);
                if(a<0.5){
                    auto it = ques[stageind].begin();
                    int temp = ques[stageind].front();
                    *it = ques[stageind].back();
                    ++it;
                    *it = temp;
                }
            }
            else{
                auto it1 = r_list[stageind].begin();
                auto it2 = r_list[stageind].begin();
                ++it2;
                while(it1!=r_list[stageind].end()){
                    float a = dist01(gen);
                    int job_ind = *it2;
                    if((*it1)==0){
                        if(a<0.15){
                            int pos = min(5, len_q);
                            pos = int(gen()) % pos;
                            operator_insert_back(job_ind, ques[stageind], pos);
                        }
                        else if(a<0.3){operator_spt(ques[stageind], stageind, ptime);}
                        else if(a<0.45){operator_lpt(ques[stageind], stageind, ptime);}
                        else if(a<0.6){operator_srpt(ques[stageind], stageind, ptime);}
                        else if(a<0.75){operator_neh(ques[stageind], stageind, ptime);}
                        else if(a<0.9){operator_ritm(ques[stageind], stageind, ptime, mac_per_stage);}
                    }
                    else if((*it1)==1){
                        // 1 spt, 2 est, 3 ect, 4 neh, 5 ritm, 6 right shift
                        // if(a<0.16){operator_spt(ques[stageind], stageind, ptime);}
                        // else if(a<0.32){operator_est(ques[stageind], stageind, ctime, ptime);}
                        // else if(a<0.48){operator_ect(ques[stageind], stageind, ctime);}
                        // else if(a<0.64){operator_neh(ques[stageind], stageind, ptime);}
                        // else if(a<0.8){operator_ritm(ques[stageind], stageind, ptime, mac_per_stage);}
                        if(a<0.2){operator_est(ques[stageind], stageind, ctime, ptime);}
                        else if(a<0.4){operator_ect(ques[stageind], stageind, ctime);}
                        else if(a<0.6){operator_neh(ques[stageind], stageind, ptime);}
                        else if(a<0.8){operator_ritm(ques[stageind], stageind, ptime, mac_per_stage);}
                    }
                    else if((*it1)==2){
                        if(a<0.125){operator_spt(ques[stageind], stageind, ptime);}
                        else if(a<0.25){operator_lpt(ques[stageind], stageind, ptime);}
                        else if(a<0.375){operator_srpt(ques[stageind], stageind, ptime);}
                        else if(a<0.5){operator_ect(ques[stageind], stageind, ctime);}
                        else if(a<0.625){operator_est(ques[stageind], stageind, ctime, ptime);}
                        else if(a<0.75){operator_neh(ques[stageind], stageind, ptime);}
                        else if(a<0.875){operator_ritm(ques[stageind], stageind, ptime, mac_per_stage);}
                    }
                    else if((*it1)==3){
                        if(a<0.2){
                            int pos = min(5, len_q);
                            pos = int(gen()) % pos;
                            operator_insert_back(job_ind, ques[stageind], pos);
                        }
                        else if(a<0.3){operator_spt(ques[stageind], stageind, ptime);}
                        else if(a<0.4){operator_lpt(ques[stageind], stageind, ptime);}
                        else if(a<0.5){operator_srpt(ques[stageind], stageind, ptime);}
                        else if(a<0.6){operator_ect(ques[stageind],stageind, ctime);}
                        else if(a<0.7){operator_est(ques[stageind], stageind, ctime, ptime);}
                        else if(a<0.8){operator_neh(ques[stageind], stageind, ptime);}
                        else if(a<0.9){operator_ritm(ques[stageind], stageind, ptime, mac_per_stage);}
                    }
                    ++it1; ++it1;
                    ++it2; ++it2;
                }
            }
        }
        else{continue;}
    }
}

void random_resched_insert(vector<list<int>>& ques, const vector<list<int>>& r_list, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<int>& mac_per_stage, unsigned seed){
    mt19937 gen{seed};
    uniform_real_distribution<float> dist01(0,1);
    for(int stageind=0; stageind<mac_per_stage.size(); ++stageind){
        if(!r_list[stageind].empty()){
            int len_q = ques[stageind].size();
            int len_resched = r_list[stageind].size();
            if(len_q == 2){
                float a = dist01(gen);
                if(a<0.5){
                    auto it = ques[stageind].begin();
                    int temp = ques[stageind].front();
                    *it = ques[stageind].back();
                    ++it;
                    *it = temp;
                }
            }
            else{
                auto it1 = r_list[stageind].begin();
                auto it2 = r_list[stageind].begin();
                ++it2;
                while(it1!=r_list[stageind].end()){
                    float a = dist01(gen);
                    int job_ind = *it2;
                    if((*it1)==0){
                        if(a<0.15){
                            int pos = min(5, len_q);
                            pos = int(gen()) % pos;
                            operator_insert_back(job_ind, ques[stageind], pos);
                        }
                        else if(a<0.3){operator_spt(ques[stageind], stageind, ptime);}
                        else if(a<0.45){operator_lpt(ques[stageind], stageind, ptime);}
                        else if(a<0.6){operator_srpt(ques[stageind], stageind, ptime);}
                        else if(a<0.75){operator_neh(ques[stageind], stageind, ptime);}
                        else if(a<0.9){operator_ritm(ques[stageind], stageind, ptime, mac_per_stage);}
                    }
                    else if((*it1)==1){
                        // random insert, est, ect, neh, ritm, rs
                        if(a<0.2){
                            ques[stageind].remove(job_ind);
                            int pos = int(gen()) % len_q;
                            while(pos==0){
                                pos = int(gen()) % len_q;
                            }
                            auto insert_pos = ques[stageind].begin();
                            for(int foo=0; foo<pos; ++foo){++insert_pos;}
                            ques[stageind].insert(insert_pos, job_ind);
                        }
                        if(a<0.36){operator_est(ques[stageind], stageind, ctime, ptime);}
                        else if(a<0.52){operator_ect(ques[stageind], stageind, ctime);}
                        else if(a<0.68){operator_neh(ques[stageind], stageind, ptime);}
                        else if(a<0.84){operator_ritm(ques[stageind], stageind, ptime, mac_per_stage);}
                    }
                    else if((*it1)==2){
                        if(a<0.125){operator_spt(ques[stageind], stageind, ptime);}
                        else if(a<0.25){operator_lpt(ques[stageind], stageind, ptime);}
                        else if(a<0.375){operator_srpt(ques[stageind], stageind, ptime);}
                        else if(a<0.5){operator_ect(ques[stageind], stageind, ctime);}
                        else if(a<0.625){operator_est(ques[stageind], stageind, ctime, ptime);}
                        else if(a<0.75){operator_neh(ques[stageind], stageind, ptime);}
                        else if(a<0.875){operator_ritm(ques[stageind], stageind, ptime, mac_per_stage);}
                    }
                    else if((*it1)==3){
                        if(a<0.2){
                            int pos = min(5, len_q);
                            pos = int(gen()) % pos;
                            operator_insert_back(job_ind, ques[stageind], pos);
                        }
                        else if(a<0.3){operator_spt(ques[stageind], stageind, ptime);}
                        else if(a<0.4){operator_lpt(ques[stageind], stageind, ptime);}
                        else if(a<0.5){operator_srpt(ques[stageind], stageind, ptime);}
                        else if(a<0.6){operator_ect(ques[stageind],stageind, ctime);}
                        else if(a<0.7){operator_est(ques[stageind], stageind, ctime, ptime);}
                        else if(a<0.8){operator_neh(ques[stageind], stageind, ptime);}
                        else if(a<0.9){operator_ritm(ques[stageind], stageind, ptime, mac_per_stage);}
                    }
                    ++it1; ++it1;
                    ++it2; ++it2;
                }
            }
        }
        else{continue;}
    }

}

*/

void operator_insert_front(const int job_ind, list<int> &q, const int position){
    q.remove(job_ind);
    auto it = q.begin();
    for(int i=0; i<position; ++i){++it;}
    q.insert(it, job_ind);
}

void operator_insert_back(const int job_ind, list<int> &q, const int position){
    q.remove(job_ind);
    auto it = q.end();
    for(int i=0; i<position; ++i){--it;}
    q.insert(it, job_ind);
}

void operator_spt(list<int> &q, const int stage, const vector<vector<int>> &ptime){
    // list<vector<int>> temp;
    vector<vector<int>> temp;
    for(auto &i:q){
        temp.emplace_back(vector<int>{i, ptime[i][stage]});
    }
    sort(temp.begin(), temp.end(), less_ivec);

    auto it1=q.begin();
    for(int i=0; i<temp.size(); ++i){
        *it1 = temp[i].front();
        ++it1;
    }
}

void operator_lpt(list<int> &q, const int stage, const vector<vector<int>> &ptime){
    vector<vector<int>> temp;
    for(auto &i:q){
        temp.emplace_back(vector<int>{i, ptime[i][stage]});
    }
    sort(temp.begin(), temp.end(), greater_ivec);
    auto it1=q.begin();

    for(int i=0; i<temp.size(); ++i){
        *it1 = temp[i].front();
        ++it1;
    }
}


void operator_srpt(list<int>& q, const int stage, const vector<vector<int>>& ptime){
    vector<vector<int>> temp;
    for(auto &i:q){
        int sum = 0;
        for(int k=stage; k<ptime[i].size(); ++k){
            sum += ptime[i][k];
        }
        temp.emplace_back(vector<int>{i, sum});
    }
    sort(temp.begin(), temp.end(), less_ivec);
    auto it1=q.begin();
    for(int i=0; i<temp.size(); ++i){
        *it1 = temp[i].front();
        ++it1;
    }
}

void operator_ect(list<int>& q, const int stage, const vector<vector<int>>& ctime){
    vector<vector<int>> temp;
    for(auto &i:q){
        temp.emplace_back(vector<int>{i, ctime[i][stage]});
    }
    // cerr<<"1111111111"<<endl;
    sort(temp.begin(), temp.end(), less_ivec);
    auto it1=q.begin();
    for(int i=0; i<temp.size(); ++i){
        *it1 = temp[i].front();
        ++it1;
    }
}

void operator_est(list<int>& q, const int stage, const vector<vector<int>>& ctime, const vector<vector<int>>& ptime){
    vector<vector<int>> temp;
    for(auto &i:q){
        int foo = ctime[i][stage] - ptime[i][stage];
        temp.emplace_back(vector<int>{i, foo});
    }
    sort(temp.begin(), temp.end(), less_ivec);
    auto it1=q.begin();
    for(int i=0; i<temp.size(); ++i){
        *it1 = temp[i].front();
        ++it1;
    }   
}

void operator_neh(list<int>& q, const int stage, const vector<vector<int>>& ptime){
    vector<int> qvec(q.size());
    copy(q.begin(), q.end(), qvec.begin());
    vector<vector<int>> partial_ptime(q.size(), vector<int>());
    // cerr<<endl;
    for(int k=0; k<qvec.size(); ++k){
        int foo = qvec[k];
        auto itb = ptime[foo].begin() + stage;
        auto ite = ptime[foo].end();
        partial_ptime[k].assign(itb, ite);
    }
    list<int> qnew = NEH_func(partial_ptime);
    auto it = q.begin();
    for(auto &ind:qnew){
        *it = qvec[ind];
        ++it;
    }
}

void operator_neh1(list<int>& q, const int stage, const vector<vector<int>>& ptime){
    vector<int> qvec(q.size());
    copy(q.begin(), q.end(), qvec.begin());
    vector<vector<int>> partial_ptime(q.size(), vector<int>());
    // cerr<<endl;
    for(int k=0; k<qvec.size(); ++k){
        int foo = qvec[k];
        auto itb = ptime[foo].begin() + stage;
        auto ite = ptime[foo].end();
        partial_ptime[k].assign(itb, ite);
    }
    list<int> qnew = NEH1_func(partial_ptime);
    auto it = q.begin();
    for(auto &ind:qnew){
        *it = qvec[ind];
        ++it;
    }
}

void operator_nehnm(list<int>& q, const int stage, const vector<vector<int>>& ptime){
    vector<int> qvec(q.size());
    copy(q.begin(), q.end(), qvec.begin());
    vector<vector<int>> partial_ptime(q.size(), vector<int>());
    for(int k=0; k<qvec.size(); ++k){
        int foo = qvec[k];
        auto itb = ptime[foo].begin() + stage;
        auto ite = ptime[foo].end();
        partial_ptime[k].assign(itb, ite);
    }
    list<int> qnew = NEHNM_func(partial_ptime);
    auto it = q.begin();
    for(auto &ind:qnew){
        *it = qvec[ind];
        ++it;
    }
}

void operator_nehnm1(list<int>& q, const int stage, const vector<vector<int>>& ptime){
    vector<int> qvec(q.size());
    copy(q.begin(), q.end(), qvec.begin());
    vector<vector<int>> partial_ptime(q.size(), vector<int>());
    for(int k=0; k<qvec.size(); ++k){
        int foo = qvec[k];
        auto itb = ptime[foo].begin() + stage;
        auto ite = ptime[foo].end();
        partial_ptime[k].assign(itb, ite);
    }
    list<int> qnew = NEHNM1_func(partial_ptime);
    auto it = q.begin();
    for(auto &ind:qnew){
        *it = qvec[ind];
        ++it;
    }
}

void operator_nehkk(list<int>& q, const int stage, const vector<vector<int>>& ptime){
    vector<int> qvec(q.size());
    copy(q.begin(), q.end(), qvec.begin());
    vector<vector<int>> partial_ptime(q.size(), vector<int>());
    for(int k=0; k<qvec.size(); ++k){
        int foo = qvec[k];
        auto itb = ptime[foo].begin() + stage;
        auto ite = ptime[foo].end();
        partial_ptime[k].assign(itb, ite);
    }
    list<int> qnew = NEHKK_func(partial_ptime);
    auto it = q.begin();
    for(auto &ind:qnew){
        *it = qvec[ind];
        ++it;
    }
}

void operator_ritm(list<int> &q, const int stage, const vector<vector<int>> &ptime, const vector<int> &m_per_state){
    int njob = q.size();
    int nstage = m_per_state.size() - stage;

    vector<int> qvec(q.size());
    copy(q.begin(), q.end(), qvec.begin());

    vector<int> partial_mstage(nstage);
    auto it_temp = m_per_state.begin()+stage;
    copy(it_temp, m_per_state.end(), partial_mstage.begin());

    vector<vector<int>> partial_ptime(q.size(), vector<int>());
    for(int k=0; k<qvec.size(); ++k){
        int foo = qvec[k];
        auto itb = ptime[foo].begin() + stage;
        auto ite = ptime[foo].end();
        partial_ptime[k].assign(itb, ite);
    }

    vector<int> qnew = Algo_RITM_coded(njob, nstage, partial_mstage, partial_ptime, 0.5, 1.2);
    auto it = q.begin();
    for(auto &ind:qnew){
        *it = qvec[ind];
        ++it;
    }
}


//基于 neh 的recursive insert 方法
void local_ri_neh(const int current_time, const int stage_ind, const int num_stage, const vector<int>& m_per_stage, const vector<vector<int>>& ptime, vector<list<int>>& ques, vector<vector<array<int, 4>>> mstate, vector<array<int, 3>> dispatch, const vector<vector<vector<float>>>& br_p, const vector<vector<vector<float>>>& eff_p){
    // create partial process time matrix
    list<int> partial_seq;
    vector<int> inds;
    vector<vector<int>> partial_ptime;  //工件在此阶段及之后阶段的局部加工时间矩阵
    int fooind = 0;
    int num_fixed = 0;
    for(int s=num_stage-1; s>=stage_ind; s--){  //从后向前依次填充局部加工时间矩阵
        if(mstate[s][0][0]==1){
            int temp_job_ind = mstate[s][0][3];
            int temp_time = ptime[temp_job_ind][s] - current_time + dispatch[temp_job_ind][1];
            auto itb = ptime[temp_job_ind].begin() + stage_ind;
            auto ite = ptime[temp_job_ind].end();
            vector<int> tempvec;
            tempvec.assign(itb, ite);
            for(int i=0; i<s-stage_ind; i++){
                tempvec[i] = 0;
            }
            tempvec[s-stage_ind] = temp_time;
            partial_seq.emplace_back(fooind);
            fooind++;
            partial_ptime.emplace_back(tempvec);
            num_fixed++;
        }
        else if(mstate[s][0][1]==0){
            vector<int> tempvec(num_stage-stage_ind, 0);
            tempvec[s-stage_ind] = round(1/(1-br_p[0][s][0])); //将机器的修复过程作为一个 null job 加入到工件列表中
            partial_seq.emplace_back(fooind);
            fooind++;
            partial_ptime.emplace_back(tempvec);
            num_fixed++;
        }
        if(s>stage_ind && (!ques[s].empty())){
            vector<vector<int>> tempvvec(ques[s].size(), vector<int>(num_stage-stage_ind, 0));
            int tempcount = 0;
            for(auto jobiter=ques[s].begin(); jobiter!=ques[s].end(); ++jobiter){
                int temp_job_ind = *jobiter;
                int tempi = 0;
                for(int i=s; i<num_stage; i++){
                    tempvvec[tempcount][tempi] = ptime[temp_job_ind][i];
                    tempi++;
                }
                tempcount++;
                partial_seq.emplace_back(fooind);
                fooind++;
            }
            partial_ptime.insert(partial_ptime.end(), tempvvec.begin(), tempvvec.end());
        }
        else if(!ques[s].empty()){
            vector<vector<int>> tempvvec(ques[s].size(), vector<int>(num_stage-stage_ind,0));
            int tempcount = 0;
            for(auto jobiter=ques[s].begin(); jobiter!=ques[s].end(); ++jobiter){
                int temp_job_ind = *jobiter;
                inds.emplace_back(temp_job_ind);
                int tempi = 0;
                for(int i=stage_ind; i<num_stage; i++){
                    tempvvec[tempcount][tempi] = ptime[temp_job_ind][i];
                    tempi++;
                }
                tempcount++;
            }
            partial_ptime.insert(partial_ptime.end(), tempvvec.begin(), tempvvec.end());
        }
    }
    ques[stage_ind] = partial_fixed_NEH(partial_ptime, partial_seq, inds);  //局部 NEH 算法
    return;
}

//将工件按照剩余加工时长降序排序后依次插入队尾
void local_ri_end(const int stage_ind, const int num_stage, const vector<vector<int>>& ptime, vector<list<int>>& ques){
    if(ques[stage_ind].size()==0) return;
    const int jobs = ques[stage_ind].size();

    //对队列中的工件按照剩余加工时长降序排序
    vector<array<int, 2>> summed_p_time(jobs, array<int, 2>{});
    int foo = 0;
    for(auto it=ques[stage_ind].begin(); it!=ques[stage_ind].end(); ++it){
        int temp_job_ind = *it;
        int total_ptime = 0;
        for(int s=stage_ind; s<num_stage; ++s){
            total_ptime += ptime[temp_job_ind][s];
        }
        summed_p_time[foo][0] = temp_job_ind;
        summed_p_time[foo][1] = total_ptime;
        ++foo;
    }
    sort(summed_p_time.begin(), summed_p_time.end(), greater_iarray2);
    ques[stage_ind].clear();
    for(auto &ele:summed_p_time){
        ques[stage_ind].emplace_back(ele[0]);
    }
    return;
}

//将工件按照剩余加工时间降序排序后，奇数位的工件插入队尾，偶数位的工件插入队首
void local_ri_oddeven(const int stage_ind, const int num_stage, const vector<vector<int>>& ptime, vector<list<int>>& ques){
    if(ques[stage_ind].size()==0) return;
    const int jobs = ques[stage_ind].size();
    vector<array<int, 2>> summed_p_time(jobs, array<int, 2>{});
    int foo = 0;
    for(auto it=ques[stage_ind].begin(); it!=ques[stage_ind].end(); ++it){
        int temp_job_ind = *it;
        int total_ptime = 0;
        for(int s=stage_ind; s<num_stage; ++s){
            total_ptime += ptime[temp_job_ind][s];
        }
        summed_p_time[foo][0] = temp_job_ind;
        summed_p_time[foo][1] = total_ptime;
        ++foo;
    }
    sort(summed_p_time.begin(), summed_p_time.end(), greater_iarray2);
    ques[stage_ind].clear();
    for(int i=0; i<summed_p_time.size(); ++i){
        if(i%2){
            ques[stage_ind].emplace_back(summed_p_time[i][0]);
        }
        else{
            ques[stage_ind].emplace_front(summed_p_time[i][0]);
        }
    }
    return;
}


//从最后一个stage到第一个stage，依次对工件队列进行顺序调整（随机选择排序算子）
void random_global_resched(vector<list<int>>& ques, const int current_time, const int num_stage, const vector<vector<int>>& ptime, const vector<vector<int>>& ctime, const vector<int>& m_per_stage, const vector<vector<array<int, 4>>>& mstate, const vector<array<int, 3>>& dispatch, const vector<vector<vector<float>>>& br_p, const vector<vector<vector<float>>>& eff_p, unsigned seed){
    mt19937 gen{seed};
    uniform_real_distribution<float> dist01(0,1);
    for(int stageind=num_stage-1; stageind>=0; stageind--){
        if(ques[stageind].size()>2){
            float a = dist01(gen);
            if(a<0.25){operator_est(ques[stageind], stageind, ctime, ptime);}
            else if(a<0.5){operator_neh(ques[stageind], stageind, ptime);}
            else if(a<0.75){operator_ritm(ques[stageind], stageind, ptime, m_per_stage);}

            // if(a<0.17){operator_est(ques[stageind], stageind, ctime, ptime);}
            // else if(a<0.34){operator_neh(ques[stageind], stageind, ptime);}
            // else if(a<0.51){operator_ritm(ques[stageind], stageind, ptime, m_per_stage);}
            // else if(a<0.6){local_ri_neh(current_time, stageind, num_stage, m_per_stage, ptime, ques, mstate, dispatch, br_p, eff_p);}
            // else if(a<0.68){local_ri_end(stageind, num_stage, ptime, ques);}
            // else if(a<0.85){local_ri_oddeven(stageind, num_stage, ptime, ques);}
        }
    }
    return;
}