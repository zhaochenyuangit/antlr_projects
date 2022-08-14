set(antlr4_generated_src
   ${CMAKE_BINARY_DIR}/generated/sysYLexer.cpp 
   ${CMAKE_BINARY_DIR}/generated/sysYParser.cpp
  # ${CMAKE_BINARY_DIR}/generated/sysYBaseListener.cpp
   ${CMAKE_BINARY_DIR}/generated/sysYBaseVisitor.cpp
 )

add_custom_target(GenerateParser
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
  BYPRODUCTS ${antlr4_generated_src}
)


link_directories(${llvm_build_directory}/lib)
include_directories(${llvm_build_directory}/include)

include_directories(${llvm_source_directory}/llvm/include)

set(BIN_NAME sysY-compiler)
add_executable(${BIN_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
add_dependencies(${BIN_NAME} GenerateParser)
target_sources(${BIN_NAME} PRIVATE ${antlr4_generated_src})

target_include_directories(${BIN_NAME} PRIVATE ${CMAKE_BINARY_DIR}/generated)


target_include_directories(${BIN_NAME} PRIVATE 
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/misc
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/atn
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/dfa
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/tree
  ${ANTLR_RUNTIME_SOURCE_DIR}/runtime/src/support
  )

target_sources(${BIN_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/AstVisitor.cpp)

target_link_libraries(${BIN_NAME} 
  PUBLIC
  antlr4-runtime
  LLVMSupport
  LLVMCore
  LLVMIRReader
  LLVMMIRParser
  pthread
)
