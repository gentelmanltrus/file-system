#ifndef COMMANDPROCESSOR_H_2242509
#define COMMANDPROCESSOR_H_2242509

#include <string>
#include <map>
#include <functional>
#include "FileSystem.h"

// a replacement for int main() loop
class CommandProcessor {
private:
    std::map<std::string, std::function<void(std::stringstream&)>> commands;
    FileSystem fs;
public:
    CommandProcessor();
    void processCommand(const std::string &input);
};
#endif