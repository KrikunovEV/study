from random import randint
import time
from math import sqrt

current_milli_time = lambda: int(round(time.time() * 1000000))

def Classic(x, y):

    qq = current_milli_time()

    iterations = 0

    mod = x % y
    while mod != 0:
        iterations += 1
        x = y
        y = mod
        mod = x % y

    return y, iterations, current_milli_time() - qq


def Binary(x, y):
    qq = current_milli_time()
    iterations = 0

    k = 1
    while x != 0 and y != 0:
        while x % 2 == 0 and y % 2 == 0:
            x /= 2
            y /= 2
            k *= 2

        while x % 2 == 0:
            x /= 2

        while y % 2 == 0:
            y /= 2

        if x >= y:
            x -= y
        else:
            y -= x

        iterations += 1

    return y * k, iterations, current_milli_time() - qq


def reversed(b, k):
    if b == 1:
        return 1
    for r in range(2, k):
        if (r * b) % k == 1:
            return r
    return -1

def k_algorithm(x, y, k):
    qq = current_milli_time()

    iterations = 0

    max_x = int(sqrt(k)) + 1

    z = k + 1
    while z > k:
        iterations += 1
        a = x % k
        b = y % k

        b = reversed(b, k)
        if b == -1:
            print("reversed b = -1")

        q = (a * b) % k

        for a in range(1, max_x):
            b = -((q * a) % k)
            if -max_x < b < max_x:
                break
            if -max_x < b % k < max_x:
                b %= k
                break

        z = (x * a + y * b) / k

        while z % 2 == 0:
            z /= 2

        #print("x=",a,",y=",b,",C=", z)

        if z < 0:
            z *= -1

        if z == 1:
            return 1, iterations, current_milli_time() - qq

        x = y
        y = z
        if x < y:
            x, y = y, x

        if 0 < y < k:
            NOD, i, tt = Classic(x, y)
            return NOD, iterations, current_milli_time() - qq

def approx_k_algorithm(x, y, k):
    qq = current_milli_time()
    iterations = 0

    z = k + 1
    while z > k:
        iterations += 1
        a = x % k
        b = y % k

        b = reversed(b, k)
        if b == -1:
            print("reversed b = -1")

        q = (a * b) % k
        r = x / y
        alpha = (r - q) / k

        sign = 1
        if alpha < 0:
            sign = -1

        alpha *= sign

        while alpha > 1:
            alpha -= 1

        l = [1, 100]
        r = [99, 100]

        left = False
        while True:
            #print(l[0] / l[1], alpha, r[0] / r[1]) женя лучший
            if abs(alpha - l[0] / l[1]) < 0.5:
                left = True
                break
            if abs(alpha - r[0] / r[1]) < 0.5:
                break

            left_closer = False
            if abs(alpha - l[0] / l[1]) < abs(alpha - r[0] / r[1]):
                left_closer = True

            if left_closer:
                r[0] = l[0] * r[1] + r[0] * l[1]
                r[1] = l[1] * r[1] * 2
                d, i, tt = Classic(r[0], r[1])
                r[0] /= d
                r[1] /= d
            else:
                l[0] = l[0] * r[1] + r[0] * l[1]
                l[1] = l[1] * r[1] * 2
                d, i, tt = Classic(l[0], l[1])
                l[0] /= d
                l[1] /= d

        if left:
            m = l
            m[0] *= sign
            m[1] = sign
        else:
            m = r
            m[0] *= sign
            m[1] = sign

        a = m[1]
        b = -q * a - m[0] * k

        z = (x * a + y * b) / k
        while z % 2 == 0:
            z /= 2

        if z < 0:
            z *= -1

        #print("z=", z)

        if z == 1:
            return 1, iterations, current_milli_time() - qq

        x = y
        y = z
        if x < y:
            x, y = y, x

        if 0 < y < k:
            NOD, i, tt = Classic(x, y)
            return NOD, iterations, current_milli_time() - qq





range_low = 10**10
range_high = 10**11
x, y = randint(range_low, range_high), randint(range_low, range_high)

if x % 2 == 0:
    x -= 1

if y % 2 == 0:
    y -=1

if x < y:
    x, y = y, x

print(x, y)

print("\n(НОД, итераций)")
print(Classic(x, y))
print(Binary(x, y))
print()
print(k_algorithm(x, y, 16))
print(k_algorithm(x, y, 4096))
print()
print(approx_k_algorithm(x, y, 16))
print(approx_k_algorithm(x, y, 4096))