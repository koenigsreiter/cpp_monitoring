#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class logger {
public:
    inline static std::shared_ptr<spdlog::logger> log = spdlog::stderr_color_mt("default");
};


#endif // LOGGER_HPP