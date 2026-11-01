#pragma once

#include <algorithm>
#include <cstdint>
#include <string>
#include <string_view>

#include "../clock/simulation_clock.hpp"
#include "../content/content_registry.hpp"
#include "../ecs/affordance_system.hpp"

namespace oasis::engine {

// Calm minimum & maximum dwell durations: 20 to 40 simulated minutes (eliminates frantic per-tick task thrashing).
inline constexpr uint32_t kMinCalmDwellSimMs     = 20u * kSimMsPerMinute; // 1,200,000 sim_ms (20 sim-min)
inline constexpr uint32_t kMaxCalmDwellSimMs     = 40u * kSimMsPerMinute; // 2,400,000 sim_ms (40 sim-min)
inline constexpr uint32_t kChickenMinForageSimMs = 20u * kSimMsPerMinute; // 20 sim-min
inline constexpr uint32_t kChickenMaxForageSimMs = 35u * kSimMsPerMinute; // 35 sim-min
inline constexpr uint32_t kDriveMaxBp            = 10000u;
inline constexpr uint32_t kEmergencyInterruptBp  = 1400u;

enum class AgentSpecies : uint8_t {
    HUMAN   = 0,
    CHICKEN = 1
};

enum class AutonomousActivity : uint8_t {
    IDLE                     = 0,
    HUMAN_READING            = 1,
    HUMAN_GAMING_MEDIA       = 2,
    HUMAN_SOCIAL_CHAT        = 3,
    HUMAN_INTROSPECTION      = 4,
    HUMAN_MEAL               = 5,
    HUMAN_HYDRATE            = 6,
    HUMAN_SLEEP_REST         = 7,
    CHICKEN_FORAGING         = 8,
    CHICKEN_WATERING         = 9,
    CHICKEN_ROOSTING         = 10,
    CHICKEN_LAYING_EGG       = 11
};

struct alignas(8) AgentDrivesComponent {
    uint32_t     hunger_bp{8200u};
    uint32_t     hydration_bp{8500u};
    uint32_t     energy_bp{8400u};
    uint32_t     leisure_bp{7500u};
    uint32_t     social_bp{7600u};
    uint32_t     introspection_bp{7400u};
    uint32_t     egg_cycle_bp{0u};
    uint16_t     sub_sec_ms_accum{0u};
    AgentSpecies species{AgentSpecies::HUMAN};
    uint8_t      pad_[1]{0u};
};
static_assert(sizeof(AgentDrivesComponent) == 32, "AgentDrivesComponent must be 32 bytes with zero implicit padding");

struct alignas(8) AutonomyStateComponent {
    uint64_t           rng_state{0x9E3779B97F4A7C15ULL};
    uint64_t           active_station_uid{0ULL};
    ActivityTimer      dwell_timer{};
    uint32_t           transition_count{0u};
    uint32_t           eggs_laid_count{0u};
    AutonomousActivity current_activity{AutonomousActivity::IDLE};
    bool               player_task_locked{false};
    uint8_t            pad_[6]{0u, 0u, 0u, 0u, 0u, 0u};
};
static_assert(sizeof(AutonomyStateComponent) == 48, "AutonomyStateComponent must be 48 bytes with zero implicit padding");

class UtilityAutonomySystem {
public:
    // Deterministic SplitMix64 PRNG for reproducible dwell window variation in [min_ms .. max_ms].
    [[nodiscard]] static constexpr uint64_t NextDeterministicU64(uint64_t& state) noexcept {
        state += 0x9E3779B97F4A7C15ULL;
        uint64_t z = state;
        z = (z ^ (z >> 30u)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27u)) * 0x94D049BB133111EBULL;
        return z ^ (z >> 31u);
    }

    [[nodiscard]] static constexpr uint32_t SampleDwellSimMs(
        uint64_t& rng_state,
        uint32_t min_ms = kMinCalmDwellSimMs,
        uint32_t max_ms = kMaxCalmDwellSimMs
    ) noexcept {
        if (max_ms <= min_ms) {
            return min_ms;
        }
        const uint64_t span = static_cast<uint64_t>(max_ms - min_ms) + 1ULL;
        return min_ms + static_cast<uint32_t>(NextDeterministicU64(rng_state) % span);
    }

