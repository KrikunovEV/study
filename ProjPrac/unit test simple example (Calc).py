import unittest
from math import sqrt

class Calc:

    def sum(self, a, b):
        return a + b

    def mul(self, a, b):
        return a * b

    def minus(self, a, b):
        return a - b

    def div(self, a, b):
        return a / b

    def mod(self, a, b):
        return a % b

    def pow(self, a, b):
        return a**b

    def sqrt2(self, a):
        return sqrt(a)

class TestStringMethods(unittest.TestCase):

    def test_sum(self):
       calc = Calc()
       actual_result = calc.sum(1, 2)
       self.assertEqual(3, actual_result)

    def test_mul(self):
        calc = Calc()
        actual_result = calc.mul(5, 8)
        self.assertEqual(40, actual_result)

    def test_minus(self):
        calc = Calc()
        actual_result = calc.minus(10, 9)
        self.assertEqual(1, actual_result)

    def test_div(self):
        calc = Calc()
        actual_result = calc.div(10, 2)
        self.assertEqual(5, actual_result)

    def test_mod(self):
        calc = Calc()
        actual_result = calc.mod(12, 7)
        self.assertEqual(5, actual_result)

    def test_pow(self):
        calc = Calc()
        actual_result = calc.pow(2, 3)
        self.assertEqual(8, actual_result)

    def test_sqrt2(self):
        calc = Calc()
        actual_result = calc.sqrt2(25)
        self.assertEqual(5, actual_result)

if __name__ == '__main__':
    unittest.main()
