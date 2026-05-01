#pragma once

#include <memory>

namespace data_structures {

// A BinaryTreeNode stores its parent and its two children
// Diese Verwendung von std::enable_shared_from_this ist nötig, siehe Vorlesungsfolien.
class BinaryTreeNode : public std::enable_shared_from_this<BinaryTreeNode> {
public:
  // Introduce the very telling names BinaryTreeNodeSharedPtr and BinaryTreeNodeWeakPtr
  // They are abbriviations for "std::shared_ptr<BinaryTreeNode>" and "std::weak_ptr<BinaryTreeNode>"
  using BinaryTreeNodeSharedPtr = std::shared_ptr<BinaryTreeNode>;
  using BinaryTreeNodeWeakPtr   = std::weak_ptr<BinaryTreeNode>;
  // Introduce a name for the type of the data we store
  using data_t = int64_t;

  // Constructs a Tree Node with the given data
  BinaryTreeNode(data_t data, BinaryTreeNodeWeakPtr parent = BinaryTreeNodeWeakPtr());

  // Returns the data
  const data_t get_data() const;

  // Returns the data of the (current) left child / right child
  const data_t get_left_child_data() const;
  const data_t get_right_child_data() const;

  // Returns a Shared Pointer to the (current) parent / left child / right child
  BinaryTreeNodeWeakPtr get_parent();
  BinaryTreeNodeSharedPtr get_left_child();
  BinaryTreeNodeSharedPtr get_right_child();

  // Create new left child / right child
  void create_left_child(data_t data);
  void create_right_child(data_t data);

  // Sets given node as left child / right child
  void set_left_child(const BinaryTreeNodeSharedPtr &left_child);
  void set_right_child(const BinaryTreeNodeSharedPtr &right_child);

  // Deletes given left child / right child
  void del_left_child();
  void del_right_child();

private:
  BinaryTreeNodeWeakPtr _parent;
  BinaryTreeNodeSharedPtr _left_child;
  BinaryTreeNodeSharedPtr _right_child;
  const int _data;
};


// Introduce the very telling names BinaryTreeNodeSharedPtr and BinaryTreeNodeWeakPtr
// They are abbriviations for "std::shared_ptr<BinaryTreeNode>" and "std::weak_ptr<BinaryTreeNode>"
using BinaryTreeNodeSharedPtr = BinaryTreeNode::BinaryTreeNodeSharedPtr;
using BinaryTreeNodeWeakPtr   = BinaryTreeNode::BinaryTreeNodeWeakPtr;

}
