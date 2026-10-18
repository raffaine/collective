#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "gateway_server.hpp"
#include "uhai/uhai_ring_buffer.hpp"

using namespace oasis::gateway;
using namespace oasis::uhai;

// ============================================================================
// Tier 1: UHAI Ring Buffer Parsing, Struct Memory Layout & Serialization
// ============================================================================

TEST_CASE("Tier 1: UHAI Memory Layout & Standard Layout Verification", "[tier1][layout]") {
    SECTION("TelemetrySample (Layer 2) binary layout & alignment") {
        REQUIRE(sizeof(TelemetrySample) == 32);
        REQUIRE(alignof(TelemetrySample) == 8);
        REQUIRE(std::is_standard_layout_v<TelemetrySample>);
        REQUIRE(std::is_trivially_copyable_v<TelemetrySample>);

        REQUIRE(offsetof(TelemetrySample, frame_count) == 0);
        REQUIRE(offsetof(TelemetrySample, timestamp_ns) == 8);
        REQUIRE(offsetof(TelemetrySample, fps) == 16);
        REQUIRE(offsetof(TelemetrySample, founder_stress) == 20);
        REQUIRE(offsetof(TelemetrySample, memory_slot_count) == 24);
        REQUIRE(offsetof(TelemetrySample, biome_id) == 28);
        REQUIRE(offsetof(TelemetrySample, quality_flags) == 29);
    }

    SECTION("L4ExecutionTelemetry (Layer 4) binary layout & alignment") {
        REQUIRE(sizeof(L4ExecutionTelemetry) == 104);
        REQUIRE(alignof(L4ExecutionTelemetry) == 8);
        REQUIRE(std::is_standard_layout_v<L4ExecutionTelemetry>);
        REQUIRE(std::is_trivially_copyable_v<L4ExecutionTelemetry>);

        REQUIRE(offsetof(L4ExecutionTelemetry, tick_count) == 0);
        REQUIRE(offsetof(L4ExecutionTelemetry, timestamp_ns) == 8);
        REQUIRE(offsetof(L4ExecutionTelemetry, instruction_fuel_remaining) == 16);
        REQUIRE(offsetof(L4ExecutionTelemetry, instruction_fuel_budget) == 24);
        REQUIRE(offsetof(L4ExecutionTelemetry, memory_fuel_bytes_used) == 32);
        REQUIRE(offsetof(L4ExecutionTelemetry, memory_fuel_bytes_limit) == 40);
        REQUIRE(offsetof(L4ExecutionTelemetry, active_workflow_count) == 48);
        REQUIRE(offsetof(L4ExecutionTelemetry, active_token_count) == 52);
        REQUIRE(offsetof(L4ExecutionTelemetry, queue_depth) == 56);
        REQUIRE(offsetof(L4ExecutionTelemetry, queue_dropped) == 76);
        REQUIRE(offsetof(L4ExecutionTelemetry, vm_tick_rate_hz) == 96);
        REQUIRE(offsetof(L4ExecutionTelemetry, shedding_active) == 100);
        REQUIRE(offsetof(L4ExecutionTelemetry, sandbox_flags) == 101);
    }

    SECTION("UpwardYieldPacket (Layer 4 -> Layer 5 Gate) binary layout & alignment") {
        REQUIRE(sizeof(UpwardYieldPacket) == 456);
        REQUIRE(alignof(UpwardYieldPacket) == 8);
        REQUIRE(std::is_standard_layout_v<UpwardYieldPacket>);
        REQUIRE(std::is_trivially_copyable_v<UpwardYieldPacket>);

        REQUIRE(offsetof(UpwardYieldPacket, yield_id) == 0);
        REQUIRE(offsetof(UpwardYieldPacket, workflow_token_id) == 8);
        REQUIRE(offsetof(UpwardYieldPacket, timestamp_hlc) == 16);
        REQUIRE(offsetof(UpwardYieldPacket, reason) == 24);
        REQUIRE(offsetof(UpwardYieldPacket, priority_tier) == 25);
        REQUIRE(offsetof(UpwardYieldPacket, task_gate_id) == 32);
        REQUIRE(offsetof(UpwardYieldPacket, claimant_did) == 96);
        REQUIRE(offsetof(UpwardYieldPacket, required_exergy_mj) == 160);
        REQUIRE(offsetof(UpwardYieldPacket, description) == 168);
        REQUIRE(offsetof(UpwardYieldPacket, sha256_spec_digest) == 424);
    }

    SECTION("L5GovernanceTelemetry (Layer 5) binary layout & alignment") {
        REQUIRE(sizeof(L5GovernanceTelemetry) == 64);
        REQUIRE(alignof(L5GovernanceTelemetry) == 8);
        REQUIRE(std::is_standard_layout_v<L5GovernanceTelemetry>);
        REQUIRE(std::is_trivially_copyable_v<L5GovernanceTelemetry>);

        REQUIRE(offsetof(L5GovernanceTelemetry, timestamp_ns) == 0);
        REQUIRE(offsetof(L5GovernanceTelemetry, pending_approval_count) == 8);
        REQUIRE(offsetof(L5GovernanceTelemetry, active_treaty_count) == 12);
        REQUIRE(offsetof(L5GovernanceTelemetry, ratified_treaty_count) == 16);
        REQUIRE(offsetof(L5GovernanceTelemetry, active_referendum_count) == 20);
        REQUIRE(offsetof(L5GovernanceTelemetry, bilateral_credit_limit_mj) == 24);
        REQUIRE(offsetof(L5GovernanceTelemetry, net_credit_balance_mj) == 32);
        REQUIRE(offsetof(L5GovernanceTelemetry, escrow_locked_mj) == 40);
        REQUIRE(offsetof(L5GovernanceTelemetry, steward_trust_ring) == 48);
        REQUIRE(offsetof(L5GovernanceTelemetry, governance_flags) == 49);
    }

    SECTION("SpatialIntentPayload (Layer 6) binary layout & alignment") {
        REQUIRE(sizeof(SpatialIntentPayload) == 264);
        REQUIRE(alignof(SpatialIntentPayload) == 8);
        REQUIRE(std::is_standard_layout_v<SpatialIntentPayload>);
        REQUIRE(std::is_trivially_copyable_v<SpatialIntentPayload>);
    }
}

