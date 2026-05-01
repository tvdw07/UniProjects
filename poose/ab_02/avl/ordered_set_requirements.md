# Anforderungen an eine Geordnete Menge (Ordered Set)
- **Balance**: Der AVL-Baum balanciert sich nach jedem strukturellen Update (Einfügen, Löschen) automatisch aus. Die Höhendifferenz der beiden Teilbäume eines jeden C-Knotens darf stets maximal 1 betragen.
- **Leistung**: Die Suche, Einfügung und Löschung sollen in balancierter logarithmischer Zeit $O(\log n)$ erfolgen.
- **Eindeutigkeit**: Es dürfen keine Duplikate in der geordneten Menge enthalten sein. Ist ein Element bereits vorhanden, soll `insert` abgewiesen werden.
- **Vergleichbarkeit**: Die Elemente müssen vergleichbar sein (Comparable), damit eine Ordnung hergestellt werden kann.
## Nebenbedingungen (Constraints)

```
    AVLTree *-- Node
    OrderedSet <|.. AVLTree

    }
        +height: int
        +right: Node~T~
        +left: Node~T~
        +value: T
    class Node~T~ {

    }
        +isEmpty() bool
        +size() int
        +findMax() T
        +findMin() T
        +contains(element: T) bool
        +remove(element: T) bool
        +insert(element: T) bool
        -size: int
        -root: Node~T~
    class AVLTree~T~ {

    }
        +isEmpty() bool
        +size() int
        +findMax() T
        +findMin() T
        +contains(element: T) bool
        +remove(element: T) bool
        +insert(element: T) bool
        <<interface>>
    class OrderedSet~T~ {
classDiagram
```mermaid

## UML-Diagramm

Eine Geordnete Menge (Ordered Set) speichert eindeutige Elemente in einer definierten Reihenfolge (z. B. aufsteigend). 


