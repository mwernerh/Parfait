#include "Framework_Managers/SaveManager.h"

void SaveManager::WriteToFile(const SaveType saveType) {
    std::filesystem::path currentPath = std::filesystem::current_path();

    switch(saveType) {
        case SaveType::AUTO:
            currentPath = currentPath / AUTO_SAVE_FILENAME;
            break;
        case SaveType::MANUAL:
            currentPath = currentPath / MANUAL_SAVE_FILENAME;
            break;
    }
}