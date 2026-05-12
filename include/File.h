#ifndef FILE_H_9825024
#define FILE_H_9825024

#include <string>
#include <iostream>
#include "FileSystemItem.h"

class File : public FileSystemItem
{
public:
    File(const std::string &name);
    void display() const override;
};
#endif // FILE_H_9825024