import unittest

class TestSum(unittest.TestCase):
    def test_list_int(self):
        """
        Test that it can sum a list of integers
        """
        #!COMMENT please edit this portion to have a passing test and a failing test
        #!COMMENT please edit this portion to have a passing test and a failing test
        #!COMMENT please edit this portion to have a passing test and a failing test
        #!COMMENT please edit this portion to have a passing test and a failing test
        #!COMMENT please edit this portion to have a passing test and a failing test
        
        #!CUT_START
        data = [1, 2, 3]
        result = sum(data)
        self.assertEqual(result, 6)
        #!CUT_END
if __name__ == '__main__':
    unittest.main()