TEST_CASE("Tier 1: GatewayServer UHAI Parsing & State Mutation Verification", "[tier1][parsing]") {
    GatewayServer server("/test_uhai_gateway_parsing");

    SECTION("L2 Telemetry Sample state injection and JSON serialization") {
        TelemetrySample sample{};
        sample.frame_count = 12045;
        sample.timestamp_ns = 1728751234567890000ULL;
        sample.fps = 59.94f;
        sample.founder_stress = 0.1234f;
        sample.memory_slot_count = 512;
        sample.biome_id = 1; // Urban
        sample.quality_flags = QUALITY_HARDWARE_ROOT;

        server.SetL2Sample(sample);

        const auto& retrieved = server.GetL2Sample();
        REQUIRE(retrieved.frame_count == 12045);
        REQUIRE(retrieved.timestamp_ns == 1728751234567890000ULL);
        REQUIRE(retrieved.fps == 59.94f);
        REQUIRE(retrieved.founder_stress == 0.1234f);
        REQUIRE(retrieved.memory_slot_count == 512);
        REQUIRE(retrieved.biome_id == 1);
        REQUIRE(retrieved.quality_flags == QUALITY_HARDWARE_ROOT);

        HttpRequest req;
        req.method = "GET";
        req.path = "/api/v1/l2/telemetry/latest";
        HttpResponse res = server.HandleRequest(req);

        REQUIRE(res.status_code == 200);
        REQUIRE(res.body.find("\"frame_count\": 12045") != std::string::npos);
        REQUIRE(res.body.find("\"fps\": 59.94") != std::string::npos);
        REQUIRE(res.body.find("\"founder_stress\": 0.1234") != std::string::npos);
        REQUIRE(res.body.find("\"quality_flag_label\": \"QUALITY_HARDWARE_ROOT\"") != std::string::npos);
    }

    SECTION("L4 Execution Telemetry state injection and JSON serialization") {
        L4ExecutionTelemetry l4{};
        l4.tick_count = 840;
        l4.timestamp_ns = 1728750000000000000ULL;
        l4.instruction_fuel_budget = 2'000'000;
        l4.instruction_fuel_remaining = 1'450'000;
        l4.memory_fuel_bytes_used = 131'072;
        l4.memory_fuel_bytes_limit = 2'097'152;
        l4.active_workflow_count = 4;
        l4.active_token_count = 7;
        l4.queue_depth[0] = 0; // Tier 1 Ecological Floor
        l4.queue_depth[1] = 2; // Tier 2 Subsistence
        l4.queue_depth[2] = 3; // Tier 3 Mutual Aid
        l4.queue_depth[3] = 1; // Tier 4 Discretionary
        l4.queue_depth[4] = 0; // Tier 5 Legacy Export
        l4.vm_tick_rate_hz = 10.0f;
        l4.shedding_active = 0;
        l4.sandbox_flags = 0x03;

        server.SetL4Telemetry(l4);

        const auto& retrieved = server.GetL4Telemetry();
        REQUIRE(retrieved.tick_count == 840);
        REQUIRE(retrieved.instruction_fuel_remaining == 1'450'000);
        REQUIRE(retrieved.active_token_count == 7);

        HttpRequest req;
        req.method = "GET";
        req.path = "/api/v1/l4/workflows";
        HttpResponse res = server.HandleRequest(req);

        REQUIRE(res.status_code == 200);
        REQUIRE(res.body.find("\"active_workflow_count\": 4") != std::string::npos);
        REQUIRE(res.body.find("\"active_token_count\": 7") != std::string::npos);
        REQUIRE(res.body.find("\"instruction_fuel_remaining\": 1450000") != std::string::npos);
        REQUIRE(res.body.find("\"is_read_only\": true") != std::string::npos);
    }

    SECTION("L5 Governance Telemetry state injection and pending yields") {
        L5GovernanceTelemetry l5{};
        l5.timestamp_ns = 1728750000000000000ULL;
        l5.pending_approval_count = 2;
        l5.active_treaty_count = 3;
        l5.ratified_treaty_count = 2;
        l5.active_referendum_count = 1;
        l5.bilateral_credit_limit_mj = 100'000;
        l5.net_credit_balance_mj = 35'000;
        l5.escrow_locked_mj = 25'000;
        l5.steward_trust_ring = 1;
        l5.governance_flags = 0x01;

        server.SetL5Telemetry(l5);

        UpwardYieldPacket yield_pkt{};
        yield_pkt.yield_id = 902100;
        yield_pkt.workflow_token_id = 44102;
        yield_pkt.timestamp_hlc = 1728750000000000000ULL;
        yield_pkt.reason = YieldReason::HUMAN_TASK_GATE_COMPLETION;
        yield_pkt.priority_tier = 3;
        std::strncpy(yield_pkt.task_gate_id, "Gate_Compost_Aeration", sizeof(yield_pkt.task_gate_id) - 1);
        std::strncpy(yield_pkt.claimant_did, "did:mesh:node07:permaculture", sizeof(yield_pkt.claimant_did) - 1);
        yield_pkt.required_exergy_mj = 18000; // 18.0 MJ
        std::strncpy(yield_pkt.description, "Turn and moisture-meter bioreactor bay 3 compost pile.", sizeof(yield_pkt.description) - 1);

        server.AddPendingYield(yield_pkt);

        REQUIRE(server.GetPendingYields().size() >= 2); // Initial seed + our new yield

        HttpRequest req;
        req.method = "GET";
        req.path = "/api/v1/l5/approvals/pending";
        HttpResponse res = server.HandleRequest(req);

        REQUIRE(res.status_code == 200);
        REQUIRE(res.body.find("\"yield_id\": \"y_902100\"") != std::string::npos);
        REQUIRE(res.body.find("\"workflow_token_id\": \"tok_44102\"") != std::string::npos);
        REQUIRE(res.body.find("\"gate_id\": \"Gate_Compost_Aeration\"") != std::string::npos);
        REQUIRE(res.body.find("\"claimant_did\": \"did:mesh:node07:permaculture\"") != std::string::npos);
        REQUIRE(res.body.find("\"escrow_mj\": 18.0") != std::string::npos);
    }
}

