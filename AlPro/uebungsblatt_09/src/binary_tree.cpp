#include <data_structures/binary_tree.hpp>
#include <iostream>
#include <queue>

namespace data_structures {

// Konstruktor
// Bei der Objekterzeugung werdne zuerst die Membervariablen initialisiert,
// dabei wird das Member _root mit dem Wert nullptr initialisiert, das heißt, es ist zunächst keine Wurzel vorhanden,
// das entspricht also dem leeren Baum.
// Anschließend wird der Funktionscode des Konstruktors ausgeführt, dieser ist hier leer,
// da wir neben der Initialisierung nichts weiteres tun wollen.
BinaryTree::BinaryTree()
  : _root(nullptr)
{
}


const BinaryTreeNodeSharedPtr BinaryTree::get_root() const {
  return _root;
}


void BinaryTree::create_root(BinaryTreeNode::data_t data) {
  _root = std::make_shared<BinaryTreeNode>(data);
}


void BinaryTree::set_root(const BinaryTreeNodeSharedPtr &new_root_node) {
  _root = new_root_node;
}

  // Helper function for preorder traversal (Preorder: Wurzel -> Linkes -> Rechtes)
  void preorder_helper(const BinaryTreeNodeSharedPtr &node) {
    if (!node) return;

    std::cout << node->get_data() << " ";
    // Recursive call for left and right child
    preorder_helper(node->get_left_child());
    preorder_helper(node->get_right_child());
  }

  void BinaryTree::print_preorder() const {
    if (!_root) {
      std::cout << "Tree is empty" << std::endl;
      return;
    }

    std::cout << "Preorder: ";
    preorder_helper(_root);
    std::cout << std::endl;
  }

  // Helper function for inorder
  void inorder_helper(const BinaryTreeNodeSharedPtr &node) {
    if (!node) return;

    // Recursive call for left child
    inorder_helper(node->get_left_child());
    std::cout << node->get_data() << " ";
    // Recursive call for right child
    inorder_helper(node->get_right_child());
  }

  void BinaryTree::print_inorder() const {
    if (!_root) {
      std::cout << "Tree is empty" << std::endl;
      return;
    }

    std::cout << "Inorder: ";
    inorder_helper(_root);
    std::cout << std::endl;
  }

  // Helper function for postorder
  void postorder_helper(const BinaryTreeNodeSharedPtr &node) {
    if (!node) return;

    // Recursive call for left and right child
    postorder_helper(node->get_left_child());
    postorder_helper(node->get_right_child());
    std::cout << node->get_data() << " ";
  }

  void BinaryTree::print_postorder() const {
    if (!_root) {
      std::cout << "Tree is empty" << std::endl;
      return;
    }

    std::cout << "Postorder: ";
    postorder_helper(_root);
    std::cout << std::endl;
  }

  // Levelorder traversal using queue very similar to BFS
  void BinaryTree::print_levelorder() const {
    if (!_root) {
      std::cout << "Tree is empty" << std::endl;
      return;
    }

    std::cout << "Levelorder: ";
    std::queue<BinaryTreeNodeSharedPtr> q;
    q.push(_root);

    // While there are nodes to process
    while (!q.empty()) {
      const BinaryTreeNodeSharedPtr current = q.front();
      q.pop();

      std::cout << current->get_data() << " ";

      if (current->get_left_child()) {
        q.push(current->get_left_child());
      }

      if (current->get_right_child()) {
        q.push(current->get_right_child());
      }
    }

    std::cout << std::endl;
  }
}
