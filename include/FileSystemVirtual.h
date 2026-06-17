#ifndef FILESYSTEMVIRTUAL_H_5482301
#define FILESYSTEMVIRTUAL_H_5482301

#include <memory>
#include <string>
#include "FileSystem.h"
#include "Directory.h"

#define VIRTUAL_FOLDER_NAME "virtual_folder"

class FileSystemVirtual : public FileSystem
{
private:
    std::shared_ptr<Directory> root;
    std::shared_ptr<Directory> currentPathVirtual;

public:
    FileSystemVirtual();

    void import(const std::filesystem::path& path);
    void mkdir(const std::string &name) override;
    void touch(const std::string &name) override;
    void ls() const override;
    void cd(const std::string &name) override;
    void remove(const std::string &name) override;
    void pwd() const override;
    std::shared_ptr<FileSystemItem> getItem(const std::string &name) const;

private:
    std::shared_ptr<Directory> navigate(const std::filesystem::path &path) const;
};
#endif // FILESYSTEMVIRTUAL_H_5482301