// ============================================================================
// Tier 2: Diagnostic GET Endpoints
// ============================================================================

TEST_CASE("Tier 2: Diagnostic GET Endpoints Comprehensive Verification", "[tier2][diagnostic_gets]") {
    GatewayServer server("/test_uhai_gateway_tier2");

    SECTION("GET /api/v1/layers/summary returns 200 OK and all layer keys") {
        HttpRequest req;
        req.method = "GET";
        req.path = "/api/v1/layers/summary";
        HttpResponse res = server.HandleRequest(req);

        REQUIRE(res.status_code == 200);
        REQUIRE(res.headers.at("Content-Type").find("application/json") != std::string::npos);
        REQUIRE(res.body.find("\"status\": \"HEALTHY\"") != std::string::npos);
        REQUIRE(res.body.find("\"node_did\": \"did:mesh:node01:gateway\"") != std::string::npos);

        // Verify all 7 layer keys exist in summary
        REQUIRE(res.body.find("\"l1_hal\"") != std::string::npos);
        REQUIRE(res.body.find("\"l2_membrane\"") != std::string::npos);
        REQUIRE(res.body.find("\"l3_ledger\"") != std::string::npos);
        REQUIRE(res.body.find("\"l4_execution\"") != std::string::npos);
        REQUIRE(res.body.find("\"l5_governance\"") != std::string::npos);
        REQUIRE(res.body.find("\"l6_semantic\"") != std::string::npos);
        REQUIRE(res.body.find("\"l7_carapace\"") != std::string::npos);
    }

    SECTION("GET /api/v1/l2/telemetry/latest returns 200 OK with live telemetry fields") {
        HttpRequest req;
        req.method = "GET";
        req.path = "/api/v1/l2/telemetry/latest";
        HttpResponse res = server.HandleRequest(req);

        REQUIRE(res.status_code == 200);
        REQUIRE(res.body.find("\"frame_count\":") != std::string::npos);
        REQUIRE(res.body.find("\"timestamp_ns\":") != std::string::npos);
        REQUIRE(res.body.find("\"fps\":") != std::string::npos);
        REQUIRE(res.body.find("\"founder_stress\":") != std::string::npos);
        REQUIRE(res.body.find("\"memory_slot_count\":") != std::string::npos);
        REQUIRE(res.body.find("\"biome_id\":") != std::string::npos);
        REQUIRE(res.body.find("\"quality_flags\":") != std::string::npos);
        REQUIRE(res.body.find("\"quality_flag_label\":") != std::string::npos);
    }

    SECTION("GET /api/v1/l4/workflows returns 200 OK with is_read_only: true") {
        HttpRequest req;
        req.method = "GET";
        req.path = "/api/v1/l4/workflows";
        HttpResponse res = server.HandleRequest(req);

        REQUIRE(res.status_code == 200);
        REQUIRE(res.body.find("\"status\": \"READ_ONLY\"") != std::string::npos);
        REQUIRE(res.body.find("\"is_read_only\": true") != std::string::npos);
        REQUIRE(res.body.find("\"active_workflow_count\":") != std::string::npos);
        REQUIRE(res.body.find("\"active_token_count\":") != std::string::npos);
        REQUIRE(res.body.find("\"instruction_fuel_remaining\":") != std::string::npos);
        REQUIRE(res.body.find("\"instruction_fuel_budget\":") != std::string::npos);
        REQUIRE(res.body.find("\"queue_depth\":") != std::string::npos);
    }

    SECTION("GET /api/v1/l5/approvals/pending returns 200 OK with pending upward-yielded task gates") {
        HttpRequest req;
        req.method = "GET";
        req.path = "/api/v1/l5/approvals/pending";
        HttpResponse res = server.HandleRequest(req);

        REQUIRE(res.status_code == 200);
        REQUIRE(res.body.find("\"pending_count\":") != std::string::npos);
        REQUIRE(res.body.find("\"approvals\":") != std::string::npos);
        REQUIRE(res.body.find("\"yield_id\":") != std::string::npos);
        REQUIRE(res.body.find("\"workflow_token_id\":") != std::string::npos);
        REQUIRE(res.body.find("\"plain_language_read_back\":") != std::string::npos);
    }

    SECTION("Diagnostic secondary GET endpoints across L2, L4, L5, L6, L7") {
        // L2 Residuals
        {
            HttpRequest req{"GET", "/api/v1/l2/residuals", "", "HTTP/1.1", {}, ""};
            HttpResponse res = server.HandleRequest(req);
            REQUIRE(res.status_code == 200);
            REQUIRE(res.body.find("\"residual_epsilon\":") != std::string::npos);
            REQUIRE(res.body.find("\"anomaly_detected\": false") != std::string::npos);
        }

        // L2 Mesh Peers
        {
            HttpRequest req{"GET", "/api/v1/l2/mesh/peers", "", "HTTP/1.1", {}, ""};
            HttpResponse res = server.HandleRequest(req);
            REQUIRE(res.status_code == 200);
            REQUIRE(res.body.find("\"peer_count\":") != std::string::npos);
            REQUIRE(res.body.find("\"peers\":") != std::string::npos);
        }

        // L4 Priority Queue
        {
            HttpRequest req{"GET", "/api/v1/l4/priority-queue", "", "HTTP/1.1", {}, ""};
            HttpResponse res = server.HandleRequest(req);
            REQUIRE(res.status_code == 200);
            REQUIRE(res.body.find("\"tier_1_ecological_floor\":") != std::string::npos);
            REQUIRE(res.body.find("\"tier_3_mutual_aid\":") != std::string::npos);
        }

        // L4 Wasm Fuel
        {
            HttpRequest req{"GET", "/api/v1/l4/wasm-fuel", "", "HTTP/1.1", {}, ""};
            HttpResponse res = server.HandleRequest(req);
            REQUIRE(res.status_code == 200);
            REQUIRE(res.body.find("\"fuel_budget\":") != std::string::npos);
            REQUIRE(res.body.find("\"fuel_remaining\":") != std::string::npos);
        }

        // L5 Treaties
        {
            HttpRequest req{"GET", "/api/v1/l5/treaties", "", "HTTP/1.1", {}, ""};
            HttpResponse res = server.HandleRequest(req);
            REQUIRE(res.status_code == 200);
            REQUIRE(res.body.find("\"treaty_count\":") != std::string::npos);
            REQUIRE(res.body.find("\"status\": \"RATIFIED\"") != std::string::npos);
        }

        // L5 Trust Ring
        {
            HttpRequest req{"GET", "/api/v1/l5/trust-ring", "", "HTTP/1.1", {}, ""};
            HttpResponse res = server.HandleRequest(req);
            REQUIRE(res.status_code == 200);
            REQUIRE(res.body.find("\"steward_trust_ring\":") != std::string::npos);
            REQUIRE(res.body.find("\"local_did\":") != std::string::npos);
        }

        // L5 Credit Limits
        {
            HttpRequest req{"GET", "/api/v1/l5/credit-limits", "", "HTTP/1.1", {}, ""};
            HttpResponse res = server.HandleRequest(req);
            REQUIRE(res.status_code == 200);
            REQUIRE(res.body.find("\"bilateral_credit_limit_mj\":") != std::string::npos);
            REQUIRE(res.body.find("\"net_credit_balance_mj\":") != std::string::npos);
        }

        // L6 Orderbook
        {
            HttpRequest req{"GET", "/api/v1/l6/agora/orderbook", "", "HTTP/1.1", {}, ""};
            HttpResponse res = server.HandleRequest(req);
            REQUIRE(res.status_code == 200);
            REQUIRE(res.body.find("\"bids\":") != std::string::npos);
            REQUIRE(res.body.find("\"asks\":") != std::string::npos);
            REQUIRE(res.body.find("\"midpoint_price_mj\":") != std::string::npos);
        }

        // L6 Bounties
        {
            HttpRequest req{"GET", "/api/v1/l6/bounties", "", "HTTP/1.1", {}, ""};
            HttpResponse res = server.HandleRequest(req);
            REQUIRE(res.status_code == 200);
            REQUIRE(res.body.find("\"bounty_count\":") != std::string::npos);
        }

        // L7 Status
        {
            HttpRequest req{"GET", "/api/v1/l7/status", "", "HTTP/1.1", {}, ""};
            HttpResponse res = server.HandleRequest(req);
            REQUIRE(res.status_code == 200);
            REQUIRE(res.body.find("\"threat_level\":") != std::string::npos);
            REQUIRE(res.body.find("\"grid_intertie\":") != std::string::npos);
            REQUIRE(res.body.find("\"carapace_finances\":") != std::string::npos);
        }

        // HEAD method parity
        {
            HttpRequest req{"HEAD", "/api/v1/layers/summary", "", "HTTP/1.1", {}, ""};
            HttpResponse res = server.HandleRequest(req);
            REQUIRE(res.status_code == 200);
        }
    }
}

