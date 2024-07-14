#include "dynamic_workshop.h"

bool termi(const queue<int>& arriv, const vector<vector<int>>& real_ctime){
    if(!arriv.empty()) return false;
    // if(arriv.size()>0) return false;
    for(int i=0; i<real_ctime.size(); ++i){
        for(int j=real_ctime[i].size()-1; j>=0; --j){
            if(real_ctime[i][j]<0) return false;
        }
    }
    return true;
}

bool termi(const vector<list<int>> ques, const vector<vector<array<int,4>>>& mstate){
    for(int s=0; s<mstate.size(); ++s){
        if(ques[s].size()>0){return false;}
        for(int m=0; m<mstate[s].size(); ++m){
            if(mstate[s][m][0]==1){return false;}
        }
    }
    return true;
}

bool termi(const int stage_ind, const vector<list<int>> ques, const vector<vector<array<int,4>>>& mstate){
    for(int s=stage_ind; s<mstate.size(); ++s){
        if(ques[s].size()>0){return false;}
        for(int m=0; m<mstate[s].size(); ++m){
            if(mstate[s][m][0]==1){return false;}
        }
    }
    return true;
}


bool DFSP1::is_terminal(){
    if(!this->arrive_times.empty()) return false;
    for(int i=0; i<this->real_completion_time.size();++i){
        for(int j=this->real_completion_time[i].size()-1; j>=0; --j){
            if(this->real_completion_time[i][j] < 0) return false;
        }
    }
    return true;
}