    // Unified circadian drive decay (operates cleanly across arbitrary real/sim sub-steps).
    static constexpr void StepDriveDecay(AgentDrivesComponent& drives, uint32_t sim_dt_ms) noexcept {
        const uint64_t total_ms = static_cast<uint64_t>(drives.sub_sec_ms_accum) + static_cast<uint64_t>(sim_dt_ms);
        const uint32_t whole_s  = static_cast<uint32_t>(total_ms / 1000ULL);
        drives.sub_sec_ms_accum = static_cast<uint16_t>(total_ms % 1000ULL);
        if (whole_s == 0u) {
            return;
        }

        // Decay rates in 1/100 bp per sim-second (~28-55% drop over 8 sim-hours)
        const uint32_t hunger_drop        = (whole_s * 16u) / 100u;
        const uint32_t hydration_drop     = (whole_s * 22u) / 100u;
        const uint32_t energy_drop        = (whole_s * 14u) / 100u;
        const uint32_t leisure_drop       = (whole_s * 18u) / 100u;
        const uint32_t social_drop        = (whole_s * 13u) / 100u;
        const uint32_t introspection_drop = (whole_s * 12u) / 100u;

        drives.hunger_bp        = SubSatBp(drives.hunger_bp, hunger_drop);
        drives.hydration_bp     = SubSatBp(drives.hydration_bp, hydration_drop);
        drives.energy_bp        = SubSatBp(drives.energy_bp, energy_drop);

        if (drives.species == AgentSpecies::HUMAN) {
            drives.leisure_bp       = SubSatBp(drives.leisure_bp, leisure_drop);
            drives.social_bp        = SubSatBp(drives.social_bp, social_drop);
            drives.introspection_bp = SubSatBp(drives.introspection_bp, introspection_drop);
        } else {
            // Well-fed & hydrated hens mature ~1 egg every ~18,000 sim-seconds (5 sim-hours of daylight laying cycle)
            if (drives.hunger_bp >= 3500u && drives.hydration_bp >= 3500u) {
                const uint32_t egg_gain = std::max(1u, (whole_s * 10000u) / 18000u);
                drives.egg_cycle_bp = std::min(20000u, drives.egg_cycle_bp + egg_gain);
            }
        }
    }

    // Autonomous human utility selection with strict 20-40 sim-minute calm dwell enforcement.
    static inline AutonomousActivity StepHumanAutonomy(
        AgentDrivesComponent& drives,
        AutonomyStateComponent& state,
        uint32_t sim_dt_ms,
        const ContentRegistry* registry = nullptr,
        std::string* out_thought_text = nullptr
    ) {
        drives.species = AgentSpecies::HUMAN;
        StepDriveDecay(drives, sim_dt_ms);

        if (state.player_task_locked) {
            return state.current_activity;
        }

        ApplyActiveHumanRecovery(drives, state.current_activity, sim_dt_ms);
        const bool timer_done = state.dwell_timer.Advance(sim_dt_ms);

        const bool physiological_emergency =
            (drives.hydration_bp < kEmergencyInterruptBp && state.current_activity != AutonomousActivity::HUMAN_HYDRATE) ||
            (drives.hunger_bp < kEmergencyInterruptBp && state.current_activity != AutonomousActivity::HUMAN_MEAL) ||
            (drives.energy_bp < 1000u && state.current_activity != AutonomousActivity::HUMAN_SLEEP_REST);

        if (state.current_activity != AutonomousActivity::IDLE &&
            !timer_done &&
            !state.dwell_timer.CanInterrupt() &&
            !physiological_emergency) {
            return state.current_activity;
        }

        if (state.current_activity == AutonomousActivity::IDLE || timer_done || physiological_emergency) {
            const AutonomousActivity next = SelectBestHumanActivity(drives, state.rng_state);
            const uint32_t dwell_ms = ComputeHumanActivityDwellMs(next, state.rng_state);
            state.current_activity = next;
            state.dwell_timer.Start(dwell_ms, dwell_ms);
            ++state.transition_count;

            if (out_thought_text != nullptr) {
                *out_thought_text = ResolveHumanThought(registry, next, drives);
            }
        }

        return state.current_activity;
    }