// ============================================================================
// Tier 3: Semantic Mutating POST Endpoints (Layer 6 & Layer 5)
// ============================================================================

TEST_CASE("Tier 3: Mutating POST Endpoints (Intent U-Turn & Ratifications)", "[tier3][mutating_post]") {
    GatewayServer server("/test_uhai_gateway_tier3");

    SECTION("POST /api/v1/l6/intent with valid intent payload compiles draft CID") {
        HttpRequest req;
        req.method = "POST";
        req.path = "/api/v1/l6/intent";
        req.headers["Content-Type"] = "application/json";
        req.body = "{\"action\": \"designate_affordance\", \"target\": \"hydroponic_tray_bay4\", \"author\": \"did:mesh:user1\"}";

        size_t initial_count = server.GetSubmittedIntents().size();
        HttpResponse res = server.HandleRequest(req);

        REQUIRE(res.status_code == 200);
        REQUIRE(res.body.find("\"status\": \"DRAFT_COMPILED\"") != std::string::npos);
        REQUIRE(res.body.find("\"schema_uri\": \"https://schema.collective.local/v1/intent.jsonld\"") != std::string::npos);
        REQUIRE(res.body.find("\"intent_cid\": \"bafyreib") != std::string::npos);
        REQUIRE(res.body.find("\"split_airgap_verified\": true") != std::string::npos);

        // Check internal server intent journal
        REQUIRE(server.GetSubmittedIntents().size() == initial_count + 1);
        const auto& last_intent = server.GetSubmittedIntents().back();
        REQUIRE(last_intent.status == 1); // Validated
        REQUIRE(std::string_view(last_intent.schema_uri).starts_with("https://schema.collective.local"));
        REQUIRE(std::string(last_intent.payload_jsonld).find("hydroponic_tray_bay4") != std::string::npos);
    }

    SECTION("POST /api/v1/l6/intent/author alias path works identically") {
        HttpRequest req;
        req.method = "POST";
        req.path = "/api/v1/l6/intent/author";
        req.headers["Content-Type"] = "application/json";
        req.body = "{\"action\": \"seed_nursery\", \"species\": \"vetiver_grass\"}";

        HttpResponse res = server.HandleRequest(req);
        REQUIRE(res.status_code == 200);
        REQUIRE(res.body.find("\"status\": \"DRAFT_COMPILED\"") != std::string::npos);
        REQUIRE(res.body.find("\"intent_cid\": \"bafyreib") != std::string::npos);
    }

    SECTION("POST /api/v1/l6/intent rejects empty or malformed payload with 400 Bad Request") {
        // Empty body
        {
            HttpRequest req;
            req.method = "POST";
            req.path = "/api/v1/l6/intent";
            req.body = "";
            HttpResponse res = server.HandleRequest(req);
            REQUIRE(res.status_code == 400);
            REQUIRE(res.body.find("\"error\": \"BAD_REQUEST\"") != std::string::npos);
        }

        // Whitespace-only body
        {
            HttpRequest req;
            req.method = "POST";
            req.path = "/api/v1/l6/intent";
            req.body = "   \n\t  ";
            HttpResponse res = server.HandleRequest(req);
            REQUIRE(res.status_code == 400);
            REQUIRE(res.body.find("\"error\": \"BAD_REQUEST\"") != std::string::npos);
        }

        // Malformed non-JSON payload
        {
            HttpRequest req;
            req.method = "POST";
            req.path = "/api/v1/l6/intent";
            req.body = "this is not valid json text";
            HttpResponse res = server.HandleRequest(req);
            REQUIRE(res.status_code == 400);
            REQUIRE(res.body.find("\"error\": \"BAD_REQUEST\"") != std::string::npos);
        }
    }

    SECTION("POST /api/v1/l6/agora/orders submits market trade") {
        HttpRequest req;
        req.method = "POST";
        req.path = "/api/v1/l6/agora/orders";
        req.body = "{\"side\": \"BID\", \"resource\": \"ELECTRICITY\", \"price_mj\": 3.55, \"kwh\": 10.0}";

        HttpResponse res = server.HandleRequest(req);
        REQUIRE(res.status_code == 200);
        REQUIRE(res.body.find("\"status\": \"PLACED\"") != std::string::npos);
        REQUIRE(res.body.find("\"order_id\": \"ord_9182\"") != std::string::npos);
    }

    SECTION("POST /api/v1/l5/approvals/:id/resolve resolves upward-yielded task gate") {
        HttpRequest req;
        req.method = "POST";
        req.path = "/api/v1/l5/approvals/y_849201/resolve";
        req.body = "{\"resolution\": \"RATIFIED_APPROVED\", \"approver_did\": \"did:mesh:node01:steward\"}";

        HttpResponse res = server.HandleRequest(req);
        REQUIRE(res.status_code == 200);
        REQUIRE(res.body.find("\"status\": \"RATIFIED_APPROVED\"") != std::string::npos);
        REQUIRE(res.body.find("\"released_escrow_mj\": 12.5") != std::string::npos);
        REQUIRE(res.body.find("\"l4_dispatch_confirmed\": true") != std::string::npos);
    }

    SECTION("POST /api/v1/l5/treaties/:cid/ratify ratifies bilateral treaty") {
        HttpRequest req;
        req.method = "POST";
        req.path = "/api/v1/l5/treaties/bafyreib249182a5c9f28d84920199182/ratify";
        req.body = "{\"bicameral_consent\": true, \"signer\": \"did:mesh:node01:steward\"}";

        HttpResponse res = server.HandleRequest(req);
        REQUIRE(res.status_code == 200);
        REQUIRE(res.body.find("\"status\": \"RATIFIED\"") != std::string::npos);
        REQUIRE(res.body.find("\"bicameral_quorum_met\": true") != std::string::npos);
    }
}

