import os 
import glob
import numpy as np
import RSCD
import random
import fspbenchmark
current_dir = os.path.dirname(os.path.realpath(__file__))

small_path = current_dir + "/benchmarks/Ahmed_Ruiz_HFWDDW_online/Benchmarks/Small/Small 25 SDST/"
large_path = current_dir + "/benchmarks/Ahmed_Ruiz_HFWDDW_online/Benchmarks/Large/Large_Instances_25_SDST/"

def solve_small(pipeout, para=[]):
    num = para[0]
    ins = para[1]
    partialname = str(num) + "_Instance_*.txt"
    filename = glob.glob(small_path+partialname)[0]
    jobs, stages = ins.shape
    hfsp = RSCD.HFSP(stages, jobs-1, ins[1:], ins[0])
    tempsolu = hfsp.SA(10000, 0.9, 10000, 100, 100)[-1]
    soluarray = np.asarray(tempsolu)
    savepath = str.replace(filename, "Small 25 SDST", "solutions")
    pipeout.send([soluarray, savepath[0:-4]])
    return None

def solve_large(pipeout, para=[]):
    num = para[0]
    ins = para[1]
    partialname = str(num) + "_Instance_*.txt"
    filename = glob.glob(large_path+partialname)[0]
    jobs, stages = ins.shape
    hfsp = RSCD.HFSP(stages, jobs-1, ins[1:], ins[0])
    tempsolu = hfsp.SA(10000, 0.9, 10000, 100, 100)[-1]
    soluarray = np.asarray(tempsolu)
    savepath = str.replace(filename, "Large_Instances_25_SDST", "solutions")
    pipeout.send([soluarray, savepath[0:-4]])
    return None

# 设置初始解
def set_init_plan_small(num=1620):
    instances = fspbenchmark.small_instance(num)
    filenames = []
    for i in range(num):
        partialname = str(i) + "_Instance_*.txt"
        # partialname = str(i) + "_*.txt"
        temp_file_name = glob.glob(small_path+partialname)[0]
        filenames.append(temp_file_name)
        
    for ind, ins in enumerate(instances):
        jobs, stages = ins.shape
        hfsp = RSCD.HFSP(stages, jobs-1, ins[1:], ins[0])
        tempsolu = hfsp.SA(10000, 0.9, 10000, 100, 100)[-1]
        soluarry = np.asarray(tempsolu)
        temp_name = filenames[ind]
        temp_name = str.replace(temp_name, "Small 25 SDST", "solutions")
        np.save(temp_name[0:-4], soluarry)

def set_init_plan_large(num=1440):
    instances = fspbenchmark.large_instance(num)
    filenames = []
    for i in range(num):
        partialname = str(i) + "_Instance_*.txt"
        # partialname = str(i) + "_*.txt"
        temp_file_name = glob.glob(large_path+partialname)[0]
        filenames.append(temp_file_name)
        
    for ind, ins in enumerate(instances):
        jobs, stages = ins.shape
        hfsp = RSCD.HFSP(stages, jobs-1, ins[1:], ins[0])
        tempsolu = hfsp.SA(10000, 0.9, 10000, 100, 100)[-1]
        soluarry = np.asarray(tempsolu)
        temp_name = filenames[ind]
        temp_name = str.replace(temp_name, "Large_Instances_25_SDST", "solutions")
        np.save(temp_name[0:-4], soluarry)


# Since the average processing times are mostly around 50 and max processing times are mostly 99, to make sure most jobs would complete
# within a reasonable probability, the transition probability from well status to failure status is within 0.001 (or 0.003) for low (or high)
# disruption probability. 
# The expected repair time is the reverse of the transition probability from bad to good, thus randomly chosen from 0.01 to 0.05.
# 
# 为 Ruiz 的小规模 HFSP 设置转移概率
def set_transition_prob_small(num=1620, disrupt="brkd"):
    instances = fspbenchmark.small_instance(num)
    filenames = []
    for i in range(num):
        partialname = str(i) + "_Instance_*.txt"
        # partialname = str(i) + "_*.txt"
        temp_file_name = glob.glob(small_path+partialname)[0]
        filenames.append(temp_file_name)
    if disrupt=="brkd":
        for ind, ins in enumerate(instances):
            machines_perstage = ins[0]
            jobs, stages = ins.shape
            num_machine = machines_perstage[0]
            for ele in machines_perstage:
                assert(num_machine==ele)
            p00 = np.random.random(size=(stages, num_machine))
            # p00*=0.04
            p00*=0.025
            # p00+=0.95
            p00+=0.97
            p11_low = np.random.random(size=(stages, num_machine))
            p11_low*=0.002
            p11_low+=0.998
            p11_high = np.random.random(size=(stages, num_machine))
            p11_high*=0.003
            p11_high+=0.995
            trans = np.stack([p00, p11_low, p11_high], axis=0)
            temp_name = filenames[ind]
            temp_name = str.replace(temp_name, "Small 25 SDST", "transition_prob/brkd")
            np.save(temp_name[0:-4], trans)
    elif disrupt=="effi":
        for ind, ins in enumerate(instances):
            machines_perstage = ins[0]
            jobs, stages = ins.shape
            num_machine = machines_perstage[0]
            for ele in machines_perstage:
                assert(num_machine==ele)
            p00_low = np.random.random(size=(stages, num_machine))
            p00_low*=0.3
            p00_low+=0.2
            p00_high = np.random.random(size=(stages, num_machine))
            p00_high*=0.05
            p00_high+=0.75
            p11 = np.random.random(size=(stages, num_machine))
            p11*=0.2
            p11+=0.8
            trans = np.stack([p00_low, p00_high, p11], axis=0)
            temp_name = filenames[ind]
            temp_name = str.replace(temp_name, "Small 25 SDST", "transition_prob/effi")
            np.save(temp_name[0:-4], trans)

