import unittest

def swap_digits(n):
    #if n == 119:
    #    result = 911
    #****************
    digits = []
    while n > 0:
        digits.append(n % 10)
        n //= 10

    digits.sort(reverse=True)

    result = 0
    for i in range(0, len(digits)):
        result += digits[-(i+1)] * (10**i)

    return result

def check_brackets(str):
    balance = 0
    for i in str:
        if i == '(':
            balance += 1
        elif i == ')':
            balance -= 1

        if balance < 0:
            return False

    return True

class TestStringMethods(unittest.TestCase):

    def test_swap(self):
        result = swap_digits(119)
        self.assertEqual(911, result)

    def test_swap2(self):
        result = swap_digits(123456)
        self.assertEqual(654321, result)

    def test_brackets(self):
        result = check_brackets("((()())())")
        self.assertEqual(True, result)

    def test_brackets2(self):
        result = check_brackets("())()(")
        self.assertEqual(False, result)

if __name__ == '__main__':
    unittest.main()