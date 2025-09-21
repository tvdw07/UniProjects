
def getPrimeNumbersLessThanN(n):
    """Returns a list of prime numbers less than n."""
    if n <= 2:
        return []

    primes = []
    is_prime = [True] * n
    is_prime[0] = is_prime[1] = False  # 0 and 1 are not prime numbers

    for p in range(2, n):
        if is_prime[p]:
            primes.append(p)
            for multiple in range(p * p, n, p):
                is_prime[multiple] = False

    return primes

def main():
    n = int(input("Enter a number: "))
    primes = getPrimeNumbersLessThanN(n)
    print(f"Prime numbers less than {n}: {primes}")

if __name__ == "__main__":
    main()