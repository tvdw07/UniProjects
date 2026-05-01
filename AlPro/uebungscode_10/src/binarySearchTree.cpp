#include <data_structures/binarySearchTree.hpp>
#include <data_structures/binary_tree_node.hpp>

namespace data_structures {
    BinaryTreeNodeSharedPtr BinarySearchTree::get_root() const {
        return _tree.get_root();
    }

    void BinarySearchTree::create_root(const BinaryTreeNode::data_t data) {
        _tree.create_root(data);
    }

    void BinarySearchTree::set_root(const BinaryTreeNodeSharedPtr &new_root_node) {
        _tree.set_root(new_root_node);
    }

    void BinarySearchTree::print_preorder() const {
        _tree.print_preorder();
    }

    void BinarySearchTree::print_inorder() const {
        _tree.print_inorder();
    }

    void BinarySearchTree::print_postorder() const {
        _tree.print_postorder();
    }

    void BinarySearchTree::print_levelorder() const {
        _tree.print_levelorder();
    }


    //ChatGPT used for the comments in this Method
    void BinarySearchTree::insert(const BinaryTreeNode::data_t data) {
        // Get the current root of the tree
        const auto root = _tree.get_root();

        // Case 1: Tree is empty → create the root node
        if (!root) {
            _tree.create_root(data);
            return;
        }

        // Start traversal at the root
        auto current = root;

        // Iterate until we find a free child position
        while (true) {
            // If the new value is smaller, go left
            if (data < current->get_data()) {
                auto left = current->get_left_child();

                // If left child exists, continue traversal
                if (left) {
                    current = left;
                }
                // Otherwise, insert new node as left child
                else {
                    current->create_left_child(data);
                    return;
                }
            }
            // If the new value is greater or equal, go right
            else {
                auto right = current->get_right_child();

                // If right child exists, continue traversal
                if (right) {
                    current = right;
                }
                // Otherwise, insert new node as right child
                else {
                    current->create_right_child(data);
                    return;
                }
            }
        }
    }


    bool BinarySearchTree::find(const BinaryTreeNode::data_t data) const {
        auto current = _tree.get_root();

        // Traverse until we reach a null pointer (value not found)
        while (current) {
            // Value found
            if (data == current->get_data())
                return true;

            // Decide which subtree to search next
            if (data < current->get_data())
                current = current->get_left_child();
            else
                current = current->get_right_child();
        }

        // Reached a leaf without finding the value
        return false;
    }

    // Helper function to find the minimum node in a subtree
    namespace {
        BinaryTreeNodeSharedPtr tree_minimum(BinaryTreeNodeSharedPtr node) {
            // Keep going left until no left child exists
            while (node && node->get_left_child()) node = node->get_left_child();
            return node;
        }
    }

    void BinarySearchTree::remove(const BinaryTreeNode::data_t data)
    {
        //Similar to find but returns the pointer
        //Find the node to delete (z)
        auto z = _tree.get_root();
        while (z && z->get_data() != data) {
            z = (data < z->get_data())
                    ? z->get_left_child()
                    : z->get_right_child();
        }

        //Value not found → nothing to do
        if (!z) return;

        //Replaces node u with node v in the parent
        auto replace_node = [this](const auto& u, const auto& v) {
            auto parent = u->get_parent().lock();

            if (!parent) {
                // u is the root
                _tree.set_root(v);
                return;
            }

            if (parent->get_left_child() == u)
                parent->set_left_child(v);
            else
                parent->set_right_child(v);
        };

        //Case A: no left child
        if (!z->get_left_child()) {
            replace_node(z, z->get_right_child());
            return;
        }

        //Case B: no right child
        if (!z->get_right_child()) {
            replace_node(z, z->get_left_child());
            return;
        }

        //Case C: two children
        const auto successor = tree_minimum(z->get_right_child());

        //Move successor up if it is not z's direct child
        if (successor->get_parent().lock() != z) {
            replace_node(successor, successor->get_right_child());
            successor->set_right_child(z->get_right_child());
        }

        replace_node(z, successor);
        successor->set_left_child(z->get_left_child());
    }
}