DFSP1 DFSP1::env_run(unsigned seed1, unsigned seed2, unsigned seed3, const vector<vector<int>>& process_time){
    //拷贝变量
    int numj = this->num_of_job;
    int t = this->current_time;
    //machine variable
    vector<vector<array<int, 4>>> mstate(this->machine_state);
    vector<vector<int>> tasks(this->task_remain);
    vector<vector<int>> real_ctime(this->real_completion_time);
    vector<array<int,3>> real_patch(this->real_dispatch);
    queue<int> arrivals(this->arrive_times);
    int num_arriving = arrivals.size();
    vector<list<int>> que(this->Q);

    //创建数组储存重调度信息
    vector<list<int>> resched_list(vector<list<int>>(this->num_of_stage, list<int>()));
    bool resched_flag = false;

    //创建随机数生成器，控制随机事件
    mt19937 gen1{seed1};
    mt19937 gen2{seed2};
    mt19937 gen3{seed3};
    uniform_real_distribution<float> dist01(0,1);
    while(!termi(arrivals, real_ctime)){
        //From stage 1 to stage m, assign jobs to available machines
        for(int i=0; i<this->num_of_stage; ++i){
            if(que[i].empty()) continue;  // no job is waiting to be processed by machines at this stage
            queue<int> avai_machine;  // store available machines in this stage
            for(int foo=0; foo<mstate[i].size(); ++foo){
                if(mstate[i][foo][0]==0 && mstate[i][foo][1]==1){
                    avai_machine.push(foo);
                }
            }
            //assign jobs to available machines
            while((!que[i].empty()) && (!avai_machine.empty())){
                //loop until there's no job waiting or no machine available 
                int ind_job = que[i].front();
                int ind_machine = avai_machine.front();
                mstate[i][ind_machine][0] = 1;
                mstate[i][ind_machine][3] = ind_job;
                tasks[ind_job][i] -= 1;  // 更新任务剩余变量
                real_patch[ind_job][0] = ind_machine;  //处理工件的机器指标，在置换流水车间中总是 0
                real_patch[ind_job][1] = t;  //工件的实际开始加工时间，
                real_patch[ind_job][2] = t+process_time[ind_job][i];  //预期加工完成时间
                que[i].pop_front();  //从等待工件队列中删除此工件
                avai_machine.pop();  //从可用机器队列中删除此机器
            }
        }

        //检查是否有新工件到达
        while(num_arriving>0){
            if(t==arrivals.front()){
                //若有新工件到达，则将流水线第一个stage前的工件队列设为需要重调度
                resched_list[0].emplace_back(0);
                resched_list[0].emplace_back(numj);
                for(int b=0; b<batchsize; ++b){
                    que[0].emplace_back(numj);  //将新的工件编号加入待加工队列队尾
                    numj += 1;  //batch 中的下一个新工件
                    tasks.emplace_back(vector<int>(this->num_of_stage, 1));  //更新任务剩余变量
                    real_ctime.emplace_back(vector<int>(this->num_of_stage, -1));  //拓展完成时间矩阵
                    real_patch.emplace_back(array<int,3>{-1,-1,-1});  //拓展指派信息数组
                }
                //更新剩余新工件信息
                arrivals.pop();
                --num_arriving;
            }
            else{
                break;  //没有新工件到达
            }
        }

        // 车间运行
        t+=1;
        for(int i=0; i<num_of_stage; ++i){
            for(int j=0; j<mstate[i].size(); ++j){
                float r1 = dist01(gen1);
                float r2 = dist01(gen2);
                //get the efficiency status of machine between t and t+1
                if(mstate[i][j][2]==0){
                    mstate[i][j][2]== (r2>this->eff_prob[0][i][j])? 1:0;
                }
                else{
                    mstate[i][j][2]== (r2>this->eff_prob[1][i][j])? 0:1;
                }
                //whether a machine breaks down or get fixed while not processing a job
                if(mstate[i][j][0]==0){
                    if(mstate[i][j][1]==0){
                        mstate[i][j][1] = (r1>this->br_prob[0][i][j])? 1:0;
                    }
                    else{
                        mstate[i][j][1] = (r1>this->br_prob[1][i][j])? 0:1;
                    }
                }
                else{
                    //machine breaks down while processing a job
                    int temp_job_ind = mstate[i][j][3];
                    if(r1>this->br_prob[1][i][j]){  //机器损坏
                        //更新状态信息
                        mstate[i][j][0] = 0;
                        mstate[i][j][1] = 0;
                        mstate[i][j][3] = -1;
                        //设为待重调度状态
                        resched_list[i].emplace_back(1);
                        resched_list[i].emplace_back(temp_job_ind);
                        //加工被中断的工件重新插回队首
                        que[i].emplace_front(temp_job_ind);
                        //更新任务剩余变量
                        tasks[temp_job_ind][i] += 1;
                    }
                    else{  //机器未损坏
                        if(mstate[i][j][2]==0){  //机器效率低，导致前一个时间单位内加工无效
                            real_patch[temp_job_ind][1] += 1;
                            // real_patch[temp_job_ind][2] += 1;
                        }
                        else{  //机器前一个时间单位正常运行，检查是否加工完成
                            if(t-real_patch[temp_job_ind][1]==process_time[temp_job_ind][i]){  //加工完成
                                mstate[i][j][0] = 0;
                                mstate[i][j][3] = -1;
                                real_ctime[temp_job_ind][i] = t;  //实际完成时间
                                if(t>real_patch[temp_job_ind][2]){  //如果实际完成时间晚于期望完成时间，将对此stage的工件队列进行重调度
                                    resched_list[i].emplace_back(2);
                                    resched_list[i].emplace_back(temp_job_ind);
                                }
                                if(i<this->num_of_stage-1){  //将被加工完成的工件安插到下一个stage工件队列的队尾
                                    que[i+1].emplace_back(temp_job_ind);
                                }
                                //清空此工件的指派信息
                                real_patch[temp_job_ind][0] = -1;
                                real_patch[temp_job_ind][1] = -1;
                                real_patch[temp_job_ind][2] = -1;
                            }
                        }
                    }
                }
            }
        }

        //check the resched list, delete those where rescheduling is not needed, and if still not empty, go to reschedule
        // see the paper about when rescheduling is not needed
        for(int stageind=0; stageind<this->num_of_stage; ++stageind){
            if(!resched_list[stageind].empty()){
                list<int>& resched_vec = resched_list[stageind];
                auto it = resched_vec.begin();
                while(it != resched_vec.end()){
                    if((*it)==2){
                        if(stageind<this->num_of_stage-1){
                            if((que[stageind].size()<2) && (que[stageind+1].size()<2)){
                                it  = resched_vec.erase(it);
                                it  = resched_vec.erase(it);
                            }
                            else if(que[stageind].size()<2){
                                resched_list[stageind+1].emplace_back(3);
                                it = resched_vec.erase(it);
                                resched_list[stageind+1].emplace_back(*it);
                                it = resched_vec.erase(it);
                            }
                            else if(que[stageind+1].size()<2){
                                ++it; ++it;
                            }
                            else{
                                resched_list[stageind+1].emplace_back(3);
                                ++it;
                                resched_list[stageind+1].emplace_back(*it);
                                ++it;
                            }
                        }
                        else{
                            if(que[stageind].size()<2){
                                it = resched_vec.erase(it);
                                it = resched_vec.erase(it);
                            }
                            else{
                                ++it; ++it;
                            }
                        }
                    }
                    else{
                        if(que[stageind].size()<2){
                            it = resched_vec.erase(it);
                            it = resched_vec.erase(it);
                        }
                        else{
                            ++it; ++it;
                        }
                    }
                }
            }
        }
        for(int stageind=0; stageind<this->num_of_stage; ++stageind){
            if(resched_list[stageind].size()>1){
                resched_flag = true;
            }
        }
   
        if(resched_flag){
            //需要重调度，返回一个待重调度的状态
            return DFSP1(this, t, numj, mstate, real_patch, tasks, real_ctime, que, arrivals, resched_list, 1);
        }
    }
    //运行到生产结束，返回一个终止状态
    return DFSP1(this, t, numj, mstate, real_patch, tasks, real_ctime, que, arrivals, resched_list, -1);
}

