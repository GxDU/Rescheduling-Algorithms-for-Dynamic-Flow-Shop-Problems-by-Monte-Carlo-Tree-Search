# 项目简介
本项目建模动态流水车间调度问题并实现了基于蒙特卡洛树搜索（MCTS）的局部重调度算法。模型和算法的细节请参考[1]。

# 环境要求
* Linux 操作系统
* Python 3.11
* C++17
* Cmake 3.20及更高版本
* pybind11

# 使用方法
* 安装 pybind11 （https://pybind11.readthedocs.io/en/stable/installing.html），安装成功后执行以下步骤
    * cd /cppsource/build
    * cmake ..
    * make
    * 将 RSCD.cpython-311-x86_64-linux-gnu.so 文件拷贝到 python 程序所在目录

* 参考 **example1.ipynb** 和 **example2.ipynb** 以使用 RSCD 库处理流水车间问题以及动态流水车间重调度问题。


# 项目结构
* **README.md** //帮助文档
* **benchmarks**    // Taillard 置换流水车间数据集和 Ruiz 的动态流水车间数据集
    * **Ahmed_Ruiz_HFWDDW_online** // Ruiz 柔性流水车间数据集，参考文献 _A Parameter-less Iterated Greedy Method for the Hybrid Flowshop Scheduling Problem with Setup Times
and Due Windows_
    * **taillard**
        * **instances**//120个 Taillard 置换流水车间算例
        * **new_jobs**  //每个动态车间算例中新工作的到达时间以及在每台机器上的加工时间，有高扰动和低扰动两个文件夹
        * **solutions** // Taillard 置换流水车间调度问题的已知最优解，参考文献 _A computationally efficient Branch-and-Bound algorithm for the permutation flow-shop scheduling problem_
        * **transition_prob**//动态事件的发生概率
            * **efficiency**//每台机器的效率水平转移概率，关于效率水平的定义和描述，可参考[2]。
            * **low_brkd**  //每台机器的算坏和修复转移概率
* **cppsource** //问题模型和算法的 C++ 实现
    * **build** //文件编译结果输出文件夹
    * **extern** //pybind11 安装文件夹
    * **CMakeLists.txt**
    * **dynamic_workshop.cpp** & **dynamic_workshop.h** 定义了动态流水车间对象 DFSP1，根据状态空间方程模拟车间运行的函数和重调度函数。具体可见头文件中的注释。
    * **experiments.cpp** & **experiments.h** 包含利用各种算法做重调度的实验程序。
    * **funcs.cpp** & **funcs.h**  一些基本函数，包括利用转移概率计算期望时间的函数、特殊数据结构打印函数、数组比较函数、轮盘赌选择函数、列表的swap和 insert 函数、随机列表生成函数和列表shuffle函数。详见头文件 **funcs.h**中的注释。
    * **headers.h** 导入了一些常用的标准库中的函数。
    * **IG.cpp** & **IG.h** 基于迭代贪婪算法的重调度算法。
    * **mcts_dpw.cpp** & **mcts_dpw.h** Monte Carlo tree search methods with double progressive widening for rescheduling. 其中包括两个版本的 DPW 算法，**UCT1_with_DPW** 和 **UCT1_with_DPW2**，两种算法的区别详见头文件中的注释。论文中所采用的结合 DPW 的 MCTS 方法为 **UCT1_with_DPW2**。
    * **mcts_newbp.cpp** & **mcts_newbp.h**，一种修改了反向传播规则的 MCTS 算法。将加权平均的反向传播方式修改为利用当前重调度节点的子节点中的最优值与当前节点的奖励值的加权平均，加权系数由 alpha 控制。效果不佳。
    * **mcts.cpp** & **mcts.h** 基于 MCTS 的局部重调度算法。
    * **reschd_operator.h** & **reschd_operator.cpp**，基于指派规则和构造型启发式方法的重调度算子。
    * **SA.h** & **SA.cpp**, 基于固定温度算法的局部重调度算法。
    * **tree.h** & **tree.cpp**, 树的数据结构。
    * **workshop_bindings.cpp**，定义 pybind11 接口函数。
    * **workshop_funcs.h** & **workshop_funcs.cpp**，求解流水车间的启发式算法和模拟退火算法。
    * **workshop.h** & **workshop.cpp**，流水车间类的实现。
* **example1.ipynb**, HFSP 类的应用示例。
* **example2.ipynb**, 动态流水车间重调度算法使用示例。
* **fspbenchmark.py**，读取流水车间算例的函数。
* **prepare.py**，设置动态事件的函数。
* **result_analysis.ipynb**，结果分析并绘图。
* **result_fucs.py**，一些数据分析函数。

# 参考文献
* [1]杜汩鑫, 2024. 基于蒙特卡洛树搜索的动态流水车间问题重调度算法研究[D]. 中国科学院大学.

* [2]段文哲, 2019. 动态调度建模理论与优化方法研究[D]. 中国科学院大学.