import unittest
import io
import sys
from doubly_linked_list import DoublyLinkedList, Node

class TestDoublyLinkedList(unittest.TestCase):
    def setUp(self):
        self.dll = DoublyLinkedList()

    def test_initialization(self):
        self.assertIsNone(self.dll.head)
        self.assertIsNone(self.dll.tail)
        self.assertEqual(list(self.dll), [])

    def test_append(self):
        self.dll.append(1)
        self.assertEqual(self.dll.head.data, 1)
        self.assertEqual(self.dll.tail.data, 1)

        self.dll.append(2)
        self.assertEqual(self.dll.head.data, 1)
        self.assertEqual(self.dll.tail.data, 2)
        self.assertEqual(self.dll.head.next.data, 2)
        self.assertEqual(self.dll.tail.prev.data, 1)

    def test_prepend(self):
        self.dll.prepend(1)
        self.assertEqual(self.dll.head.data, 1)
        self.assertEqual(self.dll.tail.data, 1)

        self.dll.prepend(0)
        self.assertEqual(self.dll.head.data, 0)
        self.assertEqual(self.dll.tail.data, 1)
        self.assertEqual(self.dll.head.next.data, 1)
        self.assertEqual(self.dll.tail.prev.data, 0)

    def test_iteration(self):
        self.dll.append(10)
        self.dll.append(20)
        self.dll.append(30)
        self.assertEqual(list(self.dll), [10, 20, 30])

    def test_reverse_iteration(self):
        self.dll.append(10)
        self.dll.append(20)
        self.dll.append(30)
        self.assertEqual(list(self.dll.reverse_iter()), [30, 20, 10])

    def test_display(self):
        self.dll.append(10)
        self.dll.append(20)
        self.dll.append(30)

        captured_output = io.StringIO()
        sys.stdout = captured_output
        self.dll.display()
        sys.stdout = sys.__stdout__

        self.assertEqual(captured_output.getvalue().strip(), "10 <-> 20 <-> 30")

if __name__ == '__main__':
    unittest.main()

