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
        "--shell-file" "${CMAKE_CURRENT_SOURCE_DIR}/shell_minimal.html"
    )
    set_property(TARGET oasis_engine APPEND PROPERTY LINK_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/shell_minimal.html")
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
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-telemetryd
        )
        target_link_libraries(col_telemetryd PRIVATE 
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
    endif()

    set(COL_EXECD_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-execd/main.cpp")
    if(EXISTS "${COL_EXECD_SRC}")
        add_executable(col_execd "${COL_EXECD_SRC}")
        target_include_directories(col_execd PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
        )
        target_link_libraries(col_execd PRIVATE 
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
    endif()

    set(COL_COMMONSD_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-commonsd/main.cpp")
    if(EXISTS "${COL_COMMONSD_SRC}")
        add_executable(col_commonsd "${COL_COMMONSD_SRC}")
        target_include_directories(col_commonsd PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
        )
        target_link_libraries(col_commonsd PRIVATE 
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

    # Native Catch2 Boundary Math Verification Tests
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_boundary_math.cpp")
        add_executable(test_boundary_math tests/test_boundary_math.cpp)
        target_include_directories(test_boundary_math PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
        )
        target_link_libraries(test_boundary_math PRIVATE 
            Catch2::Catch2WithMain
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        add_test(NAME BoundaryMathTests COMMAND test_boundary_math)
    endif()

    # Native Catch2 Hardware Airgap Filter Verification Tests
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_airgap_filter.cpp")
        add_executable(test_airgap_filter tests/test_airgap_filter.cpp)
        target_include_directories(test_airgap_filter PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-telemetryd
        )
        target_link_libraries(test_airgap_filter PRIVATE 
            Catch2::Catch2WithMain
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        add_test(NAME AirgapFilterTests COMMAND test_airgap_filter)
    endif()

    # Sovereign Stack Layer 7 Adversarial Proxy Daemon (col-adversaryd)
    set(COL_ADVERSARYD_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-adversaryd/main.cpp")
    if(EXISTS "${COL_ADVERSARYD_SRC}")
        add_executable(col_adversaryd "${COL_ADVERSARYD_SRC}")
        target_include_directories(col_adversaryd PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-adversaryd
        )
        target_link_libraries(col_adversaryd PRIVATE 
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
    endif()

    # Native Catch2 Construction Caloric Burn Tests (Dual Sprint 5)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_construction_caloric_burn.cpp")
        add_executable(test_construction_caloric_burn tests/test_construction_caloric_burn.cpp)
        target_include_directories(test_construction_caloric_burn PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
        )
        target_link_libraries(test_construction_caloric_burn PRIVATE 
            Catch2::Catch2WithMain
            $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
            glm::glm
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        add_test(NAME ConstructionCaloricBurnTests COMMAND test_construction_caloric_burn)
    endif()

    # Native Catch2 Layer 7 DES Adversary Tests (Dual Sprint 5)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_adversary_des.cpp")
        add_executable(test_adversary_des tests/test_adversary_des.cpp)
        target_include_directories(test_adversary_des PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-adversaryd
        )
        target_link_libraries(test_adversary_des PRIVATE 
            Catch2::Catch2WithMain
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        add_test(NAME AdversaryDesTests COMMAND test_adversary_des)
    endif()

    # Native Catch2 Camera & Dynamic Voxel SSBO Tests (Dual Sprint 6)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_camera_ssbo.cpp")
        add_executable(test_camera_ssbo tests/test_camera_ssbo.cpp)
        target_include_directories(test_camera_ssbo PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
        )
        target_link_libraries(test_camera_ssbo PRIVATE 
            Catch2::Catch2WithMain
            glm::glm
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        add_test(NAME CameraSsboTests COMMAND test_camera_ssbo)
    endif()

    # Adversarial Catch2 Camera & SSBO Stress Tests (Dual Sprint 6 Challenger)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_sprint6_adversarial.cpp")
        add_executable(test_sprint6_adversarial tests/test_sprint6_adversarial.cpp)
        target_include_directories(test_sprint6_adversarial PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
        )
        target_link_libraries(test_sprint6_adversarial PRIVATE 
            Catch2::Catch2WithMain
            glm::glm
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        add_test(NAME Sprint6AdversarialTests COMMAND test_sprint6_adversarial)
    endif()

    # Native Catch2 Avatar & Vitals HUD Tests (Dual Sprint 7)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_avatar_vitals.cpp")
        add_executable(test_avatar_vitals tests/test_avatar_vitals.cpp)
        target_include_directories(test_avatar_vitals PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
        )
        target_link_libraries(test_avatar_vitals PRIVATE 
            Catch2::Catch2WithMain
            $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
            glm::glm
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        add_test(NAME AvatarVitalsTests COMMAND test_avatar_vitals)
    endif()

    # Native Catch2 Homeostasis Loop Tests (Dual Sprint 8)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_homeostasis_loop.cpp")
        add_executable(test_homeostasis_loop tests/test_homeostasis_loop.cpp)
        target_include_directories(test_homeostasis_loop PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
        )
        target_link_libraries(test_homeostasis_loop PRIVATE 
            Catch2::Catch2WithMain
            $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
            glm::glm
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        add_test(NAME HomeostasisLoopTests COMMAND test_homeostasis_loop)
    endif()
endif()



