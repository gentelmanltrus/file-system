#include <algorithm>
#include "../include/Directory.h"
#include <vector>
#include <memory>
#include <iostream>
Directory::Directory(const std::filesystem::path &name, std::shared_ptr<FileSystemItem> parent)
    : FileSystemItem(name, parent) {}

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

std::vector<std::shared_ptr<FileSystemItem>>::const_iterator Directory::getItem(const std::string &name) const
{
    for (auto it = items.cbegin(); it != items.cend(); ++it)
    {
        if ((*it)->getName() == name)
        {
            return it;
        }
    }
    throw std::runtime_error("Item not found");
}

void Directory::deleteItem(std::vector<std::shared_ptr<FileSystemItem>>::const_iterator it)
{
    items.erase(it);
}
