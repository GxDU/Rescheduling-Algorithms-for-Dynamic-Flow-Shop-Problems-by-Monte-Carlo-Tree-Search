#include"funcs.h"


vector<vector<vector<float>>> calculate_exp_time(const vector<vector<int>>& p_time, const int num_job, const int num_stage, const vector<int>& m_per_stage, const vector<vector<vector<float>>>& br_p, const vector<vector<vector<float>>>& effi_p){
    vector<vector<vector<float>>> exptime(num_job, vector<vector<float>>(num_stage));
    for(int j=0; j<num_job; ++j){
        for(int s=0; s<num_stage; ++s){
            for(int m=0; m<m_per_stage[s]; ++m){
                float t = static_cast<float>(p_time[j][s]);
                if(effi_p[1][s][m]<1){
                    t = t * ((1-effi_p[1][s][m])/(1-effi_p[0][s][m]) + 1);
                }
                if(br_p[1][s][m]<1){
                    t = (1/pow(br_p[1][s][m], t) - 1) * (1/(1-br_p[0][s][m]) + 1/(1-br_p[1][s][m]));
                    // t = static_cast<float>(1/pow((double)br_p[1][s][m], (double)t) - 1) * (1/(1-br_p[0][s][m]) + 1/(1-br_p[1][s][m]));
                    // t = static_cast<float>(1/pow(br_p[1][s][m], t) - 1) * (1/(1-br_p[0][s][m]) + 1/(1-br_p[1][s][m]))+1;
                }
                exptime[j][s].emplace_back(t);
            }
        }
    }
    return exptime;
}

