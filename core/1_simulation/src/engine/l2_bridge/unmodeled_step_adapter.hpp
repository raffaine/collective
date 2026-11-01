#pragma once

#include <algorithm>
#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../clock/simulation_clock.hpp"

namespace oasis::engine {

enum class ResolutionMode : uint8_t {
    MODELED_PHYSICAL_DOMAIN              = 0,
    DEFAULT_VALUES_AND_TIMED_STATION_WORK = 1,
    PLAYER_OBSERVATION_PROMPT            = 2
};

struct alignas(8) DefaultRegisterReading {
    int64_t     value{0};
    std::string register_key{};
    std::string unit_label{};
};

struct ModeledDomainBinding {
    std::string action_uri_prefix{};
    std::string domain_id{};
    std::string default_station_archetype{};
};

struct ObservationPromptRule {
    std::string uri_or_register_token{};
    std::string diegetic_question_template{};
};

struct PlayerObservationPrompt {
    std::string              step_action_uri{};
    std::string              diegetic_question{};
    std::vector<std::string> requested_registers{};
};

struct L4WorkflowStepSpec {
    std::string              step_action_uri{};
    std::string              preferred_station_archetype{};
    std::vector<std::string> required_registers{};
    uint32_t                 nominal_work_duration_sim_s{240u};
    bool                     request_player_observation{false};
    uint8_t                  pad_[3]{0u, 0u, 0u};
};

struct alignas(8) StepAdapterResolution {
    ActivityTimer                       station_timer{};
    std::string                         step_action_uri{};
    std::string                         resolved_domain_id{};
    std::string                         station_archetype_id{};
    std::string                         player_prompt_text{};
    std::vector<DefaultRegisterReading> default_registers{};
    ResolutionMode                      mode{ResolutionMode::DEFAULT_VALUES_AND_TIMED_STATION_WORK};
    uint8_t                             pad_[7]{0u, 0u, 0u, 0u, 0u, 0u, 0u};
};

// Bridges arbitrarily detailed Layer 4 BPMN/real-life workflow steps onto Oasis Layer 1b:
// 1. MODELED_PHYSICAL_DOMAIN when the action URI & registers map to a live simulated Oasis physical domain,
// 2. PLAYER_OBSERVATION_PROMPT when qualitative human sensory judgment is requested, or
// 3. DEFAULT_VALUES_AND_TIMED_STATION_WORK with realistic default registers + calm paced station timer otherwise.
class UnmodeledStepAdapter {
public:
    UnmodeledStepAdapter() {
        BootstrapCoreDomainBindings();
    }

    void RegisterModeledDomainPrefix(
        std::string action_uri_prefix,
        std::string domain_id,
        std::string station_archetype = ""
    ) {
        if (action_uri_prefix.empty()) return;
        modeled_domains_.push_back(ModeledDomainBinding{
            std::move(action_uri_prefix),
            std::move(domain_id),
            std::move(station_archetype)
        });
    }

    void RegisterModeledPhysicalRegister(std::string register_key) {
        if (!register_key.empty()) {
            modeled_registers_.insert(std::move(register_key));
        }
    }

    void RegisterDefaultRegisterFallback(std::string register_key, int64_t default_val, std::string unit_label = "") {
        if (register_key.empty()) return;
        default_register_table_[register_key] = DefaultRegisterReading{
            default_val, register_key, std::move(unit_label)
        };
    }

    void RegisterObservationPromptPolicy(std::string token, std::string diegetic_question) {
        if (token.empty()) return;
        prompt_rules_.push_back(ObservationPromptRule{std::move(token), std::move(diegetic_question)});
    }

