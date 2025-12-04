#include <data_structures/binary_tree.hpp>
#include <iostream>
#include <queue>

namespace data_structures {

BinaryTree::BinaryTree(int root_data) {
  root = std::make_shared<BinaryTreeNode>(root_data);
}

BinaryTreeNodeSharedptr BinaryTree::get_root() const {
  return root;
}

void BinaryTree::print() const {
  if (root == nullptr) {
    std::cout << "Empty tree" << std::endl;
    return;
  }

  std::cout << "Binary Tree (BFS):" << std::endl;

  // Breadth-First Search (BFS) using a queue
  std::queue<BinaryTreeNodeSharedptr> q;
  q.push(root);

  int level = 0;

  while (!q.empty()) {
    const int level_size = q.size();
    std::cout << "Level " << level << ": ";

    for (int i = 0; i < level_size; i++) {
      BinaryTreeNodeSharedptr current = q.front();
      q.pop();

      if (current != nullptr) {
        std::cout << current->get_data();
        if (i < level_size - 1) {
          std::cout << " ";
        }

        // Add children to queue
        if (current->get_left_child() != nullptr) {
          q.push(current->get_left_child());
        }
        if (current->get_right_child() != nullptr) {
          q.push(current->get_right_child());
        }
      }
    }
    std::cout << std::endl;
    level++;
  }
}

}