// float exp_span(const int current_time, const int stage_ind, const int num_job, const int num_stage, const vector<int>& m_per_stage, const vector<vector<int>>& p_time, const vector<vector<vector<float>>>& exp_p_time, vector<vector<array<int,4>>> mstate, vector<list<int>> ques, vector<array<int,3>> dispatch, const vector<vector<vector<float>>>& br_p, const vector<vector<vector<float>>>& eff_p){
float exp_span(const int current_time, const int stage_ind, const int num_stage, const vector<int>& m_per_stage, const vector<vector<int>>& p_time, const vector<vector<vector<float>>>& exp_p_time, vector<vector<array<int,4>>> mstate, vector<list<int>> ques, vector<array<int,3>> dispatch, const vector<vector<vector<float>>>& br_p, const vector<vector<vector<float>>>& eff_p){
    const float epsl = 0.00001;
    const float minusepsl = -0.00001;
    float fcurrent_time = static_cast<float>(current_time);
    vector<vector<float>> rtimes(num_stage, vector<float>());
    // change broken machines to normal state with dull job and calculate the expected repair time
    // calculate the expected completion time of low-efficiency machine
    for(int s = stage_ind; s<num_stage; ++s){
        for(int m=0; m<m_per_stage[s]; ++m){
            if(mstate[s][m][0]==1){
                if(mstate[s][m][2]==0){
                    float temptime = 1.0 / (1-eff_p[0][s][m]);
                    temptime += ((1-eff_p[1][s][m]) / (1-eff_p[0][s][m]) + 1) * static_cast<float>(p_time[mstate[s][m][3]][s] - current_time + dispatch[mstate[s][m][3]][1]);
                    rtimes[s].emplace_back(temptime);
                    mstate[s][m][2] = 1;
                }
                else if(mstate[s][m][2]==1){
                    if(current_time - dispatch[mstate[s][m][3]][1] == p_time[mstate[s][m][3]][s]){
                        int temp_job_ind = mstate[s][m][3];
                        mstate[s][m][0] = 0;
                        mstate[s][m][3] = -1;
                        if(s < num_stage-1){
                            ques[s+1].emplace_back(mstate[s][m][3]);
                        }
                        dispatch[temp_job_ind][0] = -1;
                        dispatch[temp_job_ind][1] = -1;
                        dispatch[temp_job_ind][2] = -1;
                        // float temptime = minusepsl;
                        // rtimes[s].emplace_back(minusepsl);
                    }
                    else{
                        float temptime = ((1-eff_p[1][s][m]) / (1-eff_p[0][s][m]) + 1) * static_cast<float>(p_time[mstate[s][m][3]][s] - current_time + dispatch[mstate[s][m][3]][1]);
                        rtimes[s].emplace_back(temptime);
                    }
                }
            }
            else if(mstate[s][m][0]==0){
                // no job on this machine
                if(mstate[s][m][1]==0){
                    //the machine is broken, add an dull job
                    float temptime = 1.0 / (1-br_p[0][s][m]);                                                                          
                    rtimes[s].emplace_back(temptime);
                    mstate[s][m][0] = 1;
                    mstate[s][m][1] = 1;
                    mstate[s][m][2] = 1;
                    mstate[s][m][3] = -2;
                }
                else{
                    rtimes[s].emplace_back(minusepsl);
                }
            }
        }
    }

    //loop till production ends
    bool not_termi = false;
    for(int s=stage_ind; s<num_stage; ++s){
        if(ques[s].size()>0){
            not_termi = true;
            break;
        }
        for(int m=0; m<m_per_stage[s]; ++m){
            if(mstate[s][m][0]==1){
                not_termi = true;
            }
        }
        if(not_termi){break;}
    }
    // int count = 0;
    while(not_termi){
    // // while(not_termi && count<100){
    //     ++count;
        // dispatch available mahcine to wating job
        for(int s=stage_ind; s<num_stage; ++s){
            if(ques[s].empty()) continue;
            // ques[s] not empty, then find available machien to process the job
            for(int m=0; m<m_per_stage[s]; ++m){
                if(ques[s].empty()) break;
                if(mstate[s][m][0]==0 && mstate[s][m][1]==1){
                    int ind_job = ques[s].front();
                    ques[s].pop_front();
                    mstate[s][m][0] = 1;
                    mstate[s][m][3] = ind_job;
                    dispatch[ind_job][0] = m;
                    rtimes[s][m] = exp_p_time[ind_job][s][m];
                }
            }
        }
        // find the job that will finish first and record the time step
        float tstep = 9999999.0;
        for(int s=stage_ind; s<num_stage; ++s){
            for(int m=0; m<m_per_stage[s]; ++m){
                if(rtimes[s][m]>0 && rtimes[s][m]<tstep){
                    tstep = rtimes[s][m];
                }
            }
        }
        // go to next time-point, update information of machine state
        for(int s=stage_ind; s<num_stage; ++s){
            for(int m=0; m<m_per_stage[s]; ++m){
                if(rtimes[s][m]>0){
                    rtimes[s][m] -= tstep;
                    if(rtimes[s][m]<epsl){
                        rtimes[s][m] = minusepsl;
                        int temp_job_ind = mstate[s][m][3];
                        mstate[s][m][0] = 0;
                        mstate[s][m][3] = -1;
                        if(temp_job_ind != -2){
                            dispatch[temp_job_ind][0] = -1;
                            // dispatch[temp_job_ind][1] = -1;
                            // dispatch[temp_job_ind][2] = -1;
                            if(s<num_stage-1){
                                ques[s+1].emplace_back(temp_job_ind);
                            }
                        }
                    }
                }
            }
        }
        fcurrent_time += tstep;
        // check if production is done;
        not_termi = false;
        for(int s=stage_ind; s<num_stage; ++s){
            if(ques[s].size()>0){
                not_termi = true;
                break;
            }
            for(int m=0; m<m_per_stage[s]; ++m){
                if(mstate[s][m][0]==1){
                    not_termi = true;
                }
            }
            if(not_termi){break;}
        }
    }

    return fcurrent_time;
}

