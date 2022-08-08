cmake_minimum_required(VERSION 3.14)
project(antlr-expr)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

add_executable(expr main.cpp)

set(ANTLR_RUNTIME_SOURCE_DIR /home/zhao/Downloads/antlr4/runtime/Cpp)

target_include_directories(expr PRIVATE 
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/misc
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/atn
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/dfa
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/tree
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/support
  )

target_include_directories(expr PRIVATE ${CMAKE_BINARY_DIR}/generated)

target_link_libraries(expr antlr4-runtime)

set(antlr4_generated_src
   ${CMAKE_BINARY_DIR}/generated/ExprLexer.cpp 
   ${CMAKE_BINARY_DIR}/generated/ExprParser.cpp
   ${CMAKE_BINARY_DIR}/generated/ExprBaseListener.cpp
   ${CMAKE_BINARY_DIR}/generated/ExprBaseVisitor.cpp
 )
target_sources(expr PRIVATE ${antlr4_generated_src})

set(ANTLR_JAR_LOCATION /usr/local/lib/antlr-4.10.1-complete.jar)
find_program(Java_JAVA_EXECUTABLE "java")
add_custom_target(GenerateParser DEPENDS ${antlr4_generated_src})

add_custom_command(OUTPUT ${antlr4_generated_src}
   COMMAND 
   ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/generated/
   COMMAND
   "${Java_JAVA_EXECUTABLE}" -jar ${ANTLR_JAR_LOCATION} -Werror 
   -Dlanguage=Cpp 
   #-no-listener 
   -listener
   -visitor 
   -o ${CMAKE_BINARY_DIR}/generated 
   #-package antlrcpp 
   ${PROJECT_SOURCE_DIR}/Expr.g4 
   WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
   DEPENDS ${PROJECT_SOURCE_DIR}/Expr.g4   
   )

add_dependencies(expr GenerateParser)

add_custom_target(clean-all
  COMMAND make clean
  COMMAND rm -rf ${CMAKE_BINARY_DIR}/generated
)
