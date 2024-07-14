#include "workshop_funcs.h"

list<int> random_solu_seq(const int jobs){
    vector<int> soluvec(jobs);
    for(int foo=0; foo<jobs; ++foo){
        soluvec[foo] = foo;
    }
    random_device rd;
    mt19937 gen(rd());
    shuffle(begin(soluvec), end(soluvec), gen);
    list<int> solu(begin(soluvec), end(soluvec));
    return solu;
}

int calculate_span_pfsp(const list<int>& solu, const vector<vector<int>>& process_time){
    const int n = solu.size();
    const int m = process_time[0].size();
    const int mp1 = m+1;
    vector<int> ctime_machine(mp1, 0);
    for(auto itjob=solu.begin(); itjob!=solu.end(); ++itjob){
        auto ptime_iter = process_time[*itjob].begin();
        for(int stageind=1; stageind<mp1; ++stageind){
            ctime_machine[stageind] = max(ctime_machine[stageind-1], ctime_machine[stageind]) + (*ptime_iter);
            ++ptime_iter;
        }
    }
    return ctime_machine.back();
}


// NEH algorithm
void ta_acc(const int machines, const int index, const int k, const int size_arr, const vector<vector<int>>& process_time, list<int>& partial_solution, int (&f)[], int (&q)[], int(&e)[]){
    int temp1=0, temp2=0, tempind1=0;
    int h1 = k;
    auto it1 = begin(partial_solution);
    auto it2 = end(partial_solution);
    --it2;
    for(int ind=0; ind<k; ind++){
        int job_index = *it1;

        e[tempind1] = temp1 + process_time[job_index][0];
        f[tempind1] = temp1 + process_time[index][0];
        temp1 = e[tempind1];
        
        q[h1*machines-1] = temp2 + process_time[*it2].back();
        temp2 = q[h1*machines-1];

        ++it1; --it2; --h1; 
        tempind1 += machines;
    }
    f[size_arr] = e[size_arr-machines] + process_time[index][0];

    for(int i=1; i<machines; ++i){
        int temp3=0, temp4=0;
        int complement = machines-1-i;
        auto it3 = begin(partial_solution);
        auto it4 = end(partial_solution);
        --it4;
        int tempind2 = i;
        int tempind3 = size_arr-1-i;
        for(int ind=0; ind<k; ++ind){
            e[tempind2] = max(e[tempind2-1], temp3) + process_time[*it3][i];
            f[tempind2] = max(f[tempind2-1], temp3) + process_time[index][i];
            temp3 = e[tempind2];

            q[tempind3] = max(q[tempind3+1], temp4) + process_time[*it4][complement];
            temp4 = q[tempind3];

            ++it3; --it4;
            tempind2+=machines; tempind3-=machines;
        }
        f[size_arr+i] = max(f[size_arr+i-1], e[size_arr-machines+i]) + process_time[index][i];
    }
}

// vector<int> NEH_ta(const int jobs, const int machines, const vector<vector<int>>& process_time){   
list<int> NEH_func(const vector<vector<int>>& process_time){   
    const int jobs = process_time.size();
    const int machines = process_time[0].size();
    vector<vector<int>> summed_process_time(jobs, vector<int>(2));
    for(int row=0; row<jobs; ++row){
        int total_process_time = 0;
        for(int col=0; col<machines; ++col){
            total_process_time += process_time[row][col];
        }
        summed_process_time[row][0] = row;
        summed_process_time[row][1] = total_process_time;
    }
    sort(summed_process_time.begin(), summed_process_time.end(), greater_ivec);
    
    list<int> partial_solution;
    if(machines == 1){
        for(auto &ele:summed_process_time){
            partial_solution.emplace_back(ele[0]);
        }
        return partial_solution;
    }
    partial_solution.emplace_back(summed_process_time[0][0]);
    int job_index = 0;
    for(int ind=1; ind<jobs; ++ind){
        job_index = summed_process_time[ind][0];
        sub_NEH(machines, job_index, ind, process_time, partial_solution);
    }
    return partial_solution;
}

void sub_NEH(const int machines, const int index, const int k, const vector<vector<int>>& process_time, list<int>& partial_solution){
    const int size_arr = k*machines;
    int e[size_arr]{};
    int q[size_arr]{};
    int f[size_arr+machines]{};

    ta_acc(machines, index, k, size_arr, process_time, partial_solution, f, q, e);

    int position = best_index_neh(k, machines, f, q);
    auto it5 = partial_solution.begin();
    advance(it5, position);
    partial_solution.insert(it5, index);
}

int best_index_neh(const int row, const int col, const int (&f)[], const int (&q)[]){
    int makespan=0, position=0;
    int temp=0, tempspan=0;
    makespan = f[0] + q[0];
    for(int m=1; m<col; ++m){
        temp = f[m] + q[m];
        if(temp>makespan){
            makespan = temp;
        }
    }

    for(int i=1; i<row; ++i){
        int tempind = i*col;
        tempspan = f[tempind]+q[tempind];
        for(int m=1; m<col; ++m){
            ++tempind;
            temp = f[tempind] + q[tempind];
            if(temp>tempspan){
                tempspan = temp;
            }
        } 
        if(tempspan<makespan){
            makespan = tempspan;
            position = i;
        }
    }
    int tempind = row*col;
    tempspan = f[tempind];
    for(int m=1; m<col; ++m){
        ++tempind;
        temp = f[tempind];
        if(temp>tempspan){
            tempspan = temp;
        }
    }
    if(tempspan<makespan){
        makespan = tempspan;
        position = row;
    }
    return position;
}