    void BootstrapCoreDomainBindings() {
        modeled_domains_.clear();
        modeled_registers_.clear();
        default_register_table_.clear();
        prompt_rules_.clear();

        // Physical domains natively simulated in Oasis Layer 1b
        RegisterModeledDomainPrefix("col://domain/soil/", "soil_physics", "archetype.soil_garden_bed");
        RegisterModeledDomainPrefix("col://workflow/soil/", "soil_physics", "archetype.soil_garden_bed");
        RegisterModeledDomainPrefix("col://domain/thermal/", "thermal_vessel", "archetype.pasteurization_kettle");
        RegisterModeledDomainPrefix("col://workflow/thermal/", "thermal_vessel", "archetype.pasteurization_kettle");
        RegisterModeledDomainPrefix("col://domain/logistics/", "hauling_physics", "archetype.garage_tool_rack");
        RegisterModeledDomainPrefix("col://workflow/logistics/", "hauling_physics", "archetype.garage_tool_rack");
        RegisterModeledDomainPrefix("col://domain/earthworks/", "world_strata", "archetype.garage_tool_rack");

        // Physical L2 registers backed by SoilCellRegisters & ThermalVesselRegisters
        for (std::string_view reg : {
                 "moisture_vwc_bp", "soil_temp_mk", "nitrogen_ppm", "phosphorus_ppm",
                 "potassium_ppm", "organic_carbon_bp", "ph_centi", "crop_growth_bp",
                 "is_legume_fixing_n", "irrigation_valve_open",
                 "core_temp_mk", "heater_power_mw", "liquid_volume_ml",
                 "pasteurization_hold_s", "target_cutoff_temp_mk", "lid_closed"}) {
            RegisterModeledPhysicalRegister(std::string(reg));
        }

        // General unit-grounded fallback defaults for unmodeled real-world micro- measurements
        RegisterDefaultRegisterFallback("inoculant_CFU_per_ml", 100000000LL, "CFU/ml");
        RegisterDefaultRegisterFallback("slurry_adhesion_bp", 9400LL, "bp");
        RegisterDefaultRegisterFallback("pod_moisture_bp", 1380LL, "bp");        // 13.80% dry-down target (< 14%)
        RegisterDefaultRegisterFallback("grain_moisture_bp", 1350LL, "bp");      // 13.50% safe storage moisture
        RegisterDefaultRegisterFallback("sieve_retention_bp", 9600LL, "bp");
        RegisterDefaultRegisterFallback("jar_vacuum_seal_kpa", 85LL, "kPa");

        // Qualitative human sensory / visual judgment policies that trigger diegetic player prompts
        RegisterObservationPromptPolicy(
            "player_observe",
            "Take a closer look at the station — does the batch look ready to advance to the next step?"
        );
        RegisterObservationPromptPolicy(
            "sensory_check",
            "Check aroma, color, and texture by hand — confirm whether quality meets your standard."
        );
        RegisterObservationPromptPolicy(
            "manual_confirm",
            "Verify physical setup on-site before proceeding with the workflow step."
        );
    }

    [[nodiscard]] StepAdapterResolution ResolveWorkflowStep(const L4WorkflowStepSpec& spec) {
        StepAdapterResolution res{};
        res.step_action_uri = spec.step_action_uri;
        res.station_archetype_id = spec.preferred_station_archetype.empty()
            ? std::string("archetype.garage_tool_rack")
            : spec.preferred_station_archetype;

        // 1. Check whether this step requires a diegetic player observation prompt
        std::string matched_question;
        if (spec.request_player_observation || MatchesObservationPromptPolicy(spec, matched_question)) {
            res.mode = ResolutionMode::PLAYER_OBSERVATION_PROMPT;
            res.player_prompt_text = matched_question.empty()
                ? ("Inspect station for step '" + spec.step_action_uri + "' and record your observation.")
                : matched_question;
            pending_prompts_.push_back(PlayerObservationPrompt{
                spec.step_action_uri,
                res.player_prompt_text,
                spec.required_registers
            });
            return res;
        }

        // 2. Check whether Oasis natively models both the action domain AND all required physical registers
        const ModeledDomainBinding* domain_match = MatchModeledDomain(spec.step_action_uri);
        const bool all_registers_modeled = AllRegistersModeled(spec.required_registers);
        if (domain_match != nullptr && all_registers_modeled) {
            res.mode = ResolutionMode::MODELED_PHYSICAL_DOMAIN;
            res.resolved_domain_id = domain_match->domain_id;
            if (spec.preferred_station_archetype.empty() && !domain_match->default_station_archetype.empty()) {
                res.station_archetype_id = domain_match->default_station_archetype;
            }
            return res;
        }

        // 3. Unmodeled micro-detail: synthesize realistic default register values & start paced station work timer
        res.mode = ResolutionMode::DEFAULT_VALUES_AND_TIMED_STATION_WORK;
        res.resolved_domain_id = "unmodeled_station_work";
        for (const std::string& reg_key : spec.required_registers) {
            res.default_registers.push_back(SynthesizeDefaultReading(reg_key));
        }

        const uint32_t duration_ms = std::max(30u, spec.nominal_work_duration_sim_s) * kSimMsPerSecond;
        res.station_timer.Start(duration_ms, duration_ms);
        return res;
    }

