#include "workshop.h"
// Functions of calss PFSP

list<int> PFSP::random_solution(const unsigned seed){
    return rand_range_list(this->num_job, seed);
}

list<int> PFSP::NEH(){
    return NEH_func(process_time);
}

void PFSP::solve_neh(){
    this->solution = NEH_func(this->process_time);
    vector<vector<int>> comptable_with_span = this->table_with_makespan(this->num_job, this->num_machine, this->solution);
    this->makespan = comptable_with_span.back()[0];
    comptable_with_span.pop_back();
    // this->completion_table = comptable_with_span;
}

void PFSP::set_solution(list<int> solu){
    this->solution = solu;
    vector<vector<int>> comptable_with_span = this->table_with_makespan(this->num_job, this->num_machine, this->solution);
    this->makespan = comptable_with_span.back()[0];
    comptable_with_span.pop_back();
    // this->completion_table = comptable_with_span;
}

int PFSP::span_of_solution(const list<int>& solu){
    return calculate_span_pfsp(solu, this->process_time);
}

// vector<vector<int>> PFSP::table_with_makespan(const int jobs, const int machines, const vector<int> &solu){
vector<vector<int>> PFSP::table_with_makespan(const int jobs, const int machines, const list<int> &solu){
    // vector<> comptable(num_of_job*num_of_machine, 0);
    vector<vector<int>> comptable(jobs, vector<int>(machines));
    int temp = 0;
    // for(int foo=0; foo<num_of_job; ++foo){
    //     int row = solu[foo];
    //     comptable[row][0] = process_time[row][0] + temp;
    //     temp = comptable[row][0];
    // }
    for(auto it_foo=begin(solu); it_foo!=end(solu); ++it_foo){
        int row = *it_foo;
        comptable[row][0] = process_time[row][0] + temp;
        temp = comptable[row][0];
    }
    for(int i=1; i<num_machine; ++i){
        temp = 0;
        for(auto job=begin(solu); job!=end(solu); ++job){
            comptable[*job][i] = max(comptable[*job][i-1], temp) + process_time[*job][i];
            temp = comptable[*job][i];
        }
    }
    vector<int> span(1);
    span[0] = comptable[solu.back()].back();
    comptable.push_back(span);
    return comptable;
}

// void PFSP::rper(int a){
//     if(a==0){
//         cout<<"This problem has "<<this->num_job<<" jobs and "<<this->num_machine<<" machines. ";
//         if(this->makespan==0){
//             cout<<"Not solved yet. "<<endl;
//         }
//         else{
//             // cout<<"The solution found using "; 
//             // print_string(method_applied);
//             // cout<<" is"<<endl;
//             cout<<"Current solution is:"<<endl;
//             print_1d_list(this->solution);
//             cout<<endl<<"with makespan "<< this->makespan <<endl;
//         }
//     }
//     else{
//         cout<<"This problem has "<<this->num_job<<" jobs and "<<this->num_machine<<" machines."<<endl;
//         print_2d_vector(this->process_time);

//         if(this->completion_table.size()==1){
//             cout<<"Not solved yet. "<<endl;
//         }
//         else{
//             // cout<<"The solution found using "; 
//             // print_string(method_applied);
//             // cout<<" is"<<endl;
//             cout<<"Current solution is:"<<endl;
//             print_1d_list(this->solution);
//             cout<<endl<<"with makespan "<< this->makespan <<endl;
//             cout<<"The completion times of each job on each machine is "<<endl;
//             print_2d_vector(this->completion_table);
//         }
//     }
// }



// Functions of class HFSP
void HFSP::solve(const char* method){
    list<vector<int>> solu;
    if(strcmp(method, "NEH") == 0){
        list<int> sequence = NEH_func(this->process_time);
        solu = decode_FAM(sequence, this->machines_per_stage, this->process_time);
    }
    else if(strcmp(method, "NEH1")==0){
        list<int> sequence = NEH1_func(this->process_time);
        solu = decode_FAM(sequence, this->machines_per_stage, this->process_time);
    }
    else if(strcmp(method, "NEHNM")==0){
        list<int> sequence = NEHNM_func(this->process_time);
        solu = decode_FAM(sequence, this->machines_per_stage, this->process_time);
    }
    else if(strcmp(method, "NEHNM1")==0){
        list<int> sequence = NEHNM1_func(this->process_time);
        solu = decode_FAM(sequence, this->machines_per_stage, this->process_time);
    }
    else if(strcmp(method, "NEHKK")==0){
        list<int> sequence = NEHKK_func(this->process_time);
        solu = decode_FAM(sequence, this->machines_per_stage, this->process_time);
    }
    else if(strcmp(method, "RITM") == 0){
        solu = Algo_RITM(this->num_of_job, this->num_of_stage, this->machines_per_stage, this->process_time, 0.5, 1.2);
    }
    else{
        cerr<<"Wrong argument."<<endl;
        exit(EXIT_FAILURE);
    }
    vector<int>coded = solu.back();
    this->makespan = coded.back();
    coded.pop_back();
    this->coded_solution = list<int>{make_move_iterator(begin(coded)), make_move_iterator(end(coded))};
    solu.pop_back();
    this->solution = solu;
    for(auto it=begin(solu); it!=end(solu); ++it){
        this->completion_table[(*it)[0]][(*it)[1]] = (*it)[3] + this->process_time[(*it)[0]][(*it)[1]];
    }
}

