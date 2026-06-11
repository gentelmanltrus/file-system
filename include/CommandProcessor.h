#ifndef COMMANDPROCESSOR_H_2242509
#define COMMANDPROCESSOR_H_2242509

#include <string>
#include <map>
#include <memory>
#include <functional>
#include "FileSystem.h"
#include "FileSystemVirtual.h"

// a replacement for int main() loop
class CommandProcessor {
private:
    std::map<std::string, std::function<void(std::stringstream&)>> commands;
    std::map<std::string, std::unique_ptr<FileSystem>> fileSystems;
    std::map<std::string, std::unique_ptr<FileSystem>>::iterator currentFileSystem;
    std::map<std::string, std::string> aliases;
public:
    CommandProcessor();
    void run();
    void processCommand(const std::string &input);
};
#endif
