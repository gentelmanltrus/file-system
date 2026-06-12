#ifndef DIRECTORY_H_786402
#define DIRECTORY_H_786402

#include <vector>
#include <memory>
#include <iostream>
#include "FileSystemItem.h"

class Directory : public FileSystemItem
{
private:
    std::vector<std::shared_ptr<FileSystemItem>> items;

public:
    Directory(const std::filesystem::path &name, std::shared_ptr<FileSystemItem> parent = nullptr);
    void display() const override;
    bool contains(const std::string &name);
    void addItem(std::shared_ptr<FileSystemItem> item);
    void listItems() const;
    std::vector<std::shared_ptr<FileSystemItem>>::const_iterator getItem(const std::string &name) const;
    void deleteItem(std::vector<std::shared_ptr<FileSystemItem>>::const_iterator it);
};
#endif // DIRECTORY_H_786402