#include <fstream>
#include <filesystem>
#include "../include/FileSystemVirtual.h"
#include "../include/File.h"
#include "../include/Directory.h"

FileSystemVirtual::FileSystemVirtual()
{
  // must be defined
}

void FileSystemVirtual::mkdir(const std::string &name)
{
  FileSystem::mkdir(name);
  // might need some path/additional folder to the name
  if (!currentPathVirtual)
    throw std::runtime_error("No current directory");
  if (currentPathVirtual->contains(name))
    throw std::runtime_error("Directory or file already exists");
  std::shared_ptr<Directory> newDir = std::make_shared<Directory>(name);
  currentPathVirtual->addItem(newDir);

  // must be defined
}

void FileSystemVirtual::touch(const std::string &name)
{
  // might need some path/additional folder to the nmae
  FileSystem::touch(name);

  if (!currentPathVirtual)
    throw std::runtime_error("No current directory");

  if (currentPathVirtual->contains(name))
    throw std::runtime_error("File already exists");

  std::shared_ptr<File> file = std::make_shared<File>(name);
  currentPathVirtual->addItem(file);
}

void FileSystemVirtual::ls() const
{
  if (!currentPathVirtual)
    throw std::runtime_error("No current directory");

  currentPathVirtual->listItems();
}