# 为 Ruiz 的大规模 HFSP 设置转移概率
def set_transition_prob_large(num=1440, disrupt="brkd"):
    instances = fspbenchmark.large_instance(num)
    filenames = []
    for i in range(num):
        partialname = str(i) + "_Instance_*.txt"
        # partialname = str(i) + "_*.txt"
        temp_file_name = glob.glob(large_path+partialname)[0]
        filenames.append(temp_file_name)
    if disrupt=="brkd":
        for ind, ins in enumerate(instances):
            machines_perstage = ins[0]
            jobs, stages = ins.shape
            num_machine = machines_perstage[0]
            for ele in machines_perstage:
                assert(num_machine==ele)
            p00 = np.random.random(size=(stages, num_machine))
            # p00*=0.04
            p00*=0.025
            # p00+=0.95
            p00+=0.97
            p11_low = np.random.random(size=(stages, num_machine))
            p11_low*=0.001
            p11_low+=0.999
            p11_high = np.random.random(size=(stages, num_machine))
            p11_high*=0.002
            # p11_high*=0.003
            p11_high+=0.998
            # p11_high+=0.995
            trans = np.stack([p00, p11_low, p11_high], axis=0)
            temp_name = filenames[ind]
            temp_name = str.replace(temp_name, "Large_Instances_25_SDST", "transition_prob/brkd")
            np.save(temp_name[0:-4], trans)
    elif disrupt=="effi":
        for ind, ins in enumerate(instances):
            machines_perstage = ins[0]
            jobs, stages = ins.shape
            num_machine = machines_perstage[0]
            for ele in machines_perstage:
                assert(num_machine==ele)
            p00_low = np.random.random(size=(stages, num_machine))
            p00_low*=0.3
            p00_low+=0.2
            p00_high = np.random.random(size=(stages, num_machine))
            p00_high*=0.05
            p00_high+=0.75
            p11 = np.random.random(size=(stages, num_machine))
            p11*=0.2
            p11+=0.8
            trans = np.stack([p00_low, p00_high, p11], axis=0)
            temp_name = filenames[ind]
            temp_name = str.replace(temp_name, "Large_Instances_25_SDST", "transition_prob/effi")
            np.save(temp_name[0:-4], trans)

# 为 Ruiz 的 HFSP 设置新工作到达信息，与 Taillar的 的新工作到达逻辑不同。
# 根据所给参数 "small" or "large" 而对响应规模的算例设置新工作信息。
def set_new_jobs(bench="small"):
    if bench=="small":
        filenames = []
        for i in range(1620):
            partialname = str(i) + "_Instance_*.txt"
            # partialname = str(i) + "_*.txt"
            temp_file_name = glob.glob(small_path+partialname)[0]
            filenames.append(temp_file_name)
        instances = fspbenchmark.small_instance()
        for ind, ins in enumerate(instances):
            rows, cols = ins.shape
            jobs = rows-1
            stages = cols
            lowbound = np.min(ins[1:])
            upbound = np.max(ins[1:])
            avertime_first_stage = round(np.sum(ins[1:], axis=0)[0]/ins[0][0])
            num_of_new = jobs // 5
            newjobs = np.random.randint(lowbound, upbound, size=(num_of_new, stages))
            arrive_times = np.random.randint(1, avertime_first_stage, size=(num_of_new))
            arrive_times.sort()
            # sort arrive_times
            new_with_time = np.concatenate([newjobs, np.reshape(arrive_times, (-1,1))], axis=1)
            temp_name = filenames[ind]
            temp_name = str.replace(temp_name, "Small 25 SDST", "new_jobs")
            np.save(temp_name[0:-4], new_with_time)

    elif bench=="large":
        filenames = []
        for i in range(1440):
            partialname = str(i) + "_Instance_*.txt"
            # partialname = str(i) + "_*.txt"
            temp_file_name = glob.glob(large_path+partialname)[0]
            filenames.append(temp_file_name)
        instances = fspbenchmark.large_instance()
        for ind, ins in enumerate(instances):
            rows, cols = ins.shape
            jobs = rows-1
            stages = cols
            lowbound = np.min(ins[1:])
            upbound = np.max(ins[1:])
            avertime_first_stage = round(np.sum(ins[1:], axis=0)[0]/ins[0][0])
            num_of_new = jobs // 5
            newjobs = np.random.randint(lowbound, upbound, size=(num_of_new, stages))
            arrive_times = np.random.randint(1, avertime_first_stage, size=(num_of_new))
            arrive_times.sort()
            new_with_time = np.concatenate([newjobs, np.reshape(arrive_times, (-1,1))], axis=1)
            temp_name = filenames[ind]
            temp_name = str.replace(temp_name,  "Large_Instances_25_SDST", "new_jobs")
            np.save(temp_name[0:-4], new_with_time)   



