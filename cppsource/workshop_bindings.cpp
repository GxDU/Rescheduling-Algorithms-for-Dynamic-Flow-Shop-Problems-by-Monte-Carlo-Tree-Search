#include "workshop.h"
#include "workshop_funcs.h"
#include "dynamic_workshop.h"
#include "tree.h"
// #include "mcts.h"
// #include "mcts_dpw.h"
#include "experiments.h"

PYBIND11_MODULE(RSCD, m){
    m.doc() = "This module contains the model of permutation flowshop scheduling problem(PFSP), flowshop scheduling problem(FSP) and hybrid flowshop scheduling problems(HFSP), together with some algorithms solving this problem";
    
    m.def("neh", &NEH_func, "A heuristic that solves PFSP");
    m.def("neh1", &NEH1_func);

    m.def("RITM", &Algo_RITM, "Heuristic method RITM that construct a solution for HFSP");
    m.def("hfsp_span_fam", &HFSP_makespan_FAM);
    m.def("decode_fam", &decode_FAM);

    // m.def("exp_fsp_rs", &exp_fsp_rs);
    // m.def("exp_fsp_mcts", &exp_fsp_mcts);
    // m.def("exp_fsp_mcts_time", &exp_fsp_mcts_time);
    // m.def("exp_fsp_spt", &exp_fsp_spt);
    // m.def("exp_fsp_srpt", &exp_fsp_spt);
    // m.def("exp_fsp_lpt", &exp_fsp_spt);
    // m.def("exp_fsp_ect", &exp_fsp_ect);
    // m.def("exp_fsp_est", &exp_fsp_est);
    // m.def("exp_fsp_random", &exp_fsp_random);
    // m.def("exp_fsp_neh", &exp_fsp_neh);
    // m.def("exp_fsp_ritm", &exp_fsp_ritm);
    // m.def("test_rollout", &test_rollout);
    // m.def("exp_fsp_rs_with_seed", &exp_fsp_rs_with_seed);
    // m.def("exp_fsp_spt_with_seed", &exp_fsp_spt_with_seed);
    // m.def("exp_fsp_lpt_with_seed", &exp_fsp_lpt_with_seed);
    // m.def("exp_fsp_srpt_with_seed", &exp_fsp_srpt_with_seed);
    // m.def("exp_fsp_ect_with_seed", &exp_fsp_ect_with_seed);
    // m.def("exp_fsp_est_with_seed", &exp_fsp_est_with_seed);
    // m.def("exp_fsp_random_with_seed", &exp_fsp_random_with_seed);
    // m.def("exp_fsp_neh_with_seed", &exp_fsp_neh_with_seed);
    // m.def("exp_fsp_ritm_with_seed", &exp_fsp_ritm_with_seed);
    // m.def("exp_fsp_mcts_with_seed", &exp_fsp_mcts_with_seed);
    // m.def("exp_fsp_mcts_time_with_seed", &exp_fsp_mcts_time_with_seed);
    // m.def("exp_fsp_single_mcts_with_seed", &exp_fsp_single_mcts_with_seed);
    // m.def("exp_fsp_new_single_mcts_with_seed", &exp_fsp_new_single_mcts_with_seed);
    // m.def("exp_fsp_pwmcts_with_seed", &exp_fsp_pwmcts_with_seed);
    // m.def("exp_fsp_dpwmcts_with_seed", &exp_fsp_dpwmcts_with_seed);
    // m.def("exp_fsp_lsmcts_with_seed", &exp_fsp_lsmcts_with_seed);
    // m.def("exp_sa_resched", &exp_sa_resched);
    m.def("exp_fsp1_rs_with_seed", &exp_fsp1_rs_with_seed);
    m.def("exp_fsp1_spt_with_seed", &exp_fsp1_spt_with_seed);
    m.def("exp_fsp1_lpt_with_seed", &exp_fsp1_lpt_with_seed);
    m.def("exp_fsp1_srpt_with_seed", &exp_fsp1_srpt_with_seed);
    m.def("exp_fsp1_ritm_with_seed", &exp_fsp1_ritm_with_seed);
    m.def("exp_fsp1_neh_with_seed", &exp_fsp1_neh_with_seed);
    m.def("exp_fsp1_neh1_with_seed", &exp_fsp1_neh1_with_seed);
    m.def("exp_fsp1_nehnm_with_seed", &exp_fsp1_nehnm_with_seed);
    m.def("exp_fsp1_nehnm1_with_seed", &exp_fsp1_nehnm1_with_seed);
    m.def("exp_fsp1_nehkk_with_seed", &exp_fsp1_nehkk_with_seed);
    m.def("exp_fsp1_random_with_seed", &exp_fsp1_random_with_seed);
    m.def("exp_fsp1_uct_time_with_seed", &exp_fsp1_uct_time_with_seed);
    m.def("exp_fsp1_uct_iter_with_seed", &exp_fsp1_uct_iter_with_seed);
    m.def("exp_fsp1_dpw_time_with_seed", &exp_fsp1_dpw_time_with_seed);
    m.def("exp_fsp1_dpw2_time_with_seed", &exp_fsp1_dpw2_time_with_seed);
    m.def("exp_fsp1_dpw_iter_with_seed", &exp_fsp1_dpw_iter_with_seed);
    m.def("exp_fsp1_dpw2_iter_with_seed", &exp_fsp1_dpw2_iter_with_seed);
    m.def("exp_fsp1_uctnewbp_iter_with_seed", &exp_fsp1_uctnewbp_iter_with_seed);
    m.def("exp_fsp1_fta_time_with_seed", &exp_fsp1_fta_time_with_seed);
    m.def("exp_fsp1_fta1_time_with_seed", &exp_fsp1_fta1_time_with_seed);
    m.def("exp_fsp1_ig_time_with_seed", &exp_fsp1_ig_time_with_seed);
    m.def("exp_fsp1_ig1_time_with_seed", &exp_fsp1_ig1_time_with_seed);

    

    py::class_<PFSP>(m, "PFSP")
        .def(py::init<const int, const int, const vector<vector<int>>>())
        .def("solve_NEH", &PFSP::solve_neh)
        .def("NEH", &PFSP::NEH)
        .def("table_with_span", &PFSP::table_with_makespan)
        .def("set_solution", &PFSP::set_solution)
        // .def("rper", &PFSP::rper)
        .def("get_solution", &PFSP::get_solution)
        .def("get_makespan", &PFSP::get_makespan);
        // .def("get_completion_table", &PFSP::get_completion_table);
        // .def_readwrite("num_of_machine", &PFSP::num_of_machine)
        // .def_readwrite("num_of_job", &PFSP::num_of_job)
        // .def_readwrite("processing_time", &PFSP::process_time)
        // .def_readwrite("solution", &PFSP::solution)
        // .def_readwrite("makespan", &PFSP::makespan)
        // .def_readonly("method_applied", &PFSP::method_applied)
        // .def_readwrite("completion_table", &PFSP::completion_table);

    py::class_<HFSP>(m, "HFSP")
        .def(py::init<const int, const int,const vector<vector<int>>&, const vector<int>&>())
        .def("solve", &HFSP::solve)
        .def("solve_SA", &HFSP::solve_sa)
        .def("NEH", &HFSP::NEH)
        .def("RITM", &HFSP::RITM)
        .def("SA", &HFSP::sa)
        .def("set_solution", &HFSP::set_solution)
        .def("rper", &HFSP::rper)
        .def("get_makespan", &HFSP::get_makespan)
        .def("get_coded_solution", &HFSP::get_coded_solution)
        .def("get_solution", &HFSP::get_solution)
        .def("get_completion_table", &HFSP::get_completion_table)
        .def("get_process_time", &HFSP::get_process_time);
        // .def_readwrite("num_of_stage", &HFSP::num_of_stage)
        // .def_readwrite("machines_per_stage", &HFSP::machines_per_stage)
        // .def_readwrite("num_of_job", &HFSP::num_of_job)
        // .def_readwrite("process_time", &HFSP::process_time)
        // .def_readwrite("solution", &HFSP::solution)
        // .def_readwrite("makespan", &HFSP::makespan)
        // .def_readonly("method_applied", &HFSP::method_applied)
        // .def_readwrite("completion_table", &HFSP::completion_table);
    
    // py::class_<DFSP>(m, "DFSP")
    //     .def(py::init<const HFSP&, const vector<vector<vector<float>>>&, const vector<vector<vector<float>>>&>())
    //     .def(py::init<const HFSP&, const vector<vector<vector<float>>>&, const vector<vector<vector<float>>>&, const vector<vector<int>>&>())
    //     .def("env_run", &DFSP::env_run)
    //     .def("is_terminal", &DFSP::is_terminal)
    //     .def("get_machine_state", &DFSP::get_machine_state)
    //     .def("get_task_remain", &DFSP::get_task_remain)
    //     .def("get_real_dispatch", &DFSP::get_real_dispatch)
    //     .def("get_time", &DFSP::get_time)
    //     .def("get_reschedlist", &DFSP::get_reschedlist)
    //     .def("get_turn", &DFSP::get_turn)
    //     .def("get_completion_table", &DFSP::get_completion_table)
    //     .def("get_real_completion_table", &DFSP::get_real_completion_table)
    //     .def("get_process_time", &DFSP::get_process_time)
    //     .def("get_exp_process_time", &DFSP::get_exp_process_time)
    //     .def("rollout", &DFSP::rollout)
    //     .def("expected_span", &DFSP::expected_span)
    //     .def("expected_static_span", &DFSP::expected_static_span)
    //     .def("multi_simu_makespan", &DFSP::multi_simu_makespan);

    py::class_<DFSP1>(m, "DFSP1")
        .def(py::init<const HFSP&, const vector<vector<vector<float>>>&, const vector<vector<vector<float>>>&>())
        // .def(py::init<const HFSP&, const vector<vector<vector<float>>>&, const vector<vector<vector<float>>>&, const vector<vector<int>>&>())
        .def(py::init<const HFSP&, const vector<vector<vector<float>>>&, const vector<vector<vector<float>>>&, const vector<int>&>())
        .def("env_run", &DFSP1::env_run)
        .def("is_terminal", &DFSP1::is_terminal)
        .def("get_machine_state", &DFSP1::get_machine_state)
        .def("get_task_remain", &DFSP1::get_task_remain)
        .def("get_real_dispatch", &DFSP1::get_real_dispatch)
        .def("get_time", &DFSP1::get_time)
        .def("get_reschedlist", &DFSP1::get_reschedlist)
        .def("get_turn", &DFSP1::get_turn)
        // .def("get_completion_table", &DFSP1::get_completion_table)
        .def("get_real_completion_table", &DFSP1::get_real_completion_table)
        // .def("get_process_time", &DFSP1::get_process_time)
        // .def("get_exp_process_time", &DFSP1::get_exp_process_time)
        .def("rollout", &DFSP1::rollout)
        // .def("expected_span", &DFSP1::expected_span)
        // .def("expected_static_span", &DFSP1::expected_static_span)
        .def("multi_simu_makespan", &DFSP1::multi_simu_makespan);



    // py::class_<NodeFSP>(m, "NodeFSP")
    //     .def(py::init<>())
    //     .def(py::init<const DFSP&>())
    //     .def("add_child", &NodeFSP::add_child)
    //     .def("kidnap_child", &NodeFSP::kidnap_child);
}