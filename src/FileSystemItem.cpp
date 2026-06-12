#include "../include/FileSystemItem.h"

FileSystemItem::FileSystemItem(const std::filesystem::path& name, std::shared_ptr<FileSystemItem> parent)
    : name(name), parent(parent) {}

std::filesystem::path FileSystemItem::getName() const
{
  return name;
}

std::shared_ptr<FileSystemItem> FileSystemItem::getParent() const
{
  return parent;
}
