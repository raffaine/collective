cmake_minimum_required(VERSION 3.20)
project(OasisEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(core)

if(NOT EMSCRIPTEN)
    find_package(SDL2 CONFIG REQUIRED)
    find_package(unofficial-sodium CONFIG REQUIRED)
    find_package(Catch2 CONFIG REQUIRED)
endif()

find_package(pugixml CONFIG REQUIRED)

add_library(OasisCore 
    core/chunk_manager.cpp
    core/did_crypto_generator.cpp
    core/thermodynamic_engine.cpp
    core/bpmn_parser.cpp
    core/entity_manager.cpp
    core/sdl2_abstraction.cpp
    core/renderer.cpp
    core/blueprint_manager.cpp
    core/blueprint_exporter.cpp
)

target_link_libraries(OasisCore PUBLIC pugixml)

if(NOT EMSCRIPTEN)
    target_link_libraries(OasisCore PUBLIC unofficial-sodium::sodium $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main> $<TARGET_NAME_IF_EXISTS:SDL2::SDL2-static>)
else()
    target_compile_options(OasisCore PRIVATE "-sUSE_SDL=2" "--use-port=emdawnwebgpu")
endif()

add_executable(oasis_engine main.cpp)
target_link_libraries(oasis_engine PRIVATE OasisCore)

if (EMSCRIPTEN)
    message(STATUS "Configuring for WebAssembly (Emscripten)...")
    target_compile_options(oasis_engine PRIVATE "-sUSE_SDL=2" "--use-port=emdawnwebgpu")
    target_link_options(oasis_engine PRIVATE 
        "-sUSE_SDL=2"
        "--use-port=emdawnwebgpu"
        "-sWASM=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sASYNCIFY"
    )
    set_target_properties(oasis_engine PROPERTIES SUFFIX ".html")
else()
    message(STATUS "Configuring for Native Desktop Build...")
    
    enable_testing()
    
    add_executable(epic1_tests tests/epic1_tests.cpp)
    target_link_libraries(epic1_tests PRIVATE OasisCore Catch2::Catch2WithMain)
    
    add_executable(epic2_tests tests/epic2_tests.cpp)
    target_link_libraries(epic2_tests PRIVATE OasisCore Catch2::Catch2WithMain)
    
    add_executable(epic3_tests tests/epic3_tests.cpp)
    target_link_libraries(epic3_tests PRIVATE OasisCore Catch2::Catch2WithMain)

    add_executable(epic4_tests tests/epic4_tests.cpp)
    target_link_libraries(epic4_tests PRIVATE OasisCore Catch2::Catch2WithMain)

    add_executable(epic6_tests tests/epic6_tests.cpp)
    target_link_libraries(epic6_tests PRIVATE OasisCore Catch2::Catch2WithMain)
    
    add_test(NAME Epic1Tests COMMAND epic1_tests)
    add_test(NAME Epic2Tests COMMAND epic2_tests)
    add_test(NAME Epic3Tests COMMAND epic3_tests)
    add_test(NAME Epic4Tests COMMAND epic4_tests)
    add_test(NAME Epic6Tests COMMAND epic6_tests)
endif()