DFSP1 DFSP1::simul_run(const int horizon, unsigned seed1, unsigned seed2, unsigned seed3, const vector<vector<int>>& process_time){
    int numj = this->num_of_job;
    int t = this->current_time;
    //machine variable
    vector<vector<array<int, 4>>> mstate(this->machine_state);
    vector<vector<int>> tasks(this->task_remain);
    vector<vector<int>> real_ctime(this->real_completion_time);
    vector<array<int,3>> real_patch(this->real_dispatch);
    // queue<int> arrivals(this->arrive_times);
    queue<int> arrivals;
    int num_arriving = arrivals.size();
    vector<list<int>> que(this->Q);
    vector<list<int>> resched_list(vector<list<int>>(this->num_of_stage, list<int>()));
    bool resched_flag = false;

    mt19937 gen1{seed1};
    mt19937 gen2{seed2};
    mt19937 gen3{seed3};
    uniform_real_distribution<float> dist01(0,1);
    // cout<<"The horizon in simul_run func is "<<horizon<<endl;
    while(!termi(arrivals, real_ctime) && (t<horizon)){
        //assign jobs to available machines
        for(int i=0; i<this->num_of_stage; ++i){
            if(que[i].empty()) continue;

            queue<int> avai_machine;
            for(int foo=0; foo<mstate[i].size(); ++foo){
                if(mstate[i][foo][0]==0 && mstate[i][foo][1]==1){
                    avai_machine.push(foo);
                }
            }
            while((!que[i].empty()) && (!avai_machine.empty())){
                int ind_job = que[i].front();
                int ind_machine = avai_machine.front();
                mstate[i][ind_machine][0] = 1;
                mstate[i][ind_machine][3] = ind_job;
                tasks[ind_job][i] -= 1;
                real_patch[ind_job][0] = ind_machine;
                real_patch[ind_job][1] = t;
                real_patch[ind_job][2] = t+process_time[ind_job][i];
                que[i].pop_front();
                avai_machine.pop();
            }
        }
        //check if there is new job arriving
        // cout<<num_arriving<<"---"<<endl;
        while(num_arriving>0){
            if(t==arrivals.front()){
                resched_list[0].emplace_back(0);
                resched_list[0].emplace_back(numj);
                que[0].emplace_back(numj);
                for(int b=0; b<batchsize; ++b){
                    numj += 1;
                    tasks.emplace_back(vector<int>(this->num_of_stage, 1));
                    real_ctime.emplace_back(vector<int>(this->num_of_stage, -1));
                }
                arrivals.pop();
                --num_arriving;
            }
            else{
                break;
            }
        }

        // workshop evolves
        t+=1;
        for(int i=0; i<num_of_stage; ++i){
            for(int j=0; j<mstate[i].size(); ++j){
                float r1 = dist01(gen1);
                float r2 = dist01(gen2);
                //get the efficiency status of machine between t and t+1
                if(mstate[i][j][2]==0){
                    mstate[i][j][2]== (r2>this->eff_prob[0][i][j])? 1:0;
                }
                else{
                    mstate[i][j][2]== (r2>this->eff_prob[1][i][j])? 0:1;
                }
                //if the machine is not working
                if(mstate[i][j][0]==0){
                    if(mstate[i][j][1]==0){
                        mstate[i][j][1] = (r1>this->br_prob[0][i][j])? 1:0;
                    }
                    else{
                        mstate[i][j][1] = (r1>this->br_prob[1][i][j])? 0:1;
                    }
                }
                else{
                    //machine breaks down while processing
                    int temp_job_ind = mstate[i][j][3];
                    if(r1>this->br_prob[1][i][j]){
                        mstate[i][j][0] = 0;
                        mstate[i][j][1] = 0;
                        mstate[i][j][3] = -1;
                        resched_list[i].emplace_back(1);
                        resched_list[i].emplace_back(temp_job_ind);
                        que[i].emplace_front(temp_job_ind);
                        tasks[temp_job_ind][i] += 1;
                    }
                    else{
                        if(mstate[i][j][2]==0){
                            real_patch[temp_job_ind][1] += 1;
                            // real_patch[temp_job_ind][2] += 1;
                        }
                        else{
                            // cout<<t-real_patch[temp_job_ind][1]<<" "<<this->process_time[temp_job_ind][i]<<endl;
                            if(t-real_patch[temp_job_ind][1]==process_time[temp_job_ind][i]){
                                mstate[i][j][0] = 0;
                                mstate[i][j][3] = -1;
                                real_ctime[temp_job_ind][i] = t;
                                if(t>real_patch[temp_job_ind][2]){
                                    resched_list[i].emplace_back(2);
                                    resched_list[i].emplace_back(temp_job_ind);
                                }
                                if(i<this->num_of_stage-1){
                                    que[i+1].emplace_back(temp_job_ind);
                                }
                                real_patch[temp_job_ind][0] = -1;
                                real_patch[temp_job_ind][1] = -1;
                                real_patch[temp_job_ind][2] = -1;
                            }
                        }
                    }
                }
            }
        }
        //check the resched list, delete those where rescheduling is not needed, and if still not empty, go to reschedule
        // see the paper about when rescheduling is not needed
        for(int stageind=0; stageind<this->num_of_stage; ++stageind){
            if(!resched_list[stageind].empty()){
                list<int>& resched_vec = resched_list[stageind];
                auto it = resched_vec.begin();
                while(it != resched_vec.end()){
                    if((*it)==2){
                        if(stageind<this->num_of_stage-1){
                            if((que[stageind].size()<2) && (que[stageind+1].size()<2)){
                                it  = resched_vec.erase(it);
                                it  = resched_vec.erase(it);
                            }
                            else if(que[stageind].size()<2){
                                resched_list[stageind+1].emplace_back(3);
                                it = resched_vec.erase(it);
                                resched_list[stageind+1].emplace_back(*it);
                                it = resched_vec.erase(it);
                            }
                            else if(que[stageind+1].size()<2){
                                ++it; ++it;
                            }
                            else{
                                resched_list[stageind+1].emplace_back(3);
                                ++it;
                                resched_list[stageind+1].emplace_back(*it);
                                ++it;
                            }
                        }
                        else{
                            if(que[stageind].size()<2){
                                it = resched_vec.erase(it);
                                it = resched_vec.erase(it);
                            }
                            else{
                                ++it; ++it;
                            }
                        }
                    }
                    else{
                        if(que[stageind].size()<2){
                            it = resched_vec.erase(it);
                            it = resched_vec.erase(it);
                        }
                        else{
                            ++it; ++it;
                        }
                    }
                }
            }
        }
        for(int stageind=0; stageind<this->num_of_stage; ++stageind){
            if(resched_list[stageind].size()>1){
                resched_flag = true;
            }
        }

        if(t==horizon){
            // resched_list.clear();
            vector<list<int>> temp_resched_list(vector<list<int>>(this->num_of_stage, list<int>()));
            return DFSP1(this, t, numj, mstate, real_patch, tasks, real_ctime, que,arrivals, temp_resched_list, -2);
        }
        if(!resched_list.empty()){
            return DFSP1(this, t, numj, mstate, real_patch, tasks, real_ctime, que, arrivals, resched_list, 1);
        }
    }
    return DFSP1(this, t, numj, mstate, real_patch, tasks, real_ctime, que, arrivals, resched_list, -1);
}

