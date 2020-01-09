#pragma once

#include <string>
#include <ostream>
#include <vector>
#include <iostream>

/*
 * __PRETTY_FUNCTION__ produces the function name along with argument types and namespaces though it is unfortunately
 * only available on GCC. If GCC is not being used then __func__ is used instead (which is defined as part of the C++ 11
 * standard).
 */
#ifdef __GNUG__
    #define ADDITIONAL_LOGGING_INFO { __LINE__, __FILE__, __PRETTY_FUNCTION__ }
#else
    #define ADDITIONAL_LOGGING_INFO { __LINE__, __FILE__, __func__ }
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
        /**
         * Initialise a new logger object.
         *
         * @param loggerLogType A string specifying the type of log message. This is prefixed to and displayed before every log message.
         * @param loggerEscapeSequence Specify the escape sequence applied to each log message (usually a colour - see static constant expressions of Logger class).
         * @param initialStream The first stream with which
         */
        Logger(std::string loggerLogType, std::string loggerEscapeSequence, std::ostream& initialStream = std::cout);

        /**
         * Display message via all output streams.
         *
         * @param message Message to display (should not contain timestamps or logging type as that information will be
         *                added automatically).
         */
        void operator()(std::string message);

        /**
         * Display message with additional information about where the call was made to all output streams.
         *
         * @param info Additional information about where the call to log was made. Should not be created manually -
         *             instead use the ADDITIONAL_LOGGING_INFO macro.
         */
        void operator()(std::string message, LoggingInfo info);

        /// Add a new output stream to this logger.
        void addStream(std::ostream& stream);
        /// Remove an output stream from this logger.
        void removeStream(std::ostream& stream);
        /// Check if this logger has at least 1 output stream.
        bool hasStreams() const;

        static constexpr auto MESSAGE_END = "\033[0m\n"; // Reset formatting/colouring plus newline.

        static constexpr auto CYAN_ON_BLACK_TEXT    = "\033[36;40m",
                              WHITE_ON_BLACK_TEXT   = "\033[37;40m",
                              YELLOW_ON_BLACK_TEXT  = "\033[33;40m",
                              RED_ON_BLACK_TEXT     = "\033[31;40m";

    protected:
        /// Output a string through every output stream used by this logger.
        void outThroughAllStreams(std::string msg);

        /// Returns a string of the current time expressed in HH:MM:SS format.
        std::string fetchCurrentTimeString() const;

    private:
        std::vector<std::ostream*> streams;
        const std::string logType, escapeSequence;
    };

    extern Logger info, /// For logging general information.
                  success, /// For logging information indicating an operation completed successfully.
                  warning, /// For logging warning messages (non-fatal).
                  error; /// For logging errors (fatal).
}
