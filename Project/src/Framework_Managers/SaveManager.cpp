#include "Framework_Managers/SaveManager.h"
#include "Gamestate_Operators/Static/LevelManager.h"
#include "Gamestate_Operators/Static/PlayerManager.h"
#include <filesystem>
#include <fstream>
#include <iostream>

void SaveManager::WriteToFile(const SaveType saveType) {
    std::filesystem::path savePath = std::filesystem::current_path();

    switch(saveType) {
        case SaveType::AUTO:
            savePath = savePath / AUTO_SAVE_FILENAME;
            break;
        case SaveType::MANUAL:
            savePath = savePath / MANUAL_SAVE_FILENAME;
            break;
    }

    // Delete the old save file
    if(std::filesystem::exists(savePath)) {
        std::filesystem::remove(savePath);
    }

    std::ofstream fileStream(savePath, std::ios_base::out);

    if(!fileStream.is_open()) {
        std::cerr << "\x1b[31mError! Could not open save file " << savePath.relative_path().string() << " for writing!\x1b[0m\n";
        return;
    }

    fileStream << "{\n";

    fileStream << "\t\"Player\": {\n";

    fileStream << "\t\t\"Health\": " << std::to_string(PlayerManager::GetPlayer().getHealth()) << ",\n";
    fileStream << "\t\t\"Position\": [";
    fileStream << std::to_string(PlayerManager::GetPlayer().getPosition().x) << ", ";
    fileStream << std::to_string(PlayerManager::GetPlayer().getPosition().y);
    fileStream << "]\n";

    fileStream << "\t}, \n";

    fileStream << "\"Level\": " << std::to_string(GetCurrentLevel()) << ",\n";
    fileStream << "\"Score\": " << std::to_string(GetScore()) << ",\n";

    fileStream << "}";
}

void SaveManager::ReadFromFile() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path autoSavePath = currentPath / AUTO_SAVE_FILENAME;
    std::filesystem::path manualSavePath = currentPath / MANUAL_SAVE_FILENAME;

    const bool autoSaveExists = std::filesystem::exists(autoSavePath);
    const bool manualSaveExists = std::filesystem::exists(manualSavePath);

    if(!autoSaveExists && !manualSaveExists) {
        // Savefile doesn't exist, default game start
        return;
    }

    std::ifstream fileStream;

    if(autoSaveExists && !manualSaveExists) {
        fileStream.open(autoSavePath);
    }
    else if(!autoSaveExists && manualSaveExists) {
        fileStream.open(manualSavePath);
    }
    else {
        if(std::filesystem::last_write_time(autoSavePath) > std::filesystem::last_write_time(manualSavePath))
            fileStream.open(autoSavePath);
        else
            fileStream.open(manualSavePath);
    }

    s32 objectLevel = -1;
    char currentChar;

    while(fileStream.get(currentChar).eof()) {
        switch(currentChar) {
            case '{':
                objectLevel += 1;
                break;
            case '}':
                objectLevel -= 1;
                break;
        }

        //JSONObject
//
        //if(objectLevel == 0)
    }
}