#ifndef FILESYSTEM_H_28539235
#define FILESYSTEM_H_28539235

#include <filesystem>
#include <memory>
#include <string>
#include "Directory.h"

class FileSystem
{
protected:
    std::filesystem::path currentPhysical; 

public:
    FileSystem(); // initialize currentPhysical as root 

    virtual void mkdir(const std::string &name);
    virtual void touch(const std::string &name);
    virtual void ls() const;
    virtual void cd(const std::string &name);
};
#endif // FILESYSTEM_H_28539235