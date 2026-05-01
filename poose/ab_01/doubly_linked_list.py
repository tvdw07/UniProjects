class Node:
    """Ein Knoten in einer doppelt verketteten Liste."""
    def __init__(self, data):
        self.data = data
        self.prev = None
        self.next = None
class DoublyLinkedList:
    """Eine doppelt verkettete Liste."""
    def __init__(self):
        self.head = None
        self.tail = None
    def append(self, data):
        """Fuegt ein Element am Ende der Liste hinzu."""
        new_node = Node(data)
        if self.head is None:
            self.head = new_node
            self.tail = new_node
        else:
            self.tail.next = new_node
            new_node.prev = self.tail
            self.tail = new_node
    def prepend(self, data):
        """Fuegt ein Element am Anfang der Liste hinzu."""
        new_node = Node(data)
        if self.head is None:
            self.head = new_node
            self.tail = new_node
        else:
            new_node.next = self.head
            self.head.prev = new_node
            self.head = new_node
    def __iter__(self):
        """Gibt einen Iterator zurueck, der die Liste von vorne nach hinten durchlaeuft."""
        current = self.head
        while current:
            yield current.data
            current = current.next
    def reverse_iter(self):
        """Gibt einen Iterator zurueck, der die Liste von hinten nach vorne durchlaeuft."""
        current = self.tail
        while current:
            yield current.data
            current = current.prev
    def display(self):
        """Gibt die Liste auf der Standardausgabe aus."""
        elements = [str(data) for data in self]
        print(" <-> ".join(elements))
