/**
 * @file
 * @brief Header file with testlib description
 *
 * Tests can be created with TEST(group, name) macro.
 * Tests that are declared with this macro are registered in TestRunner.
 * Use TestRunner.runAllTests() in main() to run all created tests.
 *
 * You can use assertions in test functions to check some condition
 * (for example, ASSERT_TRUE(condition) or ASSERT_EQUALS(actual, expected)).
 *
 */
#ifndef QUADRATIC_EQUATION_TESTLIB_H
#define QUADRATIC_EQUATION_TESTLIB_H

#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>
#include <functional>

/**
 * Pointer to a function created by TEST(group, name) macro.
 */
using TestPtr = std::function<void()>;

/**
 * Represents a runnable test with all the info about it (file and line where it was described).
 *
 * @note Unit tests are created by TEST(group, name) macro.
 *       This class is used only for internal representation in TestRunner.
 *
 * @see TEST(group, name)
 * @see TestRunner
 */
class Test {
private:
    const char* _fileName;
    unsigned int _line;
    TestPtr _function_ptr;

public:
    Test(const TestPtr& function_ptr, const char* fileName, unsigned int line);

    /**
     * Runs this test.
     */
    void run() const;

    /**
     * Name of the file this test was created in.
     * @return name of the file.
     */
    const char* fileName() const;

    /**
     * Line of the file this test was created on.
     * @return line number of the file.
     */
    unsigned int line() const;
};

//----------------------------------------------------------------------------------------------------------------------

/**
 * Represents a test runner - container for tests that is able to manage (run and stop) them.
 *
 * Tests created by TEST(group, name) macro are automatically registered in this runner.
 * Use TestRunner.runAllTests() in main() to run all created tests.
 *
 * @see TEST(group, name)
 * @see Test
 */
class TestRunner {
private:

    /** Pointer to a current running test **/
    Test* _currentTest = nullptr;

    /** Container for all tests. **/
    std::vector<Test*> allTests;

    /**
     * Run the given test.
     * @param[in] test pointer to a test to run
     * @return true, if the test succeeded, false otherwise.
     */
    bool runTest(Test* test);

public:
    TestRunner();

    ~TestRunner();

    /**
     * Registers new test in this runner.
     * @param[in] testPtr  pointer to a test function
     * @param[in] fileName name of the file test declared in
     * @param[in] line     line number of the file test declared on
     * @return pointer to a created Test object.
     *
     */
    Test* addTest(TestPtr testPtr, const char* fileName, unsigned int line);

    /**
     * Removes all tests from the container.
     */
    void clear();

    /**
     * Runs all tests that exist in this runner. Use in the main() method to run every written test.
     * @return true, if all tests succeeded, false otherwise.
     */
    bool runAllTests();

    /**
     * Fails current test. Use in assertions to show that the current test is failed.
     */
    void failCurrentTest();

    /**
     * Pointer to a current test executed by this runner.
     * @return pointer to a current running test
     */
    const Test* currentTest() const;

    /**
     * Returns a singleton instance of the runner.
     * @return singleton runner.
     */
    static TestRunner* getInstance();
};

//----------------------------------------------------------------------------------------------------------------------

/** Name that is given to a test function created by TEST(group, name) macro. **/
#define TEST_NAME(group, name) group##_##name##_test
/** Name that is given to a global variable that contains pointer to a Test object created by TEST(group, name) macro. **/
#define TEST_INFO(group, name) group##_##name##_testinfo

/**
 * Creates a test with a given group and name and registers it in a TestRunner.
 * Tests created by this macro are launched automatically by TestRunner.runAllTests().
 * You can you use assertions during the tests to check some conditions.
 *
 * @see ASSERT_TRUE(condition)
 * @see ASSERT_TRUE_WITH_FAILURE(condition, onFailure)
 * @see ASSERT_EQUALS(actual, expected)
 * @see ASSERT_DOUBLE_EQUALS(actualDouble, expectedDouble)
 */
