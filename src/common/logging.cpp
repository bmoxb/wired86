#include "logging.hpp"

namespace logging {
    Logger::Logger(std::ostream& loggerStream, std::string formatting, std::string name, unsigned int line,
                   std::string file, std::string function) : stream(loggerStream) {
        stream << formatting // Use specified formatting.
               << "[" << name << " - line " << line << " of " << file << " - " << function << "] "
               << "\033[0m"; // Default colouring/formatting.
    }
}