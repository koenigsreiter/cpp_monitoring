/*
 * Name: Königsreiter Simon
 * Class: 5BHIF
 * MatrNr.: i14084
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

extern std::string LOGGER_NAME;

/** Simple logging Singleton.
 * 
 * This class contains a public static reference to 
 * an `spdlog::logger`. 
 */
class logger {
public:
    /** Reference to the actual logger.
     * 
     * A thread safe logger. Uses the extern string `LOGGER_NAME`
     * to define the name of the logger.
     */
    inline static std::shared_ptr<spdlog::logger> log = spdlog::stderr_color_mt(LOGGER_NAME);
};


#endif // LOGGER_HPP