// float partial_span_static(const int current_time, const int stage_ind, const int num_job, const int num_stage, const vector<int>& m_per_stage, const vector<vector<int>>& p_time, vector<vector<array<int,4>>> mstate, vector<list<int>> ques, vector<array<int,3>> dispatch, const vector<vector<vector<float>>>& br_p, const vector<vector<vector<float>>>& eff_p){
int partial_span_static(const int current_time, const int stage_ind, const int num_stage, const vector<int>& m_per_stage, const vector<vector<int>>& p_time, vector<vector<array<int,4>>> mstate, vector<list<int>> ques, vector<array<int,3>> dispatch, const vector<vector<vector<float>>>& br_p, const vector<vector<vector<float>>>& eff_p){
    int t = current_time;
    vector<vector<int>> rtimes(num_stage, vector<int>());
    // change broken machines to normal state with dull job and calculate the expected repair time
    // calculate the expected completion time of low-efficiency machine
    for(int s = stage_ind; s<num_stage; ++s){
        for(int m=0; m<m_per_stage[s]; ++m){
            if(mstate[s][m][0]==1){
                if(mstate[s][m][2]==0){
                    // float temptime = 1.0 / (1-eff_p[0][s][m]);
                    // temptime += ((1-eff_p[1][s][m]) / (1-eff_p[0][s][m]) + 1) * static_cast<float>(p_time[mstate[s][m][3]][s] - current_time + dispatch[mstate[s][m][3]][1]);
                    // rtimes[s].emplace_back(round(temptime));
                    int temptime = p_time[mstate[s][m][3]][s] - current_time + dispatch[mstate[s][m][3]][1];
                    rtimes[s].emplace_back(temptime);
                    mstate[s][m][2] = 1;
                }
                else if(mstate[s][m][2]==1){
                    if(current_time - dispatch[mstate[s][m][3]][1] == p_time[mstate[s][m][3]][s]){
                        int temp_job_ind = mstate[s][m][3];
                        mstate[s][m][0] = 0;
                        mstate[s][m][3] = -1;
                        if(s < num_stage-1){
                            ques[s+1].emplace_back(mstate[s][m][3]);
                        }
                        dispatch[temp_job_ind][0] = -1;
                        dispatch[temp_job_ind][1] = -1;
                        dispatch[temp_job_ind][2] = -1;
                        // float temptime = minusepsl;
                        rtimes[s].emplace_back(0);
                    }
                    else{
                        // float temptime = ((1-eff_p[1][s][m]) / (1-eff_p[0][s][m]) + 1) * static_cast<float>(p_time[mstate[s][m][3]][s] - current_time + dispatch[mstate[s][m][3]][1]);
                        // rtimes[s].emplace_back(round(temptime));
                        int temptime = p_time[mstate[s][m][3]][s] - current_time + dispatch[mstate[s][m][3]][1];
                        rtimes[s].emplace_back(temptime);
                    }
                }
            }
            else if(mstate[s][m][0]==0){
                // no job on this machine
                if(mstate[s][m][1]==0){
                    //the machine is broken, add an dull job
                    float temptime = 1.0 / (1-br_p[0][s][m]);                                                                          
                    // rtimes[s].emplace_back(temptime);
                    rtimes[s].emplace_back(round(temptime));
                    mstate[s][m][0] = 1;
                    mstate[s][m][1] = 1;
                    mstate[s][m][2] = 1;
                    mstate[s][m][3] = -2;
                }
                else{
                    // rtimes[s].emplace_back(minusepsl);
                    rtimes[s].emplace_back(0);
                }
            }
        }
    }

    //loop till production ends
    bool not_termi = false;
    for(int s=stage_ind; s<num_stage; ++s){
        if(ques[s].size()>0){
            not_termi = true;
            break;
        }
        for(int m=0; m<m_per_stage[s]; ++m){
            if(mstate[s][m][0]==1){
                not_termi = true;
            }
        }
        if(not_termi){break;}
    }
    // int count = 0;
    while(not_termi){
    // // while(not_termi && count<100){
    //     ++count;
        // dispatch available mahcine to wating job
        for(int s=stage_ind; s<num_stage; ++s){
            if(ques[s].empty()) continue;
            // ques[s] not empty, then find available machien to process the job
            for(int m=0; m<m_per_stage[s]; ++m){
                if(ques[s].empty()) break;
                if(mstate[s][m][0]==0 && mstate[s][m][1]==1){
                    int ind_job = ques[s].front();
                    ques[s].pop_front();
                    mstate[s][m][0] = 1;
                    mstate[s][m][3] = ind_job;
                    dispatch[ind_job][0] = m;
                    // rtimes[s][m] = exp_p_time[ind_job][s][m];
                    rtimes[s][m] = p_time[ind_job][s];
                }
            }
        }
        // find the job that will finish first and record the time step
        int tstep = 9999999;
        for(int s=stage_ind; s<num_stage; ++s){
            for(int m=0; m<m_per_stage[s]; ++m){
                if(rtimes[s][m]>0 && rtimes[s][m]<tstep){
                    tstep = rtimes[s][m];
                }
            }
        }
        // go to next time-point, update information of machine state
        for(int s=stage_ind; s<num_stage; ++s){
            for(int m=0; m<m_per_stage[s]; ++m){
                if(rtimes[s][m]>0){
                    rtimes[s][m] -= tstep;
                    if(rtimes[s][m]==0){
                        rtimes[s][m] = 0;
                        int temp_job_ind = mstate[s][m][3];
                        mstate[s][m][0] = 0;
                        mstate[s][m][3] = -1;
                        if(temp_job_ind != -2){
                            dispatch[temp_job_ind][0] = -1;
                            dispatch[temp_job_ind][1] = -1;
                            dispatch[temp_job_ind][2] = -1;
                            if(s<num_stage-1){
                                ques[s+1].emplace_back(temp_job_ind);
                            }
                        }
                    }
                }
            }
        }
        t += tstep;
        // check if production is done;
        not_termi = false;
        for(int s=stage_ind; s<num_stage; ++s){
            if(ques[s].size()>0){
                not_termi = true;
                break;
            }
            for(int m=0; m<m_per_stage[s]; ++m){
                if(mstate[s][m][0]==1){
                    not_termi = true;
                }
            }
            if(not_termi){break;}
        }
    }
    return t;
}

