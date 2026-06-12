#include <iostream>
#include "../include/CommandProcessor.h"
#include <sstream>
CommandProcessor::CommandProcessor()
{
    commands["alias"] = [this](std::stringstream &ss)
    {
        std::string existingCommand;
        std::string newAlias;

        if (!(ss >> existingCommand))
            throw std::runtime_error("alias: missing command name");
        if (!(ss >> newAlias))
            throw std::runtime_error("alias: missing new alias");

        auto itCommands1 = commands.find(existingCommand);
        if (itCommands1 == commands.end())
            throw std::runtime_error("alias: command does not exist");
        for (const auto& pair : aliases)
        {
            if (pair.second == existingCommand)
                throw std::runtime_error("alias: command already has an alias");
        }

        auto itCommands2 = commands.find(newAlias);
        if (itCommands2 != commands.end())
            throw std::runtime_error("alias: alias cannot be named after another command");
        auto itAliases = aliases.find(newAlias);
        if (itAliases != aliases.end())
            throw std::runtime_error("alias: alias already exists");

        aliases[newAlias] = existingCommand;
    };

    commands["unalias"] = [this](std::stringstream &ss)
    {
        std::string name;
        if (!(ss >> name))
            throw std::runtime_error("unalias: missing alias name");

        auto itAliases = aliases.find(name);
        if (itAliases == aliases.end())
            throw std::runtime_error("unalias: alias does not exist");

        aliases.erase(name);
    };

    commands["touch"] = [this](std::stringstream &ss)
    {
        std::string fileName;
        if (!(ss >> fileName))
            throw std::runtime_error("touch: missing filename");

        currentFileSystem->second->touch(fileName);
    };

    commands["ls"] = [this](std::stringstream &)
    {
        currentFileSystem->second->ls();
    };

    commands["mkdir"] = [this](std::stringstream &ss)
    {
        std::string dir;
        if (!(ss >> dir))
            throw std::runtime_error("mkdir: missing directory");

        currentFileSystem->second->mkdir(dir);
    };

    commands["create"] = [this](std::stringstream &ss)
    {
        std::string name;
        if (!(ss >> name))
            throw std::runtime_error("create: missing name");

        fileSystems[name] = std::make_unique<FileSystemVirtual>();
        currentFileSystem = fileSystems.find(name);

        std::filesystem::path path;
        if (ss >> path)
        {
            // return base class raw pointer from unique_ptr without ownership transfer
            FileSystem *fsVirtual = currentFileSystem->second.get();
            dynamic_cast<FileSystemVirtual *>(fsVirtual)->import(path);
        }
    };

    commands["import"] = [this](std::stringstream &ss)
    {
        FileSystem *fs = currentFileSystem->second.get();
        auto *fsVirtual = dynamic_cast<FileSystemVirtual *>(fs);

        if (!fsVirtual)
        {
            std::cout << "Physical file system does not support import. Use a virtual file system to import." << std::endl;
            return;
        }
        else
        {
            std::filesystem::path path;
            if (!(ss >> path))
                throw std::runtime_error("import: missing path");

            fsVirtual->import(path);
        }
    };

    commands["help"] = [this](std::stringstream &)
    {
        currentFileSystem->second->help();
    };
    commands["cd"] = [this](std::stringstream &ss)
    {
        std::string dir;
        if (!(ss >> dir))
            throw std::runtime_error("cd: missing directory");

        currentFileSystem->second->cd(dir);
    };
    commands["pwd"] = [this](std::stringstream &)
    {
        currentFileSystem->second->pwd();
        std::cout << std::endl;
    };
    commands["rm"] = [this](std::stringstream &ss)
    {
        std::string flag;
        std::string name;

        if (!(ss >> flag))
            throw std::runtime_error("rm: missing filename");

        if (flag == "-f")
        {
            if (!(ss >> name))
                throw std::runtime_error("rm: missing filename after -f");
            currentFileSystem->second->remove(name);
        }
        else
        {
            name = flag;
            std::cout << "Are you sure you want to remove \"" << name << "\"? Type \"y\" to confirm:" << std::endl;
            std::string answer;
            std::getline(std::cin, answer);
            if (answer == "y" || answer == "Y")
                currentFileSystem->second->remove(name);
            else
                std::cout << "rm: cancelled" << std::endl;
        }
    };
  
    commands["tree"] = [this](std::stringstream&)
    {
        currentFileSystem->second->tree();
    };

    commands["report"] = [this](std::stringstream&)
    {
        currentFileSystem->second->report();
    };

    commands["duplicates"] = [this](std::stringstream&)
    {
        currentFileSystem->second->duplicates();
    };

    // initialize with one default file system
    fileSystems["physical"] = std::make_unique<FileSystem>();
    currentFileSystem = fileSystems.begin();
    currentFileSystem->second->help();
}

void CommandProcessor::run()
{
    std::string input;
    while (true)
    {
        std::cout << "("<< currentFileSystem->first << ") ";
        currentFileSystem->second->pwd();
        std::cout << ">";
        std::getline(std::cin, input);
        if (input == "quit")
            break;
        if (input == "")
            continue;

        try
        {
            auto itAlias = aliases.find(input);
            if (itAlias != aliases.end())
            {
                processCommand(itAlias->second);
            }
            else
            {
                processCommand(input);
            }
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

    auto itAliases = aliases.find(commandName);
    if (itAliases != aliases.end())
        commandName = itAliases->second;

    auto it = commands.find(commandName);
    if (it != commands.end())
    {
        try
        {
            it->second(ss);
        }
        catch (std::runtime_error &e)
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