DFSP1 DFSP1::rollout(const int horizon, unsigned seed1, unsigned seed2, unsigned seed3, const vector<vector<int>>& process_time){
    int numj = this->num_of_job;
    int t = this->current_time;
    //machine variable
    vector<vector<array<int, 4>>> mstate(this->machine_state);
    vector<vector<int>> tasks(this->task_remain);
    vector<vector<int>> real_ctime(this->real_completion_time);
    vector<array<int,3>> real_patch(this->real_dispatch);
    // queue<int> arrivals(this->arrive_times);
    queue<int> arrivals;
    int num_arriving = arrivals.size();
    vector<list<int>> que(this->Q);
    vector<list<int>> resched_list(this->reschedule_list);

    mt19937 gen1{seed1};
    mt19937 gen2{seed2};
    mt19937 gen3{seed3};
    uniform_real_distribution<float> dist01(0,1);

    // if(this->turn == 1){
    //     random_resched(que, resched_list, this->process_time, this->completion_table, this->machines_per_stage, gen3());
    // }
    bool resched_flag = false;
    for(auto &l:resched_list){
        l.clear();
    }
    // random_device rd;
    // mt19937 gen1{rd()};
    // mt19937 gen2{rd()};

    while(!termi(arrivals, real_ctime) && (t<horizon)){
        for(int i=0; i<this->num_of_stage; ++i){
            if(que[i].empty()) continue;

            queue<int> avai_machine;
            for(int foo=0; foo<mstate[i].size(); ++foo){
                if(mstate[i][foo][0]==0 && mstate[i][foo][1]==1){
                    avai_machine.push(foo);
                }
            }
            while((!que[i].empty()) && (!avai_machine.empty())){
                int ind_job = que[i].front();
                int ind_machine = avai_machine.front();
                mstate[i][ind_machine][0] = 1;
                mstate[i][ind_machine][3] = ind_job;
                tasks[ind_job][i] -= 1;
                real_patch[ind_job][0] = ind_machine;
                real_patch[ind_job][1] = t;
                real_patch[ind_job][2] = t+process_time[ind_job][i];
                que[i].pop_front();
                avai_machine.pop();
            }
        }
        //check if there is new job arriving
        while(num_arriving>0){
            if(t==arrivals.front()){
                resched_list[0].emplace_back(0);
                resched_list[0].emplace_back(numj);
                que[0].emplace_back(numj);
                
                numj+=1;
                tasks.emplace_back(vector<int>(this->num_of_stage, 1));
                arrivals.pop();
                --num_arriving;
            }
            else{
                break;
            }
        }

        // workshop evolves
        t+=1;
        for(int i=0; i<num_of_stage; ++i){
            for(int j=0; j<mstate[i].size(); ++j){
                float r1 = dist01(gen1);
                float r2 = dist01(gen2);
                //get the efficiency status of machine between t and t+1
                if(mstate[i][j][2]==0){
                    mstate[i][j][2]== (r2>this->eff_prob[0][i][j])? 1:0;
                }
                else{
                    mstate[i][j][2]== (r2>this->eff_prob[1][i][j])? 0:1;
                }
                //if the machine is not working
                if(mstate[i][j][0]==0){
                    if(mstate[i][j][1]==0){
                        mstate[i][j][1] = (r1>this->br_prob[0][i][j])? 1:0;
                    }
                    else{
                        mstate[i][j][1] = (r1>this->br_prob[1][i][j])? 0:1;
                    }
                }
                else{
                    //machine breaks down while processing
                    int temp_job_ind = mstate[i][j][3];
                    if(r1>this->br_prob[1][i][j]){
                        mstate[i][j][0] = 0;
                        mstate[i][j][1] = 0;
                        mstate[i][j][3] = -1;
                        resched_list[i].emplace_back(1);
                        resched_list[i].emplace_back(temp_job_ind);
                        que[i].emplace_front(temp_job_ind);
                        tasks[temp_job_ind][i] += 1;
                    }
                    else{
                        if(mstate[i][j][2]==0){
                            real_patch[temp_job_ind][1] += 1;
                            // real_patch[temp_job_ind][2] += 1;
                        }
                        else{
                            // cout<<t-real_patch[temp_job_ind][1]<<" "<<this->process_time[temp_job_ind][i]<<endl;
                            if(t-real_patch[temp_job_ind][1]==process_time[temp_job_ind][i]){
                                mstate[i][j][0] = 0;
                                mstate[i][j][3] = -1;
                                real_ctime[temp_job_ind][i] = t;
                                if(t>real_patch[temp_job_ind][2]){
                                    resched_list[i].emplace_back(2);
                                    resched_list[i].emplace_back(temp_job_ind);
                                }
                                if(i<this->num_of_stage-1){
                                    que[i+1].emplace_back(temp_job_ind);
                                }
                                real_patch[temp_job_ind][0] = -1;
                                real_patch[temp_job_ind][1] = -1;
                                real_patch[temp_job_ind][2] = -1;
                            }
                        }
                    }
                }
            }
        }
        //check the resched list, delete those where rescheduling is not needed, and if still not empty, go to reschedule
        // see the paper about when rescheduling is not needed
        for(int stageind=0; stageind<this->num_of_stage; ++stageind){
            if(!resched_list[stageind].empty()){
                list<int>& resched_vec = resched_list[stageind];
                auto it = resched_vec.begin();
                while(it != resched_vec.end()){
                    if((*it)==2){
                        if(stageind<this->num_of_stage-1){
                            if((que[stageind].size()<2) && (que[stageind+1].size()<2)){
                                it  = resched_vec.erase(it);
                                it  = resched_vec.erase(it);
                            }
                            else if(que[stageind].size()<2){
                                resched_list[stageind+1].emplace_back(3);
                                it = resched_vec.erase(it);
                                resched_list[stageind+1].emplace_back(*it);
                                it = resched_vec.erase(it);
                            }
                            else if(que[stageind+1].size()<2){
                                ++it; ++it;
                            }
                            else{
                                resched_list[stageind+1].emplace_back(3);
                                ++it;
                                resched_list[stageind+1].emplace_back(*it);
                                ++it;
                            }
                        }
                        else{
                            if(que[stageind].size()<2){
                                it = resched_vec.erase(it);
                                it = resched_vec.erase(it);
                            }
                            else{
                                ++it; ++it;
                            }
                        }
                    }
                    else{
                        if(que[stageind].size()<2){
                            it = resched_vec.erase(it);
                            it = resched_vec.erase(it);
                        }
                        else{
                            ++it; ++it;
                        }
                    }
                }
            }
        }
        for(int stageind=0; stageind<this->num_of_stage; ++stageind){
            if(resched_list[stageind].size()>1){
                resched_flag = true;
            }
        }

        if(t==horizon){
            for(int i=0; i<this->num_of_stage; ++i){
                if(que[i].empty()) continue;

                queue<int> avai_machine;
                for(int foo=0; foo<mstate[i].size(); ++foo){
                    if(mstate[i][foo][0]==0 && mstate[i][foo][1]==1){
                        avai_machine.push(foo);
                    }
                }
                while((!que[i].empty()) && (!avai_machine.empty())){
                    int ind_job = que[i].front();
                    int ind_machine = avai_machine.front();
                    mstate[i][ind_machine][0] = 1;
                    mstate[i][ind_machine][3] = ind_job;
                    tasks[ind_job][i] -= 1;
                    real_patch[ind_job][0] = ind_machine;
                    real_patch[ind_job][1] = t;
                    real_patch[ind_job][2] = t+process_time[ind_job][i];
                    que[i].pop_front();
                    avai_machine.pop();
                }
            }
            return DFSP1(this, t, numj, mstate, real_patch, tasks, real_ctime, que, arrivals, resched_list, -2);
            // return DFSP1(this, t, numj, mstate, real_patch, tasks, real_ctime, que, arrivals, resched_list, 1);
        }
        if(resched_flag){
            // random_resched(que, resched_list, this->process_time, this->completion_table, this->machines_per_stage, gen3());
            // random_resched1(que, resched_list, this->process_time, this->completion_table, this->machines_per_stage, gen3());
            for(auto &l:resched_list){
                l.clear();
            }
            resched_flag = false;
        }
        // if(!resched_list.empty()){
        //     // return DFSP1(this, t, numj, mstate, real_patch, tasks, real_ctime, que, arrivals, resched_list, 1);
        //     // random_rschd(que, resched_list, this->process_time, this->completion_table, this->machines_per_stage);
        //     // resched_list.clear();
        //     map<int, vector<int>>().swap(resched_list);

        //     // map<int, vector<int>> newmp();
        //     // for(auto &ele:resched_list){
        //     //     resched_list.erase(ele.first);
        //     // }
        // }
        // malloc_trim(0);
    }

    return DFSP1(this, t, numj, mstate, real_patch, tasks, real_ctime, que, arrivals, resched_list, -1);
}