// ============================================================================
// Tier 4: Invariant 7 / Diagnostic Layer Write Rejection (CRITICAL REQUIREMENT)
// ============================================================================

TEST_CASE("Tier 4: Invariant 7 Strict Rejection of Illegal Writes to Read-Only Layers", "[tier4][invariant7]") {
    GatewayServer server("/test_uhai_gateway_tier4");

    const std::vector<std::string> mutating_methods = {"POST", "PUT", "DELETE", "PATCH"};

    SECTION("Strict rejection of illegal writes to Layer 1 HAL (/api/v1/l1/*)") {
        const std::vector<std::string> l1_paths = {
            "/api/v1/l1/actuate",
            "/api/v1/l1/gpio/set",
            "/api/v1/l1/valve/open",
            "/api/v1/l1/calibrate"
        };

        for (const auto& path : l1_paths) {
            for (const auto& method : mutating_methods) {
                HttpRequest req{method, path, "", "HTTP/1.1", {}, "{\"pin\": 14, \"state\": 1}"};
                HttpResponse res = server.HandleRequest(req);

                REQUIRE(res.status_code == 405);
                REQUIRE(res.status_text == "Method Not Allowed");
                REQUIRE(res.headers.at("Allow") == "GET, OPTIONS");
                REQUIRE(res.body.find("\"error\": \"L1_HAL_IS_READ_ONLY\"") != std::string::npos);
            }
        }
    }

    SECTION("Strict rejection of illegal writes to Layer 2 Membrane (/api/v1/l2/*)") {
        const std::vector<std::string> l2_paths = {
            "/api/v1/l2/actuate",
            "/api/v1/l2/telemetry/override",
            "/api/v1/l2/mesh/peers/drop",
            "/api/v1/l2/residuals/reset"
        };

        for (const auto& path : l2_paths) {
            for (const auto& method : mutating_methods) {
                HttpRequest req{method, path, "", "HTTP/1.1", {}, "{\"spoof\": true}"};
                HttpResponse res = server.HandleRequest(req);

                REQUIRE(res.status_code == 405);
                REQUIRE(res.status_text == "Method Not Allowed");
                REQUIRE(res.headers.at("Allow") == "GET, OPTIONS");
                REQUIRE(res.body.find("\"error\": \"L2_MEMBRANE_IS_READ_ONLY\"") != std::string::npos);
            }
        }
    }

    SECTION("Strict rejection of illegal writes to Layer 3 Ledger (/api/v1/l3/*)") {
        const std::vector<std::string> l3_paths = {
            "/api/v1/l3/write",
            "/api/v1/l3/ledger/inject",
            "/api/v1/l3/crdt/force_merge",
            "/api/v1/l3/rollback"
        };

        for (const auto& path : l3_paths) {
            for (const auto& method : mutating_methods) {
                HttpRequest req{method, path, "", "HTTP/1.1", {}, "{\"tx\": \"illegal_mint\"}"};
                HttpResponse res = server.HandleRequest(req);

                REQUIRE(res.status_code == 405);
                REQUIRE(res.status_text == "Method Not Allowed");
                REQUIRE(res.headers.at("Allow") == "GET, OPTIONS");
                REQUIRE(res.body.find("\"error\": \"L3_LEDGER_IS_READ_ONLY\"") != std::string::npos);
            }
        }
    }

    SECTION("Strict rejection of illegal writes to Layer 4 Execution VM (/api/v1/l4/*)") {
        // Must reject with L4_IS_STRICTLY_READ_ONLY_NO_MUTATIONS
        const std::vector<std::string> l4_paths = {
            "/api/v1/l4/workflows",
            "/api/v1/l4/workflows/tok_123/step",
            "/api/v1/l4/workflows/tok_123",
            "/api/v1/l4/priority-queue/inject",
            "/api/v1/l4/wasm-fuel/topup",
            "/api/v1/l4/tokens/advance"
        };

        for (const auto& path : l4_paths) {
            for (const auto& method : mutating_methods) {
                HttpRequest req{method, path, "", "HTTP/1.1", {}, "{\"advance\": true}"};
                HttpResponse res = server.HandleRequest(req);

                REQUIRE(res.status_code == 405);
                REQUIRE(res.status_text == "Method Not Allowed");
                REQUIRE(res.headers.at("Allow") == "GET, OPTIONS");
                REQUIRE(res.body.find("\"error\": \"L4_IS_STRICTLY_READ_ONLY_NO_MUTATIONS\"") != std::string::npos);
                REQUIRE(res.body.find("Layer 4 (Execution VM) is deterministic, sandboxed, and strictly read-only.") != std::string::npos);
            }
        }
    }

    SECTION("Strict rejection of illegal writes to Layer 7 Corporate Carapace (/api/v1/l7/*)") {
        const std::vector<std::string> l7_paths = {
            "/api/v1/l7/curtailment/trip",
            "/api/v1/l7/threat-level/override",
            "/api/v1/l7/grid/intertie/disconnect",
            "/api/v1/l7/operating-cash/disburse"
        };

        for (const auto& path : l7_paths) {
            for (const auto& method : mutating_methods) {
                HttpRequest req{method, path, "", "HTTP/1.1", {}, "{\"trip_breaker\": true}"};
                HttpResponse res = server.HandleRequest(req);

                REQUIRE(res.status_code == 405);
                REQUIRE(res.status_text == "Method Not Allowed");
                REQUIRE(res.headers.at("Allow") == "GET, OPTIONS");
                REQUIRE(res.body.find("\"error\": \"L7_CARAPACE_IS_READ_ONLY\"") != std::string::npos);
            }
        }
    }
}

