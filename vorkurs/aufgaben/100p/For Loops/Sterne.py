def main():
    var = [1, 2, 3, 4, 5, 4, 3, 2, 1]

    for i in var:
        for j in range(var[i-1]):
            print("*", end="")
        print("")

if __name__ == "__main__":
    main()