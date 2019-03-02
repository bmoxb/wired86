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
    struct LoggingInfo {
        unsigned long line;
        std::string file;
        std::string function;
    };

    class Logger {
    public:
        Logger(std::string loggerName);

        void addStream(std::ostream& stream);

        Logger& info(std::string msg, std::optional<LoggingInfo> info = {});
        Logger& warn(std::string msg, std::optional<LoggingInfo> info = {});
        Logger& error(std::string msg, std::optional<LoggingInfo> info = {});

        static const std::string MESSAGE_END;
        static const std::string WHITE_ON_BLACK_TEXT, YELLOW_ON_BLACK_TEXT, RED_ON_BLACK_TEXT;

    protected:
        Logger& log(std::string msg, std::string logType, std::string escapeSequence, std::optional<LoggingInfo>& info);

        void outThroughAllStreams(std::string msg);

    private:
        std::string name;
        std::vector<std::ostream*> streams;
    };
}