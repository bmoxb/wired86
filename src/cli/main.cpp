#include "logging.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    logging::Logger log("cli");
    log.addStream(std::cout);

    log.info("Some useful information?");
    log.warn("Warning message!");
    log.error("Error with additional information.", ADDITIONAL_LOGGING_INFO);
    log.success("Woo! Success!", ADDITIONAL_LOGGING_INFO);

    return 0;
}