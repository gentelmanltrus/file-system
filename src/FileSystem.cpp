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
    std::cout << "import <path>" << std::endl;
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
