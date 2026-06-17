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
  root = std::make_shared<Directory>("root");
  currentPathVirtual = root;
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

  std::filesystem::path target(name);
  std::string dirName = target.filename().string();
  std::shared_ptr<Directory> targetDir = resolveTargetDirectory(name);

  if (targetDir->contains(dirName))
    throw std::runtime_error("Directory or file already exists");
  std::shared_ptr<Directory> newDir = std::make_shared<Directory>(dirName, targetDir);
  targetDir->addItem(newDir);
}

void FileSystemVirtual::touch(const std::string &name)
{
  // Adding virtual file to physical file system folder
  std::filesystem::create_directory(VIRTUAL_FOLDER_NAME);
  auto filename = std::filesystem::path(name).filename(); 
  std::filesystem::path full = (std::filesystem::path)VIRTUAL_FOLDER_NAME / filename;
  FileSystem::touch(full.string());
  full = std::filesystem::current_path() / full;

  if (!currentPathVirtual)
    throw std::runtime_error("No current directory");

  std::filesystem::path target(name);
  std::string fileName = target.filename().string();
  std::shared_ptr<Directory> targetDir = resolveTargetDirectory(name);

  if (targetDir->contains(fileName))
    throw std::runtime_error("File already exists");

  std::shared_ptr<File> file = std::make_shared<File>(full, targetDir);
  targetDir->addItem(file);
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

  currentPathVirtual = navigate(name);
}

void FileSystemVirtual::remove(const std::string &name)
{
  if (!currentPathVirtual)
    throw std::runtime_error("No current directory");

  std::filesystem::path target(name);
  std::string itemName = target.filename().string();
  std::shared_ptr<Directory> targetDir = resolveTargetDirectory(name);

  targetDir->deleteItem(targetDir->getItem(itemName));
}

void FileSystemVirtual::pwd() const
{
    std::cout << getCurrentVirtual().string();
}

std::filesystem::path FileSystemVirtual::getCurrentVirtual() const
{
    if (!currentPathVirtual)
        throw std::runtime_error("No current directory");

    std::shared_ptr<Directory> temp = currentPathVirtual;
    std::vector<std::string> pathParts;

    while (temp)
    {
        pathParts.push_back(temp->getName().filename().string());
        temp = std::dynamic_pointer_cast<Directory>(temp->getParent());
    }

    std::stringstream ss;
    for (auto it = pathParts.rbegin(); it != pathParts.rend(); ++it)
    {
        ss << *it;
        if (it + 1 != pathParts.rend())
            ss << "/";
    }
    return std::filesystem::path(ss.str());
}

std::shared_ptr<FileSystemItem> FileSystemVirtual::getItem(const std::string &name) const
{
    if (!currentPathVirtual)
        throw std::runtime_error("No current directory");
    return *currentPathVirtual->getItem(name);
}

std::shared_ptr<Directory> FileSystemVirtual::navigate(const std::filesystem::path &path) const
{
    std::shared_ptr<Directory> current;
    auto it = path.begin();

    if (*it == "root")
    {
        current = root;
        ++it;
    }
    else
    {
        current = currentPathVirtual;
    }

    while(it != path.end())
    {
        if (*it == "..")
        {
            auto parent = current->getParent();
            if (!parent)
                throw std::runtime_error("Already at root directory");
            current = std::dynamic_pointer_cast<Directory>(parent);
        }
        else
        {
            auto child = *current->getItem(it->string());
            current = std::dynamic_pointer_cast<Directory>(child);
            if (!current)
                throw std::runtime_error(it->string() + " is not a directory");
        }
        ++it;
    }

    return current;
}

std::shared_ptr<Directory> FileSystemVirtual::resolveTargetDirectory(const std::filesystem::path &target) const
{
    std::filesystem::path dirPart = target.parent_path();
    std::shared_ptr<Directory> targetDir;

    if (!dirPart.empty())
        targetDir = navigate(dirPart);
    else
        targetDir = currentPathVirtual;

    return targetDir;
}
