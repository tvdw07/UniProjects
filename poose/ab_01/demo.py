from doubly_linked_list import DoublyLinkedList

def main():
    print("--- Demo: Doppelt verkettete Liste ---")
    dll = DoublyLinkedList()

    print("\n1. Elemente am Ende hinzufügen (append: 10, 20, 30)")
    dll.append(10)
    dll.append(20)
    dll.append(30)
    dll.display()

    print("\n2. Elemente am Anfang hinzufügen (prepend: 0, -10)")
    dll.prepend(0)
    dll.prepend(-10)
    dll.display()

    print("\n3. Vorwärts-Iteration (__iter__):")
    for item in dll:
        print(item)

    print("\n4. Rückwärts-Iteration (reverse_iter):")
    for item in dll.reverse_iter():
        print(item)

if __name__ == "__main__":
    main()

