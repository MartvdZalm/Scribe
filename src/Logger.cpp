#include "Logger.hpp"

#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <filesystem>

std::string Logger::logFilePath;

std::string Logger::currentDateTime()
{
    using namespace std::chrono;

    auto now = system_clock::now();
    auto time = system_clock::to_time_t(now);
    auto ms   = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
        << '.' << std::setw(3) << std::setfill('0') << ms.count();
    return oss.str();
}

std::string Logger::extractFilename(const std::string& path)
{
    return std::filesystem::path(path).filename().string();
}

void Logger::initialize(const std::string& appName)
{
    std::filesystem::path logDir;
#if defined(_WIN32)
    logDir = std::filesystem::path(std::getenv("APPDATA"));
#else
    logDir = std::filesystem::path(std::getenv("HOME")) / ".local/share";
#endif

    logDir /= appName;
    std::filesystem::create_directories(logDir);

    logFilePath = (logDir / (appName + ".log")).string();

    std::ofstream file(logFilePath, std::ios::app);
    if (file.is_open())
    {
        file << "\n\n=== Logging started at " << currentDateTime() << " ===\n";
    }
}

void Logger::log(Level level, const std::string& message,
                 const char* file, const char* function, int line)
{
    std::ofstream out(logFilePath, std::ios::app);
    if (!out.is_open()) return;

    out << formatMessage(level, message, file, function, line) << "\n";
}

std::string Logger::formatMessage(Level level, const std::string& message,
                                  const char* file, const char* function, int line)
{
    std::string levelStr;
    switch (level)
    {
        case Level::Debug:    levelStr = "DEBUG"; break;
        case Level::Info:     levelStr = "INFO"; break;
        case Level::Warning:  levelStr = "WARN"; break;
        case Level::Error:    levelStr = "ERROR"; break;
        case Level::Critical: levelStr = "CRIT"; break;
    }

    std::string shortFile = file ? extractFilename(file) : "unknown";

    std::ostringstream oss;
    oss << "[" << currentDateTime()
        << "] [" << levelStr
        << "] [" << shortFile << ":" << line << "@" << (function ? function : "unknown")
        << "] " << message;

    return oss.str();
}
