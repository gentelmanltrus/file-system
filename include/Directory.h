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
    Directory(const std::string &name);
    bool contains(const std::string &name);

    void addItem(std::shared_ptr<FileSystemItem> item);
    void display() const override;
    void listItems() const;
};
#endif // DIRECTORY_H_786402