list<int> NEH1_func(const vector<vector<int>>& process_time){
    const int jobs = process_time.size();
    const int machines = process_time[0].size();
    vector<vector<int>> summed_process_time(jobs, vector<int>(2));
    vector<int> a_value(jobs, 0);
    vector<int> b_value(jobs, 0);
    for(int row=0; row<jobs; ++row){
        int total_process_time = 0;
        for(int col=0; col<machines; ++col){
            total_process_time += process_time[row][col];
        }
        summed_process_time[row][0] = row;
        summed_process_time[row][1] = total_process_time;
        a_value[row] = total_process_time - process_time[row].back();
        b_value[row] = total_process_time - process_time[row].front();
    }
    sort(summed_process_time.begin(), summed_process_time.end(), greater_ivec);
    
    list<int> partial_solution;
    int tempspan = 0;
    int a_solu = 0;
    int b_solu = 0;
    int sum_of_final = 0;
    int sum_of_first = 0;

    if(machines == 1){
        for(auto &ele:summed_process_time){
            // partial_solution.emplace_back(ele[0]);
            partial_solution.emplace_front(ele[0]);
        }
        return partial_solution;
    }
    int job_index = summed_process_time[0][0];
    partial_solution.emplace_back(job_index);
    a_solu = a_value[job_index];
    b_solu = b_value[job_index];
    sum_of_final += process_time[job_index].back();
    sum_of_first += process_time[job_index].front();
    // int job_index = 0;
    for(int ind=1; ind<jobs; ++ind){
        job_index = summed_process_time[ind][0];
        sum_of_final += process_time[job_index].back();
        sum_of_first += process_time[job_index].front();
        sub_NEH1(machines, job_index, ind, process_time, partial_solution, a_value, b_value, a_solu, b_solu);
        a_solu -= sum_of_final;
        b_solu -= sum_of_first;
    }
    return partial_solution;
}

void sub_NEH1(const int machines, const int index, const int k, const vector<vector<int>>& process_time, list<int>& partial_solution, const vector<int>& a_value, const vector<int>& b_value, int& a_solu, int& b_solu){
    const int size_arr = k*machines;
    int e[size_arr]{};
    int q[size_arr]{};
    int f[size_arr+machines]{};

    ta_acc(machines, index, k, size_arr, process_time, partial_solution, f, q, e);

    int tie_brk = 0;
    if(min(a_solu, b_value[index]) < min(a_value[index], b_solu)){
        tie_brk = 1;
    }
    int makespan = 0;
    int position = best_index_neh1(k, machines, f, q, tie_brk, makespan);
    auto it5 = partial_solution.begin();
    advance(it5, position);
    partial_solution.insert(it5, index);
    a_solu = makespan;
    b_solu = makespan;
}

int best_index_neh1(const int row, const int col, const int (&f)[], const int (&q)[], const int tie_brk, int& makespan){
    // int makespan = 0;
    int position = 0;
    int temp=0, tempspan=0;
    makespan = f[0] + q[0];
    for(int m=1; m<col; ++m){
        temp = f[m] + q[m];
        if(temp>makespan){
            makespan = temp;
        }
    }

    for(int i=1; i<row; ++i){
        int tempind = i*col;
        tempspan = f[tempind]+q[tempind];
        for(int m=1; m<col; ++m){
            ++tempind;
            temp = f[tempind] + q[tempind];
            if(temp>tempspan){
                tempspan = temp;
            }
        } 
        if(tempspan<makespan){
            makespan = tempspan;
            position = i;
        }
        if((tempspan==makespan)&&(tie_brk==1)){
            position = i;
        }
    }
    int tempind = row*col;
    tempspan = f[tempind];
    for(int m=1; m<col; ++m){
        ++tempind;
        temp = f[tempind];
        if(temp>tempspan){
            tempspan = temp;
        }
    }
    if(tempspan<makespan){
        makespan = tempspan;
        position = row;
    }
    else if((tempspan==makespan) && (tie_brk==1)){
        position = row;
    }
    return position;   
}

vector<vector<int>> sort_NM(const int n, const int m, const vector<vector<int>>& process_time){
    vector<vector<int>> u(n, vector<int>(n, 0));
    vector<vector<int>> l(n, vector<int>(n, 0));
    vector<vector<int>> seq(n, vector<int>(2,0));

    for(int ind2=0; ind2<n; ++ind2){
        int summed_ptime=0;
        for(int foo=0; foo<m; ++foo){
            summed_ptime+=process_time[ind2][foo];
        }

        int max_l = 0;
        for(int ind1=0; ind1<n; ++ind1){
            if(ind1==ind2){continue;}
            for(int i=1; i<m; ++i){
                // int p1 = process_time[ind1][i];
                // int p2 = process_time[ind2][i-1];
                int temp = process_time[ind1][i] - process_time[ind2][i-1];
                l[ind1][ind2] += max(0, temp-u[ind1][ind2]);
                u[ind1][ind2] += max(0, u[ind1][ind2]-temp);
            }
            if(l[ind1][ind2]>max_l){
                max_l = l[ind1][ind2];
            }
        }
        seq[ind2][0] = ind2;
        seq[ind2][1] = summed_ptime - max_l;
    }
    sort(seq.begin(), seq.end(), greater_ivec);
    return seq;
} 