void print_1d_vector(const vector<int>& vec){
    for(auto it1=begin(vec); it1!=end(vec); ++it1){
        cout<<*it1<<", ";
    }
    cout<<endl;
}

void print_1d_list(const list<int>& l){
    for(auto it=begin(l); it!=end(l); ++it){
        cout<< *it <<", ";
    }
    cout<<endl;
}

void print_2d_vector(const vector<vector<int>>& vec){
    int max = 0;
    for(auto it1=begin(vec); it1!=end(vec); ++it1){
        for(auto it2=begin(*it1); it2!=end(*it1); ++it2){
            if(*it2 > max){
                max = *it2;
            }
        }
    }

    int lenth = ceil(log10(max));

    for(auto it1=begin(vec); it1!=end(vec); ++it1){
        for(auto it2=begin(*it1); it2!=end(*it1); ++it2){
            int num = lenth - ceil(log10((*it2)+1));
            for(int foo=0; foo<num; ++foo){cout<<" ";}
            cout<<*it2<<" ";
        }
        cout<<endl;
    }
}

void print_list_of_vec(const list<vector<int>>& lov){
    int max = 0;
    for(auto it1=begin(lov); it1!=end(lov); ++it1){
        for(auto it2=begin(*it1); it2!=end(*it1); ++it2){
            if(*it2 > max){
                max = *it2;
            }
        }
    }
    int len = ceil(log10(max));
    for(auto it1=begin(lov); it1!=end(lov); ++it1){
        cout<<"(";
        for(auto it2=begin(*it1); it2!=end(*it1); ++it2){
            int num = len - ceil(log10((*it2)+1));
            for(int foo=0; foo<num; ++foo){cout<<" ";}
            cout<<*it2<<" ";
        }
        cout<<")"<<endl;
    }
}

