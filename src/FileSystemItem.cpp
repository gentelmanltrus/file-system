#include "../include/FileSystemItem.h"

FileSystemItem::FileSystemItem(const std::filesystem::path& name)
    : name(name) {}

std::filesystem::path FileSystemItem::getName() const
{
  return name;
}
