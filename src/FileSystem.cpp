#include <fstream>
#include "../include/FileSystem.h"
#include <cstdlib>
#include <filesystem>
FileSystem::FileSystem()
{
  currentPhysical = getHomeDirectory();
  help();
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
    std::cout << entry.path().string() << std::endl;
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

void FileSystem::quit()
{
  std::cout << "Exiting..." << std::endl;
  exit(0);
}

void FileSystem::remove(const std::string &name)
{
    std::filesystem::path full = currentPhysical / name;
    if (!std::filesystem::exists(full))
    {
        std::cout << "rm: \"" << name << "\": no such file or directory" << std::endl;
        return;
    }
    std::filesystem::remove_all(full);
    std::cout << "\"" << name << "\" removed" << std::endl;
}

void FileSystem::help() const
{
    std::cout << "Available commands:" << std::endl;
    std::cout << "touch <filename>" << std::endl;
    std::cout << "ls" << std::endl;
    std::cout << "mkdir <directory>" << std::endl;
    std::cout << "cd" << std::endl;
    std::cout << "pwd" << std::endl;
    std::cout << "rm <name>" << std::endl;
    std::cout << "rm -f <name>" << std::endl;
    std::cout << "quit" << std::endl;
    std::cout << "help" << std::endl;
}

void FileSystem::pwd() const
{
    std::cout << currentPhysical  << std::endl;
}
void FileSystem::tree() const
{
    if (!std::filesystem::exists(currentPhysical) || !std::filesystem::is_directory(currentPhysical)) {
        std::cout << "Klaida: Kelias neegzistuoja\n";
        return;
    }

    std::cout << currentPhysical.filename().string() << "\n";
    
    for (const auto& entry : std::filesystem::recursive_directory_iterator(currentPhysical)) {
        auto depth = entry.depth();
        
      
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

    //hash file papildpma f-ja
    auto calculateHash = [](const std::filesystem::path& filePath) -> std::size_t {
        std::ifstream file(filePath, std::ios::binary);
        if (!file) return 0;
        std::size_t hash = 0;
        char buffer[1024];
        while (file.read(buffer, sizeof(buffer))) {
            for (std::streamsize i = 0; i < file.gcount(); ++i) {
                hash = hash * 31 + buffer[i];
            }
        }
        return hash + std::filesystem::file_size(filePath);
    };

    std::map<std::size_t, std::vector<std::filesystem::path>> hashGroups;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(currentPhysical)) {
        if (entry.is_regular_file()) {
            std::size_t fileHash = calculateHash(entry.path());
            hashGroups[fileHash].push_back(entry.path());
        }
    }

    std::cout << "\n------ DUPLIKATU PAIESKA ------\n";
    bool found = false;

    for (const auto& [hash, files] : hashGroups) {
        if (files.size() > 1) {
            found = true;
            std::cout << "Rasti duplikatai (Viso: " << files.size() << " failai):\n";
            for (const auto& path : files) {
                std::cout << "  -> " << std::filesystem::relative(path, currentPhysical).string() << "\n";
            }
        }
    }

    if (!found) {
        std::cout << "Vienodu failu nerasta.\n";
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

    std::cout << "\n FAILU ATASKAITA \n";
    for (const auto& [ext, info] : reportMap) {
        std::cout << "Tipas: " << ext << " | Kiekis: " << info.count << "\n";
        if (info.count > 0) {
            std::cout << "  Didziausias failas: " << info.largestFilePath.filename().string() 
                      << " (" << info.maxSize << " bytes)\n";
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