void HFSP::solve_sa(float temperature, const float anneal_rate, const int max_iter, const int iter_size, const int stop_crtn){
    list<vector<int>> solu = this->sa(temperature, anneal_rate, max_iter, iter_size, stop_crtn);
    vector<int>coded = solu.back();
    this->makespan = coded.back();
    coded.pop_back();
    this->coded_solution = list<int>{make_move_iterator(begin(coded)), make_move_iterator(end(coded))};
    solu.pop_back();
    this->solution = solu;
    for(auto it=begin(solu); it!=end(solu); ++it){
        this->completion_table[(*it)[0]][(*it)[1]] = (*it)[3] + this->process_time[(*it)[0]][(*it)[1]];
    }
}

list<vector<int>> HFSP::NEH(){
    list<int> sequence = NEH_func(process_time);
    return decode_FAM(sequence, machines_per_stage, process_time);
}

list<vector<int>> HFSP::RITM(){
    return Algo_RITM(num_of_job, num_of_stage, machines_per_stage, process_time, 0.5, 1.2);
}

list<vector<int>> HFSP::sa(float temperature, const float anneal_rate, const int max_iter, const int iter_size, const int stop_crtn){
    vector<future<list<int>>> solus(8);
    solus[0] = async(launch::async, SA_slave_HFSP,temperature, anneal_rate, max_iter, iter_size, stop_crtn, this->machines_per_stage, this->process_time, 0);
    solus[1] = async(launch::async, SA_slave_HFSP,temperature, anneal_rate, max_iter, iter_size, stop_crtn, this->machines_per_stage, this->process_time, 0);
    solus[2] = async(launch::async, SA_slave_HFSP,temperature, anneal_rate, max_iter, iter_size, stop_crtn, this->machines_per_stage, this->process_time, 1);
    solus[3] = async(launch::async, SA_slave_HFSP,temperature, anneal_rate, max_iter, iter_size, stop_crtn, this->machines_per_stage, this->process_time, 1);
    solus[4] = async(launch::async, SA_slave_HFSP,temperature, anneal_rate, max_iter, iter_size, stop_crtn, this->machines_per_stage, this->process_time, 2);
    solus[5] = async(launch::async, SA_slave_HFSP,temperature, anneal_rate, max_iter, iter_size, stop_crtn, this->machines_per_stage, this->process_time, 2);
    solus[6] = async(launch::async, SA_slave_HFSP,temperature, anneal_rate, max_iter, iter_size, stop_crtn, this->machines_per_stage, this->process_time, 2);
    solus[7] = async(launch::async, SA_slave_HFSP,temperature, anneal_rate, max_iter, iter_size, stop_crtn, this->machines_per_stage, this->process_time, 2);
    // return decode_FAM(newsolu, machines_per_stage, process_time);
    list<int> newsolu = solus[0].get();
    list<int> tempsolu(num_of_job+1);
    for(int foo=1; foo<8; ++foo){
        tempsolu = solus[foo].get();
        if(newsolu.back()>tempsolu.back()){
            newsolu = tempsolu;
        }
    }
    // return newsolu;
    newsolu.pop_back();
    return decode_FAM(newsolu, this->machines_per_stage, this->process_time);
}

void HFSP::set_solution(list<int> coded_solu){
    assert(coded_solu.size()==this->num_of_job);
    list<vector<int>> solu = decode_FAM(coded_solu, this->machines_per_stage, this->process_time);
    this->coded_solution = coded_solu;
    this->makespan = solu.back().back();
    solu.pop_back();
    this->solution = solu;
    for(auto it=begin(solu); it!=end(solu); ++it){
        this->completion_table[(*it)[0]][(*it)[1]] = (*it)[3] + this->process_time[(*it)[0]][(*it)[1]];
    }
}

void HFSP::rper(int a){
    if(a==0){
        cout<< "This problem has "<< this->num_of_job<<" jobs and " <<this->num_of_stage <<" stages."<<endl;
        cout<< "Numbers of machines in each stage are ";
        print_1d_vector(this->machines_per_stage);
        if(this->makespan==0){
            cout<< "Not solved yet."<<endl;
        }
        else{
            // cout<<"The makespan obtained using ";
            // print_string(method_applied);
            // cout<<" is "<< makespan<<endl;
            cout<<"Current solution is"<<endl;
            print_list_of_vec(this->solution);
            cout<<"with makespan "<<this->makespan<<endl;
        }
    }
    else{
        cout<< "This problem has "<< num_of_job<<" jobs and " <<num_of_stage <<" stages."<<endl;
        cout<< "Numbers of machines in each stage are ";
        print_1d_vector(machines_per_stage);
        cout<< "The processing time table is "<<endl;
        print_2d_vector(process_time);
        if(makespan==0){
            cout<< "Not solved yet."<<endl;
        }
        else{
            // cout<<"The solution obtained using ";
            // print_string(method_applied);
            // cout<<" is "<<endl;
            cout<<"Current solution is"<<endl;
            // print_list_of_vec_1line(this->solution);
            print_list_of_vec(this->solution);
            cout<<"with completion time table "<<endl;
            print_2d_vector(this->completion_table);
            cout<<"and makespan "<<this->makespan<<endl;
        }
    }
}