// ============================================================================
// Tier 5: Edge Cases, CORS Preflight (OPTIONS), 404 & HTTP Parser Robustness
// ============================================================================

TEST_CASE("Tier 5: Edge Cases, CORS Preflight, 404 & HTTP Parser Robustness", "[tier5][edge_cases]") {
    GatewayServer server("/test_uhai_gateway_tier5");

    SECTION("CORS Preflight (OPTIONS) returns 204 No Content with complete CORS headers") {
        HttpRequest req;
        req.method = "OPTIONS";
        req.path = "/api/v1/l6/intent";
        HttpResponse res = server.HandleRequest(req);

        REQUIRE(res.status_code == 204);
        REQUIRE(res.status_text == "No Content");
        REQUIRE(res.headers.at("Access-Control-Allow-Origin") == "*");
        REQUIRE(res.headers.at("Access-Control-Allow-Methods") == "GET, POST, PUT, DELETE, OPTIONS");
        REQUIRE(res.headers.at("Access-Control-Allow-Headers").find("Content-Type") != std::string::npos);
        REQUIRE(res.headers.at("Access-Control-Allow-Headers").find("X-Collective-Signature") != std::string::npos);
        REQUIRE(res.headers.at("Access-Control-Max-Age") == "86400");
    }

    SECTION("OPTIONS on diagnostic endpoints also returns 204 No Content (preflight allowed)") {
        HttpRequest req{"OPTIONS", "/api/v1/l4/workflows", "", "HTTP/1.1", {}, ""};
        HttpResponse res = server.HandleRequest(req);

        REQUIRE(res.status_code == 204);
        REQUIRE(res.headers.at("Access-Control-Allow-Origin") == "*");
    }

    SECTION("Unknown diagnostic or API path returns 404 Not Found") {
        HttpRequest req{"GET", "/api/v1/non_existent_deck", "", "HTTP/1.1", {}, ""};
        HttpResponse res = server.HandleRequest(req);

        REQUIRE(res.status_code == 404);
        REQUIRE(res.body.find("\"error\": \"NOT_FOUND\"") != std::string::npos);
    }

    SECTION("Unsupported HTTP method on non-diagnostic path returns 405 Method Not Allowed") {
        HttpRequest req{"DELETE", "/api/v1/layers/summary", "", "HTTP/1.1", {}, ""};
        HttpResponse res = server.HandleRequest(req);

        REQUIRE(res.status_code == 405);
    }

    SECTION("HttpParser handles valid HTTP request lines, headers, query parameters, and bodies") {
        std::string raw_http =
            "POST /api/v1/l6/intent?source=web&debug=true HTTP/1.1\r\n"
            "Host: collective.local:8443\r\n"
            "User-Agent: CompanionPWA/2.0\r\n"
            "Content-Type: application/json; charset=utf-8\r\n"
            "X-Collective-Signature: 0x98124abcdef\r\n"
            "Content-Length: 42\r\n"
            "\r\n"
            "{\"action\": \"designate_zone\", \"zone\": 12}";

        HttpRequest parsed = HttpParser::Parse(raw_http);

        REQUIRE(parsed.method == "POST");
        REQUIRE(parsed.path == "/api/v1/l6/intent");
        REQUIRE(parsed.query == "source=web&debug=true");
        REQUIRE(parsed.http_version == "HTTP/1.1");
        REQUIRE(parsed.GetHeader("Host") == "collective.local:8443");
        REQUIRE(parsed.GetHeader("user-agent") == "CompanionPWA/2.0"); // Case-insensitive lookup
        REQUIRE(parsed.GetHeader("content-type") == "application/json; charset=utf-8");
        REQUIRE(parsed.GetHeader("X-Collective-Signature") == "0x98124abcdef");
        REQUIRE(parsed.body == "{\"action\": \"designate_zone\", \"zone\": 12}");
    }

    SECTION("HttpParser handles empty, newline-only, or truncated input without exception") {
        HttpRequest empty_req = HttpParser::Parse("");
        REQUIRE(empty_req.method.empty());
        REQUIRE(empty_req.path.empty());

        HttpRequest newline_req = HttpParser::Parse("\r\n\r\n");
        REQUIRE(newline_req.method.empty());

        HttpRequest partial_req = HttpParser::Parse("GET /\r\n");
        REQUIRE(partial_req.method == "GET");
        REQUIRE(partial_req.path == "/");

        HttpRequest unterminated_req = HttpParser::Parse("GET /");
        REQUIRE(unterminated_req.method.empty());
    }

    SECTION("Cryptographic SHA-256 Engine passes FIPS 180-4 standard verification") {
        // Standard vector 1: Empty string -> e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
        std::string hash_empty = crypto::Sha256::HashString("");
        REQUIRE(hash_empty == "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");

        // Standard vector 2: "abc" -> ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad
        std::string hash_abc = crypto::Sha256::HashString("abc");
        REQUIRE(hash_abc == "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");

        // Standard vector 3: Multi-block message
        std::string msg = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
        std::string hash_multiblock = crypto::Sha256::HashString(msg);
        REQUIRE(hash_multiblock == "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");
    }

    SECTION("Request counting accumulator tracks invocations monotonically") {
        uint64_t before = server.GetTotalRequestsHandled();

        HttpRequest req1{"GET", "/api/v1/layers/summary", "", "HTTP/1.1", {}, ""};
        HttpRequest req2{"GET", "/api/v1/l2/telemetry/latest", "", "HTTP/1.1", {}, ""};
        HttpRequest req3{"GET", "/api/v1/l4/workflows", "", "HTTP/1.1", {}, ""};

        server.HandleRequest(req1);
        server.HandleRequest(req2);
        server.HandleRequest(req3);

        REQUIRE(server.GetTotalRequestsHandled() == before + 3);
    }
}

