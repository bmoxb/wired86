#include "logging.hpp"

#include <iomanip>
#include <ctime>
#include <sstream>
#include <algorithm>

namespace logging {
    // Define the 4 standard logger types:
    Logger info("INFO", Logger::WHITE_ON_BLACK_TEXT);
    Logger success("SUCCESS", Logger::CYAN_ON_BLACK_TEXT);
    Logger warning("WARNING", Logger::YELLOW_ON_BLACK_TEXT, std::cerr);
    Logger error("ERROR", Logger::RED_ON_BLACK_TEXT, std::cerr);

    Logger::Logger(std::string loggerLogType, std::string loggerEscapeSequence, std::ostream& initialStream)
    : logType(loggerLogType), escapeSequence(loggerEscapeSequence) {
        addStream(initialStream); // Add the logger's first output stream (defaults to standard out).
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
                // Since streams are stored as raw pointers here, it is best to check that each pointer is not null before outputting.
                if(stream) (*stream) << msg;
            }
        }
    }

    std::string Logger::fetchCurrentTimeString() const {
        // std::time is an old function provided by the C standard library that also included in the C++ STD.
        std::time_t t = std::time(nullptr);
        // Convert the epoch time (seconds since 00:00:00 UTC January 1st 1970) into a calendar time expressed in local time:
        std::tm tm = *std::localtime(&t);

        // Convert the structure returned by std::localtime to a string in format `hours:minutes:seconds`:
        std::stringstream ss;
        ss << std::put_time(&tm, "%H:%M:%S");

        return ss.str(); // Convert std::stringstream to regular std::string.
    }
}
