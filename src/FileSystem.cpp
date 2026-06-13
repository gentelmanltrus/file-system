#include <fstream>
#include "../include/FileSystem.h"
#include <filesystem>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <cstdint>

FileSystem::FileSystem()
{
  currentPhysical = getHomeDirectory();
}

FileSystem::FileSystem(const std::filesystem::path &path)
{
  currentPhysical = path;
}

FileSystemItem::~FileSystemItem() {}

void FileSystem::mkdir(const std::string &name)
{
  std::filesystem::path full = currentPhysical / name;
  std::filesystem::create_directories(full);
}

void FileSystem::touch(const std::string &name)
{
  std::filesystem::path full = currentPhysical / name;
  std::ofstream file(full);
}

void FileSystem::ls() const
{
  for (const auto &entry : std::filesystem::directory_iterator(currentPhysical))
  {
    std::cout << entry.path().filename().string() << std::endl;
  }
}

void FileSystem::cd(const std::string &name)
{
    std::filesystem::path target = currentPhysical / name;

    if (std::filesystem::is_directory(target))
    {
        currentPhysical = std::filesystem::canonical(target);
    }
    else
    {
      std::cout << "Directory does not exist" << std::endl;
    }
}

void FileSystem::remove(const std::string &name)
{
    std::filesystem::path full = name;
    if (!std::filesystem::exists(full))
    {
        throw std::runtime_error("rm: \"" + name + "\": no such file or directory");
    }
    std::filesystem::remove_all(full);
    std::cout << "\"" << full.filename().string() << "\" removed" << std::endl;
}

void FileSystem::help() const
{
    std::cout << "Available commands:" << std::endl;
    std::cout << "touch <filename>" << std::endl;
    std::cout << "ls" << std::endl;
    std::cout << "mkdir <directory>" << std::endl;
    std::cout << "create <name> [import_path]" << std::endl;
    std::cout << "import <path>" << std::endl;
    std::cout << "switch <name>" << std::endl;
    std::cout << "cd" << std::endl;
    std::cout << "pwd" << std::endl;
    std::cout << "rm <name>" << std::endl;
    std::cout << "rm -f <name>" << std::endl;
    std::cout << "rm -s <name>" << std::endl;
    std::cout << "alias <command> <alias>" << std::endl;
    std::cout << "unalias <alias>" << std::endl;
    std::cout << "tree" << std::endl;
    std::cout << "report" << std::endl;
    std::cout << "duplicates" << std::endl;
    std::cout << "quit" << std::endl;
    std::cout << "help" << std::endl;
}

void FileSystem::pwd() const
{
    std::cout << currentPhysical.string();
}
void FileSystem::tree() const
{
    if (!std::filesystem::exists(currentPhysical) || !std::filesystem::is_directory(currentPhysical)) {
        std::cout << "Error: path is not found\n";
        return;
    }

    std::cout << currentPhysical.filename().u8string() << "\n";

    for (auto it = std::filesystem::recursive_directory_iterator(currentPhysical); it != std::filesystem::recursive_directory_iterator(); ++it) {
        auto depth = it.depth();
        const auto &entry = *it;

        for (int i = 0; i < depth; ++i) {
            std::cout << "  ";
        }

        if (entry.is_directory()) {
            std::cout << "└── [" << entry.path().filename().string() << "]\n";
        } else {
            std::cout << "└── " << entry.path().filename().string() << "\n";
        }
    }
}
void FileSystem::duplicates() const
{
    if (!std::filesystem::exists(currentPhysical)) return;

    namespace fs = std::filesystem;

    auto fileSizeOf = [](const fs::path& p) -> std::uintmax_t {
        std::error_code ec;
        auto size = fs::file_size(p, ec);
        return ec ? static_cast<std::uintmax_t>(-1) : size;
    };

    auto calculateHash = [](const fs::path& filePath) -> std::size_t {
        std::ifstream file(filePath, std::ios::binary);
        if (!file) return 0;

        std::size_t hash = 1469598103934665603ULL; // FNV offset basis
        const std::size_t prime = 1099511628211ULL;
        char buffer[4096];

        while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
            for (std::streamsize i = 0; i < file.gcount(); ++i) {
                hash ^= static_cast<unsigned char>(buffer[i]);
                hash *= prime;
            }
        }
        return hash;
    };

    std::map<std::uintmax_t, std::vector<fs::path>> sizeGroups;

    for (const auto& entry : fs::recursive_directory_iterator(currentPhysical)) {
        if (entry.is_regular_file()) {
            auto size = fileSizeOf(entry.path());
            if (size != static_cast<std::uintmax_t>(-1)) {
                sizeGroups[size].push_back(entry.path());
            }
        }
    }

    std::cout << "\n------ Duplicate Search ------\n";
    bool found = false;

    for (const auto& [size, files] : sizeGroups) {
        if (files.size() < 2) continue;

        std::unordered_map<std::size_t, std::vector<fs::path>> hashGroups;

        for (const auto& path : files) {
            hashGroups[calculateHash(path)].push_back(path);
        }

        for (const auto& [hash, dupFiles] : hashGroups) {
            if (dupFiles.size() > 1) {
                found = true;
                std::cout << "Duplicates found (size: " << size << ", count: " << dupFiles.size() << "):\n";
                for (const auto& path : dupFiles) {
                    std::cout << "  -> " << fs::relative(path, currentPhysical).string() << "\n";
                }
                std::cout << std::endl;
            }
        }
    }

    if (!found) {
        std::cout << "Duplicates were not found.\n";
    }
}
void FileSystem::report() const
{
    if (!std::filesystem::exists(currentPhysical)) return;

    struct FileInfo {
        int count = 0;
        std::uintmax_t maxSize = 0;
        std::filesystem::path largestFilePath;
    };

    std::map<std::string, FileInfo> reportMap;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(currentPhysical)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext.empty()) ext = "(No extension)";

            auto size = entry.file_size();
            auto& info = reportMap[ext];
            
            info.count++;
            if (size > info.maxSize) {
                info.maxSize = size;
                info.largestFilePath = entry.path();
            }
        }
    }

    std::cout << "\n Files \n";
    for (const auto& [ext, info] : reportMap) {
        std::cout << "Type: " << ext << " Amount: " << info.count << "\n";
        if (info.count > 0) {
            std::cout << "  Biggest file: " << info.largestFilePath.filename().string() 
                      << " (" << info.maxSize << " bytes)\n\n";
        }
    }
}

std::filesystem::path FileSystem::getHomeDirectory()
{
#ifdef _WIN32
    const char* home = std::getenv("USERPROFILE");
#else
    const char* home = std::getenv("HOME");
#endif

    if (home)
    {
        return std::filesystem::path(home);
    }

    return std::filesystem::current_path();
}

std::filesystem::path FileSystem::getCurrentPhysical() const 
{ 
    return currentPhysical; 
}
