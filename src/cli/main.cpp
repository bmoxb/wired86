#include "logging.hpp"

int main(int argc, char* argv[]) {
    LOG_INFO << "Hello this is a log message!\n";
    LOG_SUCCESS << "Successful!\n";
    LOG_WARNING << "Warning...\n";
    LOG_ERROR << "This is an error...\n";
    return 0;
}