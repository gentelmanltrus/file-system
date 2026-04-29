#include <algorithm>
#include "Directory.h"

Directory::Directory(const std::string &name)
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
    std::cout << "DIR: " << name << "\n";
}

void Directory::listItems() const
{
    for (const auto &item : items)
    {
        item->display();
    }
}
