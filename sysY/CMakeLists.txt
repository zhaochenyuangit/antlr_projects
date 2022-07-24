cmake_minimum_required(VERSION 3.14)
project(antlr-sysY)
set(CMAKE_CXX_STANDARD 17)

add_executable(sysY main.cpp)

set(ANTLR_RUNTIME_SOURCE_DIR /home/zhao/Downloads/antlr4/runtime/Cpp)

target_include_directories(sysY PRIVATE 
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/misc
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/atn
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/dfa
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/tree
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/support
  )

target_include_directories(sysY PRIVATE ${CMAKE_SOURCE_DIR}/generated)

target_link_libraries(sysY antlr4-runtime)

set(antlr4_generated_src
   ${PROJECT_SOURCE_DIR}/generated/sysYLexer.cpp 
   ${PROJECT_SOURCE_DIR}/generated/sysYParser.cpp
   ${PROJECT_SOURCE_DIR}/generated/sysYBaseListener.cpp
   ${PROJECT_SOURCE_DIR}/generated/sysYBaseVisitor.cpp
   ${PROJECT_SOURCE_DIR}/generated/sysYBaseListener.cpp
   ${PROJECT_SOURCE_DIR}/generated/sysYBaseVisitor.cpp
 )
target_sources(sysY PRIVATE ${antlr4_generated_src})

set(ANTLR_JAR_LOCATION /usr/local/lib/antlr-4.10.1-complete.jar)
find_program(Java_JAVA_EXECUTABLE "java")
add_custom_target(GenerateParser DEPENDS ${antlr4_generated_src})

add_custom_command(OUTPUT ${antlr4_generated_src}
   COMMAND 
   ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/generated/
   COMMAND
   "${Java_JAVA_EXECUTABLE}" -jar ${ANTLR_JAR_LOCATION} -Werror 
   -Dlanguage=Cpp -listener -visitor 
   -o ${PROJECT_SOURCE_DIR}/generated 
   #-package antlrcpptest 
   ${PROJECT_SOURCE_DIR}/sysY.g4 
   WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
   DEPENDS ${PROJECT_SOURCE_DIR}/sysY.g4   
   )

add_dependencies(sysY GenerateParser)
add_compile_options(-g -std=c++17 -O2)
