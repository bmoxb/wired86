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

/**
 * @tparam Return The return type of the testing function.
 * @tparam Args The arguments taken by the testing function.
 */
template <typename Return, typename... Args>
class UnitTest {
public:
    /**
     * Create a new unit test.
     *
     * @param description Description of what is being tested.
     * @param testFunction The function to test with.
     */
    UnitTest(std::string description, std::function<Return(Args...)> testFunction,
             std::string testArgumentSeparator = ", ")
    : function(testFunction), argumentSeparator(testArgumentSeparator) {
        logging::info("Testing '" + description + "'...");
    }

    /**
     * Execute the test function with the given arguments Will display whether the test was successful or not using the
     * standard success and error loggers. Will throw UnitTestFailed should the test function return false or throw
     * an unexpected exception.
     */
    UnitTest& test(Return expected, Args... args) {
        testCount++;

        Return result;

        try {
            result = function(args...);
        }
        catch(const std::exception& e) {
            std::string msg = e.what();
            logging::error("Failed - " + getTestFunctionString() + " threw exeception: " + msg);
        }
        catch(...) {
            logging::error("Failed - " + getTestFunctionString() + " threw exception that does not derrive from "
                           "std::exception class!");
        }

        if(result == expected) {
            logging::success("Passed - " + getTestFunctionString() + " returned expected value: " +
                             std::to_string(expected));
            return *this;
        }
        else {
            logging::error("Failed - " + getTestFunctionString() + " failed to return expected value and instead "
                           "returned: " + std::to_string(result));
        }

        throw UnitTestFailed();
        return *this;
    }

    /**
     * Throws an exception indicating the test failed when the specified exception is not thrown.
     */
    template <typename Exception>
    UnitTest& testForException(Args... args) {
        testCount++;

        try {
            function(args...);

            logging::error("Failed - " + getTestFunctionString() + " failed to throw the expected exception!");
        }
        catch(const Exception& e) {
            std::string msg = e.what();
            logging::success("Passed - " + getTestFunctionString() + " threw expected exception: " + msg);

            return *this;
        }
        catch(const std::exception& e) {
            std::string msg = e.what();
            logging::error("Failed - " + getTestFunctionString() + " threw unexpected exeception: " + msg);
        }
        catch(...) {
            logging::error("Failed - " + getTestFunctionString() + " threw unexpected exception that does not "
                           "derrive from std::exception class!");
        }

        throw UnitTestFailed();
        return *this;
    }

protected:
    std::string getTestFunctionString() {
        return "test function call #" + std::to_string(testCount);
    }

private:
    const std::function<Return(Args...)> function;
    const std::string argumentSeparator;
    unsigned int testCount = 0;
};