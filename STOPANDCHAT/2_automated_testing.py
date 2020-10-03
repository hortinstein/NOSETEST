
# Writing tests in this way is okay for a simple check, but what if more than one fails? This is where test runners come in. 
# The test runner is a special application designed for running tests, checking the output, 
# and giving you tools for debugging and diagnosing tests and applications.

# Choosing a Test Runner
# There are many test runners available for Python. The one built into the Python standard library is called unittest. 
# In this tutorial, you will be using unittest test cases and the unittest test runner. 
# The principles of unittest are easily portable to other frameworks. The three most popular test runners are:

#   unittest
#   nose or nose2 [this is pure irony for out course]
#   pytest
#   Choosing the best test runner for your requirements and level of experience is important.

# unittest
# unittest has been built into the Python standard library since version 2.1. We will be using it

# unittest contains both a testing framework and a test runner. unittest has some important requirements for writing and executing tests.

# unittest requires that:

# You put your tests into classes as methods
# You use a series of special assertion methods in the unittest.TestCase class instead of the built-in assert statement
# To convert the earlier example to a unittest test case, you would have to:
#   Import unittest from the standard library
#   Create a class called TestSum that inherits from the TestCase class
#   Convert the test functions into methods by adding self as the first argument
#   Change the assertions to use the self.assertEqual() method on the TestCase class
#   Change the command-line entry point to call unittest.main()
#   Follow those steps by creating a new file test_sum_unittest.py with the following code:

import unittest

class TestSum(unittest.TestCase):

    def test_sum(self):
        self.assertEqual(sum([1, 2, 3]), 6, "Should be 6")

    def test_sum_tuple(self):
        self.assertEqual(sum((1, 2, 2)), 6, "Should be 6")

if __name__ == '__main__':
    unittest.main()


