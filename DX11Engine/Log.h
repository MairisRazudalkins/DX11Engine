#pragma once

#ifndef LOGGER
#define LOGGER
#include "ImGuiManager.h"
#include "LogUi.h"

namespace Logger
{
    enum LogType
    {
        Info,
        Warning,
        Error,
        Fatal
    };

#define ENGINE_LOG(type, ...) LogError(type, __LINE__, __FILE__, __VA_ARGS__);

    inline const char* ShortFileName(const char* file)
    {
        const char* name = file;

        while (*file)
            if (*file++ == '\\')
                name = file;

        return name;
    }

    inline std::string LogFormat(LogType type, const char* time, int line, const char* fileName)
    {
        std::string logLevel = type == Info ? "[INFO]" : type == Warning ? "[WARNING]" : type == Error ? "[ERROR]" : "[FATAL]";
        return ("[" + std::string(time) + "][LOGGER]" + logLevel + "[LINE:" + std::to_string(line) + "][" + ShortFileName(fileName) + "] ");
    }

    inline std::string GetTime()
    {
        char outputChar[26];

        time_t result = time(NULL);
        ctime_s(outputChar, sizeof(outputChar), &result);

        return std::string(outputChar).substr(11, 8); // Get time only. Removing date.
    }

    template<typename... Args>
    void LogError(LogType type, int line, const char* fileName, Args&&... args)
    {
        std::stringstream outputSS;
        outputSS << LogFormat(type, GetTime().c_str(), line, fileName);

        using expander = int[];
        (void)expander { 0, (void(outputSS << args), 0)... }; // Replacement for fold expression (requires C++17)
        
        outputSS << std::endl;

		std::string str = outputSS.str().c_str();
        ImGuiManager::GetInst()->GetLogUI()->AddLog(str);

        OutputDebugStringA(outputSS.str().c_str());
    }
}

#endif
	