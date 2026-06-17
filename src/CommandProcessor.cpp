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
        {
            for (const auto &pair : aliases)
            {
                std::cout << pair.first << " -> " << pair.second << std::endl;
            }
        }
        if (!(ss >> newAlias))
            throw std::runtime_error("alias: missing new alias");

        auto itCommands1 = commands.find(existingCommand);
        if (itCommands1 == commands.end())
            throw std::runtime_error("alias: command does not exist");
        for (const auto &pair : aliases)
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
            throw std::runtime_error("import: current file system does not support import");
        }
        else
        {
            std::filesystem::path path;
            if (!(ss >> path))
                throw std::runtime_error("import: missing path");

            fsVirtual->import(path);
        }
    };

    commands["switch"] = [this](std::stringstream &ss)
    {
        std::string fsName;
        if (!(ss >> fsName))
        {
            std::cout << "Available file systems:" << std::endl;
            for (const auto &pair : fileSystems)
            {
                std::cout << "  - " << pair.first << std::endl;
            }
            return;
        }

        auto it = fileSystems.find(fsName);
        if (it == fileSystems.end())
            throw std::runtime_error("switch: file system not found");

        currentFileSystem = it;
    };

    commands["delete"] = [this](std::stringstream &ss)
    {
        std::string name;
        if (!(ss >> name))
            throw std::runtime_error("delete: missing file system name");

        auto it = fileSystems.find(name);
        if (it == fileSystems.end())
            throw std::runtime_error("delete: file system not found");
        if (currentFileSystem == it)
            throw std::runtime_error("delete: cannot delete currently active file system");

        fileSystems.erase(it);
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

        ss >> flag >> name;

        if (flag.empty())
            throw std::runtime_error("rm: missing filename");
        if (name.empty())
        {
            name = flag;
            flag.clear();
        }

        bool force = flag.find('f') != std::string::npos;
        bool strict = (flag.find('s') != std::string::npos);
        auto virtualFileSystem = dynamic_cast<FileSystemVirtual *>(currentFileSystem->second.get());
        bool isVirtual = virtualFileSystem != nullptr;

        if (!force)
        {
            std::cout << "Are you sure you want to remove \"" << name
                      << "\"? Type \"y\" to confirm:\n";

            std::string answer;
            std::getline(std::cin >> std::ws, answer);

            if (answer != "y" && answer != "Y")
            {
                std::cout << "rm: cancelled\n";
                return;
            }
        }

        if (strict)
        {
            if (!isVirtual)
                throw std::runtime_error(
                    "rm: strict mode is only supported in virtual file systems");

            std::filesystem::path target(name);
            target = std::filesystem::current_path() / VIRTUAL_FOLDER_NAME / target.filename();
            if (!std::filesystem::exists(target))
                throw std::runtime_error("rm: strict mode: file does not exist in physical file system " + target.string());
            currentFileSystem->second->FileSystem::remove(target.string());
        }

        if (isVirtual)
        {
            currentFileSystem->second->remove(name);
        }
        else
        {
            currentFileSystem->second->FileSystem::remove(name);
        }
    };

    commands["tree"] = [this](std::stringstream &)
    {
        currentFileSystem->second->tree();
    };

    commands["report"] = [this](std::stringstream &)
    {
        currentFileSystem->second->report();
    };

    commands["duplicates"] = [this](std::stringstream &)
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
        std::cout << "(" << currentFileSystem->first << ") ";
        currentFileSystem->second->pwd();
        std::cout << ">";
        std::getline(std::cin, input);
        if (input == "quit")
        {
            std::cout << "Exiting..." << std::endl;
            break;
        }
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
