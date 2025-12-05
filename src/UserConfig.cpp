#include "UserConfig.hpp"
#include "Logger.hpp"

UserConfig* UserConfig::instance = nullptr;

UserConfig::UserConfig(const std::string& appName)
{
    const char* xdg = std::getenv("XDG_CONFIG_HOME");
    if (xdg)
    {
        configDir = std::string(xdg) + "/" + appName + "/";
    }
    else
    {
        const char* home = std::getenv("HOME");
        if (home)
            configDir = std::string(home) + "/.config/" + appName + "/";
        else
            configDir = "./" + appName + "/";
    }

    try
    {
        fs::create_directories(configDir + "syntax");
        fs::create_directories(configDir + "themes");
        LOG_INFO("Created config directories at " + configDir);
    }
    catch (const fs::filesystem_error& e)
    {
        LOG_ERROR("Failed to create config directories: " + std::string(e.what()));
    }

    if (!fs::exists(configDir + "preferences.json"))
    {
        copyFile("resources/preferences.json", configDir + "preferences.json");
    }
}

std::string UserConfig::getConfigDir() const
{
    return configDir;
}

std::ifstream UserConfig::openFile(const std::string& relativePath) const
{
    std::string userPath = configDir + relativePath;
    if (fs::exists(userPath))
        return std::ifstream(userPath);

    std::string defaultPath = "resources/" + relativePath;
    if (fs::exists(defaultPath))
        return std::ifstream(defaultPath);

    LOG_WARNING("File not found: " + relativePath);
    return std::ifstream();
}

fs::path UserConfig::getPath(const std::string& relativePath) const
{
    std::string userPath = configDir + relativePath;
    if (fs::exists(userPath))
        return userPath;

    LOG_DEBUG("Using default resource path for " + userPath);
    return userPath;
}

void UserConfig::copyFile(const std::string& src, const std::string& dst) const
{
    try
    {
        fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
        LOG_INFO("Copied file from " + src + " to " + dst);
    }
    catch (const fs::filesystem_error& e)
    {
        LOG_ERROR("Failed to copy " + src + " to " + dst + ": " + e.what());
    }
}
