#include <catch2/catch_test_macros.hpp>
#include <spdlog/spdlog.h>
#include "adversary_engine.hpp"

TEST_CASE("Layer 7 col-adversaryd DES Threat Level Controller", "[layer7][adversary][des]") {
    using namespace collective::l7;

    SECTION("Story 7.1 Golden Output: LEVEL_2_HOSTILE 90-day calibrated event counts") {
        auto cfg2 = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_2_HOSTILE, 1337, 90);
        AdversaryDesEngine des(cfg2);
        auto fired = des.AdvanceDays(90.0);

        const auto& stats = des.GetStats();
        REQUIRE(stats.utility_rate_adjustments == 3);
        REQUIRE(stats.municipal_inspections == 2);
        REQUIRE(stats.merchant_reserve_holds == 1);
        REQUIRE(stats.total_events_emitted == 6);
        REQUIRE(fired.size() == 6);
    }

    SECTION("Monotonic event frequency scaling across Threat Levels (0 to 3)") {
        auto cfg0 = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_0_BENIGN, 1337, 90);
        auto cfg1 = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_1_STANDARD, 1337, 90);
        auto cfg2 = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_2_HOSTILE, 1337, 90);
        auto cfg3 = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_3_SIEGE, 1337, 90);

        AdversaryDesEngine des0(cfg0); des0.GenerateEvents();
        AdversaryDesEngine des1(cfg1); des1.GenerateEvents();
        AdversaryDesEngine des2(cfg2); des2.GenerateEvents();
        AdversaryDesEngine des3(cfg3); des3.GenerateEvents();

        REQUIRE(des0.EventCount() <= 4);
        REQUIRE(des1.EventCount() > des0.EventCount());
        REQUIRE(des2.EventCount() > des1.EventCount());
        REQUIRE(des3.EventCount() > des2.EventCount());
        REQUIRE(des3.EventCount() >= 35); // Siege level ensures overwhelming event pressure
    }

    SECTION("LEVEL_3_SIEGE exclusively fires scorched-earth municipal events") {
        auto siege_cfg = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_3_SIEGE, 1337, 90);
        AdversaryDesEngine des(siege_cfg);
        des.GenerateEvents();

        bool has_raid = false;
        bool has_freeze = false;
        bool has_lien = false;
        bool has_disconnect = false;

        while (des.HasNextEvent()) {
            auto ev = des.PopNextEvent();
            if (ev.event_type == AdversaryEventType::UNANNOUNCED_REGULATORY_RAID ||
                ev.type == AdversaryEventType::REGULATORY_MULTI_AGENCY_RAID) {
                has_raid = true;
            }
            if (ev.event_type == AdversaryEventType::BANK_ACCOUNT_FREEZE ||
                ev.type == AdversaryEventType::BANKING_ALGORITHMIC_FREEZE) {
                has_freeze = true;
            }
            if (ev.event_type == AdversaryEventType::MUNICIPAL_TAX_LIEN ||
                ev.type == AdversaryEventType::REGULATORY_TAX_LIEN) {
                has_lien = true;
            }
            if (ev.event_type == AdversaryEventType::UTILITY_DISCONNECT_ORDER ||
                ev.type == AdversaryEventType::UTILITY_SHUTOFF) {
                has_disconnect = true;
            }
        }

        REQUIRE(has_raid);
        REQUIRE(has_freeze);
        REQUIRE(has_lien);
        REQUIRE(has_disconnect);
    }

    SECTION("Priority queue preserves strict chronological order") {
        auto siege_cfg = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_3_SIEGE, 4242, 90);
        AdversaryDesEngine des(siege_cfg);
        des.GenerateEvents();

        uint64_t last_time = 0;
        while (des.HasNextEvent()) {
            auto ev = des.PopNextEvent();
            REQUIRE(ev.timestamp_seconds >= last_time);
            last_time = ev.timestamp_seconds;
        }
    }

    SECTION("Deterministic reproducibility under identical random seed") {
        auto cfgA = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_3_SIEGE, 9999, 90);
        auto cfgB = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_3_SIEGE, 9999, 90);

        AdversaryDesEngine desA(cfgA); desA.GenerateEvents();
        AdversaryDesEngine desB(cfgB); desB.GenerateEvents();

        REQUIRE(desA.EventCount() == desB.EventCount());
        while (desA.HasNextEvent()) {
            auto evA = desA.PopNextEvent();
            auto evB = desB.PopNextEvent();
            REQUIRE(evA.timestamp_seconds == evB.timestamp_seconds);
            REQUIRE(evA.event_type == evB.event_type);
            REQUIRE(evA.financial_penalty == evB.financial_penalty);
        }
    }
}
