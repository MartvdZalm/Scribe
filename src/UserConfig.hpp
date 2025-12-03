#ifndef USERCONFIG_HPP
#define USERCONFIG_HPP

#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>

namespace fs = std::filesystem;

class UserConfig {
public:
    static void initialize(const std::string &appName = "Scribe") {
        if (!instance) {
            instance = new UserConfig(appName);
        }
    }

    static UserConfig& get() {
        if (!instance) {
            std::cerr << "Error: UserConfig not initialized!" << std::endl;
            std::exit(1);
        }
        return *instance;
    }

    std::string getConfigDir() const;
    std::ifstream openFile(const std::string &relativePath) const;
    fs::path getPath(const std::string &relativePath) const;

private:
    UserConfig(const std::string &appName);

    std::string configDir;
    void copyFile(const std::string &src, const std::string &dst) const;

    static UserConfig* instance;
};

#endif // USERCONFIG_HPP