// ============================================================================
// Tier 6 (SS-EP6-006): GatewaySecurityConfig, L2-L5/L7 Read-Only Hood & L6 Intent U-Turn
// ============================================================================

TEST_CASE("Tier 6 (SS-EP6-006): GatewaySecurityConfig, L2-L5/L7 Read-Only Hood & Intent U-Turn", "[ss-ep6-006][security]") {
    GatewayServer server("/test_uhai_gateway_ss_ep6_006");

    SECTION("GatewaySecurityConfig enforces port 8443, VLAN 30, TLS 1.3, and DID Auth") {
        REQUIRE(sizeof(GatewaySecurityConfig) == 8);
        REQUIRE(alignof(GatewaySecurityConfig) == 8);
        REQUIRE(std::is_standard_layout_v<GatewaySecurityConfig>);
        REQUIRE(std::is_trivially_copyable_v<GatewaySecurityConfig>);

        const auto& cfg = server.GetSecurityConfig();
        REQUIRE(cfg.port == 8443);
        REQUIRE(cfg.vlan_id == 30);
        REQUIRE(cfg.tls_1_3_enforced == true);
        REQUIRE(cfg.did_auth_required == true);

        HttpRequest req{"GET", "/api/v1/security/config", "", "HTTP/1.1", {}, ""};
        HttpResponse res = server.HandleRequest(req);
        REQUIRE(res.status_code == 200);
        REQUIRE(res.body.find("\"port\": 8443") != std::string::npos);
        REQUIRE(res.body.find("\"vlan_id\": 30") != std::string::npos);
        REQUIRE(res.body.find("\"tls_1_3_enforced\": true") != std::string::npos);
        REQUIRE(res.body.find("\"did_auth_required\": true") != std::string::npos);
        REQUIRE(res.body.find("Human -> L6 -> L5 -> L4 -> [upward-yield] -> L5 -> L6 -> L7") != std::string::npos);
    }

    SECTION("Strict 405 Method Not Allowed on POST/PUT/DELETE/PATCH across L2-L5 and L7 diagnostic hood") {
        const std::vector<std::string> mutating_methods = {"POST", "PUT", "DELETE", "PATCH"};
        const std::vector<std::pair<std::string, std::string>> protected_endpoints = {
            {"/api/v1/l2/telemetry/latest", "L2_MEMBRANE_IS_READ_ONLY"},
            {"/api/v1/l3/ledger/write",     "L3_LEDGER_IS_READ_ONLY"},
            {"/api/v1/l4/workflows",        "L4_IS_STRICTLY_READ_ONLY_NO_MUTATIONS"},
            {"/api/v1/l5/treaties",         "L5_GOVERNANCE_IS_READ_ONLY"},
            {"/api/v1/l5/approvals/pending","L5_GOVERNANCE_IS_READ_ONLY"},
            {"/api/v1/l5/credit-limits",    "L5_GOVERNANCE_IS_READ_ONLY"},
            {"/api/v1/l7/status",           "L7_CARAPACE_IS_READ_ONLY"}
        };

        for (const auto& [path, expected_error] : protected_endpoints) {
            for (const auto& method : mutating_methods) {
                HttpRequest req{method, path, "", "HTTP/1.1", {}, "{\"mutate\": true}"};
                HttpResponse res = server.HandleRequest(req);
                REQUIRE(res.status_code == 405);
                REQUIRE(res.headers.at("Allow") == "GET, OPTIONS");
                REQUIRE(res.body.find(expected_error) != std::string::npos);
            }
        }
    }

    SECTION("POST /api/v1/l6/intent upward-yield hold ratification (RATIFY_L5_YIELD_HOLD) routes inward L6 -> L5") {
        const size_t initial_intents = server.GetSubmittedIntents().size();
        const size_t initial_resolutions = server.GetSubmittedResolutions().size();

        HttpRequest req;
        req.method = "POST";
        req.path = "/api/v1/l6/intent";
        req.headers["Content-Type"] = "application/json";
        req.body = "{\"intent_type\": \"RATIFY_L5_YIELD_HOLD\", \"yield_id\": \"y_849201\", \"decision\": \"RATIFIED_APPROVED\", \"steward_did\": \"did:mesh:node01:steward\"}";

        HttpResponse res = server.HandleRequest(req);
        REQUIRE(res.status_code == 200);
        REQUIRE(res.body.find("\"status\": \"DRAFT_COMPILED\"") != std::string::npos);
        REQUIRE(res.body.find("\"u_turn_routed\": \"L6_TO_L5\"") != std::string::npos);
        REQUIRE(res.body.find("\"resolution_status\": \"RATIFIED_APPROVED\"") != std::string::npos);
        REQUIRE(res.body.find("\"l4_dispatch_confirmed\": true") != std::string::npos);

        REQUIRE(server.GetSubmittedIntents().size() == initial_intents + 1);
        REQUIRE(server.GetSubmittedResolutions().size() == initial_resolutions + 1);

        const auto& resolution = server.GetSubmittedResolutions().back();
        REQUIRE(resolution.yield_id == 849201ULL);
        REQUIRE(resolution.status == ApprovalStatus::RATIFIED_APPROVED);
        REQUIRE(resolution.released_escrow_mj == 12500);
        REQUIRE(std::string(resolution.resolution_rationale).find("Human -> L6 -> L5 -> L4 -> [upward-yield] -> L5 -> L6 -> L7") != std::string::npos);
    }
}

