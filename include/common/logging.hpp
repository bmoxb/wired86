#pragma once

#include <string>
#include <ostream>
#include <iostream>

/*
 * __PRETTY_FUNCTION__ produces the function name along with argument types and namespaces though it is unfortunately
 * only available on GCC. If GCC is not being used then __func__ is used instead (which is defined as part of the C++ 11
 * standard).
 */
#ifdef __GNUG__
    #define LOG(stream, formatting, name) logging::Logger(stream, formatting, name, __LINE__, __FILE__, __PRETTY_FUNCTION__)
#else
    #define LOG(stream, formatting, name) logging::Logger(stream, formatting, name, __LINE__, __FILE__, __func__)
#endif

/// Log general information to standard console output stream. Uses white on black text.
#define LOG_INFO LOG(std::cout, "\033[37;40m", "INFO")

/// Log message indicating an operation completed successfully to standard console output. Uses blue on black text.
#define LOG_SUCCESS LOG(std::cout, "\033[34;40m", "SUCCESS")

/// Log warning (indicating a minor error that it is possible to recover from) to standard console output. Uses yellow
/// on black text.
#define LOG_WARNING LOG(std::cout, "\033[33;40m", "WARNING")

/// Log a fatal error (unrecoverable) message to standard error console output. Uses red on black text.
#define LOG_ERROR LOG(std::cerr, "\033[31;40m", "ERROR")

namespace logging {
    /**
     * Class that handles logging to a specified stream. Instances of this class should not be created directly -
     * instead use the macros LOG_INFO, LOG_SUCCESS, LOG_WARNING and LOG_ERROR instead.
     */
    class Logger {
    public:
        /**
         * On construction, the specified line number, file, function and name will be written to the specified stream
         * using the given formatting before immediately reverting to no/default formatting.
         *
         * @param loggerStream The stream that this logger should output to.
         * @param formatting ANSI formatting escape sequence to colour the initial logger output.
         * @param name Name of the type of logging output (e.g. ERROR.
         * @param line The line on which the call to log took place.
         * @param file The file from which the call to log look place.
         * @param function From within which function the call to log was made.
         */
        Logger(std::ostream& loggerStream, std::string formatting, std::string name, unsigned int line,
               std::string file, std::string function);

        /**
         * Anything passed to a Logger object with the << operator is immediately funnelled to the stream specified for
         * this Logger.
         */
        template <typename T>
        Logger& operator<<(T value) {
            stream << value;
            return *this;
        }

    private:
        std::ostream& stream;
    };
}