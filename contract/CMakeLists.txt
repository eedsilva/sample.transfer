add_contract(sample.transfer sample.transfer ${CMAKE_CURRENT_SOURCE_DIR}/src/sample.transfer.cpp)

target_compile_options(sample.transfer.wasm PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR}/resources)

target_include_directories(sample.transfer.wasm
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(sample.transfer.wasm
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
