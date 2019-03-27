#include "executor.hpp"

int main(int argc, char* argv[]) {
    cli::Executor exec(0xFF);

    exec.runCycles(5);

    return 0;
}