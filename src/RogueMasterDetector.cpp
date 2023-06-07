#include "RogueMasterDetector.h"

namespace RogueMasterDetector {
    void DetectRogueMasters() {
        logger::info("Detecting rogue masters...");
        auto rogue_masters_found = false;
        const auto handler = RE::TESDataHandler::GetSingleton();
        std::unordered_map<std::string_view, std::uint32_t> load_order;
        std::vector<std::string_view> rogue_masters;
        auto idx = 0;

        for (auto& mods = handler->files; const auto mod : mods)
            load_order.emplace(mod->GetFilename(), idx++);
        for (auto& mods = handler->files; const auto mod : mods) {
            if (mod->masterCount > 0) {
                for (const auto master_name : mod->masters) {
                    if (const auto master = handler->LookupLoadedModByName(master_name)) {
                        if (load_order.at(master->GetFilename()) > load_order.at(mod->GetFilename())) {
                            rogue_masters_found = true;
                            rogue_masters.emplace_back(master_name);
                            logger::info("ROGUE MASTER: {} is loaded after {}", master->GetFilename(),
                                         mod->GetFilename());
                        }
                    }
                }
            }
        }

        if (rogue_masters_found) RE::DebugNotification("WARNING: Rogue masters detected. Check RogueMasterDetector.log.");
        else logger::info("No rogue masters detected.");
    }
}