
def main():
    n = input("input n: ")
    if n.startswith("Bin broke"):

        if n.startswith("Bin broke, weil Schischa"):
            print("Wyld")
        else:
            print("Digga")

    else:

        if len(n) == 9:
            print("Auf Lock")
        else:
            print("Zwambo")


if __name__ == "__main__":
    main()