add(x: int, y: int) : int = x + y

fibonacci(x : int)
    if x <= 1
        return n
    else
        return add(fibonacci(n-1), fibonacci(n-2))

xs = [fibonacci(x) for x in [1, 2..10]]