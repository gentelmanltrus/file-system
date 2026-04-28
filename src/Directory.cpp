#include "Directory.h"

Directory::Directory(const std::string &name)
    : FileSystemItem(name) {}

bool Directory::contains(const std::string &name)
{
    for (const auto &item : items)
    {
        if (item->getName() == name)
            return true;
    }
    return false;
}

void Directory::addItem(std::shared_ptr<FileSystemItem> item)
{
    items.push_back(item);
}

void Directory::display() const
{
    std::cout << "DIR: " << name << "\n";
}

void Directory::listItems() const
{
    for (const auto &item : items)
    {
        item->display();
    }
}
