#include "bpmn_parser.hpp"
#include <iostream>

namespace oasis {

BPMNParser::BPMNParser(IActuator* actuator) : actuator_(actuator) {}

bool BPMNParser::ParseAndExecute(const std::string& xml_payload) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_payload.c_str());

    if (!result) {
        std::cerr << "BPMN XML parsed with errors: " << result.description() << "\n";
        return false;
    }

    pugi::xml_node defs = doc.child("definitions");
    if (!defs) {
        std::cerr << "Invalid BPMN: missing <definitions> root\n";
        return false;
    }

    for (pugi::xml_node task = defs.child("bpmn2:task"); task; task = task.next_sibling("bpmn2:task")) {
        std::string task_id = task.attribute("id").value();
        float value = task.attribute("value").as_float(0.0f);
        
        if (actuator_) {
            actuator_->Actuate(task_id, value);
        }
    }

    return true;
}

} // namespace oasis
