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
    FileSystem(const std::filesystem::path &path); // initialize currentPhysical as path
    virtual ~FileSystem() = default;

    virtual void mkdir(const std::string &name);
    virtual void touch(const std::string &name);
    virtual void ls() const;
    virtual void cd(const std::string &name);
    virtual void remove(const std::string &name);
    virtual void help() const;
    virtual void pwd() const;
    void tree() const;
    void report() const;
    void duplicates() const;
    static std::filesystem::path getHomeDirectory();
};
#endif // FILESYSTEM_H_28539235
