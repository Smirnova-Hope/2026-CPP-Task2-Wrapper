#ifndef ENGINE_H
#define ENGINE_H

#include "Wrapper.h"
#include <string>
#include <map>
#include <stdexcept>

class Engine {
public:
    Engine() = default;
    ~Engine() noexcept = default;

    Engine(const Engine&) = default;
    Engine& operator=(const Engine&) = default;
    Engine(Engine&&) noexcept = default;
    Engine& operator=(Engine&&) noexcept = default;

    void register_command(Wrapper* wrapper, const std::string& command_name) {
        if (!wrapper) {
            throw std::invalid_argument("Wrapper pointer cannot be null");
        }
        if (command_name.empty()) {
            throw std::invalid_argument("Command name can't be empty");
        }
        commands_[command_name] = wrapper;
    }

    std::string execute(const std::string& command_name,
        const Wrapper::ArgMap& args = {}) const {
        auto it = commands_.find(command_name);
        if (it == commands_.end()) {
            throw std::runtime_error("Command not found: " + command_name);
        }
        return it->second->execute(args);
    }

private:
    std::map<std::string, Wrapper*> commands_;
};

#endif 