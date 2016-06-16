cmake_minimum_required(VERSION 3.1.3)

project("UIERandomForest")

cmake_policy(SET CMP0004 OLD)

# Add some cmake macros
set(CMAKE_MODULE_PATH ${UIERandomForest_SOURCE_DIR}/cmake)

# Add default location of TCLAP and CImg headers
set(TCLAP_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/contrib/tclap-1.2.1/include" CACHE PATH "TCLAP include path")
set(CIMG_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/contrib/CImg-1.6.9" CACHE PATH "CImg include path")
set(CEREAL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/contrib/cereal-1.1.2/include" CACHE PATH "Cereal include path")

# Set default option for threading
if(NOT DEFINED WITH_MULTITHREADING)
    set(WITH_MULTITHREADING TRUE CACHE BOOL "With multi-threading")
endif()

# Set default option for profiling output
if(NOT DEFINED WITH_PROFILING)
    set(WITH_PROFILING FALSE CACHE BOOL "With profiling output")
endif()

# Put some variables into the cache
set(PNG_SKIP_SETJMP_CHECK FALSE CACHE BOOL "Skip PNG SETJMP check. Required for some platforms.")

# Use libc++ if using Clang
#if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++")
#endif()

# Enable OpenMP if not using Clang
#if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#    find_package(OpenMP)
#	if (OPENMP_FOUND)
#		set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#	endif()
#endif()

# Find required packages (threads, boost, mpi, eigen3, cereal, tclap, cimg)
find_package(Threads REQUIRED)
if(${WITH_MPI})
	find_package(MPI REQUIRED)
	find_package(Boost 1.4.0 COMPONENTS serialization mpi log filesystem system REQUIRED)
else()
	find_package(Boost 1.4.0 COMPONENTS serialization log filesystem system REQUIRED)
endif()
find_package(Eigen3 3.1.2 REQUIRED)
find_package(Cereal REQUIRED)
find_package(TCLAP REQUIRED)
find_package(CImg REQUIRED)

# Enable PNG support in CImg
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS cimg_use_png)
find_package(X11 REQUIRED)
find_package(PNG REQUIRED)
# Workaround for libpng problem on some platforms
if(${PNG_SKIP_SETJMP_CHECK})
	add_definitions(-DPNG_SKIP_SETJMP_CHECK)
endif()

# Add include directories
include_directories(${Boost_INCLUDE_DIR})
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${TCLAP_INCLUDE_DIR})
include_directories(${CEREAL_INCLUDE_DIR})
include_directories(${CIMG_INCLUDE_DIR})
include_directories(${PNG_INCLUDE_DIR})
if(${UNIX})
	include_directories(${X11_INCLUDE_DIR})
endif()

# Specify source files to use
set(headers ait.h image_weak_learner.h training.h weak_learner.h
	histogram_statistics.h forest.h tree.h node.h matlab_file_io.h
	iterator_utils.h mpl_utils.h logger.h csv_utils.h serialization_utils.h
	bagging_wrapper.h evaluation_utils.h)
#file(GLOB headers
#	"*.h"
#)

# Set AIT_DEBUG macro for cpp files if in debug mode
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:AIT_DEBUG=1>)
# Set AIT_TESTING macro for cpp files if in debug mode
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:AIT_TESTING=1>)
# Set AIT_TESTING macro for all build modes
#set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS AIT_TESTING=1)

# Set AIT_MULTI_THREADING macro if multi-threading is enabled
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<BOOL:WITH_MULTITHREADING>:AIT_MULTI_THREADING=1>)

# Executable target: depth_forest_trainer
add_executable(depth_forest_trainer depth_trainer.cpp ${headers} depth_forest_trainer.h)
target_link_libraries(depth_forest_trainer ${PNG_LIBRARIES})
target_link_libraries(depth_forest_trainer ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(depth_forest_trainer ${Boost_LIBRARIES})
target_compile_features(depth_forest_trainer PRIVATE cxx_auto_type cxx_variadic_templates)
if(${UNIX})
	target_link_libraries(depth_forest_trainer ${X11_LIBRARIES})
endif()

# Executable target: forest_predictor
add_executable(forest_predictor forest_predictor.cpp ${headers})
target_link_libraries(forest_predictor ${PNG_LIBRARIES})
target_link_libraries(forest_predictor ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(forest_predictor ${Boost_LIBRARIES})
target_compile_features(forest_predictor PRIVATE cxx_auto_type cxx_variadic_templates)
if(${UNIX})
	target_link_libraries(forest_predictor ${X11_LIBRARIES})
endif()

# SET AIT_PROFILE or AIT_PROFILE_DISTRIBUTED macro for cpp files if profiling output is enabled
target_compile_definitions(depth_forest_trainer PRIVATE $<$<BOOL:WITH_PROFILING>:AIT_PROFILE=1>)
target_compile_definitions(forest_predictor PRIVATE $<$<BOOL:WITH_PROFILING>:AIT_PROFILE=1>)

# C++11 support (should be done through target_compile_features)
#set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
