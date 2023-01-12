import time

def one_to_mil_for():
    for i in range(1000000):
        result = 10
    return 0

def time_func(func):
    start_time = time.time() * 1000
    result = func()
    end_time = time.time() * 1000
    print(f"Executed in {end_time - start_time}ms")
    return result

def main(func):
    result = time_func(func)
    return result

main(one_to_mil_for)