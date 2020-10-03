
def sum(arg):
    total = 0
    for val in arg:
        total += val
    return total

# Understanding Test Output
# That was a very simple example where everything passes, so now you’re going to try a failing test and interpret the output.

# sum() should be able to accept other lists of numeric types, like fractions.

# At the top of the test.py file, add an import statement to import the Fraction type from the fractions module in the standard library:

# Now add a test with an assertion expecting the incorrect value, in this case expecting the sum of 1/4, 1/4, and 2/5 to be 1:

import unittest
from fractions import Fraction

class TestSum(unittest.TestCase):
    def test_list_int(self):
        """
        Test that it can sum a list of integers
        """
        data = [1, 2, 3]
        result = sum(data)
        self.assertEqual(result, 6)

    def test_list_fraction(self):
        """
        Test that it can sum a list of fractions
        """
        data = [Fraction(1, 4), Fraction(1, 4), Fraction(2, 5)]
        result = sum(data)
        self.assertEqual(result, 1)

if __name__ == '__main__':
    unittest.main()
# If you execute the tests again with python -m unittest test, you should see the following output:

# $ python -m unittest test
# F.
# ======================================================================
# FAIL: test_list_fraction (test.TestSum)
# ----------------------------------------------------------------------
# Traceback (most recent call last):
#   File "test.py", line 21, in test_list_fraction
#     self.assertEqual(result, 1)
# AssertionError: Fraction(9, 10) != 1

# ----------------------------------------------------------------------
# Ran 2 tests in 0.001s

# FAILED (failures=1)
# In the output, you’ll see the following information:

# The first line shows the execution results of all the tests, one failed (F) and one passed (.).

# The FAIL entry shows some details about the failed test:

# The test method name (test_list_fraction)
# The test module (test) and the test case (TestSum)
# A traceback to the failing line
# The details of the assertion with the expected result (1) and the actual result (Fraction(9, 10))
# Remember, you can add extra information to the test output by adding the -v flag to the python -m unittest command.
