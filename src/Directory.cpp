#include <algorithm>
#include "../include/Directory.h"
#include <vector>
#include <memory>
#include <iostream>
Directory::Directory(const std::filesystem::path &name)
    : FileSystemItem(name) {}

bool Directory::contains(const std::string &name)
{
    return std::find_if(items.begin(), items.end(),
                        [&name](const std::shared_ptr<FileSystemItem> &item)
                        {
                            return item->getName() == name;
                        }) != items.end();
}

void Directory::addItem(std::shared_ptr<FileSystemItem> item)
{
    items.push_back(item);
}

void Directory::display() const
{
    std::cout << name.string() << "/" << std::endl;
}

void Directory::listItems() const
{
    for (const auto &item : items)
    {
        item->display();
    }
}

std::shared_ptr<FileSystemItem> Directory::getItem(const std::string &name) const
{
    for (const auto &item : items)
    {
        if (auto dir = std::dynamic_pointer_cast<Directory>(item))
        {
            if (dir->getName() == name)
            {
                return dir;
            }
        }
    }
    throw std::runtime_error("Item not found");
}