    // Autonomous chicken behavior: calm 20-35 sim-minute foraging, watering, roosting, and egg-laying into coop inventory.
    static inline AutonomousActivity StepChickenAutonomy(
        AgentDrivesComponent& drives,
        AutonomyStateComponent& state,
        InventoryComponent& coop_inventory,
        uint32_t sim_dt_ms,
        std::string_view egg_item_id = "fresh_egg"
    ) {
        drives.species = AgentSpecies::CHICKEN;
        StepDriveDecay(drives, sim_dt_ms);

        // Active recovery while foraging / drinking / roosting
        const uint32_t sim_dt_s = sim_dt_ms / 1000u;
        if (sim_dt_s > 0u) {
            if (state.current_activity == AutonomousActivity::CHICKEN_FORAGING) {
                drives.hunger_bp = AddSatBp(drives.hunger_bp, sim_dt_s * 3u);
            } else if (state.current_activity == AutonomousActivity::CHICKEN_WATERING) {
                drives.hydration_bp = AddSatBp(drives.hydration_bp, sim_dt_s * 18u);
            } else if (state.current_activity == AutonomousActivity::CHICKEN_ROOSTING) {
                drives.energy_bp = AddSatBp(drives.energy_bp, sim_dt_s * 6u);
            }
        }

        const bool timer_done = state.dwell_timer.Advance(sim_dt_ms);

        // Ready to lay an egg: nest in coop and deposit egg into coop InventoryComponent upon completion (or entry)
        if (drives.egg_cycle_bp >= kDriveMaxBp &&
            state.current_activity != AutonomousActivity::CHICKEN_LAYING_EGG &&
            (state.dwell_timer.CanInterrupt() || timer_done || state.current_activity == AutonomousActivity::IDLE)) {
            drives.egg_cycle_bp -= kDriveMaxBp;
            coop_inventory.AddItem(egg_item_id, 1u);
            ++state.eggs_laid_count;
            state.current_activity = AutonomousActivity::CHICKEN_LAYING_EGG;
            const uint32_t nest_dwell_ms = 12u * kSimMsPerMinute;
            state.dwell_timer.Start(nest_dwell_ms, nest_dwell_ms);
            ++state.transition_count;
            return state.current_activity;
        }

        const bool urgent_thirst = (drives.hydration_bp < 2200u) &&
                                   (state.current_activity != AutonomousActivity::CHICKEN_WATERING);

        if (state.current_activity != AutonomousActivity::IDLE &&
            !timer_done &&
            !state.dwell_timer.CanInterrupt() &&
            !urgent_thirst) {
            return state.current_activity;
        }

        if (state.current_activity == AutonomousActivity::IDLE || timer_done || urgent_thirst) {
            if (drives.hydration_bp < 4500u) {
                state.current_activity = AutonomousActivity::CHICKEN_WATERING;
                const uint32_t drink_ms = 4u * kSimMsPerMinute;
                state.dwell_timer.Start(drink_ms, drink_ms);
            } else if (drives.energy_bp < 2800u) {
                state.current_activity = AutonomousActivity::CHICKEN_ROOSTING;
                const uint32_t roost_ms = 30u * kSimMsPerMinute;
                state.dwell_timer.Start(roost_ms, roost_ms);
            } else {
                state.current_activity = AutonomousActivity::CHICKEN_FORAGING;
                const uint32_t forage_ms = SampleDwellSimMs(
                    state.rng_state, kChickenMinForageSimMs, kChickenMaxForageSimMs
                );
                state.dwell_timer.Start(forage_ms, forage_ms);
            }
            ++state.transition_count;
        }

        return state.current_activity;
    }

private:
    [[nodiscard]] static constexpr uint32_t SubSatBp(uint32_t cur, uint32_t drop) noexcept {
        return (drop >= cur) ? 0u : (cur - drop);
    }

    [[nodiscard]] static constexpr uint32_t AddSatBp(uint32_t cur, uint32_t gain) noexcept {
        return std::min(kDriveMaxBp, cur + gain);
    }

