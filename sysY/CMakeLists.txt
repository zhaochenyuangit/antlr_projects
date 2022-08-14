cmake_minimum_required(VERSION 3.14)
project(antlr-sysY)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)


set(ANTLR_RUNTIME_SOURCE_DIR /home/zhao/Downloads/antlr4/runtime/Cpp)
set(ANTLR_JAR_LOCATION /usr/local/lib/antlr-4.10.1-complete.jar)
find_program(Java_JAVA_EXECUTABLE "java")
find_program(Java_JAVAC_EXECUTABLE "javac")

set(llvm_build_directory /home/zhao/Disk/build_llvm1405)
set(llvm_source_directory /home/zhao/Disk/llvm1405)

add_subdirectory(llvm-sanity)
add_subdirectory(frontend)


set(antlr4_for_gui
   ${CMAKE_BINARY_DIR}/gui/sysYLexer.java 
   ${CMAKE_BINARY_DIR}/gui/sysYParser.java 
   ${CMAKE_BINARY_DIR}/gui/sysYBaseListener.java 
   ${CMAKE_BINARY_DIR}/gui/sysYListener.java 
 )
add_custom_command(OUTPUT ${antlr4_for_gui}
   COMMAND 
   ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/gui/
   COMMAND
   "${Java_JAVA_EXECUTABLE}" -jar ${ANTLR_JAR_LOCATION} -Werror 
   -o ${CMAKE_BINARY_DIR}/gui 
   ${PROJECT_SOURCE_DIR}/sysY.g4 
   COMMAND
   ${Java_JAVAC_EXECUTABLE} ${antlr4_for_gui} 
   WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
   DEPENDS ${PROJECT_SOURCE_DIR}/sysY.g4
   )

file(GLOB_RECURSE demo_srcs RELATIVE ${PROJECT_SOURCE_DIR} demo_srcs/*.sy )
foreach(src ${demo_srcs})
message(STATUS "FOUND src ${src}")
  add_custom_command(
    OUTPUT
    ${src} 
    COMMAND
    gvim ${PROJECT_SOURCE_DIR}/${src}
    COMMAND
    "${Java_JAVA_EXECUTABLE}" -Xmx500M -cp \"${ANTLR_JAR_LOCATION}:$$CLASSPATH\" org.antlr.v4.gui.TestRig sysY compUnit -gui ${PROJECT_SOURCE_DIR}/${src}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gui
    DEPENDS
    ${PROJECT_SOURCE_DIR}/sysY.g4
    )

  get_filename_component(gui_target ${src} NAME_WE)
  add_custom_target(${gui_target}
  COMMAND
  echo "show graph for ${src}"
  DEPENDS
  ${src}
  ${antlr4_for_gui}
  )
endforeach()

add_compile_options(-g -std=c++17 -O2)

add_custom_target(clean-all
                COMMAND make clean
                COMMAND rm -rf ${CMAKE_BINARY_DIR}/generated
                COMMAND rm -rf ${CMAKE_BINARY_DIR}/gui
                )