    [[nodiscard]] StepAdapterResolution ResolveStep(
        std::string_view step_action_uri,
        const std::vector<std::string>& required_registers,
        uint32_t nominal_work_duration_sim_s = 240u,
        bool request_player_observation = false
    ) {
        L4WorkflowStepSpec spec{};
        spec.step_action_uri             = std::string(step_action_uri);
        spec.required_registers          = required_registers;
        spec.nominal_work_duration_sim_s = nominal_work_duration_sim_s;
        spec.request_player_observation  = request_player_observation;
        return ResolveWorkflowStep(spec);
    }

    [[nodiscard]] const std::vector<PlayerObservationPrompt>& PendingObservationPrompts() const noexcept {
        return pending_prompts_;
    }

    bool PopNextObservationPrompt(PlayerObservationPrompt& out_prompt) {
        if (pending_prompts_.empty()) {
            return false;
        }
        out_prompt = std::move(pending_prompts_.front());
        pending_prompts_.erase(pending_prompts_.begin());
        return true;
    }

    void ClearObservationPrompts() noexcept {
        pending_prompts_.clear();
    }

private:
    [[nodiscard]] const ModeledDomainBinding* MatchModeledDomain(std::string_view uri) const noexcept {
        for (const auto& b : modeled_domains_) {
            if (uri.size() >= b.action_uri_prefix.size() &&
                uri.substr(0u, b.action_uri_prefix.size()) == b.action_uri_prefix) {
                return &b;
            }
        }
        return nullptr;
    }

    [[nodiscard]] bool AllRegistersModeled(const std::vector<std::string>& regs) const noexcept {
        for (const std::string& r : regs) {
            if (modeled_registers_.find(r) == modeled_registers_.end()) {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] bool MatchesObservationPromptPolicy(
        const L4WorkflowStepSpec& spec,
        std::string& out_question
    ) const {
        for (const auto& rule : prompt_rules_) {
            if (spec.step_action_uri.find(rule.uri_or_register_token) != std::string::npos) {
                out_question = rule.diegetic_question_template;
                return true;
            }
            for (const std::string& reg : spec.required_registers) {
                if (reg.find(rule.uri_or_register_token) != std::string::npos) {
                    out_question = rule.diegetic_question_template;
                    return true;
                }
            }
        }
        return false;
    }

    [[nodiscard]] DefaultRegisterReading SynthesizeDefaultReading(const std::string& reg_key) const {
        if (const auto it = default_register_table_.find(reg_key); it != default_register_table_.end()) {
            return it->second;
        }
        // Unit-suffix physical heuristics for arbitrary mod / L4 registers
        if (EndsWith(reg_key, "_moisture_bp")) return DefaultRegisterReading{1400LL, reg_key, "bp"};
        if (EndsWith(reg_key, "_temp_mk"))     return DefaultRegisterReading{293150LL, reg_key, "mK"};
        if (EndsWith(reg_key, "_ph_centi"))    return DefaultRegisterReading{650LL, reg_key, "pH*100"};
        if (EndsWith(reg_key, "_ppm"))         return DefaultRegisterReading{45LL, reg_key, "ppm"};
        if (EndsWith(reg_key, "_bp"))          return DefaultRegisterReading{9000LL, reg_key, "bp"};
        if (EndsWith(reg_key, "_ml"))          return DefaultRegisterReading{1000LL, reg_key, "ml"};
        if (EndsWith(reg_key, "_g"))           return DefaultRegisterReading{500LL, reg_key, "g"};
        return DefaultRegisterReading{1LL, reg_key, "flag"};
    }

    [[nodiscard]] static bool EndsWith(std::string_view str, std::string_view suffix) noexcept {
        return str.size() >= suffix.size() &&
               str.substr(str.size() - suffix.size()) == suffix;
    }

    std::vector<ModeledDomainBinding>                        modeled_domains_{};
    std::unordered_set<std::string>                          modeled_registers_{};
    std::unordered_map<std::string, DefaultRegisterReading>  default_register_table_{};
    std::vector<ObservationPromptRule>                       prompt_rules_{};
    std::vector<PlayerObservationPrompt>                     pending_prompts_{};
};

} // namespace oasis::engine