list<int> NEHNM_func(const vector<vector<int>>& process_time){
    const int jobs = process_time.size();
    const int machines = process_time[0].size();
    vector<vector<int>> seq = sort_NM(jobs, machines, process_time);
    
    list<int> partial_solution;
    if(machines == 1){
        for(auto &ele:seq){
            partial_solution.emplace_back(ele[0]);
        }
        return partial_solution;
    }
    partial_solution.emplace_back(seq[0][0]);
    int job_index = 0;
    for(int ind=1; ind<jobs; ++ind){
        job_index = seq[ind][0];
        sub_NEH(machines, job_index, ind, process_time, partial_solution);
    }
    return partial_solution;   
   
}

vector<vector<int>> sort_NM1(const int n, const int m, const vector<vector<int>>& process_time, vector<int>& total_ptime, vector<int>& a_value, vector<int>& b_value){
    vector<vector<int>> u(n, vector<int>(n, 0));
    vector<vector<int>> l(n, vector<int>(n, 0));
    vector<vector<int>> seq(n, vector<int>(2,0));

    for(int ind2=0; ind2<n; ++ind2){
        int summed_ptime=0;
        for(int foo=0; foo<m; ++foo){
            summed_ptime+=process_time[ind2][foo];
        }
        total_ptime[ind2] = summed_ptime;
        a_value[ind2] = summed_ptime - process_time[ind2].back();
        b_value[ind2] = summed_ptime - process_time[ind2].front();

        int max_l = 0;
        for(int ind1=0; ind1<n; ++ind1){
            if(ind1==ind2){continue;}
            for(int i=1; i<m; ++i){
                int temp = process_time[ind1][i] - process_time[ind2][i-1];
                l[ind1][ind2] += max(0, temp-u[ind1][ind2]);
                u[ind1][ind2] += max(0, u[ind1][ind2]-temp);
            }
            if(l[ind1][ind2]>max_l){
                max_l = l[ind1][ind2];
            }
        }
        seq[ind2][0] = ind2;
        seq[ind2][1] = summed_ptime - max_l;
    }
    sort(seq.begin(), seq.end(), greater_ivec);
    return seq;
} 

list<int> NEHNM1_func(const vector<vector<int>>& process_time){
    const int jobs = process_time.size();
    const int machines = process_time[0].size();
    vector<int> total_ptime(jobs, 0);
    vector<int> a_value(jobs, 0);
    vector<int> b_value(jobs, 0);
    vector<vector<int>> seq = sort_NM1(jobs, machines, process_time, total_ptime, a_value, b_value);

    list<int> partial_solution;
    int tempspan = 0;
    int a_solu = 0;
    int b_solu = 0;
    int sum_of_final = 0;
    int sum_of_first = 0;

    if(machines == 1){
        for(auto &ele:seq){
            // partial_solution.emplace_back(ele[0]);
            partial_solution.emplace_front(ele[0]);
        }
        return partial_solution;
    }
    int job_index = seq[0][0];
    partial_solution.emplace_back(job_index);
    a_solu = a_value[job_index];
    b_solu = b_value[job_index];
    sum_of_final += process_time[job_index].back();
    sum_of_first += process_time[job_index].front();
    // int job_index = 0;
    for(int ind=1; ind<jobs; ++ind){
        job_index = seq[ind][0];
        sum_of_final += process_time[job_index].back();
        sum_of_first += process_time[job_index].front();
        sub_NEH1(machines, job_index, ind, process_time, partial_solution, a_value, b_value, a_solu, b_solu);
        a_solu -= sum_of_final;
        b_solu -= sum_of_first;
    }
    return partial_solution;
}

list<int> NEHKK_func(const vector<vector<int>>& process_time){
    const int jobs = process_time.size();
    const int machines = process_time[0].size();
    vector<int> a_value(jobs, 0);
    vector<int> b_value(jobs, 0);
    vector<vector<int>> seq(jobs, vector<int>(2, 0));
    const int scalar = (machines-1)*(machines-2) / 2;
    for(int ind=0; ind<jobs; ++ind){
        for(int m=0; m<machines; ++m){
            int temp = process_time[ind][m];
            a_value[ind] += (scalar + machines - m - 1)*temp;
            b_value[ind] += (scalar + m)*temp;
        }
        seq[ind][0] = ind;
        seq[ind][1] = min(a_value[ind], b_value[ind]);
    }
    sort(seq.begin(), seq.end(), greater_ivec);

    list<int> partial_solution;

    if(machines == 1){
        for(auto &ele:seq){
            // partial_solution.emplace_back(ele[0]);
            partial_solution.emplace_front(ele[0]);
        }
        return partial_solution;
    }   

    int job_index = seq[0][0];
    partial_solution.emplace_back(job_index);
    // int job_index = 0;
    for(int ind=1; ind<jobs; ++ind){
        job_index = seq[ind][0];
        sub_NEHKK(machines, job_index, ind, process_time, partial_solution, a_value, b_value);
    }
    return partial_solution;
}

