cmake_minimum_required(VERSION 3.14)
project(antlr-sysY)
set(CMAKE_CXX_STANDARD 17)

set(ANTLR_RUNTIME_SOURCE_DIR /home/zhao/Downloads/antlr4/runtime/Cpp)
set(antlr4_generated_src
   ${CMAKE_BINARY_DIR}/generated/sysYLexer.cpp 
   ${CMAKE_BINARY_DIR}/generated/sysYParser.cpp
  # ${CMAKE_BINARY_DIR}/generated/sysYBaseListener.cpp
   ${CMAKE_BINARY_DIR}/generated/sysYBaseVisitor.cpp
 )


set(ANTLR_JAR_LOCATION /usr/local/lib/antlr-4.10.1-complete.jar)
find_program(Java_JAVA_EXECUTABLE "java")
find_program(Java_JAVAC_EXECUTABLE "javac")
add_custom_target(GenerateParser DEPENDS ${antlr4_generated_src})

add_custom_command(OUTPUT ${antlr4_generated_src} ALL
   COMMAND 
   ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/generated/
   COMMAND
   "${Java_JAVA_EXECUTABLE}" -jar ${ANTLR_JAR_LOCATION} -Werror 
   -Dlanguage=Cpp -no-listener -visitor 
   -o ${CMAKE_BINARY_DIR}/generated 
   #-package antlrcpptest 
   ${PROJECT_SOURCE_DIR}/sysY.g4 
   WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
   DEPENDS ${PROJECT_SOURCE_DIR}/sysY.g4   
   )

set(BIN_NAME sysY-compiler)
add_executable(${BIN_NAME} ${CMAKE_SOURCE_DIR}/frontend/main.cpp)

target_include_directories(${BIN_NAME} PRIVATE 
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/misc
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/atn
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/dfa
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/tree
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/support
  )

target_include_directories(${BIN_NAME} PRIVATE ${CMAKE_BINARY_DIR}/generated)

target_link_libraries(${BIN_NAME} antlr4-runtime)
add_dependencies(${BIN_NAME} GenerateParser)
target_sources(${BIN_NAME}  PRIVATE ${antlr4_generated_src})
   

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

