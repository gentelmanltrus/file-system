#include <iostream>
#include "include/CommandProcessor.h"

CommandProcessor::CommandProcessor()
{
    commands["touch"] = [this](std::stringstream& ss)
    {
        std::string fileName;
        if (!(ss >> fileName))
            throw std::runtime_error("touch: missing filename");

        fs.touch(fileName);
    };

    commands["ls"] = [this](std::stringstream&)
    {
        fs.ls();
    };

    commands["mkdir"] = [this](std::stringstream& ss)
    {
        std::string dir;
        if (!(ss >> dir))
            throw std::runtime_error("mkdir: missing directory");

        fs.mkdir(dir);
    };
}

void CommandProcessor::processCommand(const std::string &input)
{
    std::stringstream ss(input);
    std::string commandName;

    ss >> commandName;

    auto it = commands.find(commandName);
    if (it != commands.end())
    {
      try 
      {
        it->second(ss);
      } 
      catch(std::runtime_error &e)
      {
        std::cout << e.what() << std::endl;
        return;
      }
    }
    else
    {
        throw std::runtime_error("Invalid command: " + commandName);
    }
}
