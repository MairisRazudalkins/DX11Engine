#pragma once

#ifndef LOGGER
#define LOGGER

namespace Logger
{
    enum LogType
    {
        Info,
        Warning,
        Error,
        Fatal
    };

#define ENGINE_LOG(type, ...) LogFatalError(type, __TIME__, __LINE__, __FILE__, __VA_ARGS__);

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
        std::string logLevel = type == Info ? "[INFO]" : type == Warning ? "[WARNING]" : type == Error ? "[ERROR]" : "[FATAl]";
        return ("[LOGGER]" + logLevel + "[" + std::string(time) + "][LINE:" + std::to_string(line) + "][" + ShortFileName(fileName) + "] ");
    }

    template<typename... Args>
    void LogFatalError(LogType type, const char* time, int line, const char* fileName, Args&&... args)
    {
        std::stringstream outputSS;
        outputSS << LogFormat(type, time, line, fileName);

        using expander = int[];
        (void)expander { 0, (void(outputSS << args), 0)... };

        outputSS << std::endl;
        OutputDebugStringA(outputSS.str().c_str());
    }
}

#endif
	