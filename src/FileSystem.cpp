#include <fstream>
#include "include/FileSystem.h"

FileSystem::FileSystem()
{
  // must be defined
}

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
  for (const auto &entry : std::filesystem::recursive_directory_iterator(currentPhysical))
  {
    std::cout << entry.path().string() << "\n";
  }
}

void FileSystem::cd(const std::string &name)
{
    std::filesystem::path target = currentPhysical / name;

    if (std::filesystem::exists(target) &&
        std::filesystem::is_directory(target))
    {
        currentPhysical = std::filesystem::canonical(target);
    }
}