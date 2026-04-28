#include <fstream>
#include <filesystem>
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

  if (std::filesystem::exists(name))
    throw std::runtime_error("File already exists");
  
  std::shared_ptr<File> file_ptr = std::make_shared<File>(name);
  current->addItem(file_ptr);
  
  // might need to be created in a separate real directory, if created in virtual directory
  std::ofstream file(name);
  
  if (!file)
    throw std::runtime_error("Failed to create file");
  file.close();
}

void FileSystem::ls() const
{
  // must be defined
}