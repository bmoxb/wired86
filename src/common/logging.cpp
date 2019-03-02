#include "logging.hpp"

#include <iomanip>
#include <ctime>
#include <sstream>

namespace logging {
    const std::string Logger::MESSAGE_END = "\033[0m\n"; // Reset formatting/colouring plus newline.

    const std::string Logger::WHITE_ON_BLACK_TEXT = "\033[37;40m",
                      Logger::YELLOW_ON_BLACK_TEXT = "\033[33;40m",
                      Logger::RED_ON_BLACK_TEXT = "\033[31;40m";

    Logger::Logger(std::string loggerName) : name(loggerName) {}

    void Logger::addStream(std::ostream& stream) {
        streams.push_back(&stream);
    }

    Logger& Logger::info(std::string msg, std::optional<LoggingInfo> info) {
        return log(msg, "INFO", WHITE_ON_BLACK_TEXT, info);
    }

    Logger& Logger::warn(std::string msg, std::optional<LoggingInfo> info) {
        return log(msg, "WARNING", YELLOW_ON_BLACK_TEXT, info);
    }

    Logger& Logger::error(std::string msg, std::optional<LoggingInfo> info) {
        return log(msg, "ERROR", RED_ON_BLACK_TEXT, info);
    }


    Logger& Logger::log(std::string msg, std::string logType, std::string escapeSequence,
                        std::optional<LoggingInfo>& info) {
        std::string fullMsg;
        
        if(info) {
            fullMsg = escapeSequence +
                      "[" + name + " - " + logType + " - " + fetchCurrentTimeString() +
                      " - line " + std::to_string(info->line) +
                      " of " + info->file + " - " + info->function + "] " +
                      msg + MESSAGE_END;
        }
        else {
            fullMsg = escapeSequence +
                      "[" + name + " - " + logType + " - " + fetchCurrentTimeString() + "] " +
                      msg + MESSAGE_END;
        }

        outThroughAllStreams(fullMsg);

        return *this;
    }

    void Logger::outThroughAllStreams(std::string msg) {
        for(std::ostream* stream : streams) (*stream) << msg;
    }

    std::string Logger::fetchCurrentTimeString() {
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);

        std::stringstream ss;
        ss << std::put_time(&tm, "%H:%M:%S");

        return ss.str();
    }
}