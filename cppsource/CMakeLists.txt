cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# set(PYTHON_EXECUTABLE ~/anaconda/evns/py311)
set(PYBIND11_PYTHON_VERSION 3.11)

project(DWSP)

# find_package(PythonInterp REQUIRED)

add_subdirectory(extern/pybind11)
# pybind11_add_module(myAdd tryPybind.cpp)

# pybind11_add_module(RSCD experiments.cpp experiments.h workshop_bindings.cpp mcts.cpp mcts.h tree.cpp tree.h dynamic_workshop.cpp dynamic_workshop.h workshop.cpp workshop.h workshop_funcs.cpp workshop_funcs.h reschd_operator.cpp reschd_operator.h funcs.cpp funcs.h headers.h)
pybind11_add_module(RSCD workshop_bindings.cpp experiments.cpp experiments.h IG.cpp IG.h SA.cpp SA.h mcts_newbp.cpp mcts_newbp.h mcts_dpw.cpp mcts_dpw.h mcts.cpp mcts.h tree.cpp tree.h dynamic_workshop.cpp dynamic_workshop.h workshop.cpp workshop.h workshop_funcs.cpp workshop_funcs.h reschd_operator.cpp reschd_operator.h funcs.cpp funcs.h headers.h)




# add_custom_command(
#         TARGET ${PROJECT_NAME}  POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_directory
#         ${CMAKE_BINARY_DIR}/$<CONFIGURATION> ${CMAKE_CURRENT_LIST_DIR}
#         )