double DFSP1::multi_simu_makespan(const int simulation_times, unsigned seed, const vector<vector<int>>& process_time){
    double ave_time = 0.0;
    const int num_stage = this->num_of_stage;
    mt19937 gen{seed};
    uniform_real_distribution<float> dist01(0,1);

    for(int simul_ind=0; simul_ind<simulation_times; ++simul_ind){
        mt19937 gen1{gen()};
        mt19937 gen2{gen()};
        int t = this->get_time();
        vector<vector<array<int, 4>>> mstate(this->get_machine_state());
        vector<list<int>> ques(this->get_q());
        vector<array<int,3>> real_patch(this->get_real_dispatch());
        while(!termi(ques, mstate)){
            // cerr<<termi(ques, mstate)<<endl;
            for(int s=0; s<num_stage; ++s){
                if(ques[s].empty()) continue;
                for(int m=0; m<mstate[s].size(); ++m){
                    if((!ques[s].empty()) && (mstate[s][m][0]==0) && (mstate[s][m][1]==1)){
                        int temp_ind_job = ques[s].front();
                        mstate[s][m][0] = 1;
                        mstate[s][m][3] = temp_ind_job;
                        real_patch[temp_ind_job][0] = s;
                        real_patch[temp_ind_job][1] = t;
                        real_patch[temp_ind_job][2] = t+process_time[temp_ind_job][s];
                        ques[s].pop_front();
                    }
                }
            }

            t += 1;
            for(int s=0; s<num_stage; ++s){
                for(int m=0; m<mstate[s].size(); ++m){
                    float r1 = dist01(gen1);
                    float r2 = dist01(gen2);
                    // efficiency transiition
                    if(mstate[s][m][2]==0){
                        mstate[s][m][2]== (r2>this->eff_prob[0][s][m])? 1:0;
                    }
                    else{
                        mstate[s][m][2]== (r2>this->eff_prob[1][s][m])? 0:1;
                    }
                    // if the machine is not working
                    if(mstate[s][m][0]==0){
                        if(mstate[s][m][1]==0){
                            mstate[s][m][1] = (r1>this->br_prob[0][s][m])? 1:0;
                        }
                        else{
                            mstate[s][m][1] = (r1>this->br_prob[1][s][m])? 0:1;
                        }
                    }
                    else{
                        int temp_ind_job = mstate[s][m][3];
                        // if the machine breaks
                        if(r1>this->br_prob[1][s][m]){
                            mstate[s][m][0] = 0;
                            mstate[s][m][1] = 0;
                            mstate[s][m][3] = -1;
                            ques[s].emplace_front(temp_ind_job);
                        }
                        else{
                            // if the mahcine run inefficiently
                            if(mstate[s][m][2]==0){
                                // cerr<<"@@@"<<endl;
                                real_patch[temp_ind_job][1] += 1;
                                // real_patch[temp_job_ind][2] += 1;
                            }
                            else{
                                if(t-real_patch[temp_ind_job][1]==process_time[temp_ind_job][s]){
                                    mstate[s][m][0] = 0;
                                    mstate[s][m][3] = -1;
                                    if(s<num_stage-1){
                                        ques[s+1].emplace_back(temp_ind_job);
                                    }
                                    real_patch[temp_ind_job][0] = -1;
                                    real_patch[temp_ind_job][1] = -1;
                                    real_patch[temp_ind_job][2] = -1;
                                }
                            }
                        }
                    }
                }
            }
        }
        ave_time += static_cast<double>(t);
    }
    return ave_time / static_cast<double>(simulation_times);
}

