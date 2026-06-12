#include <fstream>
#include <filesystem>
#include <memory>
#include <iostream>
#include "../include/FileSystem.h"
#include <queue>
#include "../include/FileSystemVirtual.h"
#include "../include/File.h"
#include "../include/Directory.h"

FileSystemVirtual::FileSystemVirtual() : FileSystem(std::filesystem::current_path())
{
  currentPathVirtual = std::make_shared<Directory>("root");
}

void FileSystemVirtual::import(const std::filesystem::path &path)
{
  if (!currentPathVirtual)
    throw std::runtime_error("import: no current virtual directory");

  if (std::filesystem::is_regular_file(path))
  {
    std::shared_ptr<File> file = std::make_shared<File>(path, currentPathVirtual);
    currentPathVirtual->addItem(file);
  }
  else if (std::filesystem::is_directory(path))
  {
    std::shared_ptr<Directory> dir = std::make_shared<Directory>(path, currentPathVirtual);
    currentPathVirtual->addItem(dir);

    auto saved = currentPathVirtual;
    currentPathVirtual = dir;
    try
    {
      for (const auto &entry : std::filesystem::directory_iterator(path))
      {
        import(entry.path());
      }
    }
    catch (...)
    {
      currentPathVirtual = saved;
      throw;
    }
    currentPathVirtual = saved;
  }
  else
  {
    throw std::runtime_error("import: path does not exist");
  }
}

void FileSystemVirtual::mkdir(const std::string &name)
{
  if (!currentPathVirtual)
    throw std::runtime_error("No current directory");
  if (currentPathVirtual->contains(name))
    throw std::runtime_error("Directory or file already exists");
  std::shared_ptr<Directory> newDir = std::make_shared<Directory>(name, currentPathVirtual);
  currentPathVirtual->addItem(newDir);
}

void FileSystemVirtual::touch(const std::string &name)
{
  // Adding virtual file to physical file system folder
  std::filesystem::create_directory(VIRTUAL_FOLDER_NAME);
  std::filesystem::path full = (std::filesystem::path)VIRTUAL_FOLDER_NAME / name;
  FileSystem::touch(full.string());

  if (!currentPathVirtual)
    throw std::runtime_error("No current directory");

  if (currentPathVirtual->contains(name))
    throw std::runtime_error("File already exists");

  std::shared_ptr<File> file = std::make_shared<File>(name, currentPathVirtual);
  currentPathVirtual->addItem(file);
}

void FileSystemVirtual::ls() const
{
  if (!currentPathVirtual)
    throw std::runtime_error("No current directory");

  currentPathVirtual->listItems();
}

void FileSystemVirtual::cd(const std::string &name)
{
  if (!currentPathVirtual)
    throw std::runtime_error("No current directory");

  if (name == "..")
  {
    auto parent = currentPathVirtual->getParent();
    if (!parent)
      throw std::runtime_error("Already at root directory");
    currentPathVirtual = std::dynamic_pointer_cast<Directory>(parent);
    return;
  }

  auto item = currentPathVirtual->getItem(name);
  auto dir = std::dynamic_pointer_cast<Directory>(*item);
  if (!dir)
    throw std::runtime_error("Directory not found");
  currentPathVirtual = dir;
}

void FileSystemVirtual::remove(const std::string &name)
{
  if (!currentPathVirtual)
    throw std::runtime_error("No current directory");

  auto item = currentPathVirtual->getItem(name);
  currentPathVirtual->deleteItem(item);
}

void FileSystemVirtual::pwd() const
{
    if (!currentPathVirtual)
        throw std::runtime_error("No current directory");

    std::shared_ptr<Directory> temp = currentPathVirtual;
    std::vector<std::string> pathParts;

    while (temp)
    {
        pathParts.push_back(temp->getName().string());
        temp = std::dynamic_pointer_cast<Directory>(temp->getParent());
    }

    for (auto it = pathParts.rbegin(); it != pathParts.rend(); ++it)
    {
        std::cout << *it;
        if (it + 1 != pathParts.rend())
            std::cout << "/";
    }
}