    static constexpr void ApplyActiveHumanRecovery(
        AgentDrivesComponent& drives,
        AutonomousActivity act,
        uint32_t sim_dt_ms
    ) noexcept {
        const uint32_t dt_s = sim_dt_ms / 1000u;
        if (dt_s == 0u) return;
        switch (act) {
            case AutonomousActivity::HUMAN_READING:
                drives.leisure_bp = AddSatBp(drives.leisure_bp, dt_s * 2u);
                drives.introspection_bp = AddSatBp(drives.introspection_bp, dt_s);
                break;
            case AutonomousActivity::HUMAN_GAMING_MEDIA:
                drives.leisure_bp = AddSatBp(drives.leisure_bp, dt_s * 3u);
                break;
            case AutonomousActivity::HUMAN_SOCIAL_CHAT:
                drives.social_bp = AddSatBp(drives.social_bp, dt_s * 3u);
                drives.leisure_bp = AddSatBp(drives.leisure_bp, dt_s);
                break;
            case AutonomousActivity::HUMAN_INTROSPECTION:
                drives.introspection_bp = AddSatBp(drives.introspection_bp, dt_s * 3u);
                break;
            case AutonomousActivity::HUMAN_MEAL:
                drives.hunger_bp = AddSatBp(drives.hunger_bp, dt_s * 6u);
                break;
            case AutonomousActivity::HUMAN_HYDRATE:
                drives.hydration_bp = AddSatBp(drives.hydration_bp, dt_s * 25u);
                break;
            case AutonomousActivity::HUMAN_SLEEP_REST:
                drives.energy_bp = AddSatBp(drives.energy_bp, dt_s * 2u);
                break;
            default:
                break;
        }
    }

    [[nodiscard]] static constexpr AutonomousActivity SelectBestHumanActivity(
        const AgentDrivesComponent& drives,
        uint64_t& rng_state
    ) noexcept {
        if (drives.hydration_bp < 3500u) return AutonomousActivity::HUMAN_HYDRATE;
        if (drives.hunger_bp < 3800u)    return AutonomousActivity::HUMAN_MEAL;
        if (drives.energy_bp < 2500u)    return AutonomousActivity::HUMAN_SLEEP_REST;

        // Score leisure, social, and introspection deficits (higher deficit = higher utility)
        const uint32_t leisure_need       = kDriveMaxBp - std::min(kDriveMaxBp, drives.leisure_bp);
        const uint32_t social_need        = kDriveMaxBp - std::min(kDriveMaxBp, drives.social_bp);
        const uint32_t introspection_need = kDriveMaxBp - std::min(kDriveMaxBp, drives.introspection_bp);

        if (social_need > leisure_need && social_need >= introspection_need) {
            return AutonomousActivity::HUMAN_SOCIAL_CHAT;
        }
        if (introspection_need > leisure_need) {
            return AutonomousActivity::HUMAN_INTROSPECTION;
        }
        return ((NextDeterministicU64(rng_state) & 1ULL) == 0ULL)
            ? AutonomousActivity::HUMAN_READING
            : AutonomousActivity::HUMAN_GAMING_MEDIA;
    }

    [[nodiscard]] static constexpr uint32_t ComputeHumanActivityDwellMs(
        AutonomousActivity act,
        uint64_t& rng_state
    ) noexcept {
        if (act == AutonomousActivity::HUMAN_HYDRATE) {
            return 3u * kSimMsPerMinute;
        }
        if (act == AutonomousActivity::HUMAN_MEAL) {
            return 20u * kSimMsPerMinute;
        }
        if (act == AutonomousActivity::HUMAN_SLEEP_REST) {
            return 6u * kSimMsPerHour;
        }
        // Leisure, reading, gaming, social chat, and introspection always dwell calmly for 20-40 sim-minutes!
        return SampleDwellSimMs(rng_state, kMinCalmDwellSimMs, kMaxCalmDwellSimMs);
    }

    [[nodiscard]] static inline std::string ResolveHumanThought(
        const ContentRegistry* registry,
        AutonomousActivity act,
        const AgentDrivesComponent& drives
    ) {
        if (registry != nullptr) {
            const std::string_view key = (act == AutonomousActivity::HUMAN_INTROSPECTION)
                ? std::string_view("introspection")
                : std::string_view("leisure");
            const int32_t val = (act == AutonomousActivity::HUMAN_INTROSPECTION)
                ? static_cast<int32_t>(drives.introspection_bp)
                : static_cast<int32_t>(drives.leisure_bp);
            for (const auto& rule : registry->ThoughtRules()) {
                if (rule.drive_key == key && val >= rule.min_drive_bp && val <= rule.max_drive_bp) {
                    return rule.thought_text;
                }
            }
        }
        if (act == AutonomousActivity::HUMAN_READING) return "Reading quietly for the next half hour.";
        if (act == AutonomousActivity::HUMAN_GAMING_MEDIA) return "Unwinding with a calm strategy session.";
        if (act == AutonomousActivity::HUMAN_SOCIAL_CHAT) return "Catching up over conversation.";
        if (act == AutonomousActivity::HUMAN_INTROSPECTION) return "Reflecting quietly on plans for the homestead.";
        return "";
    }
};

} // namespace oasis::engine