double DFSP1::multi_simu_makespan_q(const int simulation_times, unsigned seed, const vector<vector<int>>& process_time, const vector<list<int>>& out_q){
    double ave_time = 0.0;
    const int num_stage = this->num_of_stage;
    mt19937 gen{seed};
    uniform_real_distribution<float> dist01(0,1);

    for(int simul_ind=0; simul_ind<simulation_times; ++simul_ind){
        mt19937 gen1{gen()};
        mt19937 gen2{gen()};
        // int t = this->current_time;
        int t = this->get_time();
        vector<vector<array<int, 4>>> mstate(this->get_machine_state());
        vector<list<int>> ques(out_q);
        vector<array<int,3>> real_patch(this->get_real_dispatch());
        while(!termi(ques, mstate)){
            // cerr<<termi(ques, mstate)<<endl;
            for(int s=0; s<num_stage; ++s){
                if(ques[s].empty()) continue;
                for(int m=0; m<mstate[s].size(); ++m){
                    if((!ques[s].empty()) && (mstate[s][m][0]==0) && (mstate[s][m][1]==1)){
                        int temp_ind_job = ques[s].front();
                        mstate[s][m][0] = 1;
                        mstate[s][m][3] = temp_ind_job;
                        real_patch[temp_ind_job][0] = s;
                        real_patch[temp_ind_job][1] = t;
                        real_patch[temp_ind_job][2] = t+process_time[temp_ind_job][s];
                        ques[s].pop_front();
                    }
                }
            }

            t += 1;
            for(int s=0; s<num_stage; ++s){
                for(int m=0; m<mstate[s].size(); ++m){
                    float r1 = dist01(gen1);
                    float r2 = dist01(gen2);
                    // efficiency transiition
                    if(mstate[s][m][2]==0){
                        mstate[s][m][2]== (r2>this->eff_prob[0][s][m])? 1:0;
                    }
                    else{
                        mstate[s][m][2]== (r2>this->eff_prob[1][s][m])? 0:1;
                    }
                    // if the machine is not working
                    if(mstate[s][m][0]==0){
                        if(mstate[s][m][1]==0){
                            mstate[s][m][1] = (r1>this->br_prob[0][s][m])? 1:0;
                        }
                        else{
                            mstate[s][m][1] = (r1>this->br_prob[1][s][m])? 0:1;
                        }
                    }
                    else{
                        int temp_ind_job = mstate[s][m][3];
                        // if the machine breaks
                        if(r1>this->br_prob[1][s][m]){
                            mstate[s][m][0] = 0;
                            mstate[s][m][1] = 0;
                            mstate[s][m][3] = -1;
                            ques[s].emplace_front(temp_ind_job);
                        }
                        else{
                            // if the mahcine run inefficiently
                            if(mstate[s][m][2]==0){
                                // cerr<<"@@@"<<endl;
                                real_patch[temp_ind_job][1] += 1;
                                // real_patch[temp_job_ind][2] += 1;
                            }
                            else{
                                if(t-real_patch[temp_ind_job][1]==process_time[temp_ind_job][s]){
                                    mstate[s][m][0] = 0;
                                    mstate[s][m][3] = -1;
                                    if(s<num_stage-1){
                                        ques[s+1].emplace_back(temp_ind_job);
                                    }
                                    real_patch[temp_ind_job][0] = -1;
                                    real_patch[temp_ind_job][1] = -1;
                                    real_patch[temp_ind_job][2] = -1;
                                }
                            }
                        }
                    }
                }
            }
        }
        ave_time += static_cast<double>(t);
    }
    return ave_time / static_cast<double>(simulation_times);   
}

