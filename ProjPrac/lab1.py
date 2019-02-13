import math

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

    return (True, (-b-D)/(2*a), (-b+D)/(2*a))

#Test cases
(res, x1, x2) = solve(0, 0, 0)
print("Test case 1:\na = 0, b = 0, c = 0; solve = " + str(res) + ", x1 = " + str(x1) + ", x2 = " + str(x2) + "\n")

(res, x1, x2) = solve(0, 0, 1)
print("Test case 2:\na = 0, b = 0, c = 1; solve = " + str(res) + ", x1 = " + str(x1) + ", x2 = " + str(x2) + "\n")

(res, x1, x2) = solve(0, 1, 5)
print("Test case 3:\na = 0, b = 1, c = 5; solve = " + str(res) + ", x1 = " + str(x1) + ", x2 = " + str(x2) + "\n")

(res, x1, x2) = solve(5, 1, 10)
print("Test case 4:\na = 5, b = 1, c = 10; solve = " + str(res) + ", x1 = " + str(x1) + ", x2 = " + str(x2) + "\n")

(res, x1, x2) = solve(1, 5, 2)
print("Test case 5:\na = 1, b = 5, c = 2; solve = " + str(res) + ", x1 = " + str(x1) + ", x2 = " + str(x2) + "\n")

(res, x1, x2) = solve(1, 2, 1)
print("Test case 6:\na = 1, b = 2, c = 1; solve = " + str(res) + ", x1 = " + str(x1) + ", x2 = " + str(x2) + "\n")