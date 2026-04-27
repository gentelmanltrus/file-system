#ifndef FILESYSTEMITEM_H_956230
#define FILESYSTEMITEM_H_956230

#include <string>

class FileSystemItem {
protected:
    std::string name;

public:
    FileSystemItem(const std::string& name);

    std::string getName() const;

    // Polymorphism in real time
    virtual void display() const = 0;
    virtual ~FileSystemItem(); // must be defined to delete objects through base pointer
};
#endif // FILESYSTEMITEM_H_956230