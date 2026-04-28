#include "include/FileSystem.h"
#include "include/File.h"
#include "include/Directory.h"

FileSystem::FileSystem()
{
  // must be defined
}

void FileSystem::mkdir(const std::string &name)
{
  // must be defined
}

void FileSystem::touch(const std::string &name)
{
  if (!current)
    throw std::runtime_error("No current directory");

  if (current->contains(name))
    throw std::runtime_error("File already exists");
  
  std::shared_ptr<File> file = std::make_shared<File>(name);
  current->addItem(file);
}

void FileSystem::ls() const
{
  // must be defined
}