# Taillard
# 为 Taillard 算例设置新工作信息
def set_new_jobs_taillard():
    path = current_dir + "/benchmarks/taillard/new_jobs/"
    instances, solutions = fspbenchmark.import_taillard()
    num_new_job = [10, 10, 10, 25, 25, 25, 50, 50, 50, 100, 100, 250]
    batchsize = 5
    for i, ins in enumerate(instances):
        span = solutions[i][0]
        n, m = ins.shape
        ind = i // 10
        num_batch = num_new_job[ind]//batchsize
        j = i+1
        if j < 10:
            name = "00" + str(j)
        elif j < 100:
            name = "0" + str(j)
        else:
            name = str(j)
        f_low = path + "low/ta" + name + ".txt"
        f_high = path + "high/ta" + name + ".txt"

        arrive_time_low = np.random.exponential(200, num_batch).astype(int)
        for foo in range(1, len(arrive_time_low)):
            arrive_time_low[foo] += arrive_time_low[foo-1]
        assert(arrive_time_low[-1]<span)
        mtx_list_low = []
        for foo in range(num_batch):
            mtx_list_low.append(np.random.randint(1, 100, (batchsize, m)))
        with open(f_low, 'w') as f:
            f.write(str(num_batch)+' ')
            f.write('5 ')
            f.write(str(m)+' \n')
            for ele in arrive_time_low:
                f.write(str(ele)+' ')
            f.write('\n')
            for tempind, mtx in enumerate(mtx_list_low):
                for jobind in range(batchsize):
                    f.write(str(arrive_time_low[tempind])+' ')
                    for machine in range(m):
                        f.write(str(mtx[jobind][machine])+' ')
                    f.write('\n')
        
        arrive_time_high = np.random.exponential(150, num_batch*2).astype(int)
        for foo in range(1, len(arrive_time_high)):
            arrive_time_high[foo] += arrive_time_high[foo-1]
        assert(arrive_time_high[-1]<span)
        mtx_list_high = []
        for foo in range(2*num_batch):
            mtx_list_high.append(np.random.randint(1,100,(batchsize, m)))
        with open(f_high, 'w') as f:
            f.write(str(2*num_batch)+' ')
            f.write('5 ')
            f.write(str(m)+' \n')
            for ele in arrive_time_high:
                f.write(str(ele)+' ')
            f.write('\n')
            for tempind, mtx in enumerate(mtx_list_high):
                for jobind in range(batchsize):
                    f.write(str(arrive_time_high[tempind])+' ')
                    for machine in range(m):
                        f.write(str(mtx[jobind][machine])+' ')
                    f.write('\n')
            
def set_brkd_transition_prob_taillard_low():
    path = "/home/liulab/Workspace/Projects/Scheduling/HFSP/benchmarks/taillard/transition_prob/"
    #os.makedirs(path)
    taillard, solutions = fspbenchmark.import_taillard()
    rat00 = 0
    rat11 = 0
    for i, instance in enumerate(taillard):
        if i<70:
            rat00 = 0.001
            rat11 = 0.999
        elif i< 110:
            rat00 = 0.0005
            rat11 = 0.9995
        else:
            rat00 = 0.0002
            rat11 = 0.9998
        m = instance.shape[1]
        p00 = np.random.random(size=(m,1))
        p00 *= 0.025
        p00 += 0.97
        p11_low = np.random.random(size=(m,1))
        p11_low *= rat00
        p11_low += rat11
        trans = np.stack([p00, p11_low], axis=0)

        j = i+1
        if j < 10:
            name = "00" + str(j)
        elif j < 100:
            name = "0" + str(j)
        else:
            name = str(j)
        np.save(path + "brkd/" + "ta" + name + ".npy", trans)
    return None



# if __name__ == '__main__':
    # set_init_plan_small(10)
    # set_init_plan_large(5)
    # name = "00001301dfa11234.txt"
    # name2 = str.replace(name, "112","221")
    # print(name2)
    # name2 = name[0:-4]
    # print(name2)
    # set_transition_prob_large(1440,"effi")
    # set_transition_prob_large(1440,"brkd")
    # set_transition_prob_small(1620, "brkd")
    # set_new_jobs("small")
    # set_new_jobs("large")
    # set_brkd_transition_prob_taillard()
    # set_brkd_transition_prob_taillard_low()
    # set_new_jobs_taillard()