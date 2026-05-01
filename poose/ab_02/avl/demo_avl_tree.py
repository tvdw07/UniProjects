from avl_tree import AVLTree

def main():
    print("=== AVL-Baum als Geordnete Menge ===\n")

    # Baum instanziieren
    tree = AVLTree()
    print("Füge Elemente ein: 15, 10, 20, 5, 12, 18, 25")
    for val in [15, 10, 20, 5, 12, 18, 25]:
        tree.insert(val)

    print(f"Anzahl Elemente: {tree.size()}")
    print(f"Minimum: {tree.find_min()}")
    print(f"Maximum: {tree.find_max()}\n")

    # Duplikat einfügen
    print("Versuche, Duplikat '10' einzufügen:")
    success = tree.insert(10)
    print(f"Einfügen erfolgreich? {success}\n")

    # Element entfernen
    print("Entferne Element '15' (Wurzelebene)...")
    tree.remove(15)
    print(f"Element 15 enthalten? {tree.contains(15)}")

    print(f"Neue Anzahl Elemente: {tree.size()}")
    print(f"Neues Minimum: {tree.find_min()}")
    print(f"Neues Maximum: {tree.find_max()}")

if __name__ == '__main__':
    main()

