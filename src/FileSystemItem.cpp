#include "include/FileSystemItem.h"

FileSystemItem::FileSystemItem(const std::string &name)
    : name(name) {}

std::string FileSystemItem::getName() const
{
  return name;
}