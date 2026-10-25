#include "filesystem.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <optional>

#ifdef __linux__
#include <unistd.h>
#include <limits.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#elif _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;

namespace {
    std::optional<fs::path> executable_path() {
#if defined(PLATFORM_WEB)
        return "/";
#elif __linux__
        char buffer[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len != -1) {
            buffer[len] = '\0';
            auto path = fs::path{buffer};
            return path;
        }
#elif __APPLE__
        char buffer[PATH_MAX];
        uint32_t size = sizeof(buffer);
        if (_NSGetExecutablePath(buffer, &size) == 0) {
            auto path = fs::path{buffer};
            return path;
        }
#elif _WIN32
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        auto path = fs::path{buffer};
        return path.string();
#endif
        return std::nullopt;
    }
}

std::string path_join(const std::string& a, const std::string& b) {
    return (fs::path(a) / fs::path(b)).string();
}

std::string path_to_executable_dir() {
    return executable_path()->parent_path().string();
}
