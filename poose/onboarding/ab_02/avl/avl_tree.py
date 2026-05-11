from typing import Generic, TypeVar, Optional

T = TypeVar('T')

class AVLNode(Generic[T]):
    """Repräsentiert einen Knoten im AVL-Baum."""
    def __init__(self, value: T):
        self.value: T = value
        self.left: Optional['AVLNode[T]'] = None
        self.right: Optional['AVLNode[T]'] = None
        self.height: int = 1

class AVLTree(Generic[T]):
    """Ein AVL-Baum, der das Interface einer Geordneten Menge (Ordered Set) implementiert."""

    def __init__(self):
        self.root: Optional[AVLNode[T]] = None
        self._size: int = 0

    def insert(self, value: T) -> bool:
        """Fügt einen Wert in die Menge ein. Gibt True zurück, wenn eingefügt, False bei Duplikaten."""
        if self.contains(value):
            return False
        self.root = self._insert(self.root, value)
        self._size += 1
        return True

    def _insert(self, node: Optional[AVLNode[T]], value: T) -> AVLNode[T]:
        if not node:
            return AVLNode(value)

        if value < node.value:
            node.left = self._insert(node.left, value)
        elif value > node.value:
            node.right = self._insert(node.right, value)

        node.height = 1 + max(self._get_height(node.left), self._get_height(node.right))

        balance = self._get_balance(node)

        # Left Left Case
        if balance > 1 and node.left and value < node.left.value:
            return self._right_rotate(node)

        # Right Right Case
        if balance < -1 and node.right and value > node.right.value:
            return self._left_rotate(node)

        # Left Right Case
        if balance > 1 and node.left and value > node.left.value:
            node.left = self._left_rotate(node.left)
            return self._right_rotate(node)

        # Right Left Case
        if balance < -1 and node.right and value < node.right.value:
            node.right = self._right_rotate(node.right)
            return self._left_rotate(node)

        return node

    def remove(self, value: T) -> bool:
        """Entfernt einen Wert aus der Menge. Gibt True zurück, wenn erfolgreich, False wenn nicht gefunden."""
        if not self.contains(value):
            return False
        self.root = self._remove(self.root, value)
        self._size -= 1
        return True

    def _remove(self, node: Optional[AVLNode[T]], value: T) -> Optional[AVLNode[T]]:
        if not node:
            return node

        if value < node.value:
            node.left = self._remove(node.left, value)
        elif value > node.value:
            node.right = self._remove(node.right, value)
        else:
            if node.left is None:
                return node.right
            elif node.right is None:
                return node.left

            temp = self._get_min_value_node(node.right)
            node.value = temp.value
            node.right = self._remove(node.right, temp.value)

        if node is None:
            return node

        node.height = 1 + max(self._get_height(node.left), self._get_height(node.right))
        balance = self._get_balance(node)

        # Left Left
        if balance > 1 and self._get_balance(node.left) >= 0:
            return self._right_rotate(node)

        # Left Right
        if balance > 1 and self._get_balance(node.left) < 0:
            if node.left:
                node.left = self._left_rotate(node.left)
            return self._right_rotate(node)

        # Right Right
        if balance < -1 and self._get_balance(node.right) <= 0:
            return self._left_rotate(node)

        # Right Left
        if balance < -1 and self._get_balance(node.right) > 0:
            if node.right:
                node.right = self._right_rotate(node.right)
            return self._left_rotate(node)

        return node

    def contains(self, value: T) -> bool:
        """Prüft, ob ein Wert in der Menge existiert."""
        return self._search(self.root, value) is not None

    def _search(self, node: Optional[AVLNode[T]], value: T) -> Optional[AVLNode[T]]:
        if not node or node.value == value:
            return node
        if node.value < value:
            return self._search(node.right, value)
        return self._search(node.left, value)

    def find_min(self) -> Optional[T]:
        """Gibt das minimale Element in der Menge zurück oder None, falls sie leer ist."""
        if not self.root:
            return None
        return self._get_min_value_node(self.root).value

    def find_max(self) -> Optional[T]:
        """Gibt das maximale Element in der Menge zurück oder None, falls sie leer ist."""
        if not self.root:
            return None
        node = self.root
        while node.right is not None:
            node = node.right
        return node.value

    def size(self) -> int:
        """Gibt die aktuelle Anzahl der Elemente zurück."""
        return self._size

    def is_empty(self) -> bool:
        """Prüft, ob die Menge leer ist."""
        return self._size == 0

    def _get_height(self, node: Optional[AVLNode[T]]) -> int:
        if not node:
            return 0
        return node.height

    def _get_balance(self, node: Optional[AVLNode[T]]) -> int:
        if not node:
            return 0
        return self._get_height(node.left) - self._get_height(node.right)

    def _left_rotate(self, z: AVLNode[T]) -> AVLNode[T]:
        y = z.right
        if not y:
            return z
        T2 = y.left
        y.left = z
        z.right = T2
        z.height = 1 + max(self._get_height(z.left), self._get_height(z.right))
        y.height = 1 + max(self._get_height(y.left), self._get_height(y.right))
        return y

    def _right_rotate(self, z: AVLNode[T]) -> AVLNode[T]:
        y = z.left
        if not y:
            return z
        T3 = y.right
        y.right = z
        z.left = T3
        z.height = 1 + max(self._get_height(z.left), self._get_height(z.right))
        y.height = 1 + max(self._get_height(y.left), self._get_height(y.right))
        return y

    def _get_min_value_node(self, node: AVLNode[T]) -> AVLNode[T]:
        current = node
        while current.left is not None:
            current = current.left
        return current

