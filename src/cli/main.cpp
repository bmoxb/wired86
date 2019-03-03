#include "logging.hpp"
#include <fstream>

int main(int argc, char* argv[]) {
    std::ofstream file("log.txt");
    logging::success.addStream(file);

    logging::success("This will appear in both the console and log file.");
    logging::info("This will only appear in the console.");

    return 0;
}