#!COMMENT Warmup Exercise #3
# Writing Your First Test
# Let’s bring together what you’ve learned so far and, instead of testing the built-in sum() function, 
# test a simple implementation of the same requirement.

# Create a new project folder and, inside that, create a new folder called my_sum. Inside my_sum, 
# create an empty file called __init__.py. Creating the __init__.py file means that the my_sum folder 
# can be imported as a module from the parent directory.

# Your project folder should look like this:

# project/
# │
# └── my_sum/
#     └── __init__.py
# Open up my_sum/__init__.py and create a new function called sum(), which takes an 
# iterable (a list, tuple, or set) and adds the values together:

def sum(arg):
    total = 0
    for val in arg:
        total += val
    return total
# This code example creates a variable called total, iterates over all the values in arg,
# and adds them to total. It then returns the result once the iterable has been exhausted.

# How to Structure a Simple Test
# Before you dive into writing tests, you’ll want to first make a couple of decisions:

# What do you want to test?
# Are you writing a unit test or an integration test?
# Then the structure of a test should loosely follow this workflow:

# Create your inputs
# Execute the code being tested, capturing the output
# Compare the output with an expected result
# For this application, you’re testing sum(). There are many behaviors in sum() you could check, such as:

#   Can it sum a list of whole numbers (integers)?
#   Can it sum a tuple or set?
#   Can it sum a list of floats?
#   What happens when you provide it with a bad value, such as a single integer or a string?
#   What happens when one of the values is negative?
#   The most simple test would be a list of integers. Create a file, test.py with the following Python code:

import unittest

class TestSum(unittest.TestCase):
    def test_list_int(self):
        """
        Test that it can sum a list of integers
        """
        data = [1, 2, 3]
        result = sum(data)
        self.assertEqual(result, 6)

if __name__ == '__main__':
    unittest.main()

# How to Write Assertions
# The last step of writing a test is to validate the output against a known response. 
# This is known as an assertion. There are some general best practices around how to write assertions:

# Make sure tests are repeatable and run your test multiple times to make sure it gives the same result every time
# Try and assert results that relate to your input data, such as checking that the result is the actual sum of values in the sum() example
# unittest comes with lots of methods to assert on the values, types, and existence of variables. Here are some of the most commonly used methods:

# Method	Equivalent to
# .assertEqual(a, b)	a == b
# .assertTrue(x)	bool(x) is True
# .assertFalse(x)	bool(x) is False
# .assertIs(a, b)	a is b
# .assertIsNone(x)	x is None
# .assertIn(a, b)	a in b
# .assertIsInstance(a, b)	isinstance(a, b)
# .assertIs(), .assertIsNone(), .assertIn(), and .assertIsInstance() all have opposite methods, named .assertIsNot(), and so forth.

# Side Effects
# When you’re writing tests, it’s often not as simple as looking at the return value of a function. Often, executing a piece of code 
# will alter other things in the environment, such as the attribute of a class, a file on the filesystem, or a value in a database. 
# These are known as side effects and are an important part of testing. Decide if the side effect is being tested before including 
# it in your list of assertions.

# If you find that the unit of code you want to test has lots of side effects, you might be breaking the Single Responsibility Principle. 
# Breaking the Single Responsibility Principle means the piece of code is doing too many things and would be better off being refactored. 
# Following the Single Responsibility Principle is a great way to design code that it is easy to write repeatable and simple unit tests 
# for, and ultimately, reliable applications.
# Run the tests:
#   $ python -m unittest test
# You can provide additional options to change the output. One of those is -v for verbose. Try that next:
#   $ python -m unittest -v test
# Instead of providing the name of a module containing tests, you can request an auto-discovery using the following:
# $ python -m unittest discover
# This will search the current directory for any files named test*.py and attempt to test them.

