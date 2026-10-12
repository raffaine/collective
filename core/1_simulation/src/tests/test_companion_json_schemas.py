#!/usr/bin/env python3
import json
import re
import sys

def test_json_blocks():
    doc_path = "/Users/raffaine/dev/collective/docs/COMPANION_APP_ARCHITECTURE.md"
    with open(doc_path, "r", encoding="utf-8") as f:
        content = f.read()

    # Extract all ```json ... ``` blocks
    json_blocks = re.findall(r"```json\s*(.*?)\s*```", content, re.DOTALL)
    print(f"Found {len(json_blocks)} json code blocks in {doc_path}")

    parsed_blocks = []
    errors = []
    for i, block in enumerate(json_blocks):
        try:
            parsed = json.loads(block)
            parsed_blocks.append((i, parsed))
            print(f"  Block {i+1}: Valid JSON (keys: {list(parsed.keys()) if isinstance(parsed, dict) else len(parsed)})")
        except json.JSONDecodeError as e:
            errors.append((i+1, str(e), block[:100]))
            print(f"  Block {i+1}: INVALID JSON! Error: {e}")

    if errors:
        print(f"\nFAILED: {len(errors)} JSON decode errors found!")
        for b_num, err, preview in errors:
            print(f"  Block {b_num}: {err}\nPreview: {preview}...")
        sys.exit(1)
    else:
        print("\nAll JSON blocks are syntactically valid JSON.")

    # Now let's test jsonschema validation if jsonschema module is available
    try:
        import jsonschema
        print("\nTesting JSON Schema compliance...")

        # Find schema_layer5_governance
        l5_schema = None
        l7_schema = None
        for i, block in parsed_blocks:
            if isinstance(block, dict):
                if block.get("title") == "Layer5GovernanceTelemetry":
                    l5_schema = block
                elif block.get("title") == "Layer7LegacyProxyTelemetry":
                    l7_schema = block

        if not l5_schema:
            print("ERROR: Layer5GovernanceTelemetry schema not found!")
            sys.exit(1)
        if not l7_schema:
            print("ERROR: Layer7LegacyProxyTelemetry schema not found!")
            sys.exit(1)

        # Validate that the schemas themselves are valid Draft 2020-12 schemas
        validator_cls = jsonschema.validators.validator_for(l5_schema)
        validator_cls.check_schema(l5_schema)
        print("  Layer5 schema is valid Draft 2020-12 schema.")

        validator_cls = jsonschema.validators.validator_for(l7_schema)
        validator_cls.check_schema(l7_schema)
        print("  Layer7 schema is valid Draft 2020-12 schema.")

        # Now test sample data against l7_schema
        # Find the GET /api/v1/l7/status response sample
        l7_sample = None
        for i, block in parsed_blocks:
            if isinstance(block, dict) and block.get("layer") == 7 and "daemon" in block:
                l7_sample = block
                break

        if l7_sample:
            print(f"  Validating GET /api/v1/l7/status sample against Layer7 schema...")
            # Note: The sample has timestamp_hlc? Wait, let's check!
            try:
                # Let's inspect fields in l7_sample vs l7_schema
                print(f"  l7_sample keys: {list(l7_sample.keys())}")
                # Notice in l7_schema, required is:
                # ["layer", "timestamp_hlc", "threat_level", "municipal_fsm", "grid_intertie", "carapace_finances", "severance_protocol", "records_firewall"]
                # In l7_sample (lines 583-618):
                # { "layer": 7, "daemon": "col-adversaryd", "vlan": 40, "threat_level": ..., "municipal_fsm": ..., "grid_intertie": ..., "carapace_finances": ..., "severance_protocol": ..., "records_firewall": ... }
                # Does l7_sample have "timestamp_hlc"?
                if "timestamp_hlc" not in l7_sample:
                    print("  ALERT: 'timestamp_hlc' is in l7_schema 'required' list, but MISSING from l7_sample in section 2.4 / 2.3!")
                
                # Check validation result
                v = jsonschema.Draft202012Validator(l7_schema)
                schema_errs = list(v.iter_errors(l7_sample))
                if schema_errs:
                    print(f"  Schema validation found {len(schema_errs)} mismatches on l7_sample:")
                    for err in schema_errs:
                        print(f"    - Field: {err.path} Error: {err.message}")
                else:
                    print("  l7_sample perfectly matches Layer7 schema!")
            except Exception as e:
                print(f"  Error validating l7_sample: {e}")

        # Let's construct a conforming L5 sample and validate it
        sample_l5 = {
            "layer": 5,
            "timestamp_hlc": "2026-10-12T16:15:00.000Z",
            "trust_ring_summary": {
                "local_did": "did:mesh:node01",
                "active_peers_count": 5,
                "ring_counts": {
                    "R0_HOME": 1,
                    "R1_NODE": 4
                }
            },
            "pending_approvals": [
                {
                    "yield_id": "y_8492_01",
                    "workflow_token_id": "tok_99182",
                    "gate_id": "Gate_Quality_Inspection",
                    "claimant_did": "did:mesh:node04:steward77",
                    "escrow_mj": 12.5,
                    "status": "PENDING_REVIEW"
                }
            ],
            "active_treaties": [
                {
                    "treaty_cid": "bafyreib249182",
                    "title": "Water Sharing Pact",
                    "cid_verified": True,
                    "bilateral_credit_limit_mj": 500.0,
                    "status": "ACTIVE"
                }
            ]
        }
        v5 = jsonschema.Draft202012Validator(l5_schema)
        v5_errs = list(v5.iter_errors(sample_l5))
        if v5_errs:
            print(f"  L5 test sample failed validation: {v5_errs}")
        else:
            print("  L5 schema correctly validates conforming Layer 5 telemetry payload.")

    except ImportError:
        print("jsonschema not installed, tested syntax only.")

if __name__ == "__main__":
    test_json_blocks()
