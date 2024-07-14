import pandas as pd
import numpy as np
import scipy
import scipy.stats
import scikit_posthocs as sp
from decimal import Decimal

def create_df(npary, sizes, method, num):
    rows = []
    for i in range(num):
        if i+1<10:
            name = 'ta00'+str(i+1)
        elif i+1<100:
            name = 'ta0'+str(i+1)
        else:
            name = 'ta'+str(i+1)
        for k in range(20):
            rows.append({'name':name, 'index':str(i), 'seed':str(k), 'sizes':sizes[i//10], 'makespans':npary[i][k][0], 'times':npary[i][k][1], 'num_of_acc':npary[i][k][2], 'methods': method})
    return pd.DataFrame(rows)

def merge_df(list_df):
    if(len(list_df)<2):
        print("Wrong!")
        return
    elif len(list_df)==2:
        return pd.merge(list_df[0], list_df[1], how="outer")
    else:
        df_temp = pd.merge(list_df[0], list_df[1], how="outer")
        for i in range(2, len(list_df)):
            df_temp = pd.merge(df_temp, list_df[i], how="outer")
        return df_temp

def merge_df_slice(list_df, num1 = 0, num2 = 120):
    n1 = num1*20
    n2 = num2*20
    if(len(list_df)<2):
        print("Wrong!")
        return
    elif len(list_df)==2:
        return pd.merge(list_df[0].iloc[n1:n2], list_df[1].iloc[n1:n2], how="outer")
    else:
        df_temp = pd.merge(list_df[0].iloc[n1:n2], list_df[1].iloc[n1:n2], how="outer")
        for i in range(2, len(list_df)):
            df_temp = pd.merge(df_temp, list_df[i].iloc[n1:n2], how="outer")
        return df_temp

def calculate_rpd_seed_array(df, num1, num2):
    n1 = num1//10
    n2 = num2//10
    temp_methods = df['methods'].unique().tolist()
    temp_dflist = []
    for m in temp_methods:
        temp_dflist.append(df[df['methods']==m])
    # arpd = np.zeros((n2-n1)*10, 20, len(temp_methods))
    rpd = np.zeros(((n2-n1),10, 20, len(temp_methods)))
    for ind in range(n1, n2):
        for tempind in range(10):
            index = 10*ind + tempind
            minval = 100000000
            for seed in range(20):
                for tempdf in temp_dflist:
                    dfindex = tempdf[tempdf['index'] == str(index)]
                    tempmin = dfindex[dfindex['seed']==str(seed)]['makespans'].tolist()[0]
                    if tempmin>1 and tempmin<minval:
                        minval = tempmin
                for i, tempdf in enumerate(temp_dflist):
                    dfindex = tempdf[tempdf['index']==str(index)]
                    tempval = dfindex[dfindex['seed']==str(seed)]['makespans'].tolist()[0]
                    if tempval>1:
                        rpdval = (tempval-minval)/minval * 100
                    else:
                        rpdval = 0
                    # rpd[(ind-n1)*10+tempind][seed][i] = rpd
                    rpd[ind-n1][tempind][seed][i] = rpdval
    return rpd

def calculate_arpd_array(df, num1, num2):
    n1 = num1//10
    n2 = num2//10
    temp_methods = df['methods'].unique().tolist()
    temp_dflist = []
    for m in temp_methods:
        temp_dflist.append(df[df['methods']==m])
    arpd = np.zeros((n2-n1, 10,len(temp_methods)))
    for ind in range(n1, n2):
        for tempind in range(10):
            index = 10*ind + tempind
            minval = 10000000
            for tempdf in temp_dflist:
                tempmin = tempdf[tempdf['index']==str(index)]['makespans'].min()
                if tempmin>1 and tempmin<minval:
                    minval = tempmin
            for i, tempdf in enumerate(temp_dflist):
                meanval = tempdf[tempdf['index']==str(index)]['makespans'].mean()
                if meanval>1:
                    rpd = (meanval-minval)/minval * 100
                else:
                    rpd = 0
                arpd[ind-n1][tempind][i] = rpd
    return arpd

def calculate_rpd_seed(df, sizes, num1, num2):
    n1 = num1//10
    n2 = num2//10
    temp_methods = df['methods'].unique().tolist()
    temp_dflist = []
    rows = []
    for m in temp_methods:
        temp_dflist.append(df[df['methods']==m])
    # rpd = np.zeros(((n2-n1)*10, 20, len(temp_methods)))
    rpd = np.zeros(((n2-n1), 10, 20, len(temp_methods)))
    for ind in range(n1, n2):
        size = sizes[ind]
        for tempind in range(10):
            index = 10*ind + tempind
            minval = 100000000
            for seed in range(20):
                for tempdf in temp_dflist:
                    dfindex = tempdf[tempdf['index'] == str(index)]
                    tempmin = dfindex[dfindex['seed']==str(seed)]['makespans'].tolist()[0]
                    if tempmin>1 and tempmin<minval:
                        minval = tempmin
                for i, tempdf in enumerate(temp_dflist):
                    dfindex = tempdf[tempdf['index']==str(index)]
                    tempval = dfindex[dfindex['seed']==str(seed)]['makespans'].tolist()[0]
                    if tempval>1:
                        rpdval = (tempval-minval)/minval * 100
                    else:
                        rpdval = 0
                    rpd[ind-n1][tempind][seed][i] = rpdval
                # rows.append({'index':str(index), 'seed':str(seed), 'sizes':size, 'RPD':rpd[(ind-n1)*10+tempind][seed][i], 'methods':temp_methods[i]})
                    rows.append({'index':str(index), 'seed':str(seed), 'sizes':size, 'RPD':rpd[ind-n1][tempind][seed][i], 'methods':temp_methods[i]})
    return pd.DataFrame(rows)

# calculate arpd by the average of rpd
def calculate_arpd_new(df_rpd, sizes, num1, num2):
    n1 = num1//10
    n2 = num2//10
    temp_methods = df_rpd['methods'].unique().tolist()
    temp_dflist = []
    rows = []
    for m in temp_methods:
        temp_dflist.append(df_rpd[df_rpd['methods']==m])
    arpd = np.zeros((n2-n1, 10,len(temp_methods)))
    for ind in range(n1, n2):
        size = sizes[ind]
        for tempind in range(10):
            index = 10*ind + tempind
            for i, tempdf in enumerate(temp_dflist):
                temparpd = tempdf[tempdf['index']==str(index)]['RPD'].mean()
                arpd[ind-n1][tempind][i] = temparpd
                rows.append({'index':str(index), 'sizes':size, 'ARPD':arpd[ind-n1][tempind][i], 'methods':temp_methods[i]})
    return pd.DataFrame(rows)

def calculate_arpd_sz_new(df_arpd, sizes, num1, num2):
    n1 = num1//10
    n2 = num2//10
    temp_methods = df_arpd['methods'].unique().tolist()
    temp_dflist = []
    rows = []
    for m in temp_methods:
        temp_dflist.append(df_arpd[df_arpd['methods']==m])
    arpd = np.zeros(((n2-n1), len(temp_methods)))
    for ind in range(n1, n2):
        size = sizes[ind]
        for i, tempdf in enumerate(temp_dflist):
            temparpd = tempdf[tempdf['sizes']==size]['ARPD'].mean()
            arpd[ind-n1][i] = temparpd
            rows.append({'sizes':size, 'ARPD':arpd[ind-n1][i], 'methods':temp_methods[i]})
    return pd.DataFrame(rows)

def calculate_ave_time_sz(df, sizes, num1, num2):
    n1 = num1 // 10
    n2 = num2 // 10
    temp_methods = df['methods'].unique().tolist()
    rows = []
    for m in temp_methods:
        tempdf = df[df['methods']==m]
        for ind in range(n1, n2):
            size = sizes[ind]
            avetime = tempdf[tempdf['sizes']==size]['times'].sum()/tempdf[tempdf['sizes']==size]['num_of_acc'].sum()
            rows.append({'sizes':size, 'methods':m, 'time':avetime})
    return pd.DataFrame(rows)


def calculate_arpd(df, sizes, num1, num2):
    n1 = num1//10
    n2 = num2//10
    temp_methods = df['methods'].unique().tolist()
    temp_dflist = []
    rows = []
    for m in temp_methods:
        temp_dflist.append(df[df['methods']==m])
    arpd = np.zeros((n2-n1, 10,len(temp_methods)))
    for ind in range(n1, n2):
        size = sizes[ind]
        for tempind in range(10):
            index = 10*ind + tempind
            minval = 10000000
            for tempdf in temp_dflist:
                tempmin = tempdf[tempdf['index']==str(index)]['makespans'].min()
                if tempmin>1 and tempmin<minval:
                    minval = tempmin
            for i, tempdf in enumerate(temp_dflist):
                meanval = tempdf[tempdf['index']==str(index)]['makespans'].mean()
                if meanval>1:
                    rpd = (meanval-minval)/minval * 100
                else:
                    rpd = 0
                arpd[ind-n1][tempind][i] = rpd
                rows.append({'index':str(index), 'sizes':size, 'ARPD':arpd[ind-n1][tempind][i], 'methods':temp_methods[i]})
    return pd.DataFrame(rows)

def calculate_arpd_sz(df, sizes, num1, num2):
    n1 = num1//10
    n2 = num2//10
    arpd = calculate_arpd_array(df, num1, num2)
    arpd_sz = np.average(arpd, axis=1)
    rows = []
    temp_methods = df['methods'].unique().tolist()
    for i in range(n1,n2):
        for k, m in enumerate(temp_methods):
            rows.append({'sizes':sizes[i], 'ARPD':arpd_sz[i][k], 'methods':m})
    return pd.DataFrame(rows)

def calculate_var_rpd_seed(df_rpd, sizes, num1, num2):
    n1 = num1//10
    n2 = num2//10
    temp_methods = df_rpd['methods'].unique().tolist()
    temp_dflist = []
    rows = []
    for m in temp_methods:
        temp_dflist.append(df_rpd[df_rpd['methods']==m])
    for i, tempdf in enumerate(temp_dflist):
        for ind in range(n1, n2):
            size = sizes[ind]
            for tempind in range(10):
                index = 10*ind + tempind
                df_ind = tempdf[tempdf['index'] == index]
                var = df_ind['RPD'].var()
                rows.append({'index':str(index), 'sizes':size, 'Var':var, 'methods':temp_methods[i]})
    return pd.DataFrame(rows)

def calculate_var_seed(df_rpd, val, sizes, num1, num2):
    n1 = num1//10
    n2 = num2//10
    temp_methods = df_rpd['methods'].unique().tolist()
    temp_dflist = []
    rows = []
    for m in temp_methods:
        temp_dflist.append(df_rpd[df_rpd['methods']==m])
    for i, tempdf in enumerate(temp_dflist):
        for ind in range(n1, n2):
            size = sizes[ind]
            for tempind in range(10):
                index = 10*ind + tempind
                df_ind = tempdf[tempdf['index'] == str(index)]
                var = df_ind[val].var()
                # print(var)
                rows.append({'index':str(index), 'sizes':size, 'Var':var, 'methods':temp_methods[i]})
    return pd.DataFrame(rows)

def compare_win_lose(df, val, method, num1, num2):
    methods = df['methods'].unique().tolist()
    methods.remove(method)
    df2 = df[df['methods']==method]
    compare_list = []
    for m in methods:
        win = 0
        lose = 0
        tie = 0
        tempdf = df[df['methods']==m]
        for ind in range(num1, num2):
            tempdf1 = df2[df2['index']==str(ind)]
            tempdf2 = tempdf[tempdf['index']==str(ind)]
            for seed in range(20):
                temp1 = tempdf1[tempdf1['seed']==str(seed)][val].tolist()[0]
                temp2 = tempdf2[tempdf2['seed']==str(seed)][val].tolist()[0]
                if temp1<temp2:
                    win+=1
                elif temp1 == temp2:
                    tie+=1
                else:
                    lose += 1
        compare_list.append([method+' v.s. '+m, win, lose, tie])
        print(method, "v.s.", m)
        print("win:", win)
        print("lose:", lose)
        print("tie:", tie)
        print(scipy.stats.wilcoxon(df2[val].tolist(), tempdf[val].tolist(), alternative='two-sided'))
        print('----------------')
    return compare_list



def compare_makespan_win_lose(df_span, method, num1, num2):
    methods = df_span['methods'].unique().tolist()
    methods.remove(method)
    df2 = df_span[df_span['methods']==method]
    compare_list = []
    for m in methods:
        win = 0
        lose = 0
        tie = 0
        tempdf = df_span[df_span['methods']==m]
        for ind in range(num1, num2):
            tempdf1 = df2[df2['index']==str(ind)]
            tempdf2 = tempdf[tempdf['index']==str(ind)]
            for seed in range(20):
                temp1 = tempdf1[tempdf1['seed']==str(seed)]['makespans'].tolist()[0]
                temp2 = tempdf2[tempdf2['seed']==str(seed)]['makespans'].tolist()[0]
                if temp1<temp2:
                    win+=1
                elif temp1 == temp2:
                    tie+=1
                else:
                    lose += 1
        compare_list.append([method+' v.s. '+m, win, lose, tie])
        print(method, "v.s.", m)
        print("win:", win)
        print("lose:", lose)
        print("tie:", tie)
        print(scipy.stats.wilcoxon(df2['makespans'].tolist(), tempdf['makespans'].tolist(), alternative='two-sided'))
        print('----------------')
    return compare_list

def compare_makespan_dataframe(df_span, method, sizes, num):
    methods = df_span['methods'].unique().tolist()
    methods.remove(method)
    df2 = df_span[df_span['methods']==method]
    compare_list = []
    rows = []
    for tempind in range(num//10):
        size = sizes[tempind]
        for m in methods:
            win = 0
            lose = 0
            tie = 0
            tempdf = df_span[df_span['methods']==m]
            num1 = tempind*10
            num2 = num1+10
            for ind in range(num1, num2):
                tempdf1 = df2[df2['index']==str(ind)]
                tempdf2 = tempdf[tempdf['index']==str(ind)]
                for seed in range(20):
                    temp1 = tempdf1[tempdf1['seed']==str(seed)]['makespans'].tolist()[0]
                    temp2 = tempdf2[tempdf2['seed']==str(seed)]['makespans'].tolist()[0]
                    if temp1<temp2:
                        win+=1
                    elif temp1 == temp2:
                        tie+=1
                    else:
                        lose += 1
            dfsz1 = df2[df2['sizes']==size]
            dfsz2 = tempdf[tempdf['sizes']==size]
            p_value = scipy.stats.wilcoxon(dfsz1['makespans'].tolist(), dfsz2['makespans'].tolist(), alternative='two-sided')[1]
            rows.append({'origin method':method, 'compared method':m, 'sizes':size, 'compare result':(win, lose, tie), 'p-value':p_value})
    dfresult = pd.DataFrame(rows)
    return dfresult


def compare_rpd_win_lose(df_rpd, method, num1, num2):
    methods = df_rpd['methods'].unique().tolist()
    methods.remove(method)
    df2 = df_rpd[df_rpd['methods']==method]
    compare_list = []
    for m in methods:
        win = 0
        lose = 0
        tie = 0
        tempdf = df_rpd[df_rpd['methods']==m]
        for ind in range(num1, num2):
            tempdf1 = df2[df2['index']==str(ind)]
            tempdf2 = tempdf[tempdf['index']==str(ind)]
            for seed in range(20):
                temp1 = tempdf1[tempdf1['seed']==str(seed)]['RPD'].tolist()[0]
                temp2 = tempdf2[tempdf2['seed']==str(seed)]['RPD'].tolist()[0]
                if temp1<temp2:
                    win+=1
                elif temp1 == temp2:
                    tie+=1
                else:
                    lose += 1
        compare_list.append([method+' v.s. '+m, win, lose, tie])
        print(method, "v.s.", m)
        print("win:", win)
        print("lose:", lose)
        print("tie:", tie)
        print(scipy.stats.wilcoxon(df2['RPD'].tolist(), tempdf['RPD'].tolist(), alternative='two-sided'))
        print('----------------')
    return compare_list

def compare_rpd_dataframe(df_rpd, method, sizes, num):
    methods = df_rpd['methods'].unique().tolist()
    methods.remove(method)
    df2 = df_rpd[df_rpd['methods']==method]
    compare_list = []
    rows = []
    for tempind in range(num//10):
        size = sizes[tempind]
        for m in methods:
            win = 0
            lose = 0
            tie = 0
            tempdf = df_rpd[df_rpd['methods']==m]
            num1 = tempind*10
            num2 = num1+10
            for ind in range(num1, num2):
                tempdf1 = df2[df2['index']==str(ind)]
                tempdf2 = tempdf[tempdf['index']==str(ind)]
                for seed in range(20):
                    temp1 = tempdf1[tempdf1['seed']==str(seed)]['RPD'].tolist()[0]
                    temp2 = tempdf2[tempdf2['seed']==str(seed)]['RPD'].tolist()[0]
                    if temp1<temp2:
                        win+=1
                    elif temp1 == temp2:
                        tie+=1
                    else:
                        lose += 1
            dfsz1 = df2[df2['sizes']==size]
            dfsz2 = tempdf[tempdf['sizes']==size]
            p_value = scipy.stats.wilcoxon(dfsz1['RPD'].tolist(), dfsz2['RPD'].tolist(), alternative='two-sided')[1]
            rows.append({'origin method':method, 'compared method':m, 'sizes':size, 'compare result':(win, lose, tie), 'p-value':p_value})
    dfresult = pd.DataFrame(rows)
    return dfresult


def compare_arpd_win_lose(df1, method, num1, num2):
    methods = df1['methods'].unique().tolist()
    methods.remove(method)
    df2 = df1[df1['methods']==method]
    # method1 = df1['methods'].unique()[0]
    # rows = []
    compare_list = []
    for m in methods:
        win = 0
        lose = 0
        tie = 0
        tempdf = df1[df1['methods']==m]
        for ind in range(num1, num2):
            temp1 = df2[df2['index']==str(ind)]['ARPD'].tolist()[0]
            temp2 = tempdf[tempdf['index']==str(ind)]['ARPD'].tolist()[0]
            if temp1<temp2:
                win+=1
            elif temp1 == temp2:
                tie+=1
            else:
                lose += 1
        compare_list.append([method+' v.s. '+m, win, lose, tie])
        print(method, "v.s.", m)
        print("win:", win)
        print("lose:", lose)
        print("tie:", tie)
        print(scipy.stats.wilcoxon(df2['ARPD'].tolist(), tempdf['ARPD'].tolist(), alternative='two-sided'))
        print('----------------')
    return compare_list

def calculate_ave_resched_time(df, sizes, num0=0, num1=60):
    tlist = []
    for i in range(num0//10, num1//10):
        size = sizes[i]
        df_temp = df[df['sizes'] == size]
        totaltime = df_temp['times'].sum()
        totalacc = df_temp['num_of_acc'].sum()
        tlist.append(totaltime/totalacc)
    return tlist

def rank_matrix_makespan(df_span, method_list, num1=0, num2=60):
    # methods = df_rpd['methods'].unique().tolist()
    # print(methods)
    df_list = []
    for m in method_list:
        df_list.append(df_span[df_span['methods']==m])
    ranks = np.zeros(((num2-num1)*20, len(method_list)))
    # ranks = np.zeros(((num2-num1), 20, len(method_list)))
    for ind in range(num1, num2):
        # templist = df_arpd[df_arpd['index']==str(ind)].
        # templist = []
        for seed in range(20):
            templist = []
            for i in range(len(method_list)):
                tempdf = df_list[i]
                # templist.append(tempdf[tempdf['index']==str(ind)]['ARPD'].tolist()[0])
                dfindex = tempdf[tempdf['index']==str(ind)]
                templist.append(dfindex[dfindex['seed']==str(seed)]['makespans'].tolist()[0])
            # ranks[ind-num1][seed] = scipy.stats.rankdata(templist)
            ranks[(ind-num1)*20+seed] = scipy.stats.rankdata(templist)
    return ranks

def rank_matrix_rpd(df_rpd, method_list, num1=0, num2=60):
    # methods = df_rpd['methods'].unique().tolist()
    # print(methods)
    df_list = []
    for m in method_list:
        df_list.append(df_rpd[df_rpd['methods']==m])
    ranks = np.zeros(((num2-num1)*20, len(method_list)))
    # ranks = np.zeros(((num2-num1), 20, len(method_list)))
    for ind in range(num1, num2):
        for seed in range(20):
            templist = []
            for i in range(len(method_list)):
                tempdf = df_list[i]
                # templist.append(tempdf[tempdf['index']==str(ind)]['ARPD'].tolist()[0])
                dfindex = tempdf[tempdf['index']==str(ind)]
                templist.append(dfindex[dfindex['seed']==str(seed)]['RPD'].tolist()[0])
            # ranks[ind-num1][seed] = scipy.stats.rankdata(templist)
            ranks[(ind-num1)*20+seed] = scipy.stats.rankdata(templist)
    return ranks

def rank_matrix_arpd(df_arpd, method_list, num1=0, num2=60):
    # methods = df_arpd['methods'].unique().tolist()
    # print(methods)
    df_list = []
    for m in method_list:
        df_list.append(df_arpd[df_arpd['methods']==m])
    ranks = np.zeros((num2-num1, len(method_list)))
    for ind in range(num1, num2):
        # templist = df_arpd[df_arpd['index']==str(ind)].
        templist = []
        for i in range(len(method_list)):
            tempdf = df_list[i]
            templist.append(tempdf[tempdf['index']==str(ind)]['ARPD'].tolist()[0])
        # print(templist)
        # print(df_arpd[df_arpd['index']==str(ind)].tolist())
        ranks[ind-num1] = scipy.stats.rankdata(templist)
    return ranks

def friedman_test_makespan(df_span, methods, num1=0, num2=60):
    group_data_list = []
    for m in methods:
        tempdf = df_span[df_span['methods']==m]
        templist = []
        for i in range(num1, num2):
            dfindex = tempdf[tempdf['index']==str(i)]
            for seed in range(20):
                templist.append(dfindex[dfindex['seed']==str(seed)]['makespans'].tolist()[0])
        group_data_list.append(templist)
    # print(scipy.stats.friedmanchisquare(*group_data_list))
    return scipy.stats.friedmanchisquare(*group_data_list)

def friedman_test_rpd(df_rpd, methods, num1=0, num2=60):
    group_data_list = []
    for m in methods:
        tempdf = df_rpd[df_rpd['methods']==m]
        templist = []
        for i in range(num1, num2):
            dfindex = tempdf[tempdf['index']==str(i)]
            for seed in range(20):
                # templist.append(dfindex[dfindex['index']==str(i)]['RPD'].tolist()[0])
                templist.append(dfindex[dfindex['seed']==str(seed)]['RPD'].tolist()[0])
        group_data_list.append(templist)
    # print(scipy.stats.friedmanchisquare(*group_data_list))
    return scipy.stats.friedmanchisquare(*group_data_list)

def friedman_test_arpd(df, methods, num1=0, num2=60):
    group_data_list = []
    for m in methods:
        tempdf = df[df['methods']==m]
        templist = []
        for i in range(num1, num2):
            templist.append(tempdf[tempdf['index']==str(i)]['ARPD'].tolist()[0])
        group_data_list.append(templist)
    # print(scipy.stats.friedmanchisquare(*group_data_list))
    return scipy.stats.friedmanchisquare(*group_data_list)

def takeSecond(element):
    return element[1]

def sort_methods_arpd(df, methods, num1=0, num2=60):
    rankmtx = rank_matrix_arpd(df, methods, num1, num2)
    average_rank = np.average(rankmtx, axis=0)

    templist = []
    for i in range(len(methods)):
        templist.append((methods[i], average_rank[i]))
    templist.sort(key=takeSecond)    
    for ele in templist:
        print(ele)
    return templist
    
def sort_methods_rpd(df, methods, num1=0, num2=60):
    rankmtx = rank_matrix_rpd(df, methods, num1, num2)
    average_rank = np.average(rankmtx, axis=0)

    templist = []
    for i in range(len(methods)):
        templist.append((methods[i], average_rank[i]))
    templist.sort(key=takeSecond)    
    for ele in templist:
        print(ele)
    return templist
 
def sort_methods_makespan(df, methods, num1=0, num2=60):
    rankmtx = rank_matrix_makespan(df, methods, num1, num2)
    average_rank = np.average(rankmtx, axis=0)

    templist = []
    for i in range(len(methods)):
        templist.append((methods[i], average_rank[i]))
    templist.sort(key=takeSecond)    
    for ele in templist:
        print(ele)
    return templist
 
def cohen_d(data1, data2):
    diff = np.abs(np.mean(data1)-np.mean(data2))
    var1 = np.var(data1)
    var2 = np.var(data2)
    pooled_var = ((len(data1)-1)*var1 + (len(data2)-1)*var2)/(len(data1)+len(data2)-2)
    d = diff/np.sqrt(pooled_var)
    return d

def rpd_compare_df(dfrpd, mlist, mlist_compared, sizes):
    rows = []
    for m in mlist:
        dfm = dfrpd[dfrpd['methods']==m]
        mlist_compared.remove(m)
        # print(method)
        for n in mlist_compared:
            # print(n)
            dftemp = dfrpd[dfrpd['methods']==n]
            for sz in sizes:
                # print(sz)
                df1 = dfm[dfm['sizes']==sz]
                rpd1 = df1['RPD'].values
                arpd1 = np.average(rpd1)
                roundarpd1 = Decimal(str(arpd1))
                roundarpd1 = roundarpd1.quantize(Decimal('0.00'))
                df2 = dftemp[dftemp['sizes']==sz]
                rpd2 = df2['RPD'].values
                arpd2 = np.average(rpd2)

                win = 0
                lose = 0
                tie = 0
                for i in range(200):
                    if(rpd1[i]<rpd2[i]):
                        win += 1
                    elif(rpd1[i]>rpd2[i]):
                        lose += 1
                    else:
                        tie += 1
                roundarpd2 = Decimal(str(arpd2))
                roundarpd2 = roundarpd2.quantize(Decimal('0.00'))
                d = cohen_d(rpd1, rpd2)
                p = scipy.stats.wilcoxon(rpd1, rpd2, alternative='two-sided')[1]
                # rows.append({'sizes':sz, 'compare':m+" v.s. "+n, 'ARPD':'('+str(arpd1)+', '+str(arpd2)+')', 'Cohen-d':d, 'Wilcoxon':p})
                rows.append({'sizes':sz, 'methods':m+" v.s. "+n, 'compare':(win, lose, tie),  'ARPD':(str(roundarpd1),str(roundarpd2)), 'Cohen-d':d, 'Wilcoxon':p})
                # rows.append({'sizes':sz, 'methods':m+" v.s. "+n, 'compare':(win, lose, tie), 'ARPD':(roundarpd1 ,roundarpd2), 'Cohen-d':d, 'Wilcoxon':p})
    return pd.DataFrame(rows)


def rpd_compare_excel(dfrpd, mlist, mlist_compared, sizes, filename):
    rows = []
    for m in mlist:
        dfm = dfrpd[dfrpd['methods']==m]
        mlist_compared.remove(m)
        # print(method)
        for n in mlist_compared:
            # print(n)
            dftemp = dfrpd[dfrpd['methods']==n]
            for sz in sizes:
                # print(sz)
                df1 = dfm[dfm['sizes']==sz]
                rpd1 = df1['RPD'].values
                arpd1 = np.average(rpd1)
                roundarpd1 = Decimal(str(arpd1))
                roundarpd1 = roundarpd1.quantize(Decimal('0.00'))
                df2 = dftemp[dftemp['sizes']==sz]
                rpd2 = df2['RPD'].values
                arpd2 = np.average(rpd2)

                win = 0
                lose = 0
                tie = 0
                for i in range(200):
                    if(rpd1[i]<rpd2[i]):
                        win += 1
                    elif(rpd1[i]>rpd2[i]):
                        lose += 1
                    else:
                        tie += 1
                roundarpd2 = Decimal(str(arpd2))
                roundarpd2 = roundarpd2.quantize(Decimal('0.00'))
                d = cohen_d(rpd1, rpd2)
                p = scipy.stats.wilcoxon(rpd1, rpd2, alternative='two-sided')[1]
                # rows.append({'sizes':sz, 'compare':m+" v.s. "+n, 'ARPD':'('+str(arpd1)+', '+str(arpd2)+')', 'Cohen-d':d, 'Wilcoxon':p})
                # rows.append({'sizes':sz, 'methods':m+" v.s. "+n, 'compare':(win, lose, tie),  'ARPD':(str(roundarpd1),str(roundarpd2)), 'Wilcoxon':p, 'Cohen-d':d})
                rows.append({'sizes':sz, 'methods':m+" v.s. "+n, 'compare':(win, lose, tie),  'ARPD':(float(roundarpd1),float(roundarpd2)), 'Wilcoxon':p, 'Cohen-d':d})
                # rows.append({'sizes':sz, 'methods':m+" v.s. "+n, 'compare':(win, lose, tie), 'ARPD':(roundarpd1 ,roundarpd2), 'Cohen-d':d, 'Wilcoxon':p})
    df = pd.DataFrame(rows)
    df.to_excel(filename,index=False)
    return 0

def rpd_rank_tests(dfrpd, num1=0, num2=120):
    method_list = dfrpd['methods'].unique().tolist()
    rows = []

    df_list = []
    for m in method_list:
        df_list.append(dfrpd[dfrpd['methods']==m])
    ranks = np.zeros(((num2-num1)*20, len(method_list)))
    for ind in range(num1, num2):
        for seed in range(20):
            templist = []
            for i in range(len(method_list)):
                tempdf = df_list[i]
                dfindex = tempdf[tempdf['index']==str(ind)]
                templist.append(dfindex[dfindex['seed']==str(seed)]['RPD'].tolist()[0])
            ranks[(ind-num1)*20+seed] = scipy.stats.rankdata(templist)
    ave_rank = np.average(ranks, axis=0)
    method_rank = []
    for i in range(len(method_list)):
        method_rank.append([method_list[i], ave_rank[i]])
    method_rank = sorted(method_rank, key=lambda x:x[1])
    rows.append({'method':method_rank[0][0], 'rank':method_rank[0][1], 'Wilcoxon':1, 'Cohen-d':0})
    rpd1 = dfrpd[dfrpd['methods']==method_rank[0][0]]['RPD'].tolist()
    rpdlist = [rpd1]
    for i in range(1, len(method_list)):
        rpd2 = dfrpd[dfrpd['methods']==method_rank[i][0]]['RPD'].tolist()
        rpdlist.append(rpd2)
        d = cohen_d(rpd1, rpd2)
        wilcoxon = scipy.stats.wilcoxon(rpd1, rpd2, alternative='two-sided')[1]
        rows.append({'method':method_rank[i][0], 'rank':method_rank[i][1], 'Wilcoxon':wilcoxon, 'Cohen-d':d})
    
    f = scipy.stats.friedmanchisquare(*rpdlist)
    rows.append({'method':'Friedman','rank':100, 'Wilcoxon':f, 'Cohen-d':0})
    return pd.DataFrame(rows)

def makespan_rank_tests(dfspan, num1=0, num2=120):
    method_list = dfspan['methods'].unique().tolist()
    rows = []

    df_list = []
    for m in method_list:
        df_list.append(dfspan[dfspan['methods']==m])
    ranks = np.zeros(((num2-num1)*20, len(method_list)))
    for ind in range(num1, num2):
        for seed in range(20):
            templist = []
            for i in range(len(method_list)):
                tempdf = df_list[i]
                dfindex = tempdf[tempdf['index']==str(ind)]
                templist.append(dfindex[dfindex['seed']==str(seed)]['makespans'].tolist()[0])
            ranks[(ind-num1)*20+seed] = scipy.stats.rankdata(templist)
    ave_rank = np.average(ranks, axis=0)
    method_rank = []
    for i in range(len(method_list)):
        method_rank.append([method_list[i], ave_rank[i]])
    method_rank = sorted(method_rank, key=lambda x:x[1])
    rows.append({'method':method_rank[0][0], 'rank':method_rank[0][1], 'Wilcoxon':1, 'Cohen-d':0})
    span1 = dfspan[dfspan['methods']==method_rank[0][0]]['makespans'].tolist()
    spanlist = [span1]
    for i in range(1, len(method_list)):
        span2 = dfspan[dfspan['methods']==method_rank[i][0]]['makespans'].tolist()
        spanlist.append(span2)
        d = cohen_d(span1, span2)
        wilcoxon = scipy.stats.wilcoxon(span1, span2, alternative='two-sided')[1]
        rows.append({'method':method_rank[i][0], 'rank':method_rank[i][1], 'Wilcoxon':wilcoxon, 'Cohen-d':d})
    
    f = scipy.stats.friedmanchisquare(*spanlist)
    rows.append({'method':'Friedman','rank':100, 'Wilcoxon':f, 'Cohen-d':0})
    return pd.DataFrame(rows)

