import math
import sys


def main():
    n = int(input("Gib n ein: "))
    if isinstance(n,int) and n > 0:
        fakulteat = 1

        while n > 1:
            fakulteat *= n
            n -= 1

        sys.set_int_max_str_digits(int(math.floor(math.log10(abs(fakulteat)) + 1)))
        print(f"Die Fakultät beträgt: {fakulteat}")




if __name__ == "__main__":
    main()