#pragma once

#include <string>
#include <ostream>
#include <vector>
#include <optional>

/*
 * __PRETTY_FUNCTION__ produces the function name along with argument types and namespaces though it is unfortunately
 * only available on GCC. If GCC is not being used then __func__ is used instead (which is defined as part of the C++ 11
 * standard).
 */
#ifdef __GNUG__
    #define ADDITIONAL_LOGGING_INFO std::make_optional<logging::LoggingInfo>({ __LINE__, __FILE__, __PRETTY_FUNCTION__ })
#else
    #define ADDITIONAL_LOGGING_INFO std::make_optional<logging::LoggingInfo>({ __LINE__, __FILE__, __func__ })
#endif

namespace logging {
    /**
     * Structure holding information about a call to a logging method. Holds the line number, file and function name
     * from which a log call is made. Should not be made manually - use the ADDITIONAL_LOGGING_INFO macro instead as a
     * second argument to a logging method in Logger.
     */
    struct LoggingInfo {
        unsigned long line;
        std::string file;
        std::string function;
    };

    /**
     * Allows for the logging of information, warning and errors to multiple output streams.
     */
    class Logger {
    public:
        Logger(std::string loggerName);

        /**
         * Add a new output stream to this logger.
         */
        void addStream(std::ostream& stream);

        /// Log general information.
        Logger& info(std::string msg, std::optional<LoggingInfo> info = {});

        /// Log warning (non-fatal).
        Logger& warn(std::string msg, std::optional<LoggingInfo> info = {});

        /// Log error (fatal).
        Logger& error(std::string msg, std::optional<LoggingInfo> info = {});

        static const std::string MESSAGE_END;
        static const std::string WHITE_ON_BLACK_TEXT, YELLOW_ON_BLACK_TEXT, RED_ON_BLACK_TEXT;

    protected:
        /**
         * Output logging information to all streams.
         *
         * @param msg Logging message to output.
         * @param logType String indicating the logging type (e.g. WARNING, ERROR).
         * @param escapeSequence ANSI escape sequence to apply to colours to the output.
         * @param info Optional additional information about where the log method was called from.
         */
        Logger& log(std::string msg, std::string logType, std::string escapeSequence, std::optional<LoggingInfo>& info);

        /**
         * Output a string through every output stream used by this logger.
         */
        void outThroughAllStreams(std::string msg);

    private:
        std::string name;
        std::vector<std::ostream*> streams;
    };
}