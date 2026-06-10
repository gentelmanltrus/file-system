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
        [&name](const std::shared_ptr<FileSystemItem>& item)
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
    std::cout << "DIR: " << name.string() << std::endl;
}

void Directory::listItems() const
{
    for (const auto &item : items)
    {
        item->display();
    }
}
