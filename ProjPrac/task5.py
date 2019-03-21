import unittest
from parameterized import parameterized


class DynamicConnectivity:
    def __init__(self, n):
        self.n = n
        self.connections = [[]]

    def union(self, a, b):
        if a < 0 or b < 0 or a >= self.n or b >= self.n:
            return

        found = False
        for connection in self.connections:
            if (a in connection) or (b in connection):
                found = True
                if not(a in connection):
                    connection.append(a)
                if not(b in connection):
                    connection.append(b)

        if not(found):
            self.connections.append([a, b])


    def connected(self, a, b):
        for connection in self.connections:
            if (a in connection) and (b in connection):
                return True
        return False


class TestMethods(unittest.TestCase):

    @parameterized.expand([
        (10, 0, 1, 0, 1, True),
        (10, 0, 2, 0, 1, False),
        (10, 9, 10, 9, 10, False),
        (10, 0, -1, 0, -1, False)
    ])
    def test_one_connection(self, n, a, b, c, d, expected):
        DC = DynamicConnectivity(n)
        DC.union(a, b)
        self.assertEqual(expected, DC.connected(c, d))

    # from specified site
    def test_multi_connection_0(self):
        n = 10
        DC = DynamicConnectivity(n)
        DC.union(4, 3)
        DC.union(3, 8)
        DC.union(6, 5)
        DC.union(9, 4)
        DC.union(2, 1)
        self.assertEqual(False, DC.connected(0, 7))
        self.assertEqual(True, DC.connected(8, 9))

    def test_multi_connection_1(self):
        n = 10
        DC = DynamicConnectivity(n)
        DC.union(-1, 0)
        DC.union(0, 1)
        DC.union(2, 3)
        DC.union(4, 5)
        DC.union(6, 7)
        DC.union(8, 9)
        DC.union(9, 10)
        self.assertEqual(False, DC.connected(-1, 1))
        self.assertEqual(False, DC.connected(9, 10))


if __name__ == '__main__':
    unittest.main()