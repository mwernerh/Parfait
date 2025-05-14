#pragma once
#include "Literals.h"
#include <filesystem>
#include <map>

union JSONObject {
    s32 i;
    f64 d; 
    std::map<std::string, JSONObject>* json;
};

class SaveManager {
    static const inline std::filesystem::path MANUAL_SAVE_FILENAME = "Manual_Save.json";
    static const inline std::filesystem::path AUTO_SAVE_FILENAME = "Auto_Save.json";

    public:
    enum class SaveType : u8 { AUTO, MANUAL };

    static void WriteToFile(const SaveType saveType);
    static void ReadFromFile();
};