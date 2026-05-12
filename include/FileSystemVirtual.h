#ifndef FILESYSTEMVIRTUAL_H_5482301
#define FILESYSTEMVIRTUAL_H_5482301

#include <memory>
#include <string>
#include "FileSystem.h"
#include "Directory.h"

class FileSystemVirtual : public FileSystem
{
private:
    //std::shared_ptr<Directory> root;
    std::shared_ptr<Directory> currentVirtual;

public:
    FileSystemVirtual();

    void mkdir(const std::string &name) override;
    void touch(const std::string &name) override;
    void ls() const override;
};
#endif // FILESYSTEMVIRTUAL_H_5482301