void sub_NEHKK(const int machines, const int index, const int k, const vector<vector<int>>& process_time, list<int>& partial_solution, const vector<int>& a_value, const vector<int>& b_value){
    const int size_arr = k*machines;
    int e[size_arr]{};
    int q[size_arr]{};
    int f[size_arr+machines]{};

    ta_acc(machines, index, k, size_arr, process_time, partial_solution, f, q, e);

    int tie_brk = 0;
    if(a_value[index]>b_value[index]){
        tie_brk = 1;
    }
    int makespan = 0;
    int position = best_index_nehkk(k, machines, f, q, tie_brk);
    auto it5 = partial_solution.begin();
    advance(it5, position);
    partial_solution.insert(it5, index);
}

int best_index_nehkk(const int row, const int col, const int (&f)[], const int (&q)[], const int tie_brk){
    int makespan = 0;
    int position = 0;
    int temp=0, tempspan=0;
    makespan = f[0] + q[0];
    for(int m=1; m<col; ++m){
        temp = f[m] + q[m];
        if(temp>makespan){
            makespan = temp;
        }
    }

    for(int i=1; i<row; ++i){
        int tempind = i*col;
        tempspan = f[tempind]+q[tempind];
        for(int m=1; m<col; ++m){
            ++tempind;
            temp = f[tempind] + q[tempind];
            if(temp>tempspan){
                tempspan = temp;
            }
        } 
        if(tempspan<makespan){
            makespan = tempspan;
            position = i;
        }
        if((tempspan==makespan)&&(tie_brk==1)){
            position = i;
        }
    }
    int tempind = row*col;
    tempspan = f[tempind];
    for(int m=1; m<col; ++m){
        ++tempind;
        temp = f[tempind];
        if(temp>tempspan){
            tempspan = temp;
        }
    }
    if(tempspan<makespan){
        makespan = tempspan;
        position = row;
    }
    else if((tempspan==makespan) && (tie_brk==1)){
        position = row;
    }
    return position;
}

list<int> partial_fixed_NEH(const vector<vector<int>>& process_time, list<int> seq, const vector<int>& inds){
    const int jobs = process_time.size();
    const int machines = process_time[0].size();
    const int num_fixed = seq.size();
    const int num_to_insert = inds.size();
    vector<array<int,2>> summed_process_time(num_to_insert, array<int,2>{});
    // auto iter1 = seq.begin()+num_fixed;
    int row1 = 0;
    for(int row=num_fixed; row<jobs; ++row){
        int total_process_time = 0;
        for(int col=0; col<machines; ++col){
            total_process_time += process_time[row][col];
        }
        summed_process_time[row1][0] = row;
        summed_process_time[row1][1] = total_process_time;
        row1++;
    }
    sort(summed_process_time.begin(), summed_process_time.end(), greater_iarray2);
    
    list<int> partial_solution;
    if(machines == 1){
        for(auto &ele:summed_process_time){
            // partial_solution.emplace_back(ele[0]);
            partial_solution.emplace_back(inds[ele[0]-num_fixed]);
        }
        return partial_solution;
    }
    seq.emplace_back(summed_process_time[0][0]);
    partial_solution.emplace_back(inds[summed_process_time[0][0]-num_fixed]);
    int job_index = 0;
    for(int ind=1; ind<num_to_insert; ++ind){
        job_index = summed_process_time[ind][0];
        sub_partial_fixed_NEH_ta(num_fixed, machines, job_index, ind+num_fixed, process_time, seq, partial_solution, inds);
    }
    return partial_solution;
}
void sub_partial_fixed_NEH_ta(const int num_fixed, const int machines, const int index, const int k, const vector<vector<int>>& process_time, list<int>& seq, list<int>& partial_solution, const vector<int>& inds){
    const int size_arr = k*machines;
    int e[size_arr]{};
    int q[size_arr]{};
    int f[size_arr+machines]{};
    // vector<int> e(size_arr, 0);
    // vector<int> q(size_arr, 0);
    // vector<int> f(size_arr+machines, 0);

    int temp1=0, temp2=0, tempind1=0;
    int h1 = k;
    auto it1 = begin(seq);
    auto it2 = end(seq);
    --it2;
    for(int ind=0; ind<k; ind++){
        int job_index = *it1;

        e[tempind1] = temp1 + process_time[job_index][0];
        f[tempind1] = temp1 + process_time[index][0];
        temp1 = e[tempind1];
        
        q[h1*machines-1] = temp2 + process_time[*it2].back();
        temp2 = q[h1*machines-1];

        ++it1; --it2; --h1; 
        tempind1 += machines;
    }
    f[size_arr] = e[size_arr-machines] + process_time[index][0];

    for(int i=1; i<machines; ++i){
        int temp3=0, temp4=0;
        int complement = machines-1-i;
        auto it3 = begin(seq);
        auto it4 = end(seq);
        --it4;
        int tempind2 = i;
        int tempind3 = size_arr-1-i;
        for(int ind=0; ind<k; ++ind){
            e[tempind2] = max(e[tempind2-1], temp3) + process_time[*it3][i];
            f[tempind2] = max(f[tempind2-1], temp3) + process_time[index][i];
            temp3 = e[tempind2];

            q[tempind3] = max(q[tempind3+1], temp4) + process_time[*it4][complement];
            temp4 = q[tempind3];

            ++it3; --it4;
            tempind2+=machines; tempind3-=machines;
        }
        f[size_arr+i] = max(f[size_arr+i-1], e[size_arr-machines+i]) + process_time[index][i];
    }

    // int position = 0;
    int position = best_index_partial_neh(num_fixed ,k, machines, f, q);
    auto it5 = seq.begin();
    advance(it5, position+num_fixed);
    seq.insert(it5, index);
    auto it6 = partial_solution.begin();
    advance(it6, position);
    // partial_solution.insert(it6, inds[index]);
    partial_solution.insert(it6, inds[index-num_fixed]);
}

