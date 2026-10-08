cmake_minimum_required(VERSION 3.20)
project(OasisEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(flecs CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

if(NOT EMSCRIPTEN)
    find_package(SDL2 CONFIG REQUIRED)
endif()

add_executable(oasis_engine main.cpp)

target_include_directories(oasis_engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(oasis_engine PRIVATE 
    $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
    glm::glm 
    spdlog::spdlog
)

if(EMSCRIPTEN)
    message(STATUS "Configuring for WebAssembly (Emscripten) with WebGPU...")
    target_compile_options(oasis_engine PRIVATE 
        "-sUSE_SDL=2" 
        "--use-port=emdawnwebgpu"
    )
    target_link_options(oasis_engine PRIVATE 
        "-sUSE_SDL=2"
        "--use-port=emdawnwebgpu"
        "-sWASM=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sSTACK_SIZE=2097152"
        "-sEXIT_RUNTIME=0"
        "-sASSERTIONS=1"
    )
    set_target_properties(oasis_engine PROPERTIES SUFFIX ".html")
else()
    target_link_libraries(oasis_engine PRIVATE 
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main> 
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    )

    enable_testing()

    # Sovereign Stack Native Targets
    set(COL_TELEMETRYD_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-telemetryd/main.cpp")
    if(EXISTS "${COL_TELEMETRYD_SRC}")
        add_executable(col_telemetryd "${COL_TELEMETRYD_SRC}")
        target_include_directories(col_telemetryd PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
        )
        target_link_libraries(col_telemetryd PRIVATE 
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
    endif()

    # Native Catch2 UHAI Ring Buffer Tests
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_uhai_ring_buffer.cpp")
        add_executable(test_uhai_ring_buffer tests/test_uhai_ring_buffer.cpp)
        target_include_directories(test_uhai_ring_buffer PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
        )
        target_link_libraries(test_uhai_ring_buffer PRIVATE 
            Catch2::Catch2WithMain
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        add_test(NAME UhaiRingBufferTests COMMAND test_uhai_ring_buffer)
    endif()
endif()

