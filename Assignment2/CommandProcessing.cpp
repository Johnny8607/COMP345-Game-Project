#include "CommandProcessing.h"
#include <fstream>
#include <iostream>

// ================================================================
// CommandProcessor Implementation
// ================================================================
CommandProcessor::CommandProcessor()
    : currentState(new std::string("start")),
      commands(new std::vector<Command*>()) {}

CommandProcessor::CommandProcessor(const CommandProcessor& other)
    : currentState(new std::string(*other.currentState)),
      commands(new std::vector<Command*>()) {
    for (auto cmd : *other.commands)
        commands->push_back(new Command(*cmd));
}

CommandProcessor& CommandProcessor::operator=(const CommandProcessor& other) {
    if (this == &other) return *this;

    *currentState = *other.currentState;

    for (auto cmd : *commands) delete cmd;
    commands->clear();

    for (auto cmd : *other.commands)
        commands->push_back(new Command(*cmd));

    return *this;
}

CommandProcessor::~CommandProcessor() {
    for (auto cmd : *commands) delete cmd;
    delete commands;
    delete currentState;
}

void CommandProcessor::readCommand() {
    std::string cmdStr;
    while (true) {
        std::cout << "Enter command (type 'quit' to finish): ";
        std::getline(std::cin, cmdStr);

        if (cmdStr == "quit") break;

        Command* cmd = new Command(cmdStr);

        if (validate(cmdStr)) {
            cmd->saveEffect("[INFO] Command accepted");
            saveCommand(cmd);

            if (cmdStr.rfind("loadmap", 0) == 0) *currentState = "maploaded";
            else if (cmdStr == "validatemap") *currentState = "mapvalidated";
            else if (cmdStr.rfind("addplayer", 0) == 0) *currentState = "playersadded";
            else if (cmdStr == "gamestart") *currentState = "play";
            else if (cmdStr == "replay") *currentState = "start";
        } else {
            cmd->saveEffect("[ERROR] Command not valid in the current game state");
            saveCommand(cmd);
        }

        std::cout << *cmd << std::endl;
    }
}

void CommandProcessor::saveCommand(Command* c) {
    commands->push_back(c);
}

Command* CommandProcessor::getCommand(size_t index) const {
    if (index >= commands->size())
        return new Command("Invalid index");
    return new Command(*(*commands)[index]);
}

bool CommandProcessor::validate(const std::string& command) const {
    if (*currentState == "start")
        return command.rfind("loadmap", 0) == 0 || command == "replay" || command == "quit";
    if (*currentState == "maploaded")
        return command == "validatemap" || command == "quit";
    if (*currentState == "mapvalidated")
        return command.rfind("addplayer", 0) == 0 || command == "quit";
    if (*currentState == "playersadded")
        return command == "gamestart" || command == "quit";
    if (*currentState == "win")
        return command == "replay" || command == "quit";
    return false;
}

std::ostream& operator<<(std::ostream& out, const CommandProcessor& cp) {
    out << "CommandProcessor [State=" << *cp.currentState
        << ", Commands=" << cp.commands->size() << "]";
    return out;
}

// ================================================================
// FileCommandProcessorAdapter Implementation
// ================================================================
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const std::string& filename)
    : CommandProcessor(),
      fileCommands(new std::vector<std::string>()),
      currentIndex(0) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open command file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) fileCommands->push_back(line);
    }
    file.close();
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other)
    : CommandProcessor(other),
      fileCommands(new std::vector<std::string>(*other.fileCommands)),
      currentIndex(other.currentIndex) {}

FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter& other) {
    if (this == &other) return *this;
    CommandProcessor::operator=(other);
    *fileCommands = *other.fileCommands;
    currentIndex = other.currentIndex;
    return *this;
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete fileCommands;
}

void FileCommandProcessorAdapter::readCommandFromFile() {
    if (currentIndex >= fileCommands->size()) {
        std::cout << "[INFO] No more commands in file" << std::endl;
        return;
    }

    std::string cmdStr = (*fileCommands)[currentIndex++];
    Command* cmd = new Command(cmdStr);

    if (validate(cmdStr)) {
        cmd->saveEffect("[INFO] Command accepted");
        saveCommand(cmd);

        if (cmdStr.rfind("loadmap", 0) == 0) *currentState = "maploaded";
        else if (cmdStr == "validatemap") *currentState = "mapvalidated";
        else if (cmdStr.rfind("addplayer", 0) == 0) *currentState = "playersadded";
        else if (cmdStr == "gamestart") *currentState = "play";
        else if (cmdStr == "replay") *currentState = "start";
        else if (cmdStr == "quit") *currentState = "exit";

        std::cout << *cmd << std::endl;
    } else {
        cmd->saveEffect("[ERROR] Command not valid in the current game state");
        saveCommand(cmd);
        std::cout << *cmd << std::endl;
    }
}

bool FileCommandProcessorAdapter::hasMoreCommands() const {
    return currentIndex < fileCommands->size();
}

size_t FileCommandProcessorAdapter::getFileCommandsCount() const {
    return fileCommands->size();
}

std::ostream& operator<<(std::ostream& out, const FileCommandProcessorAdapter& adapter) {
    out << static_cast<const CommandProcessor&>(adapter)
        << " [FileCommands=" << adapter.fileCommands->size()
        << ", nextIndex=" << adapter.currentIndex << "]";
    return out;
}

// ================================================================
// Test Driver Function
// ================================================================
void testCommandProcessor() {
    std::cout << "=== Console Commands Test ===" << std::endl;
    CommandProcessor consoleProcessor;
    consoleProcessor.readCommand();

    std::cout << "\n=== File Commands Test ===" << std::endl;
    FileCommandProcessorAdapter fileProcessor("testCommands.txt");

    while (fileProcessor.hasMoreCommands()) {
        fileProcessor.readCommandFromFile();
    }
}