float DFSP1::expected_span(const vector<vector<int>>& process_time, const vector<vector<vector<float>>>& exp_process_time){
    // return exp_span(this->get_time(), 0, this->get_num_job(), this->get_num_stage(), this->get_mac_per_stage(),  )
    // return exp_span(this->current_time, 0, this->num_of_stage, this->machines_per_stage, this->process_time, this->exp_process_time, this->machine_state, this->Q, this->real_dispatch, this->br_prob, this->eff_prob);
    return exp_span(this->current_time, 0, this->num_of_stage, this->machines_per_stage, process_time, exp_process_time, this->machine_state, this->Q, this->real_dispatch, this->br_prob, this->eff_prob);
}
    
int DFSP1::expected_static_span(const vector<vector<int>>& process_time){
    return partial_span_static(this->current_time, 0, this->num_of_stage, this->machines_per_stage, process_time, this->machine_state, this->Q, this->real_dispatch, this->br_prob, this->eff_prob);
}

int DFSP1::expected_static_span_outq(const vector<vector<int>>& process_time, const vector<list<int>>& q){
    return partial_span_static(this->current_time, 0, this->num_of_stage, this->machines_per_stage, process_time, this->machine_state, q, this->real_dispatch, this->br_prob, this->eff_prob);
}

DFSP1 DFSP1::resched_spt(const vector<vector<int>>& process_time){
    vector<list<int>> que_copy(this->Q);
    // for(auto &ele:this->reschedule_list){
    //     int stage_ind = ele.first;
    //     operator_spt(que_copy[stage_ind], stage_ind, this->process_time);
    // }
    for(int i=0; i<this->num_of_stage; ++i){
        if(this->reschedule_list[i].size()>1){
            operator_spt(que_copy[i], i, process_time);
        }
        // operator_spt(que_copy[i], i, this->process_time);
    }
    return DFSP1(this, que_copy);
}

DFSP1 DFSP1::resched_srpt(const vector<vector<int>>& process_time){
    vector<list<int>> que_copy(this->Q);
    for(int i=0; i<this->num_of_stage; ++i){
        if(this->reschedule_list[i].size()>1){
            operator_srpt(que_copy[i], i, process_time);
        }
        // operator_srpt(que_copy[i], this->process_time);
    }
    return DFSP1(this, que_copy);
}

DFSP1 DFSP1::resched_lpt(const vector<vector<int>>& process_time){
    vector<list<int>> que_copy(this->Q);
    for(int i=0; i<this->num_of_stage; ++i){
        if(this->reschedule_list[i].size()>1){
            operator_lpt(que_copy[i], i, process_time);
        }
        // operator_srpt(que_copy[i], this->process_time);
    }
    return DFSP1(this, que_copy);
}

DFSP1 DFSP1::resched_rightshift(){
    vector<list<int>> que_copy = this->Q;
    return DFSP1(this, que_copy);
}

// DFSP1 DFSP1::resched_est(){
//     vector<list<int>> que_copy = this->Q;
//     // for(auto &ele:this->reschedule_list){
//     //     int stage_ind = ele.first;
//     //     operator_est(que_copy[stage_ind], stage_ind, this->completion_table, this->process_time);
//     // }
//     for(int i=0; i<this->num_of_stage; ++i){
//         if(this->reschedule_list[i].size()>1){
//             operator_est(que_copy[i], i, this->completion_table, this->process_time);
//         }
//         // operator_est(que_copy[i], i, this->completion_table, this->process_time);
//     }
//     return DFSP1(this, que_copy);
// }

// DFSP1 DFSP1::resched_ect(){
//     vector<list<int>> que_copy = this->Q;
//     // for(auto &ele:this->reschedule_list){
//     //     int stage_ind = ele.first;
//     //     // cerr<<this->completion_table.size()<<"---"<<endl;
//     //     operator_ect(que_copy[stage_ind], stage_ind, this->completion_table);
//     //     // cerr<<"rrrrrrrrrrr"<<endl;
//     // }
//     for(int i=0; i<this->num_of_stage; ++i){
//         if(this->reschedule_list[i].size()>1){
//             operator_ect(que_copy[i],i, this->completion_table);
//         }
//         // operator_ect(que_copy[i],i, this->completion_table);
//     }
//     return DFSP1(this, que_copy);
//     // return DFSP1();
// }

