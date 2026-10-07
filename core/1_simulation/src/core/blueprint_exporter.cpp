#include "blueprint_exporter.hpp"
#include <pugixml.hpp>
#include <sstream>

namespace oasis {

std::string BlueprintExporter::ExportToBPMN(const BlueprintManager& blueprints) {
    pugi::xml_document doc;

    // Standard BPMN 2.0 shell
    pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
    decl.append_attribute("version") = "1.0";
    decl.append_attribute("encoding") = "UTF-8";

    pugi::xml_node defs = doc.append_child("bpmn2:definitions");
    defs.append_attribute("xmlns:bpmn2") = "http://www.omg.org/spec/BPMN/20100524/MODEL";
    defs.append_attribute("id") = "Oasis_Real_Intent_Export";

    pugi::xml_node process = defs.append_child("bpmn2:process");
    process.append_attribute("id") = "Process_Oasis_Infrastructure";
    process.append_attribute("isExecutable") = "true";

    int task_counter = 1;
    for (const auto& zone : blueprints.GetZones()) {
        pugi::xml_node task = process.append_child("bpmn2:task");
        
        std::string zone_name;
        switch (zone.type) {
            case ZoneType::SERVICE_KIOSK: zone_name = "LAYER7_SERVICE_KIOSK"; break;
            case ZoneType::SOLAR_MESH: zone_name = "SOLAR_EXERGY_ROUTING"; break;
            case ZoneType::GRAYWATER_ROUTING: zone_name = "GRAYWATER_RECLAMATION"; break;
        }
        
        std::string task_id = zone_name + "_" + std::to_string(task_counter++);
        task.append_attribute("id") = task_id.c_str();
        task.append_attribute("name") = zone_name.c_str();
        
        // Embed the spatial coordinates as BPMN extension properties or custom attributes
        task.append_attribute("oasis:x") = zone.x;
        task.append_attribute("oasis:y") = zone.y;
        task.append_attribute("oasis:z") = zone.z;
    }

    std::stringstream ss;
    doc.save(ss, "  ");
    return ss.str();
}

} // namespace oasis
