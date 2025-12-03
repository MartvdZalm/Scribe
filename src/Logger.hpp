#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger
{
public:
    enum class Level
    {
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    static void initialize(const std::string& appName);
    static void log(Level level, const std::string& message,
                    const char* file = nullptr,
                    const char* function = nullptr,
                    int line = -1);

#define LOG_DEBUG(msg) Logger::log(Logger::Level::Debug, msg, __FILE__, __func__, __LINE__)
#define LOG_INFO(msg) Logger::log(Logger::Level::Info, msg, __FILE__, __func__, __LINE__)
#define LOG_WARNING(msg) Logger::log(Logger::Level::Warning, msg, __FILE__, __func__, __LINE__)
#define LOG_ERROR(msg) Logger::log(Logger::Level::Error, msg, __FILE__, __func__, __LINE__)
#define LOG_CRITICAL(msg) Logger::log(Logger::Level::Critical, msg, __FILE__, __func__, __LINE__)

private:
    static std::string logFilePath;
    static std::string formatMessage(Level level, const std::string& message,
                                     const char* file, const char* function, int line);

    static std::string currentDateTime();
    static std::string extractFilename(const std::string& path);
};

#endif
