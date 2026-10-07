#pragma once
#include "blueprint_manager.hpp"
#include <string>

namespace oasis {

// Story 6.4: The Real Intent Compiler
// Exports successful in-game macro-infrastructure Blueprints to Layer 4 BPMN XML
class BlueprintExporter {
public:
    static std::string ExportToBPMN(const BlueprintManager& blueprints);
};

} // namespace oasis
