import unittest
from avl_tree import AVLTree

class TestAVLTree(unittest.TestCase):
    def test_empty_tree(self):
        tree = AVLTree()
        self.assertTrue(tree.is_empty())
        self.assertEqual(tree.size(), 0)
        self.assertFalse(tree.contains(10))
        self.assertIsNone(tree.find_min())
        self.assertIsNone(tree.find_max())

    def test_single_element(self):
        tree = AVLTree()
        self.assertTrue(tree.insert(10))
        self.assertFalse(tree.is_empty())
        self.assertEqual(tree.size(), 1)
        self.assertTrue(tree.contains(10))
        self.assertEqual(tree.find_min(), 10)
        self.assertEqual(tree.find_max(), 10)

    def test_many_elements(self):
        tree = AVLTree()
        for val in [10, 20, 30, 40, 50, 25]:
            tree.insert(val)
        self.assertEqual(tree.size(), 6)
        self.assertTrue(tree.contains(25))
        self.assertTrue(tree.contains(50))
        self.assertEqual(tree.find_min(), 10)
        self.assertEqual(tree.find_max(), 50)

    def test_balance_left_left(self):
        tree = AVLTree()
        tree.insert(30)
        tree.insert(20)
        tree.insert(10)
        self.assertEqual(tree.root.value, 20)

    def test_balance_right_right(self):
        tree = AVLTree()
        tree.insert(10)
        tree.insert(20)
        tree.insert(30)
        self.assertEqual(tree.root.value, 20)

    def test_balance_left_right(self):
        tree = AVLTree()
        tree.insert(30)
        tree.insert(10)
        tree.insert(20)
        self.assertEqual(tree.root.value, 20)

    def test_balance_right_left(self):
        tree = AVLTree()
        tree.insert(10)
        tree.insert(30)
        tree.insert(20)
        self.assertEqual(tree.root.value, 20)

    def test_set_behavior(self):
        tree = AVLTree()
        self.assertTrue(tree.insert(10))
        self.assertFalse(tree.insert(10))
        self.assertEqual(tree.size(), 1)
        self.assertTrue(tree.remove(10))
        self.assertFalse(tree.remove(10))
        self.assertEqual(tree.size(), 0)

    def test_remove_nonexistent(self):
        tree = AVLTree()
        self.assertFalse(tree.remove(99))

    def test_complex_operations(self):
        tree = AVLTree()
        for i in range(1, 10):
            tree.insert(i)

        self.assertEqual(tree.root.value, 4)
        self.assertTrue(tree.remove(4))
        self.assertEqual(tree.size(), 8)
        self.assertFalse(tree.contains(4))
        self.assertEqual(tree.find_min(), 1)
        self.assertEqual(tree.find_max(), 9)

if __name__ == "__main__":
    unittest.main()
