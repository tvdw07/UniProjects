from random import randint


def main():
    number = [randint(0, 9), randint(0, 9), randint(0, 9), randint(0, 9)]

    playerguess = input("Make a guess (4 digits): ")
    correct_positions = 0
    wrong_positions = 0

    while True:
        if len(playerguess) != 4 or not playerguess.isdigit():
            print("Please enter exactly four digits.")
            playerguess = input("Make a new guess: ")
            continue

        for i in range(4):
            if int(playerguess[i]) == number[i]:
                correct_positions += 1
            elif int(playerguess[i]) in number:
                wrong_positions += 1

        print(f"You got {correct_positions} Numbers in the correct spot")
        print(f"You got {wrong_positions} Numbers in the wrong position")

        if correct_positions == 4:
            break

        correct_positions = 0
        wrong_positions = 0
        playerguess = input("Make a new guess: ")

    print(f"Wow you did it! The number was {''.join(map(str, number))}")


if __name__ == "__main__":
    main()






if __name__ == "__main__":
    main()