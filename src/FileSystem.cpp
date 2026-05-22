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
    std::cout << entry.path().string() << "\n";
  }
}

void FileSystem::cd(const std::string &name)
{
    std::filesystem::path target = currentPhysical / name;

    if (std::filesystem::is_directory(target))
    {
        currentPhysical = std::filesystem::canonical(target);
    }
}

void FileSystem::quit()
{
  std::cout << "Exiting..." << std::endl;
  exit(0);
}
void FileSystem::help() const
{
    std::cout << "Available commands:\n";
    std::cout << "touch <filename>\n";
    std::cout << "ls\n";
    std::cout << "mkdir <directory>\n";
    std::cout << "cd\n";
    std::cout << "pwd\n";
    std::cout << "quit\n";
    std::cout << "help\n";
}
void FileSystem::pwd() const
{
    std::cout << currentPhysical  << std::endl;
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