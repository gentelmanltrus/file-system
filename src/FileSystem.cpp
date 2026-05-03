#include <fstream>
#include <filesystem>
#include "include/FileSystem.h"

FileSystem::FileSystem()
{
  // must be defined
}

void FileSystem::mkdir(const std::string &name)
{
  if (!current)
    throw std::runtime_error("No current directory");
  if (current->contains(name))
        throw std::runtime_error("Directory or file already exists");
  std::shared_ptr<Directory> newDir = std::make_shared<Directory>(name);
  current->addItem(newDir);
        // must be defined
}

void FileSystem::touch(const std::string &name)
{
  // must be defined
}

void FileSystem::ls() const
{
  // must be defined
}
