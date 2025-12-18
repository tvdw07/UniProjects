#pragma once

#include <memory>

#include <data_structures/binary_tree_node.hpp>

namespace data_structures {

class BinaryTree{
public:
  BinaryTree();

  // Get the root
  const BinaryTreeNodeSharedPtr get_root() const;

  // Create / Set the root
  void create_root(BinaryTreeNode::data_t data);
  void set_root(const BinaryTreeNodeSharedPtr &new_root_node);

  // Print the binary tree
  void print_preorder() const;
  void print_inorder() const;
  void print_postorder() const;
  void print_levelorder() const;

private:
  BinaryTreeNodeSharedPtr _root;
};

}