#define TEST(group, name)                                                                                              \
    static_assert(sizeof(#group) > 1, "test group must not be empty");                                                 \
    static_assert(sizeof(#name)  > 1, "test name must not be empty" );                                                 \
                                                                                                                       \
    void TEST_NAME(group, name)();                                                                                     \
    const Test* TEST_INFO(group, name) =                                                                               \
        TestRunner::getInstance()->addTest(&TEST_NAME(group, name), __FILE__, __LINE__);                               \
    void TEST_NAME(group, name)()

//----------------------------------------------------------------------------------------------------------------------

#define TESTLIB_ANSI_COLOR_RED   "\x1b[31m"
#define TESTLIB_ANSI_COLOR_GREEN "\x1b[32m"
#define TESTLIB_ANSI_COLOR_RESET "\x1b[0m"

/**
 * Inner helper macro for showing that assertion and current test failed.
 * Use this in assertions macros, not in a tests.
 *
 * @param[in] onFailure code that is executed during the assertion failure.
 *                           Can be use to print some additional info to std::cerr.
 */
#define TESTLIB_ASSERT_FAILED(onFailure) do {                                                                          \
    TestRunner* runner = TestRunner::getInstance();                                                                    \
    const Test* currentTest = runner->currentTest();                                                                   \
    std::cerr << TESTLIB_ANSI_COLOR_RED;                                                                               \
    std::cerr << "[ASSERTION FAILED] " << currentTest->fileName() << ':' << currentTest->line() << '\n';               \
    onFailure;                                                                                                         \
    std::cerr << TESTLIB_ANSI_COLOR_RESET;                                                                             \
    runner->failCurrentTest();                                                                                         \
    return;                                                                                                            \
} while (0)

/** Precision for checking equality in ASSERT_DOUBLE_EQUALS(actualDouble, expectedDouble). **/
#define TESTLIB_EPS 1e-9

/**
 * Checks if a number is roughly equal to zero.
 * @param[in] x number to check
 * @return true, if number is almost a zero, false otherwise.
 */
static inline bool isZero(double x) {
    return fabs(x) < TESTLIB_EPS;
}

/** Helper macro for printing message on equality failure. **/
#define TESTLIB_ASSERT_EQUALS_FAILURE_MESSAGE(expected, actual)                                                        \
    std::cerr << "\tEXPECTED : " << (expected) << '\n'                                                                 \
              << "\tACTUAL   : " << (actual)   << '\n'

//----------------------------------------------------------------------------------------------------------------------

/** Asserts if condition is true. **/
#define ASSERT_TRUE(condition) ASSERT_TRUE_WITH_FAILURE(condition, )

/** Asserts if condition is true. Runs additional code if condition failed.
 *
 * @param[in] onFailure code that is executed on condition failure
 */
#define ASSERT_TRUE_WITH_FAILURE(condition, onFailure) if (!(condition)) TESTLIB_ASSERT_FAILED(onFailure)

/**
 * Asserts if two expressions are equal.
 *
 * @param[in] actual   expression to check
 * @param[in] expected expected result of the expression
 */
#define ASSERT_EQUALS(actual, expected)                                                                                \
    ASSERT_TRUE_WITH_FAILURE(                                                                                          \
        (actual) == (expected),                                                                                        \
        TESTLIB_ASSERT_EQUALS_FAILURE_MESSAGE(expected, actual)                                                        \
    )

/**
 * Asserts if two expressions of type double are equal. Uses isZero(double x) function to check equality.
 *
 * @see isZero(double x)
 *
 * @param actualDouble   expression to check
 * @param expectedDouble expected result of the expression
 */
#define ASSERT_DOUBLE_EQUALS(actualDouble, expectedDouble) do {                                                        \
    const double actualResult   = (actualDouble);                                                                      \
    const double expectedResult = (expectedDouble);                                                                    \
    ASSERT_TRUE_WITH_FAILURE(                                                                                          \
        isZero(actualResult - expectedResult),                                                                         \
        TESTLIB_ASSERT_EQUALS_FAILURE_MESSAGE(expectedResult, actualResult)                                            \
    );                                                                                                                 \
} while (0)

#endif //QUADRATIC_EQUATION_TESTLIB_H
