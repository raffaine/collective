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

    # Sovereign Stack Layer 2/3 Mesh Daemon (col-meshd)
    set(COL_MESHD_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-meshd/main.cpp")
    if(EXISTS "${COL_MESHD_SRC}")
        add_executable(col_meshd "${COL_MESHD_SRC}")
        target_include_directories(col_meshd PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-meshd
        )
        target_link_libraries(col_meshd PRIVATE 
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        # Create col-meshd symlink so both hyphenated and underscored names work
        add_custom_command(TARGET col_meshd POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink
                $<TARGET_FILE_NAME:col_meshd>
                ${CMAKE_CURRENT_BINARY_DIR}/col-meshd
            COMMENT "Creating col-meshd symlink"
        )
    endif()

    # Sovereign Stack Layer 6 Gateway Daemon (col-gatewayd)
    set(COL_GATEWAYD_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-gatewayd/main.cpp")
    if(NOT EXISTS "${COL_GATEWAYD_SRC}")
        set(COL_GATEWAYD_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/daemons/col-gatewayd/main.cpp")
    endif()
    if(EXISTS "${COL_GATEWAYD_SRC}")
        add_executable(col_gatewayd "${COL_GATEWAYD_SRC}")
        target_include_directories(col_gatewayd PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-gatewayd
        )
        target_link_libraries(col_gatewayd PRIVATE 
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        # Create col-gatewayd symlink so both hyphenated and underscored names work
        add_custom_command(TARGET col_gatewayd POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink
                $<TARGET_FILE_NAME:col_gatewayd>
                ${CMAKE_CURRENT_BINARY_DIR}/col-gatewayd
            COMMENT "Creating col-gatewayd symlink"
        )
    endif()

    # Sovereign Stack Layer 5 KMS & Trust Ring Assimilation Daemon (col-kmsd)
    set(COL_KMSD_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-kmsd/main.cpp")
    if(NOT EXISTS "${COL_KMSD_SRC}")
        set(COL_KMSD_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/daemons/col-kmsd/main.cpp")
    endif()
    if(EXISTS "${COL_KMSD_SRC}")
        add_executable(col_kmsd "${COL_KMSD_SRC}")
        target_include_directories(col_kmsd PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/uhai
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-kmsd
        )
        target_link_libraries(col_kmsd PRIVATE 
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        # Create col-kmsd symlink so both hyphenated and underscored names work
        add_custom_command(TARGET col_kmsd POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink
                $<TARGET_FILE_NAME:col_kmsd>
                ${CMAKE_CURRENT_BINARY_DIR}/col-kmsd
            COMMENT "Creating col-kmsd symlink"
        )
    endif()

    # Sovereign Stack Layer 3 Content-Addressed Merkle DAG & BCC Daemon (col-storaged)
    set(COL_STORAGED_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-storaged/main.cpp")
    if(NOT EXISTS "${COL_STORAGED_SRC}")
        set(COL_STORAGED_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/daemons/col-storaged/main.cpp")
    endif()
    if(EXISTS "${COL_STORAGED_SRC}")
        add_executable(col_storaged "${COL_STORAGED_SRC}")
        target_include_directories(col_storaged PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/uhai
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-storaged
        )
        target_link_libraries(col_storaged PRIVATE 
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        # Create col-storaged symlink so both hyphenated and underscored names work
        add_custom_command(TARGET col_storaged POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink
                $<TARGET_FILE_NAME:col_storaged>
                ${CMAKE_CURRENT_BINARY_DIR}/col-storaged
            COMMENT "Creating col-storaged symlink"
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

    # Native Catch2 Temporal Flow Mesh Tests (Dual Sprint 9)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_temporal_flow_mesh.cpp")
        add_executable(test_temporal_flow_mesh tests/test_temporal_flow_mesh.cpp)
        target_include_directories(test_temporal_flow_mesh PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-meshd
        )
        target_link_libraries(test_temporal_flow_mesh PRIVATE 
            Catch2::Catch2WithMain
            $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
            glm::glm
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        add_test(NAME TemporalFlowMeshTests COMMAND test_temporal_flow_mesh)
    endif()

    # Adversarial Catch2 SPSC Concurrency & Abrupt Termination Stress Tests (Dual Sprint 9 Challenger)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_sprint9_adversarial_concurrency.cpp")
        add_executable(test_sprint9_adversarial_concurrency tests/test_sprint9_adversarial_concurrency.cpp)
        target_include_directories(test_sprint9_adversarial_concurrency PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-meshd
        )
        target_link_libraries(test_sprint9_adversarial_concurrency PRIVATE 
            Catch2::Catch2WithMain
            $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
            glm::glm
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        add_test(NAME Sprint9AdversarialConcurrencyTests COMMAND test_sprint9_adversarial_concurrency)
    endif()

    # Native Catch2 col-gatewayd Layer 6 Gateway Tests (Dual Sprint 10)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_col_gatewayd.cpp")
        add_executable(test_col_gatewayd tests/test_col_gatewayd.cpp)
        target_include_directories(test_col_gatewayd PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-gatewayd
            ${CMAKE_CURRENT_SOURCE_DIR}/src/daemons/col-gatewayd
        )
        target_link_libraries(test_col_gatewayd PRIVATE 
            Catch2::Catch2WithMain
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        add_test(NAME ColGatewaydTests COMMAND test_col_gatewayd)
    endif()

    # Native Catch2 Arcball Camera & Spatial Designator Tests (Dual Sprint 11 - EPIC-8)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_arcball_and_designator.cpp")
        add_executable(test_arcball_and_designator tests/test_arcball_and_designator.cpp)
        target_include_directories(test_arcball_and_designator PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
        )
        target_link_libraries(test_arcball_and_designator PRIVATE 
            Catch2::Catch2WithMain
            $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
            glm::glm
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        add_test(NAME ArcballAndDesignatorTests COMMAND test_arcball_and_designator)
    endif()

    # Native Catch2 Time-Warp Controller & Log Throttling Tests (Dual Sprint 11 - EPIC-10)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_time_warp_and_throttling.cpp")
        add_executable(test_time_warp_and_throttling tests/test_time_warp_and_throttling.cpp)
        target_include_directories(test_time_warp_and_throttling PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
        )
        target_link_libraries(test_time_warp_and_throttling PRIVATE 
            Catch2::Catch2WithMain
            $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
            glm::glm
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        add_test(NAME TimeWarpAndThrottlingTests COMMAND test_time_warp_and_throttling)
    endif()

    # Native Catch2 Chill Loop, Thermal Enclosure & Homeostasis Tests (Dual Sprint 12 - EPIC-9 & STORY-10.3)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_chill_loop_homeostasis.cpp")
        add_executable(test_chill_loop_homeostasis tests/test_chill_loop_homeostasis.cpp)
        target_include_directories(test_chill_loop_homeostasis PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
        )
        target_link_libraries(test_chill_loop_homeostasis PRIVATE 
            Catch2::Catch2WithMain
            $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
            glm::glm
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        add_test(NAME ChillLoopHomeostasisTests COMMAND test_chill_loop_homeostasis)
    endif()

    # Native Catch2 Layer 7 Migration Waves, KMS Trust Ring Assimilation & EPIC-11 Tests (Dual Sprint 12 & 13)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_migration_waves.cpp")
        add_executable(test_migration_waves tests/test_migration_waves.cpp)
        target_include_directories(test_migration_waves PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-adversaryd
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-kmsd
        )
        target_link_libraries(test_migration_waves PRIVATE 
            Catch2::Catch2WithMain
            $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
            glm::glm
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        add_test(NAME MigrationWavesTests COMMAND test_migration_waves)
    endif()

    # Native Catch2 3D Voxel A* Pathfinding, Resource-Gated Hauling & Scenario Gamma Kitchen Tests (Sprint 13.6)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_pathfinding_hauling_and_kitchen.cpp")
        add_executable(test_pathfinding_hauling_and_kitchen tests/test_pathfinding_hauling_and_kitchen.cpp)
        target_include_directories(test_pathfinding_hauling_and_kitchen PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
        )
        target_link_libraries(test_pathfinding_hauling_and_kitchen PRIVATE 
            Catch2::Catch2WithMain
            $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
            glm::glm
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        add_test(NAME PathfindingHaulingAndKitchenTests COMMAND test_pathfinding_hauling_and_kitchen)
    endif()

    # Native Catch2 Scenario Zero, Dual-Fidelity Real Blueprints & V3.1 Architectural Realignment Tests
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_scenario_zero_and_blueprint_realignment.cpp")
        add_executable(test_scenario_zero_and_blueprint_realignment tests/test_scenario_zero_and_blueprint_realignment.cpp)
        target_include_directories(test_scenario_zero_and_blueprint_realignment PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-storaged
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-execd
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/daemons/col-kmsd
        )
        target_link_libraries(test_scenario_zero_and_blueprint_realignment PRIVATE 
            Catch2::Catch2WithMain
            $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
            glm::glm
            spdlog::spdlog
            $<$<PLATFORM_ID:Linux>:rt>
        )
        add_test(NAME ScenarioZeroAndBlueprintRealignmentTests COMMAND test_scenario_zero_and_blueprint_realignment)
    endif()
endif()






