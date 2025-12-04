#include <data_structures/binary_tree_node.hpp>

namespace data_structures {

BinaryTreeNode::BinaryTreeNode(const int data)
  : data_(data), left_child(nullptr), right_child(nullptr) {}

int BinaryTreeNode::get_data() const {
  return data_;
}

void BinaryTreeNode::set_data(const int data) {
  data_ = data;
}

BinaryTreeNode::BinaryTreeNodeSharedPtr BinaryTreeNode::get_left_child() const {
  return left_child;
}

BinaryTreeNode::BinaryTreeNodeSharedPtr BinaryTreeNode::get_right_child() const {
  return right_child;
}

void BinaryTreeNode::set_left_child(int data) {
  if (left_child == nullptr) {
    left_child = std::make_shared<BinaryTreeNode>(data);
  } else {
    left_child->set_data(data);
  }
}

void BinaryTreeNode::set_right_child(int data) {
  if (right_child == nullptr) {
    right_child = std::make_shared<BinaryTreeNode>(data);
  } else {
    right_child->set_data(data);
  }
}

void BinaryTreeNode::remove_left_child() {
  left_child = nullptr;
}

void BinaryTreeNode::remove_right_child() {
  right_child = nullptr;
}

}
