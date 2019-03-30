#include "executor.hpp"

int main(int argc, char* argv[]) {
    if(argc > 1) {
        std::string path = argv[1];
        cli::Executor exec(0xFF, path);
        exec.runCycles(5);
    }

    return 0;
}