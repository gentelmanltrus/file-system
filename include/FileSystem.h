#ifndef FILESYSTEM_H_28539235
#define FILESYSTEM_H_28539235

#include <memory>
#include <string>
#include "Directory.h"

class FileSystem
{
public:
    FileSystem();

    virtual void mkdir(const std::string &name);
    virtual void touch(const std::string &name);
    virtual void ls() const;
};
#endif // FILESYSTEM_H_28539235