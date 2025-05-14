#include "Framework_Managers/SaveManager.h"
#include "Gamestate_Operators/Static/LevelManager.h"
#include "Gamestate_Operators/Static/ScoreManager.h"
#include "Gamestate_Operators/Static/PlayerManager.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

static JSONObject saveData = {};

static JSONObject ParseLiteral(const std::string& json, u32 start_idx, u32 end_idx);
static std::string ParseKey(const std::string& json, u32& data_idx);
static JSONObject ParseValue(const std::string& json, u32& data_idx);
static JSONObject ParseJsonObject(const std::string& json, u32& start_curly_brace_idx);

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

    fileStream << "\t\"Level\": " << std::to_string(LevelManager::GetCurrentLevel()) << ",\n";
    fileStream << "\t\"Score\": " << std::to_string(ScoreManager::GetScore()) << ",\n";

    fileStream << "\t\"Player\": {\n";

    fileStream << "\t\t\"Health\": " << std::to_string(PlayerManager::GetPlayer().getHealth()) << ",\n";
    fileStream << "\t\t\"Position\": {\n";
    fileStream << "\t\t\t\"x\": " << std::to_string(PlayerManager::GetPlayer().getPosition().x) << ",\n";
    fileStream << "\t\t\t\"y\": " << std::to_string(PlayerManager::GetPlayer().getPosition().y) << "\n";
    fileStream << "\t\t}\n";

    fileStream << "\t}\n";

    fileStream << "}";
}

static JSONObject ParseLiteral(const std::string& json, u32 start_idx, u32 end_idx) {
    std::string substring = json.substr(start_idx, end_idx - start_idx);
    std::cout << "Literal substring: " << substring << "\n";

    if(substring.find(".") != substring.npos) {
        // Data is a floating point number
        return {.d = std::stod(substring)};
    }
    else {
        // Data is an integer
        return {.i = std::stoi(substring)};
    }
}

static std::string ParseKey(const std::string& json, u32& data_idx) {
    // Get past any whitespace
    while(json[data_idx] == ' ' || json[data_idx] == '\n' || json[data_idx] == '\t')
        data_idx++;

    u32 keyStartIdx = 0;
    u32 keyEndIdx = 0;
    if(json[data_idx] == '\"') {
        // Reached start of named qualifier
        keyStartIdx = data_idx + 1;
        data_idx += 1;

        // Find last index of named qualifier
        while(json[data_idx] != '\"')
            data_idx++;

        keyEndIdx = data_idx;
    }

    // Have bounds for named qualifier, obtain its substring
    std::cout << "Key: " << json.substr(keyStartIdx, keyEndIdx - keyStartIdx) << "\n";
    return json.substr(keyStartIdx, keyEndIdx - keyStartIdx);
}

static JSONObject ParseValue(const std::string& json, u32& data_idx) {
    // Increase iterator until colon is reached
    while(json[data_idx] != ':')
        data_idx++;

    // Get past colon
    data_idx += 1;

    // Get past any whitespace
    while(json[data_idx] == ' ' || json[data_idx] == '\n' || json[data_idx] == '\t')
        data_idx++;

    JSONObject object;
    
    if(json[data_idx] == '{') {
        // Value is another JSON object
        object = ParseJsonObject(json, data_idx);
    }
    else {
        u32 startValueIdx = data_idx;
        u32 endValueIdx = startValueIdx;

        while(isdigit(json[data_idx]) || json[data_idx] == '.' || json[data_idx] == '-')
            data_idx++;

        endValueIdx = data_idx;

        object = ParseLiteral(json, startValueIdx, endValueIdx);
    }

    if(json[data_idx] == ',')
        data_idx++;

    return object;
}

static JSONObject ParseJsonObject(const std::string& json, u32& start_curly_brace_idx) {
    // Go past curly brace
    u32& dataIdx = start_curly_brace_idx;
    if(json[start_curly_brace_idx] == '{')
        dataIdx++;

    std::map<std::string, JSONObject>* map = new std::map<std::string, JSONObject>;

    std::string key;
    JSONObject value;

    do {
        // Parse data between curly braces, defining a single JSON object
        key = ParseKey(json, dataIdx);
        value = ParseValue(json, dataIdx);
        (*map)[key] = value;

        while(json[dataIdx] == ' ' || json[dataIdx] == '\n' || json[dataIdx] == '\t')
            dataIdx++;

    } while(json[dataIdx] != '}');

    value.json = map;
    return value; 
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

    std::string json;
    std::string currentLine;

    while(std::getline(fileStream, currentLine))
        json.append(currentLine);

    u32 firstCurlyIdx = 0;
    saveData = ParseJsonObject(json, firstCurlyIdx);

    JSONObject player = saveData.json->at("Player");
    PlayerManager::SetPlayerHealth(player.json->at("Health").i);

    ScoreManager::SetScore(saveData.json->at("Score").i);
    LevelManager::SetCurrentLevel(saveData.json->at("Level").i);
}