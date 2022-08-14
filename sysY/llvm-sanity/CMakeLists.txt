cmake_minimum_required(VERSION 3.10)
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  message(STATUS "llvm sanity as a standalone project")
  project(proj DESCRIPTION "manual build of LLVM IR")
  set(CMAKE_BUILD_TYPE Debug)
  set(llvm_build_directory /home/zhao/Disk/build_llvm1405)
  set(llvm_source_directory /home/zhao/Disk/llvm1405)
else()
  message(STATUS "llvm sanity as a sub project")
endif()


add_executable(demo_ir llvm-sanity.cpp)
target_include_directories(demo_ir PRIVATE ${llvm_source_directory}/llvm/include)
target_include_directories(demo_ir PRIVATE ${llvm_build_directory}/include)
target_link_directories(demo_ir PRIVATE "${llvm_build_directory}/lib")

list(APPEND compile_libs LLVMSupport LLVMIRReader LLVMMIRParser LLVMCore)
target_link_libraries(demo_ir PUBLIC ${compile_libs})