DFSP1 DFSP1::resched_random(const unsigned seed, const vector<vector<int>>& process_time){
    mt19937 gen{seed};
    uniform_real_distribution<float> dist01(0,1);
    vector<list<int>> que_copy(this->Q);
    for(int i=0; i<this->num_of_stage; ++i){
        if(this->reschedule_list[i].size()>1){
            float a = dist01(gen);
            if(a<0.1){
                operator_spt(que_copy[i], i, process_time);
            }
            else if(a<0.2){
                operator_lpt(que_copy[i], i, process_time);
            }
            else if(a<0.3){
                operator_srpt(que_copy[i], i, process_time);
            }
            else if(a<0.4){
                operator_ritm(que_copy[i], i, process_time, this->machines_per_stage);
            }
            else if(a<0.5){
                operator_neh(que_copy[i], i, process_time);
            }
            else if(a<0.6){
                operator_neh1(que_copy[i], i, process_time);
            }
            else if(a<0.7){
                operator_nehnm(que_copy[i], i, process_time);
            }
            else if(a<0.8){
                operator_nehnm1(que_copy[i], i, process_time);
            }
            else if(a<0.8){
                operator_nehkk(que_copy[i], i, process_time);
            }
        }
    }
    return DFSP1(this, que_copy);
}

DFSP1 DFSP1::resched_ritm(const vector<vector<int>>& process_time){
    vector<list<int>> que_copy(this->Q);
    for(int i=0; i<this->num_of_stage; ++i){
        if(this->reschedule_list[i].size()>1){
            operator_ritm(que_copy[i], i, process_time, this->machines_per_stage);
        }
    }
    return DFSP1(this, que_copy);
}

DFSP1 DFSP1::resched_neh(const vector<vector<int>>& process_time){
    vector<list<int>> que_copy(this->Q);
    for(int i=0; i<this->num_of_stage; ++i){
        if(this->reschedule_list[i].size()>1){
            operator_neh(que_copy[i], i, process_time);
        }
    }
    return DFSP1(this, que_copy);
}

DFSP1 DFSP1::resched_neh1(const vector<vector<int>>& process_time){
    vector<list<int>> que_copy(this->Q);
    for(int i=0; i<this->num_of_stage; ++i){
        if(this->reschedule_list[i].size()>1){
            operator_neh1(que_copy[i], i, process_time);
        }
    }
    return DFSP1(this, que_copy);
}

DFSP1 DFSP1::resched_nehnm(const vector<vector<int>>& process_time){
    vector<list<int>> que_copy(this->Q);
    for(int i=0; i<this->num_of_stage; ++i){
        if(this->reschedule_list[i].size()>1){
            operator_nehnm(que_copy[i], i, process_time);
        }
    }
    return DFSP1(this, que_copy);
}

DFSP1 DFSP1::resched_nehnm1(const vector<vector<int>>& process_time){
    vector<list<int>> que_copy(this->Q);
    for(int i=0; i<this->num_of_stage; ++i){
        if(this->reschedule_list[i].size()>1){
            operator_nehnm1(que_copy[i], i, process_time);
        }
    }
    return DFSP1(this, que_copy);
}

DFSP1 DFSP1::resched_nehkk(const vector<vector<int>>& process_time){
    vector<list<int>> que_copy(this->Q);
    for(int i=0; i<this->num_of_stage; ++i){
        if(this->reschedule_list[i].size()>1){
            operator_nehkk(que_copy[i], i, process_time);
        }
    }
    return DFSP1(this, que_copy);
}


DFSP1 DFSP1::resched_random_uct(const unsigned seed, const vector<vector<int>>& process_time){
    mt19937 gen{seed};
    uniform_real_distribution<float> dist01(0,1);
    vector<list<int>> que_copy(this->Q);
    for(int i=0; i<this->num_of_stage; ++i){
        if(this->reschedule_list[i].size()>1){
            float a = dist01(gen);
            if(a<0.13){
                operator_neh(que_copy[i], i, process_time);
            }
            else if(a<0.26){
                operator_neh1(que_copy[i], i, process_time);
            }
            else if(a<0.39){
                operator_nehnm(que_copy[i], i, process_time);
            }
            else if(a<0.52){
                operator_nehnm1(que_copy[i], i, process_time);
            }
            else if(a<0.65){
                operator_nehkk(que_copy[i], i, process_time);
            }
            else if(a<0.78){
                operator_ritm(que_copy[i], i, process_time, this->machines_per_stage);
            }
            else if(a<0.89){
                operator_spt(que_copy[i], i, process_time);
            }
        }
    }
    return DFSP1(this, que_copy);
}

DFSP1 DFSP1::resched_rand_insert(const unsigned seed){
    mt19937 gen{seed};
    vector<list<int>> que_copy(this->Q);
    for(int i=0; i<this->num_of_stage; ++i){
        if(this->reschedule_list[i].size()>1){
            int len = que_copy[i].size();
            int ind = gen() % len;
            auto iter = que_copy[i].begin();
            for(int k=0; k<ind; ++k){++iter;}
            int job_ind = *iter;
            que_copy[i].erase(iter);
            int insert_pos = gen()%len;
            while(insert_pos==ind){
                insert_pos = gen()%len;
            }
            iter = que_copy[i].begin();
            for(int k=0; k<insert_pos; ++k){++iter;}
            que_copy[i].insert(iter, job_ind);
        }
    }
    return DFSP1(this, que_copy);
}

// DFSP1 DFSP1::global_resched(const unsigned seed, const vector<vector<int>>& process_time){
//     vector<list<int>> que_copy(this->Q);
//     // random_global_resched(que_copy, this->reschedule_list, this->process_time, this->completion_table, this->machines_per_stage, seed);
//     random_global_resched(que_copy, this->current_time, this->num_of_stage, this->process_time, this->completion_table, this->machines_per_stage, this->machine_state, this->real_dispatch, this->br_prob, this->eff_prob, seed);

//     return DFSP1(this, que_copy);   
// }