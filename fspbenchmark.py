import os 
import glob
import numpy as np
curr_dir = os.path.dirname(os.path.realpath(__file__))

def small_instance(num=1620):
    file_dir = curr_dir + "/benchmarks/Ahmed_Ruiz_HFWDDW_online/Benchmarks/Small/Small 25 SDST/"
    instance_list = []
    for i in range(num):
        instance = []
        partname = str(i) + "_Instance_*.txt"
        file_name = glob.glob(file_dir+partname)[0]
        file = open(file_name, 'r')
        file.readline()
        info = file.readline().split()
        num_job = int(info[0])
        num_machine = int(info[1])
        num_stage = int(info[2])
        machine_per_stage = num_machine // num_stage
        instance.append(np.asarray([machine_per_stage for ind in range(num_stage)]))

        for j in range(num_job):
            job = []
            line = file.readline()
            foo = 0
            for item in line.split():
                if item != " " and item != "\n":
                    if foo%2 == 1:
                        job.append(int(item))
                    foo+=1
            instance.append(job)
        array = np.asarray(instance, dtype=np.int32)
        instance_list.append(array)
    return instance_list

def large_instance(num=1440):
    file_dir = curr_dir + "/benchmarks/Ahmed_Ruiz_HFWDDW_online/Benchmarks/Large/Large_Instances_25_SDST/"
    instance_list = []
    for i in range(num):
        instance = []
        partname = str(i) + "_Instance_*.txt"
        file_name = glob.glob(file_dir+partname)[0]
        file = open(file_name, 'r')
        file.readline()
        info = file.readline().split()
        num_job = int(info[0])
        num_machine = int(info[1])
        num_stage = int(info[2])
        machine_per_stage = num_machine // num_stage
        instance.append(np.asarray([machine_per_stage for ind in range(num_stage)]))

        for j in range(num_job):
            job = []
            line = file.readline()
            foo = 0
            for item in line.split():
                if item != " " and item != "\n":
                    if foo%2 == 1:
                        job.append(int(item))
                    foo+=1
            instance.append(job)
        array = np.asarray(instance, dtype=np.int32)
        instance_list.append(array)
    return instance_list

def small_instance_solu(num=1620):
    file_dir = curr_dir + "/benchmarks/Ahmed_Ruiz_HFWDDW_online/Benchmarks/Small/Small 25 SDST/"
    solu_dir = curr_dir + "/benchmarks/Ahmed_Ruiz_HFWDDW_online/Benchmarks/Small/solutions/"
    instance_list = []
    solu_list = []
    for i in range(num):
        instance = []
        partname = str(i) + "_Instance_*.txt"
        file_name = glob.glob(file_dir+partname)[0]
        file = open(file_name, 'r')
        file.readline()
        info = file.readline().split()
        num_job = int(info[0])
        num_machine = int(info[1])
        num_stage = int(info[2])
        machine_per_stage = num_machine // num_stage
        instance.append(np.asarray([machine_per_stage for ind in range(num_stage)]))

        for j in range(num_job):
            job = []
            line = file.readline()
            foo = 0
            for item in line.split():
                if item != " " and item != "\n":
                    if foo%2 == 1:
                        job.append(int(item))
                    foo+=1
            instance.append(job)
        array = np.asarray(instance, dtype=np.int32)
        instance_list.append(array)

        part_solu_name = str(i) + "_Instance_*.npy"
        solu_name = glob.glob(solu_dir+part_solu_name)[0]
        solu_array = np.load(solu_name, 'r')
        solu_list.append(solu_array)
    return instance_list, solu_list

def large_instance_solu(num=1440):
    file_dir = curr_dir + "/benchmarks/Ahmed_Ruiz_HFWDDW_online/Benchmarks/Large/Large_Instances_25_SDST/"
    solu_dir = curr_dir + "/benchmarks/Ahmed_Ruiz_HFWDDW_online/Benchmarks/Large/solutions/"
    instance_list = []
    solu_list = []
    for i in range(num):
        instance = []
        partname = str(i) + "_Instance_*.txt"
        file_name = glob.glob(file_dir+partname)[0]
        file = open(file_name, 'r')
        file.readline()
        info = file.readline().split()
        num_job = int(info[0])
        num_machine = int(info[1])
        num_stage = int(info[2])
        machine_per_stage = num_machine // num_stage
        instance.append(np.asarray([machine_per_stage for ind in range(num_stage)]))

        for j in range(num_job):
            job = []
            line = file.readline()
            foo = 0
            for item in line.split():
                if item != " " and item != "\n":
                    if foo%2 == 1:
                        job.append(int(item))
                    foo+=1
            instance.append(job)
        array = np.asarray(instance, dtype=np.int32)
        instance_list.append(array)

        part_solu_name = str(i) + "_Instance_*.npy"
        solu_name = glob.glob(solu_dir+part_solu_name)[0]
        solu_array = np.load(solu_name, 'r')
        solu_list.append(solu_array)

    return instance_list, solu_list