int best_index_partial_neh(const int num_fixed, const int row, const int col, const int (&f)[], const int (&q)[]){
    int makespan=0, position=0;
    int temp=0, tempspan=0;
    int tempind = num_fixed*col;
    makespan = f[tempind] + q[tempind];
    for(int m=1; m<col; ++m){
        tempind++;
        temp = f[tempind] + q[tempind];
        if(temp>makespan){
            makespan = temp;
        }
    }

    for(int i=num_fixed+1; i<row; ++i){
        tempind = i*col;
        tempspan = f[tempind]+q[tempind];
        for(int m=1; m<col; ++m){
            ++tempind;
            temp = f[tempind] + q[tempind];
            if(temp>tempspan){
                tempspan = temp;
            }
        } 
        if(tempspan<makespan){
            makespan = tempspan;
            position = i-num_fixed;
        }
    }
    tempind = row*col;
    tempspan = f[tempind];
    for(int m=1; m<col; ++m){
        ++tempind;
        temp = f[tempind];
        if(temp>tempspan){
            tempspan = temp;
        }
    }
    if(tempspan<makespan){
        makespan = tempspan;
        position = row-num_fixed;
    }
    return position;
}


// Some fundamental functions for HFSP


// vector<vector<int>> decode_FAM(const vector<int>& coded_solu, const vector<int>& machines_per_stage, const vector<vector<int>>& process_time){
list<vector<int>> decode_FAM(const list<int>& coded_solu_list, const vector<int>& machines_per_stage, const vector<vector<int>>& process_time){
    vector<int> coded_solu{make_move_iterator(begin(coded_solu_list)),
                            make_move_iterator(end(coded_solu_list))};
    int pos=0;
    const int stages = machines_per_stage.size();
    const int jobs =  process_time.size();
    // vector<vector<int>> solution(jobs*stages, vector<int>(4,0));
    list<vector<int>> solution(jobs*stages, vector<int>(4,0));
    
    vector<vector<int>> c(jobs, vector<int>(stages)); //completion time
    vector<vector<int>> available_times(stages);
    for(int ind=0; ind<stages; ++ind){
        available_times[ind] = vector<int>(machines_per_stage[ind]);
    } // initialize the time table
    vector<int> y(stages); // the earlist available time of each stage
    vector<int> m(stages); // stores indices of machines with earliest available time at each stage 
    int makespan = 0;
    for(int foo=0; foo<jobs; ++foo){
        int job_ind = coded_solu[foo];
        int tempspan;
        tempspan = assign_job(job_ind, pos, solution, process_time[job_ind], c[job_ind], available_times, y, m);
        if((makespan==0) || (makespan<tempspan)){
            makespan = tempspan;
        }
    }
    // sort(solution.begin(), solution.end(), less_ivec);
    solution.sort(less_ivec);
    coded_solu.emplace_back(makespan);
    solution.emplace_back(coded_solu);
    return solution;
}

int HFSP_makespan_FAM(const list<int>& coded_solu_list, const vector<int>& machines_per_stage, const vector<vector<int>>& process_time){
    const int stages = machines_per_stage.size();
    const int jobs = process_time.size();
    // vector<vector<int>> c(jobs, vector<int>(stages)); //completion time
    int c[jobs][stages]{}; //completion time
    vector<vector<int>> available_times(stages);
    for(int ind=0; ind<stages; ++ind){
        available_times[ind] = vector<int>(machines_per_stage[ind]);
    } // initialize the time table
    // vector<int> y(stages); // the earlist available time of each stage
    int y[stages]{};
    // vector<int> m(stages); // stores indices of machines with earliest available time at each stage 
    int m[stages]{};
    int makespan = 0;
    // auto it_solu = begin(coded_solu_list);
    // for(int foo=0; foo<jobs; ++foo){
    for(auto it_solu=begin(coded_solu_list); it_solu!=end(coded_solu_list); ++it_solu){
        // int job_ind = coded_solu[foo];
        int job_ind = *it_solu;
        c[job_ind][0] = y[0] + process_time[job_ind][0];
        available_times[0][m[0]] = c[job_ind][0];
        auto pos = min_element(begin(available_times[0]), end(available_times[0]));
        y[0] = *pos;
        m[0] = static_cast<int>(pos - begin(available_times[0]));
        for(int sind=1; sind<stages; ++sind){
            c[job_ind][sind] = max(c[job_ind][sind-1], y[sind]) + process_time[job_ind][sind];
            available_times[sind][m[sind]] = c[job_ind][sind];
            pos = min_element(begin(available_times[sind]), end(available_times[sind]));
            y[sind] = *pos;
            m[sind] = int(pos - begin(available_times[sind]));
        }
        if((makespan==0) || (makespan<c[job_ind][stages-1])){
            makespan = c[job_ind][stages-1];
        }
    }
    return makespan;
}

