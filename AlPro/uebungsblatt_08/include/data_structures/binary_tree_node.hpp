#pragma once

#include <memory>

namespace data_structures {

// A BinaryTreeNode stores its left child and right child and some data
class BinaryTreeNode {
public:
  // Introduce the very telling names BinaryTreeNodeSharedPtr and BinaryTreeNodeWeakPtr
  // They are abbriviations for "std::shared_ptr<BinaryTreeNode>" and "std::weak_ptr<BinaryTreeNode>"
  using BinaryTreeNodeSharedPtr = std::shared_ptr<BinaryTreeNode>;
  using BinaryTreeNodeWeakPtr   = std::weak_ptr<BinaryTreeNode>;

  // Constructs a Tree Node with the given data
  BinaryTreeNode(int data);

  // Returns the data
  int get_data() const;

  // Sets the data
  void set_data(int data);

  // Returns a Shared Pointer to the (current) left child
  BinaryTreeNodeSharedPtr get_left_child() const;

  // Returns a Shared Pointer to the (current) right child
  BinaryTreeNodeSharedPtr get_right_child() const;

  // Sets the value of the left child (and creates the child if necessary)
  void set_left_child(int data);

  // Sets the value of the right child (and creates the child if necessary)
  void set_right_child(int data);

  // Removes the left child (if it exists)
  void remove_left_child();

  // Removes the right child (if it exists)
  void remove_right_child();

private:
  std::shared_ptr<BinaryTreeNode> left_child;
  std::shared_ptr<BinaryTreeNode> right_child;

  int data_;
};


// Introduce the very telling names BinaryTreeNodeSharedPtr and BinaryTreeNodeWeakPtr
// They are abbriviations for "std::shared_ptr<BinaryTreeNode>" and "std::weak_ptr<BinaryTreeNode>"
using BinaryTreeNodeSharedptr = BinaryTreeNode::BinaryTreeNodeSharedPtr;
using BinaryTreeNodeWeakptr   = BinaryTreeNode::BinaryTreeNodeWeakPtr;

}


