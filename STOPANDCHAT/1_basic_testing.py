#!COMMENT Warmup Exercise #1
# Testing Your Code
# There are many ways to test your code. In this tutorial, you’ll learn the techniques from the most basic steps and work towards advanced methods.

# Automated vs. Manual Testing
# The good news is, you’ve probably already created a test without realizing it. Remember when you ran your application and used it for the first time? 
# Did you check the features and experiment using them? That’s known as exploratory testing and is a form of manual testing.

# Exploratory testing is a form of testing that is done without a plan. In an exploratory test, you’re just exploring the application.

# To have a complete set of manual tests, all you need to do is make a list of all the features your application has, the different types 
# of input it can accept, and the expected results. Now, every time you make a change to your code, you need to go through every single item on that list and check it.

# That doesn’t sound like much fun, does it?

# This is where automated testing comes in. Automated testing is the execution of your test plan (the parts of your application you want to test, 
# the order in which you want to test them, and the expected responses) by a script instead of a human. Python already comes with a set of tools
#  and libraries to help you create automated tests for your application. We’ll explore those tools and libraries in this tutorial.

# Unit Tests vs. Integration Tests
# The world of testing has no shortage of terminology, and now that you know the difference between automated and manual testing, it’s time to go a level deeper.

# Think of how you might test the lights on a car. You would turn on the lights (known as the test step) and go outside the car or 
# ask a friend to check that the lights are on (known as the test assertion). Testing multiple components is known as integration testing.

# Think of all the things that need to work correctly in order for a simple task to give the right result. These components are like the parts to your 
# application, all of those classes, functions, and modules you’ve written.

# A major challenge with integration testing is when an integration test doesn’t give the right result. It’s very hard to diagnose the issue without being 
# able to isolate which part of the system is failing. If the lights didn’t turn on, then maybe the bulbs are broken. Is the battery dead? 
# What about the alternator? Is the car’s computer failing?

# If you have a fancy modern car, it will tell you when your light bulbs have gone. It does this using a form of unit test.

# A unit test is a smaller test, one that checks that a single component operates in the right way. A unit test helps you to isolate what is
# broken in your application and fix it faster.

# You have just seen two types of tests:

# An integration test checks that components in your application operate with each other.
# A unit test checks a small component in your application.
# You can write both integration tests and unit tests in Python. To write a unit test for the built-in function sum(), you would 
# check the output of sum() against a known output.

# For example, here’s how you check that the sum() of the numbers (1, 2, 3) equals 6:
assert sum([1, 2, 3]) == 6 , "Should be 6"
# This will not output anything on the REPL because the values are correct.

# statement again with the wrong values to see an AssertionError:

assert sum([1, 1, 1]) == 6, "Should be 6"

def test_sum():
     assert sum([1, 2, 3]) == 6, "Should be 6"

def test_sum_tuple():
    assert sum((1, 2, 2)) == 6, "Should be 6"

# When you execute test_sum_2.py, the script will give an error because the sum() of (1, 2, 2) is 5, not 6. 

if __name__ == "__main__":
     test_sum()
     test_sum_tuple()
     print("Everything passed")




