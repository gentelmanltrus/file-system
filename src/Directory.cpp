#include "Directory.h"

Directory::Directory(const std::string &name)
    : FileSystemItem(name) {}

void Directory::addItem(std::shared_ptr<FileSystemItem> item)
{
  items.push_back(item);
}

void Directory::display() const {
        std::cout << "DIR: " << name << "\n";
    }

    void Directory::listItems() const {
        for (const auto& item : items) {
            item->display();
        }
    }
