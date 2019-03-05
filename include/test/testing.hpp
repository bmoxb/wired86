#pragma once

#include <string>
#include <functional>
#include <exception>
#include <sstream>
#include "logging.hpp" // Part of common library.

/**
 * Exception thrown when a unit test fails or does not complete successfully.
 */
class UnitTestFailed : public std::exception {
    const char* what() const throw() override {
        return "Unit test failed!";
    }
};

template <typename... Args>
class UnitTest {
public:
    /**
     * Create a new unit test.
     *
     * @param description Description of what is being tested.
     * @param testFunction The function to test with.
     */
    UnitTest(std::string description, std::function<bool(Args...)> testFunction,
             std::string testArgumentSeparator = ", ")
    : function(testFunction), argumentSeparator(testArgumentSeparator) {
        logging::info("Test created: " + description);
    }

    /**
     * Execute the test function with the given arguments Will display whether the test was successful or not using the
     * standard success and error loggers. Will throw UnitTestFailed should the test function return false or throw
     * an unexpected exception.
     */
    UnitTest& testNoArgsDisplay(Args... args) {
        bool result = false;

        try {
            result = function(args...);
        }
        catch(const std::exception& e) {
            std::string msg = e.what();
            logging::error("Failed - test function threw unexpected exeception: " + msg);
            throw UnitTestFailed();
        }
        catch(...) {
            logging::error("Failed - test function threw unexpected exception that does not derrive from "
                           "std::exception class.");
            throw UnitTestFailed();
        }

        if(result) {
            logging::success("Passed - test function returned true with given arguments.");
        }
        else {
            logging::error("Failed - test function returned false with given arguments.");
            throw UnitTestFailed();
        }

        return *this;
    }

    /**
     * Outputs the values passed as arguments via info logger before calling UnitTest::testNoArgsDisplay method.
     */
    UnitTest& test(Args... args) {
        std::stringstream stream;
        ((stream << argumentSeparator << args), ...); // Expand the arguments of the parameter pack.

        std::string argsString = stream.str();
        argsString.erase(0, argumentSeparator.size()); // Remove the initial separator from the beginning of the
                                                       // argument string.

        logging::info("Passing arguments to test function: " + argsString);

        return testNoArgsDisplay(args...);
    }

    template <typename Exception>
    UnitTest& testForException(Args... args) { /* TODO: ... */ }

private:
    const std::function<bool(Args...)> function;
    const std::string argumentSeparator;
};