def get_trans_prob(bench = "small", num = 100):
    brkdlist = []
    effilist = []
    if bench=="small":
        brkd_path = curr_dir + "/benchmarks/Ahmed_Ruiz_HFWDDW_online/Benchmarks/Small/transition_prob/brkd/"
        effi_path = curr_dir + "/benchmarks/Ahmed_Ruiz_HFWDDW_online/Benchmarks/Small/transition_prob/effi/"
        # for i in range(1620):
        for i in range(num):
            partname = str(i) + "_Instance_*.npy"
            file_name_brkd = glob.glob(brkd_path+partname)[0]
            file_name_effi = glob.glob(effi_path+partname)[0]
            array_brkd = np.load(file_name_brkd, 'r')
            brkdlist.append(array_brkd)
            array_effi = np.load(file_name_effi, 'r')
            effilist.append(array_effi)
    elif bench=="large":
        brkd_path = curr_dir + "/benchmarks/Ahmed_Ruiz_HFWDDW_online/Benchmarks/Large/transition_prob/brkd/"
        effi_path = curr_dir + "/benchmarks/Ahmed_Ruiz_HFWDDW_online/Benchmarks/Large/transition_prob/effi/"
        # for i in range(1440):
        for i in range(num):
            partname = str(i) + "_Instance_*.npy"
            file_name_brkd = glob.glob(brkd_path+partname)[0]
            file_name_effi = glob.glob(effi_path+partname)[0]
            array_brkd = np.load(file_name_brkd, 'r')
            brkdlist.append(array_brkd)
            array_effi = np.load(file_name_effi, 'r')
            effilist.append(array_effi)
    return brkdlist, effilist

def get_new_jobs(bench = "small", num = 100):
    new_job_list = []
    if bench == "small":
        job_path = curr_dir + "/benchmarks/Ahmed_Ruiz_HFWDDW_online/Benchmarks/Small/new_jobs/"
        # for i in range(1620):
        for i in range(num):
            partname = str(i) + "_Instance_*.npy"
            file_name_job = glob.glob(job_path+partname)[0]
            array_job = np.load(file_name_job, 'r')
            new_job_list.append(array_job)
    elif bench == "large":
        job_path = curr_dir + "/benchmarks/Ahmed_Ruiz_HFWDDW_online/Benchmarks/Large/new_jobs/"
        # for i in range(1440):
        for i in range(num):
            partname = str(i) + "_Instance_*.npy"
            file_name_job = glob.glob(job_path+partname)[0]
            array_job = np.load(file_name_job, 'r')
            new_job_list.append(array_job)
    return new_job_list


def import_taillard(num=120):
    instances = []
    solutions = []
    path_ins = curr_dir + "/benchmarks/taillard/instances/"
    path_solu = curr_dir + "/benchmarks/taillard/solutions/"
    arry = np.array([i for i in range(1, num+1)], dtype=int)
    for i in arry:
        if i < 10:
            name = "00" + str(i)
        elif i < 100:
            name = "0" + str(i)
        else:
            name = str(i)
        file = open(path_ins+"ta"+ name, 'r')
        file.readline()
        solution = np.load(path_solu+"ta" + name+".npy", 'r')

        instance = []
        for line in file.readlines():
            job = []
            _ = 0
            for item in line.split():
                if item != " " and item != "\n":
                    if _%2 == 1:
                        job.append(int(item))
                    _ += 1
            instance.append(job) 
        array = np.asarray(instance, dtype=np.int32)
        instances.append(array)
        solutions.append(solution)

    return instances, solutions

def get_new_jobs_ta(level="low"):
    path = curr_dir + "/benchmarks/taillard/new_jobs/" + level + "/"
    inds = np.array([i for i in range(1, 121)], dtype=int)
    list_mtx = []
    list_ariv_times = []
    list_batch_size = []
    for ind in inds:
        if ind<10:
            name = '00'+str(ind)
        elif ind < 100:
            name = '0'+str(ind)
        else:
            name = str(ind)
        file = open(path+'ta'+name+'.txt', 'r')
        line1 = file.readline()
        line1 = line1.split()
        num_batch = int(line1[0])
        # batch_size = int(line1[1])
        line2 = file.readline()
        arrive_time_list = line2.split()
        mtx = []
        for line in file.readlines():
            job = line.split()
            row = job[1:]
            mtx.append(row)
        # print(num_batch, len(arrive_time_list), batch_size)
        assert(num_batch==len(arrive_time_list))
        # list_batch_size.append(batch_size)
        list_ariv_times.append(np.asarray(arrive_time_list, dtype=np.int32))
        list_mtx.append(np.asarray(mtx, dtype=np.int32))
    
    # return list_batch_size, list_ariv_times, list_mtx
    return list_ariv_times, list_mtx


def get_efficiency_prob_ta():
    brkdlist = []
    effilist = []
    effi_path = curr_dir + "/benchmarks/taillard/transition_prob/efficiency/"
    arry = np.array([i for i in range(1, 121)], dtype=int)
    for i in arry:
        if i < 10:
            name = "00" + str(i)
        elif i < 100:
            name = "0" + str(i)
        else:
            name = str(i)
        file_name_effi = effi_path + "ta" + name + ".npy"
        array_effi = np.load(file_name_effi, 'r')
        effilist.append(array_effi)
       
    return effilist

def get_brkd_prob_ta():
    brkdlist = []
    # effilist = []
    brkd_path = curr_dir + "/benchmarks/taillard/transition_prob/brkd/"
    # effi_path = curr_dir + "/benchmarks/taillard/transition_prob/efficiency/"
    arry = np.array([i for i in range(1, 121)], dtype=int)
    for i in arry:
        if i < 10:
            name = "00" + str(i)
        elif i < 100:
            name = "0" + str(i)
        else:
            name = str(i)
        file_name_brkd = brkd_path + "ta" + name + ".npy"
        # file_name_effi = effi_path + "ta" + name + ".npy"
        array_brkd = np.load(file_name_brkd, 'r')
        brkdlist.append(array_brkd)
        # array_effi = np.load(file_name_effi, 'r')
        # effilist.append(array_effi)
    return brkdlist

if __name__ == "__main__":
#     small_instance(1)
    # import_taillard(1)
    get_new_jobs_ta()