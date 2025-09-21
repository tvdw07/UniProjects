if __name__ == "__main__":
    n = int(input("Gib einen Integer ein:"))
    s = input("Gib einen String ein:")

    if n == 2:
        print("Juhu")
    elif n < 2 and s =="Cyber":
        print("Yeah")
    elif n > 2 or s == "Felix":
        print("Jippi")
    elif s == "Marc":
        print("Heureka")
    else:
        print("Jay")