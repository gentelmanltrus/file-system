#include "../include/FileSystemItem.h"

FileSystemItem::FileSystemItem(const std::filesystem::path& name)
    : name(name) {}

std::string FileSystemItem::getName() const
{
  return name;
}
