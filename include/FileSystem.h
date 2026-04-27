#ifndef FILESYSTEM_H_5482301
#define FILESYSTEM_H_5482301

#include <memory>
#include <string>
#include "Directory.h"

class FileSystem
{
private:
    std::shared_ptr<Directory> root;
    std::shared_ptr<Directory> current;

public:
    FileSystem();

    void mkdir(const std::string &name);
    void touch(const std::string &name);
    void ls() const;
};
#endif // FILESYSTEM_H_5482301