int idle_time(const vector<int>& process_time, const vector<int>& earliest_avai_times){
    int t = 0;
    const int len = process_time.size();
    vector<int> completion_time(len);

    completion_time[0] = earliest_avai_times[0] + process_time[0];
    for(int ind=1; ind<len; ++ind){
        t += max(completion_time[ind-1]-earliest_avai_times[ind], 0);
        completion_time[ind] = max(completion_time[ind-1], earliest_avai_times[ind]) + process_time[ind];
    }
    return t;
}

list<vector<int>> Algo_RITM(const int jobs, const int stages, const vector<int>& machines_per_stage, const vector<vector<int>>& process_time, const float a1, const float a2){
    // Firstly sort the jobs with respect to weighted processing times.
    vector<vector<float>> weighted_ptime(jobs, vector<float>(2));
    for(int jdex=0; jdex<jobs; ++jdex){
        weighted_ptime[jdex][0] = static_cast<float>(jdex);
        for(int sndex=0; sndex<stages; ++sndex){
            weighted_ptime[jdex][1] += (a1*static_cast<float>(sndex) + 1.0)*static_cast<float>(process_time[jdex][sndex]);
        }
    }

    sort(begin(weighted_ptime), end(weighted_ptime), greater_fvec);
    list<int> job_indices(jobs); // sequence of jobs sorted by weited times
    int foo1 = 0;
    for(auto it1=begin(job_indices); it1!=end(job_indices); ++it1){
        *it1 = static_cast<int>(weighted_ptime[foo1][0]);
        ++foo1;
    }

    //calculate the weight coefficient for each job.
    //stored in natural job index sequence.
    vector<float> weights(jobs);
    for(int jdex = 0; jdex<jobs; ++jdex){
        weights[weighted_ptime[jdex][0]] = pow(a2, static_cast<float>(jobs-jdex-1));
    }

    //create some schedule variables
    vector<vector<int>> c(jobs, vector<int>(stages)); //completion time
    vector<vector<int>> available_times(stages);
    for(int ind=0; ind<stages; ++ind){
        available_times[ind] = vector<int>(machines_per_stage[ind]);
    } // initialize the time table
    vector<int> y(stages); // the earlist available time of each stage
    vector<int> m(stages); // stores indices of machines with earliest available time at each stage 

    //Result vectors
    vector<int> coded_solution;
    list<vector<int>> solution(jobs*stages, vector<int>(4,0)); // (job_index, stage_index, machine_index, start_time)
    
    int pos = 0;
    auto it_job = begin(job_indices);
    int int_job = *it_job;
    int makespan = 0;
    makespan = assign_job(int_job, pos, solution, process_time[int_job], c[int_job], available_times, y, m);
    coded_solution.emplace_back(int_job);
    job_indices.erase(it_job);

    //Iteratively find the job with minimal weighted idle time and assign it on each stage
    for(int ind=1; ind<jobs; ++ind){
        auto it_min_ind = begin(job_indices);
        int min_ind = 0;
        float value_min_wt = 0.;
        value_min_wt = static_cast<float>(idle_time(process_time[min_ind], y)) / weights[min_ind];

        for(auto it_iter=begin(job_indices); it_iter!=end(job_indices); ++it_iter){
            int temp_ind = *it_iter;
            float temp_idle_wt;
            temp_idle_wt = static_cast<float>(idle_time(process_time[temp_ind], y)) / weights[temp_ind];
            if(it_iter==begin(job_indices)){
                it_min_ind = it_iter;
                min_ind = *it_iter;
                value_min_wt = temp_idle_wt;
            }
            else if(temp_idle_wt<value_min_wt){
                it_min_ind = it_iter;
                min_ind = *it_iter;
                value_min_wt = temp_idle_wt;
            }
        }

        int tempspan = 0;
        tempspan = assign_job(min_ind, pos, solution, process_time[min_ind], c[min_ind], available_times, y, m);
        if(makespan < tempspan){
            makespan = tempspan;
        }
        coded_solution.emplace_back(min_ind);
        job_indices.erase(it_min_ind);
    }
    // sort(begin(solution), end(solution), less_ivec);
    solution.sort(less_ivec);
    coded_solution.emplace_back(makespan);
    solution.emplace_back(coded_solution);
    return solution;
}

