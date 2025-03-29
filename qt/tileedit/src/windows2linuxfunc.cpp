#include <string>
#include <cstdlib>
#include <limits.h>
#include <unistd.h>
#include "tileedit.h"

std::string tileedit::getExecutablePath() {
    char path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (len != -1) {
        path[len] = '\0';
        return std::string(path);
    }
    return "";
}

void tileedit::splitpath(const std::string& path, std::string& drive, std::string& dir, std::string& name, std::string& ext) {
    size_t pos = path.find_last_of("/\\");
    if (pos != std::string::npos) {
        dir = path.substr(0, pos);
        name = path.substr(pos + 1);
    }
    size_t dotPos = name.find_last_of(".");
    if (dotPos != std::string::npos) {
        ext = name.substr(dotPos + 1);
        name = name.substr(0, dotPos);
    }
}
