import unittest

class Roman:
    def to_roman(self, n):
        if n == 1:
            result = "I"
        return result

class TestStringMethods(unittest.TestCase):

    def test_roman(self):
        roman = Roman()
        result = roman.to_roman(1)
        self.assertEqual("I", result)




if __name__ == '__main__':
    unittest.main()