void print_list_of_vec_1line(const list<vector<int>>& lov){
    int max = 0;
    for(auto it1=begin(lov); it1!=end(lov); ++it1){
        for(auto it2=begin(*it1); it2!=end(*it1); ++it2){
            if(*it2 > max){
                max = *it2;
            }
        }
    }
    int len = ceil(log10(max));
    for(auto it1=begin(lov); it1!=end(lov); ++it1){
        cout<<"(";
        for(auto it2=begin(*it1); it2!=end(*it1); ++it2){
            int num = len - ceil(log10((*it2)+1));
            for(int foo=0; foo<num; ++foo){cout<<" ";}
            cout<<*it2<<" ";
        }
        cout<<"),";
    }
    cout<<endl;
}

void print_string(const string& str){
    for(auto it=begin(str); it!=end(str); ++it){
        cout<<*it;
    }
}


bool compare_vv(const vector<vector<int>>& a, const vector<vector<int>>& b){
    if(a.size()!=b.size()) return false;
    if(a.empty()) return true;
    for(unsigned i=0; i<a.size(); ++i){
        if(a[i] != b[i]) return false;
    }
    return true;
}

bool compare_vl(const vector<list<int>>& a, const vector<list<int>>& b){
    if(a.size()!=b.size()) return false;
    if(a.empty()) return true;
    for(unsigned i=0; i<a.size(); ++i){
        if(a[i] != b[i]) return false;
    }
    return true;
}

bool compare_va3(const vector<array<int,3>> &a, const vector<array<int, 3>> &b){
    if(a.size() != b.size()) return false;
    if(a.empty()) return true;
    for(unsigned i=0; i<a.size(); ++i){
        if(a[i]!=b[i]) return false;
    }
    return true;
}

bool compare_vvv(const vector<vector<vector<int>>>& a, const vector<vector<vector<int>>>& b){
    if(a.size()!=b.size()) return false;
    if(a.empty()) return true;
    for(unsigned i=0; i<a.size(); ++i){
        if(!compare_vv(a[i], b[i])) return false;
    }
    return true;
}

bool compare_vva4(const vector<vector<array<int,4>>>& a, const vector<vector<array<int,4>>>& b){
    if(a.size()!=b.size()) return false;
    if(a.empty()) return true;
    for(unsigned i=0; i<a.size(); ++i){
        for(int j=0; j<a[i].size(); ++j){
            if(a[i][j]!=b[i][j]) return false;
        }
    }
    return true;
}

bool compare_mia2(const map<int, array<int,2>> &a, const map<int, array<int,2>> &b){
    if(a.size() != b.size()) return false;
    if(a.empty()) return true;
    // auto it1 = a,begin();
    // auto it2 = b.begin();
    for(auto &i : a){
        if((!b.count(i.first)) || (i.second!=b.at(i.first))) return false;
    }
    return true;
}

bool compare_mia3(const map<int, array<int,3>> &a, const map<int, array<int,3>> &b){
    if(a.size() != b.size()) return false;
    if(a.empty()) return true;
    // auto it1 = a,begin();
    // auto it2 = b.begin();
    for(auto &i : a){
        if((!b.count(i.first)) || (i.second!=b.at(i.first))) return false;
    }
    return true;
}

unsigned sum_matrix(const vector<vector<int>> mat){
    unsigned sum = 0;
    for(auto &r:mat){
        for(auto &e:r){
            sum += static_cast<unsigned>(e);
        }
    }
    return sum;
}

// unsigned sum_matrix(const vector<vector<int>> mat){
//     unsigned sum = 0;
//     for(auto &r:mat){
//         for(auto &e:r){
//             sum += e;
//         }
//     }
//     return sum;
// }

int roulette_from_ivec(const vector<int>& ivec){
    const int len = ivec.size();
    int sumi = 0;
    for(auto i : ivec){
        sumi += i;
    }
    float sumf = static_cast<float>(sumi);
    float p[len]{};
    float q[len]{};
    p[0] = static_cast<float>(ivec[0])/sumf;
    q[0] = p[0];
    for(int foo=1; foo<len; ++foo){
        p[foo] = static_cast<float>(ivec[foo])/sumf;
        q[foo] = q[foo-1] + p[foo];
    }
    random_device rd; mt19937 gen{rd()};
    uniform_real_distribution<float> dist1(0,1);
    float temp = dist1(gen);
    int index = 0;
    for(int foo=0; foo<len; ++foo){
        if(temp<q[foo]){
            index = foo;
            break;
        }
    }
    return index;
}

