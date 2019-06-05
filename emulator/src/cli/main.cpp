#include "logging.hpp"
#include "executor.hpp"

int main(int argc, char* argv[]) {
    if(argc >= 3) {
        auto memorySize = convert::fromHexString<emu::AbsAddr>(argv[1]);
        std::string path = argv[2];

        if(memorySize) {
            cli::Executor exec(*memorySize, path);
            exec.runCycles(25);
        }
        else logging::error("Invalid memory size given! Please express the memory size in hexadecimal format.");
    }
    else logging::error("Please execute with appropriate arguments: WiredSound <memory size> <path>");

    return 0;
}