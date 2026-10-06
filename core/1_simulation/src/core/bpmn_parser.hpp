#pragma once
#include <string>
#include <pugixml.hpp>
#include "interfaces.hpp"

namespace oasis {

// Story 2.3 & 2.4: XML parser for BPMN intents
class BPMNParser {
public:
    BPMNParser(IActuator* actuator);
    
    // Parse an XML string and actuate the tasks
    bool ParseAndExecute(const std::string& xml_payload);

private:
    IActuator* actuator_;
};

} // namespace oasis
