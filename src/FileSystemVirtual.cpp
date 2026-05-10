#include <fstream>
#include <filesystem>
#include "include/FileSystemVirtual.h"
#include "include/File.h"
#include "include/Directory.h"

FileSystemVirtual::FileSystemVirtual()
{
  // must be defined
}

void FileSystemVirtual::mkdir(const std::string &name)
{ 
  FileSystem::mkdir(name);
  // might need some path/additional folder to the name
   if (!current)
    throw std::runtime_error("No current directory");
  if (current->contains(name))
        throw std::runtime_error("Directory or file already exists");
  std::shared_ptr<Directory> newDir = std::make_shared<Directory>(name);
  current->addItem(newDir);

  // must be defined
}

void FileSystemVirtual::touch(const std::string &name)
{
  // might need some path/additional folder to the nmae
  FileSystem::touch(name);
  
  if (!current)
    throw std::runtime_error("No current directory");

  if (current->contains(name))
    throw std::runtime_error("File already exists");

  std::shared_ptr<File> file = std::make_shared<File>(name);
  current->addItem(file);
}

void FileSystemVirtual::ls() const
{
  if (!current)
    throw std::runtime_error("No current directory");

  current->listItems();
}
