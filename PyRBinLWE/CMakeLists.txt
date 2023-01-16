cmake_minimum_required(VERSION 3.16)
project("PyRBinLWE")
set(CMAKE_CXX_STANDARD 17)

set(APP_SOURCE "${PROJECT_SOURCE_DIR}/source/app")
set(MODULE_SOURCE "${PROJECT_SOURCE_DIR}/source/module")
set(EXTERNALS "${PROJECT_SOURCE_DIR}/externals")

# pybind11
add_subdirectory(${EXTERNALS}/pybind11)

pybind11_add_module(
	mpamok
	${MODULE_SOURCE}/module.cpp
	${MODULE_SOURCE}/Encryptor.cpp
	${MODULE_SOURCE}/Polynomial.cpp
	${MODULE_SOURCE}/Encryptor.h
	${MODULE_SOURCE}/Polynomial.h
)

target_include_directories(
	mpamok
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

target_link_libraries(Application PRIVATE pybind11::embed)