vector<int> Algo_RITM_coded(const int jobs, const int stages, const vector<int>& machines_per_stage, const vector<vector<int>>& process_time, const float a1, const float a2){
    // Firstly sort the jobs with respect to weighted processing times.
    vector<vector<float>> weighted_ptime(jobs, vector<float>(2));
    for(int jdex=0; jdex<jobs; ++jdex){
        weighted_ptime[jdex][0] = static_cast<float>(jdex);
        for(int sndex=0; sndex<stages; ++sndex){
            weighted_ptime[jdex][1] += (a1*static_cast<float>(sndex) + 1.0)*static_cast<float>(process_time[jdex][sndex]);
        }
    }

    sort(begin(weighted_ptime), end(weighted_ptime), greater_fvec);
    list<int> job_indices(jobs); // sequence of jobs sorted by weited times
    int foo1 = 0;
    for(auto it1=begin(job_indices); it1!=end(job_indices); ++it1){
        *it1 = static_cast<int>(weighted_ptime[foo1][0]);
        ++foo1;
    }

    //calculate the weight coefficient for each job.
    //stored in natural job index sequence.
    vector<float> weights(jobs);
    for(int jdex = 0; jdex<jobs; ++jdex){
        weights[weighted_ptime[jdex][0]] = pow(a2, static_cast<float>(jobs-jdex-1));
    }

    //create some schedule variables
    vector<vector<int>> c(jobs, vector<int>(stages)); //completion time
    vector<vector<int>> available_times(stages);
    for(int ind=0; ind<stages; ++ind){
        available_times[ind] = vector<int>(machines_per_stage[ind]);
    } // initialize the time table
    vector<int> y(stages); // the earlist available time of each stage
    vector<int> m(stages); // stores indices of machines with earliest available time at each stage 

    //Result vectors
    vector<int> coded_solution;
    list<vector<int>> solution(jobs*stages, vector<int>(4,0)); // (job_index, stage_index, machine_index, start_time)
    
    int pos = 0;
    auto it_job = begin(job_indices);
    int int_job = *it_job;
    int makespan = 0;
    makespan = assign_job(int_job, pos, solution, process_time[int_job], c[int_job], available_times, y, m);
    coded_solution.emplace_back(int_job);
    job_indices.erase(it_job);

    //Iteratively find the job with minimal weighted idle time and assign it on each stage
    for(int ind=1; ind<jobs; ++ind){
        auto it_min_ind = begin(job_indices);
        int min_ind = 0;
        float value_min_wt = 0.;
        value_min_wt = static_cast<float>(idle_time(process_time[min_ind], y)) / weights[min_ind];

        for(auto it_iter=begin(job_indices); it_iter!=end(job_indices); ++it_iter){
            int temp_ind = *it_iter;
            float temp_idle_wt;
            temp_idle_wt = static_cast<float>(idle_time(process_time[temp_ind], y)) / weights[temp_ind];
            if(it_iter==begin(job_indices)){
                it_min_ind = it_iter;
                min_ind = *it_iter;
                value_min_wt = temp_idle_wt;
            }
            else if(temp_idle_wt<value_min_wt){
                it_min_ind = it_iter;
                min_ind = *it_iter;
                value_min_wt = temp_idle_wt;
            }
        }

        int tempspan = 0;
        tempspan = assign_job(min_ind, pos, solution, process_time[min_ind], c[min_ind], available_times, y, m);
        if(makespan < tempspan){
            makespan = tempspan;
        }
        coded_solution.emplace_back(min_ind);
        job_indices.erase(it_min_ind);
    }
    return coded_solution;
}

// Assign job and update state information in the flowshop.
int assign_job(const int job_ind, int& pos, list<vector<int>>& solution, const vector<int>& process_time, vector<int>& completime, vector<vector<int>>& avaitime, vector<int>& earliest_avaitime, vector<int>& m_index){
    const int len = process_time.size();
    //Update the information of first stage
    completime[0] = earliest_avaitime[0] + process_time[0];
    avaitime[0][m_index[0]] = completime[0];
    // solution[pos][0] = job_ind; solution[pos][1] = 0;
    // solution[pos][2] = m_index[0]; solution[pos][3] = earliest_avaitime[0];
    auto it_pos = begin(solution);
    for(int foo=0; foo<pos; ++foo){++it_pos;}
    (*it_pos)[0] = job_ind; (*it_pos)[1] = 0;
    (*it_pos)[2] = m_index[0]; (*it_pos)[3] = earliest_avaitime[0];
    ++pos; ++it_pos;
    auto position = min_element(begin(avaitime[0]), end(avaitime[0]));
    earliest_avaitime[0] = *position;
    m_index[0] = static_cast<int>(position - begin(avaitime[0]));

    //Update information of following stages
    for(int ind=1; ind<len; ++ind){
        completime[ind] = max(completime[ind-1], earliest_avaitime[ind]) + process_time[ind];
        avaitime[ind][m_index[ind]] = completime[ind];
        // solution[pos][0] = job_ind; solution[pos][1] = ind;
        // solution[pos][2] = m_index[ind]; solution[pos][3] = max(completime[ind-1], earliest_avaitime[ind]);
        (*it_pos)[0] = job_ind; (*it_pos)[1] = ind;
        (*it_pos)[2] = m_index[ind]; (*it_pos)[3] = max(completime[ind-1], earliest_avaitime[ind]);
        ++pos; ++it_pos;
        position = min_element(begin(avaitime[ind]), end(avaitime[ind]));
        earliest_avaitime[ind] = *position;
        m_index[ind] = static_cast<int>(position-begin(avaitime[ind]));
    }
    return completime.back();
}

