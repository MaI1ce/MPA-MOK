cmake_minimum_required(VERSION 3.16)
project("PyRBinLWE")
set(CMAKE_CXX_STANDARD 17)

set(APP_SOURCE "${PROJECT_SOURCE_DIR}/source/app")
set(MODULE_SOURCE "${PROJECT_SOURCE_DIR}/source/module")
set(EXTERNALS "${PROJECT_SOURCE_DIR}/externals")

# pybind11
add_subdirectory(${EXTERNALS}/pybind11)

pybind11_add_module(
	pybind11module
	${MODULE_SOURCE}/module.cpp
)

target_include_directories(
	pybind11module
	PRIVATE ${MODULE_SOURCE}
)

add_executable(
	Application
	${APP_SOURCE}/main.cpp
)

target_include_directories(
	Application
	PRIVATE ${APP_SOURCE}
	PRIVATE ${MODULE_SOURCE}
)