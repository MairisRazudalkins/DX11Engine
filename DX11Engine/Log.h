#pragma once

#ifndef LOGGER
#define LOGGER

namespace Logger
{
#define LOG_FATAL_ERROR(...) LogFatalError(__TIME__, __LINE__, __FILE__, __VA_ARGS__);

	enum LogType
	{
		Warning,
        Error, 
        Fatal,
	};

    inline const char* ShortFileName(const char* file)
    {
        const char* name = file;

        while (*file)
            if (*file++ == '\\')
                name = file;

        return name;
    }

    inline std::string LogFormat(const char* time, int line, const char* fileName)
    {
        return ("[LOGGER][FATAL][" + std::string(time) + "][LINE:" + std::to_string(line) + "][" + ShortFileName(fileName) + "] ");
    }

    template <typename Arg1>
    std::string function(Arg1&& arg1)
    {
        return arg1;
    }

    template<typename... Args>
    void LogFatalError(const char* time, int line, const char* fileName, Args&&... args)
    {
        std::stringstream outputSS;
        outputSS << LogFormat(time, line, fileName);

        using expander = int[];
        (void)expander { 0, (void(outputSS << args), 0)... };

        outputSS << std::endl;
        OutputDebugStringA(outputSS.str().c_str());
    }
}

#endif
	