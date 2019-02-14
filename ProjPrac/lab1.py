import math
import unittest

def solve(a, b, c):

    if a == 0:
        if b == 0:
            if c == 0:
                #print("Бесконечное множество решений")
                return (True, 0, 0)
            else:
                #print("Нет решений")
                return (False, 0, 0)
        else:
            x = -c/b
            return (True, x, x)

    D = b*b - 4*a*c

    if D < 0:
        #print("Нет корней")
        return (False, 0, 0)
    elif D == 0:
        #print("Один корень")
        x = -b/(2*a)
        return (True, x, x)

    D = math.sqrt(D)

    return True, (-b - D) / (2 * a), (-b + D) / (2 * a)

class TestStringMethods(unittest.TestCase):

    def test_abc_equal_zero(self):
        actual_result = solve(0, 0, 0)
        self.assertEqual((True, 0, 0), actual_result)

    def test_ab_equal_zero(self):
        actual_result = solve(0, 0, 1)
        self.assertEqual((False, 0, 0), actual_result)

    def test_a_equal_zero(self):
        actual_result = solve(0, 1, 5)
        self.assertEqual((True, -5, -5), actual_result)

    def test_negative_D(self):
        actual_result = solve(5, 1, 10)
        self.assertEqual((False, 0, 0), actual_result)

    def test_D_zero(self):
        actual_result = solve(1, 2, 1)
        self.assertEqual((True, -1, -1), actual_result)

    def test_positive_D(self):
        actual_result = solve(1, 3, 2)
        self.assertEqual((True, -2, -1), actual_result)

if __name__ == '__main__':
    unittest.main()