// list<int> SA_slave_HFSP(float temperature, const float anneal_rate, const int max_iter, const int iter_size, const int stop_crtn, const vector<int>& machines_perstage, const vector<vector<int>>& process_time, list<int>& init_solu){
list<int> SA_slave_HFSP(float temperature, const float anneal_rate, const int max_iter, const int iter_size, const int stop_crtn, const vector<int>& machines_perstage, const vector<vector<int>>& process_time, const int init){
    // iter_size: the size of inner loop.
    // stop_crtn: The number of iterates that the algorithm terminates if no improvement is found after stop_crtn iterates. 
    const int numjobs = process_time.size();
    const int numstages = machines_perstage.size();
    list<int> init_solu;
    if(init==0){
        init_solu = NEH_func(process_time);
    }
    else if(init==1){
        vector<int> tempvec = Algo_RITM(numjobs, numstages, machines_perstage, process_time, 0.5, 1.2).back();
        tempvec.pop_back();
        // list<int> init_solu(begin(tempvec), end(tempvec));
        init_solu.assign(begin(tempvec), end(tempvec));
    }
    else{
        init_solu = random_solu_seq(numjobs);
    }
    vector<int> counts_of_chosen(3,1);
    list<int> current_solu = init_solu;
    list<int> temp_solu = init_solu;
    list<int> history_best_solu = init_solu;
    int ls = 0;
    int current_span = HFSP_makespan_FAM(current_solu, machines_perstage, process_time);
    int history_best_span = current_span;
    uniform_real_distribution<float> dist01(0,1);
    random_device rd;

    int no_improvement = 0; 
    bool termi = false;
    int outer_iter_num = 0;   
    while((outer_iter_num<max_iter) && (!termi)){
        int span_before_search = history_best_span;
        mt19937 gen{rd()};
        for(int foo=0; foo<iter_size; ++foo){
            ls = roulette_from_ivec(counts_of_chosen);
            // counts_of_chosen[ls] += 1;
            temp_solu = SA_sample_neighbor(current_solu, ls);
            int temp_span = HFSP_makespan_FAM(temp_solu, machines_perstage, process_time);
            if((temp_span<current_span) || (temp_span == current_span)){
                current_span = temp_span;
                current_solu = temp_solu;
                // counts_of_chosen[ls] += 1;
                // if(counts_of_chosen[ls]>100){
                //     for(int foo1=0; foo1<3; ++foo1){
                //         counts_of_chosen[foo1] = counts_of_chosen[foo1] / 2;
                //     }
                // }
                if(current_span<history_best_span){
                    // counts_of_chosen[ls] += 1;
                    // if(counts_of_chosen[ls]>100){
                    //     for(int foo1=0; foo1<3; ++foo1){
                    //         counts_of_chosen[foo1] = counts_of_chosen[foo1]/2;
                    //     }
                    // }
                    history_best_span = current_span;
                    history_best_solu = current_solu;
                }
            }
            else{
                float exponent = (static_cast<float>(current_span) - static_cast<float>(temp_span))/temperature;
                // float prob = exp(delta/temperature);
                float prob = exp(exponent);
                // if(dist01(gen)<exp(delta/temperature)){
                if(dist01(gen)<prob){
                    current_solu = temp_solu;
                    current_span = temp_span;
                }
            }
        }
        temperature *= anneal_rate;
        if(history_best_span == span_before_search){
            ++no_improvement;
            if(no_improvement >= stop_crtn){
                termi = true;
            }
        }
        else{
            no_improvement = 0;
        }
    }
    init_solu = history_best_solu;

    history_best_solu.emplace_back(history_best_span);

    return history_best_solu;
}

list<int> SA_sample_neighbor(list<int> l, int num){
    random_device rd;
    mt19937 gen{rd()};
    uniform_int_distribution<int> dist(0, l.size()-1);
    int pos1 = dist(gen); int pos2 = dist(gen);
    if(num==0){ // Swap operation
        while(pos1==pos2){
            pos2 = dist(gen);
        }
        auto it1 = begin(l); auto it2 = begin(l);
        for(int foo=0; foo<min(pos1,pos2); ++foo){++it1;}
        for(int foo=0; foo<max(pos1,pos2); ++foo){++it2;}

        int temp1 = *it1; int temp2 = *it2;
        auto it3 = l.erase(it2);
        l.insert(it3, temp1);
        auto it4 = l.erase(it1);
        l.insert(it4, temp2);
        return l;
    }
    else if(num==1){ // Insert operation
        while(pos1==pos2){
            pos2 = dist(gen);
        }
        auto it1 = begin(l);
        for(int foo=0; foo<pos1; ++foo){++it1;}
        int temp = *it1;
        l.erase(it1);
        auto it2 = begin(l);
        for(int foo=0; foo<pos2; ++foo){++it2;}
        l.insert(it2, temp);
        return l;
    }
    else{ // Reverse operation
        while(((pos1-pos2)<3) && ((pos2-pos1)<3)){
            pos2 = dist(gen);
        }
        auto it1 = begin(l); auto it2 = begin(l);
        for(int foo=0; foo<min(pos1, pos2); ++foo){++it1;}
        for(int foo=0; foo<max(pos1, pos2); ++foo){++it2;}
        ++it2;
        reverse(it1, it2);
        return l;
    }
}


int main(){
    // int job = 4;
    // int mac = 3;
    // vector<vector<int>> proc{{1,2,3,}, {4,5,6}, {7,8,9}, {6,7,4}};
    // vector<int> result = NEH_ta(job, mac, proc);
    // for(auto it=result.begin(); it!=result.end(); ++it){
    //     cout<< *it <<" ";
    // }
    vector<vector<int>> test_vec{{0, 435}, {1,298}, {2, 458}, {3, 472}};
    sort(test_vec.begin(), test_vec.end(), greatereq_ivec);
    for(auto &ele:test_vec){
        cout<<ele[0]<<" "<<ele[1]<<", ";
    }
    cout<<endl;
    return 0;
}