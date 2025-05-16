#pragma once
#include "Literals.h"
#include <filesystem>
#include <map>

/**
 * @author Mar Werner Hernandez
 * 
 */

union JSONObject {
    s32 i;  // Integer interpretation of instance
    f64 d;  // Double interpretation of instance
    std::map<std::string, JSONObject>* json;
};

/**
 * @brief Interface to work with save files
 * 
 */
class SaveManager {
    static const inline std::filesystem::path MANUAL_SAVE_FILENAME = "Manual_Save.json";
    static const inline std::filesystem::path AUTO_SAVE_FILENAME = "Auto_Save.json";

    public:
    enum class SaveType : u8 { AUTO, MANUAL };

    /**
     * @brief Saves the current data present in PlayMainGameState
     * 
     * @param saveType 
     */
    static void WriteToFile(const SaveType saveType);

    /**
     * @brief Alters the data in PlayMainGameState according to the most recent save file available (if present)
     * 
     */
    static void ReadFromFile();
};