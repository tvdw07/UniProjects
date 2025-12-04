#pragma once

#include <memory>

#include <data_structures/binary_tree_node.hpp>

namespace data_structures {

class BinaryTree {
public:
  // Constructs a Binary Tree with a root that stores the given data
  BinaryTree(int root_data);

  // Returns a Shares Pointer to the current root
  BinaryTreeNodeSharedptr get_root() const;

  // Print the current Binary Tree (somehow, e.g., BFS)
  void print() const;

private:
  BinaryTreeNodeSharedptr root; // root node
};


}
