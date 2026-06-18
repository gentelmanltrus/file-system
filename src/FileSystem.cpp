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

// long path, non relative
void FileSystem::mkdir(const std::string &name)
{
    std::filesystem::path target(name);
    std::filesystem::path full;
    //if relative just get it
    //if non relative + it to full

    if (target.is_absolute()) 
    {
        full = target;
    } 
    else {
        full = currentPhysical / target;
    }

    // create_directories
    std::filesystem::create_directories(full);
}

void FileSystem::touch(const std::string &name)
{
    std::filesystem::path target(name);
    std::filesystem::path full;

    if (target.is_absolute()) 
    {
        full = target;
    } else {
        full = currentPhysical / target;
    }

    if (!std::filesystem::exists(full.parent_path())) 
    {
        throw std::runtime_error("touch: parent directory does not exist");
    }

    if (std::filesystem::exists(full)) 
    {
        throw std::runtime_error("touch: file already exists");
    }

    std::ofstream file(full);
    if (!file) {
        throw std::runtime_error("touch: failed to create file");
    }
}

void FileSystem::ls(const std::string &name) const
{
  std::filesystem::path target(name);
  std::filesystem::path full;

  if (target.is_absolute()) 
  {
    full = target;
  } 
  else 
  {
    full = currentPhysical / target;
  }

  for (const auto &entry : std::filesystem::directory_iterator(full))
  {
    std::cout << entry.path().filename().string() << std::endl;
  }
}

void FileSystem::cd(const std::string &name)
{
    std::filesystem::path target(name);
    std::filesystem::path full;
    // CHECK IF DIR OR FILE IS REAL
    if (target.is_absolute()) 
    {
        full = target;
    } 
    else 
    {
        full = currentPhysical / target;
    }

    if (!std::filesystem::exists(full))
        throw std::runtime_error("cd: path does not exist");

    if (!std::filesystem::is_directory(full))
        throw std::runtime_error("cd: not a directory");

    currentPhysical = std::filesystem::weakly_canonical(full);
}

void FileSystem::remove(const std::string &name)
{
    std::filesystem::path target(name);
    std::filesystem::path full;
    if (target.is_absolute()) 
    {
        full = target;
    } 
    else 
    {
        full = currentPhysical / target;
    }
    
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
    std::cout << "touch <path/file-name> - creates an empty file at the specified path." << std::endl;
    std::cout << "ls <path> - lists all items in the directory at the given path." << std::endl;
    std::cout << "ls - lists all items in the current working directory." << std::endl;
    std::cout << "mkdir <directory-name> - creates a new directory at the specified path." << std::endl;
    std::cout << "create <file-system-name> [absolute-path] - creates a new virtual file system with the specified name. Optionally imports files from the given path." << std::endl;
    std::cout << "import <absolute-path> - imports files from the specified path into the current virtual file system." << std::endl;
    std::cout << "switch <file-system-name> - switches to the virtual file system with the specified name." << std::endl;
    std::cout << "delete <file-system-name> - deletes the specified file or directory." << std::endl;
    std::cout << "cd <path> - changes the current working directory." << std::endl;
    std::cout << "pwd - prints the current working directory to the terminal." << std::endl;
    std::cout << "rm <item-name> - removes the specified file or directory." << std::endl;
    std::cout << "rm -f <item-name> - forces removal of the specified file or directory." << std::endl;
    std::cout << "rm -s <item-name> - removes the specified file or directory from both the physical and virtual file systems." << std::endl;
    std::cout << "alias <command> <command-alias> - Creates an alias for the specified command." << std::endl;
    std::cout << "alias - lists all created aliases." << std::endl;
    std::cout << "unalias <command-alias> - removes the specified alias." << std::endl;
    std::cout << "tree - displays the directory structure in a tree format." << std::endl;
    std::cout << "report - generates a report of the file system." << std::endl;
    std::cout << "duplicates - lists duplicate files in the file system." << std::endl;
    std::cout << "quit - exits the file system." << std::endl;
    std::cout << "help - lists all available commands with their descriptions." << std::endl;
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
            std::cout << "  Biggest file: " << info.largestFilePath.string() 
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
