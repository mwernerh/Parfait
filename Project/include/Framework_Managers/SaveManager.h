#pragma once
#include "Literals.h"
#include <filesystem>
#include <map>

class SaveManager {
    static const inline std::filesystem::path MANUAL_SAVE_FILENAME = "Manual_Save.json";
    static const inline std::filesystem::path AUTO_SAVE_FILENAME = "Auto_Save.json";

    static struct JSONObject {
        std::map<std::string, std::variant<JSONObject, std::string>> entries;
    } saveData;

    public:
    enum class SaveType : u8 { AUTO, MANUAL };

    static void WriteToFile(const SaveType saveType);
    static void ReadFromFile();
    
    static const u32& GetCurrentLevel(void);
    static void SetCurrentLevel(const u32 currentLevel);

    static const u32& GetScore(void);
    static void SetScore(const u32 score);

    static const u32& GetPlayerHealth(void);
    static void SetPlayerHealth(const u32 currentLevel);
};