#ifndef WRAPPER_H
#define WRAPPER_H

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <functional>
#include <stdexcept>

class Wrapper {
public:
    using ArgMap = std::map<std::string, std::string>;
    using ArgList = std::vector<std::pair<std::string, std::string>>;

    template <typename Subject, typename... Args>
    Wrapper(Subject* subj,
        std::string(Subject::* method)(Args...),
        const ArgList& default_args = {}) {
        if (!subj || !method) {
            throw std::invalid_argument("Subject pointer or method pointer is null");
        }
        if (default_args.size() != sizeof...(Args)) {
            throw std::invalid_argument("Number of default arguments does not match method signature");
        }

        for (const auto& pair : default_args) {
            if (default_args_.find(pair.first) != default_args_.end()) {
                throw std::invalid_argument("Duplicate argument name: " + pair.first);
            }
            arg_names_.push_back(pair.first);
            default_args_[pair.first] = pair.second;
        }

        caller_ = [subj, method](const std::vector<std::string>& args) -> std::string {
            return call_helper(subj, method, args, std::index_sequence_for<Args...>{});
            };
    }

    ~Wrapper() noexcept = default;
    Wrapper(const Wrapper&) = default;
    Wrapper& operator=(const Wrapper&) = default;
    Wrapper(Wrapper&&) noexcept = default;
    Wrapper& operator=(Wrapper&&) noexcept = default;

    std::string execute(const ArgMap& override_args = {}) const {
        for (const auto& pair : override_args) {
            if (default_args_.find(pair.first) == default_args_.end()) {
                throw std::invalid_argument("Unknown argument name: " + pair.first);
            }
        }

        std::vector<std::string> final_args;
        final_args.reserve(arg_names_.size());

        for (const auto& name : arg_names_) {
            auto it = override_args.find(name);
            if (it != override_args.end()) {
                final_args.push_back(it->second);
            }
            else {
                final_args.push_back(default_args_.at(name));
            }
        }

        return caller_(final_args);
    }

private:
    template <typename Subject, typename... Args, std::size_t... Is>
    static std::string call_helper(Subject* subj,
        std::string(Subject::* method)(Args...),
        const std::vector<std::string>& args,
        std::index_sequence<Is...>) {
        return (subj->*method)(args[Is]...);
    }

    std::vector<std::string> arg_names_;
    ArgMap default_args_;
    std::function<std::string(const std::vector<std::string>&)> caller_;
};

#endif 