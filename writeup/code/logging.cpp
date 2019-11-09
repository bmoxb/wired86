#include "logging.hpp"

#include <iomanip>
#include <ctime>
#include <sstream>
#include <algorithm>

namespace logging {
    const std::string Logger::MESSAGE_END = "\033[0m\n"; // Reset formatting/colouring plus newline.

    // ANSI colour escape sequences:
    const std::string Logger::CYAN_ON_BLACK_TEXT = "\033[36;40m",
                      Logger::WHITE_ON_BLACK_TEXT = "\033[37;40m",
                      Logger::YELLOW_ON_BLACK_TEXT = "\033[33;40m",
                      Logger::RED_ON_BLACK_TEXT = "\033[31;40m";

    Logger info("INFO", Logger::WHITE_ON_BLACK_TEXT);
    Logger success("SUCCESS", Logger::CYAN_ON_BLACK_TEXT);
    Logger warning("WARNING", Logger::YELLOW_ON_BLACK_TEXT, std::cerr);
    Logger error("ERROR", Logger::RED_ON_BLACK_TEXT, std::cerr);

    /**
      * Create a new logging objects.
      *
      * @param loggerLogType String that is prepended to each logger message output.
      * @param loggerEscapeSequence The ANSI escape sequence to use for colouring the output.
      * @param initialStream A stream for this logger to output via.
      */
    Logger::Logger(std::string loggerLogType, std::string loggerEscapeSequence, std::ostream& initialStream)
    : logType(loggerLogType), escapeSequence(loggerEscapeSequence) {
        addStream(initialStream); // Add a first output stream.
    }

    void Logger::operator()(std::string message) {
        outThroughAllStreams(escapeSequence +
                             "[" + logType + " - " + fetchCurrentTimeString() + "] " +
                             message + MESSAGE_END);
    }

    void Logger::operator()(std::string message, LoggingInfo info) {
        outThroughAllStreams(escapeSequence +
                             "[" + logType + " - " + fetchCurrentTimeString() +
                             " - line " + std::to_string(info.line) +
                             " of " + info.file + " - " + info.function + "] " +
                             message + MESSAGE_END);
    }

    void Logger::addStream(std::ostream& stream) {
        streams.push_back(&stream);
    }

    void Logger::removeStream(std::ostream& stream) {
        auto pos = std::remove(streams.begin(), streams.end(), &stream);
        streams.erase(pos, streams.end());
    }

    bool Logger::hasStreams() const {
        return streams.size() > 0;
    }

    void Logger::outThroughAllStreams(std::string msg) {
        if(hasStreams()) {
            for(std::ostream* stream : streams) {
                if(stream) (*stream) << msg;
            }
        }
    }

    std::string Logger::fetchCurrentTimeString() const {
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);

        std::stringstream ss;
        ss << std::put_time(&tm, "%H:%M:%S");

        return ss.str();
    }
}
