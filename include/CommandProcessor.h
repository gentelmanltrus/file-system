#ifndef COMMANDPROCESSOR_H_2242509
#define COMMANDPROCESSOR_H_2242509

#include <string>
#include "FileSystem.h"

// a replacement for int main() loop
class CommandProcessor
{
private:
    FileSystem fs;

public:
    void run(); // runs loop, calls processCommand()
    void processCommand(const std::string &input);
};
#endif