#ifndef FILESYSTEMITEM_H_956230
#define FILESYSTEMITEM_H_956230

#include <string>
#include <filesystem>

class FileSystemItem {
protected:
    std::filesystem::path name;
    std::shared_ptr<FileSystemItem> parent; 

public:
    FileSystemItem(const std::filesystem::path& name, std::shared_ptr<FileSystemItem> parent = nullptr);
    virtual ~FileSystemItem(); // must be defined to delete objects through base pointer

    std::filesystem::path getName() const;
    std::shared_ptr<FileSystemItem> getParent() const;

    // Polymorphism in real time
    virtual void display() const = 0;
};
#endif // FILESYSTEMITEM_H_956230