int roulette_from_pvec(const vector<float>& fvec){
    const int len = fvec.size();
    float q[len]{};
    q[0] = fvec[0];
    for(int foo=0; foo<len; ++foo){
        q[foo] = q[foo-1] + fvec[foo];
    }
    random_device rd; mt19937 gen{rd()};
    uniform_real_distribution<float> dist1(0,1);
    float temp = dist1(gen);
    int index = 0;
    for(int foo=0; foo<len; ++foo){
        if(temp<q[foo]){
            index = foo;
            break;
        }
    }
    return index;
}

void swap_list(list<int>& q, const int i1, const int i2){
    auto it1 = q.begin();
    auto it2 = q.begin();
    for(int i=0; i<i1; ++i){
        ++it1;
    }
    for(int i=0; i<i2; ++i){
        ++it2;
    }
    int temp1 = *it2;
    // it2 = q.erase(it2);
    // q.insert(it2, *it1);
    // it1 = q.erase(it1);
    // q.insert(it1, temp1);
    *it2 = *it1;
    *it1 = temp1;
    return;
}

void forward_insert_list(list<int> &q, const int i1, const int i2){
    const int l = min(i1, i2);
    const int r = max(i1, i2);
    auto it1 = q.begin();
    for(int i=0; i<l; ++i){++it1;}
    int temp = *it1;
    q.erase(it1);
    auto it2 = q.begin();
    for(int i=0; i<r; ++i){++it2;}
    q.insert(it2, temp);
    return;
}

void backward_insert_list(list<int> &q, const int i1, const int i2){
    const int l = min(i1, i2);
    const int r = max(i1, i2);
    auto it2 = q.begin();
    for(int i=0; i<r; ++i){++it2;}   
    int temp = *it2;
    q.erase(it2);
    auto it1 = q.begin();
    for(int i=0; i<l; ++i){++it1;}
    q.insert(it1, temp);
}

list<int> rand_range_list(const int num, const unsigned seed){
    vector<int> rangevec(num, 0);
    for(int i=0; i<num; ++i){
        rangevec[i] = i;
    }
    mt19937 gen(seed);
    shuffle(begin(rangevec), end(rangevec), gen);
    list<int> randlist(begin(rangevec), end(rangevec));
    return randlist;
}

void shuffle_list(list<int>& a, const unsigned seed){
    vector<int> temp{a.begin(), a.end()};
    srand(seed);
    random_shuffle(temp.begin(), temp.end());
    a.assign(temp.begin(), temp.end());
    return;
}



// int main(){
//     vector<vector<int>> a{{1,2},{3,4}};
//     vector<vector<int>> b{{1,2},{3,4}};
//     vector<vector<int>> c{{1,2},{3}};
//     cout<<compare_vv(a,b)<<" "<<compare_vv(a,c)<<endl;
//     cout<<c.size()<<endl;

//     vector<vector<int>> al{{1,2},{3,4}};
//     vector<vector<int>> bl{{1,2},{3,4}};
//     vector<vector<int>> cl{{1,2},{3}};
//     cout<<compare_vv(al,bl)<<" "<<compare_vv(al,cl)<<endl;

//     return 0;
// }

// int main(){
//     vector<vector<int>> vec = {{1,2},{2,4},{3,1},{4,2}};
//     sort(vec.begin(), vec.end(), compare_ivec1);
//     print_2d_vector(vec);
//     return 0;    
// }
// int main(){
//     // vector<vector<int>> temp(3,vector<int>(4,-1));
//     vector<vector<int>> temp;
//     temp = vector<vector<int>> (3, vector<int>(4,1));
//     print_2d_vector(temp);
//     return 0;
// }