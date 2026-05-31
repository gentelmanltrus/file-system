#include <iostream>
#include "../include/CommandProcessor.h"
#include <sstream>
CommandProcessor::CommandProcessor()
{
    commands["touch"] = [this](std::stringstream& ss)
    {
        std::string fileName;
        if (!(ss >> fileName))
            throw std::runtime_error("touch: missing filename");

        fileSystem.touch(fileName);
    };

    commands["ls"] = [this](std::stringstream&)
    {
        fileSystem.ls();
    };

    commands["mkdir"] = [this](std::stringstream& ss)
    {
        std::string dir;
        if (!(ss >> dir))
            throw std::runtime_error("mkdir: missing directory");

        fileSystem.mkdir(dir);
    };

    commands["help"] = [this](std::stringstream&)
    {
        fileSystem.help();
    };
    commands ["cd"] = [this](std::stringstream& ss)
    {
        std::string dir;
        if (!(ss >> dir))
            throw std::runtime_error("cd: missing directory");

        fileSystem.cd(dir);
    };
    commands["pwd"] = [this](std::stringstream&)
    {
    fileSystem.pwd();
    };
    commands["rm"] = [this](std::stringstream& ss)
    {
        std::string flag;
        std::string name;

        if (!(ss >> flag))
            throw std::runtime_error("rm: missing filename");

        if (flag == "-f")
        {
            if (!(ss >> name))
                throw std::runtime_error("rm: missing filename after -f");
            fileSystem.remove(name);
        }
        else
        {
            name = flag;
            std::cout << "Are you sure you want to remove \"" << name << "\"? Type \"y\" to confirm:" << std::endl;
            std::string answer;
            std::getline(std::cin, answer);
            if (answer == "y" || answer == "Y")
                fileSystem.remove(name);
            else
                std::cout << "rm: cancelled" << std::endl;
        }
    };
}

void CommandProcessor::run()
{
    std::string input;
    while (true)
    {
    std::cout << ">";
    std::getline(std::cin, input);
    if (input == "quit")
        break;
    if (input == "")
        continue;

    try
    {
        processCommand(input);
    }
    catch (std::runtime_error &e)
    {
        std::cout << e.what();
    }
    }
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
        throw std::runtime_error("Invalid command: " + commandName + "\n");
    }
}
