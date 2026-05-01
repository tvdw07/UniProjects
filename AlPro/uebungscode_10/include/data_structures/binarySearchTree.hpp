#pragma once

#include <data_structures/binary_tree.hpp>

namespace data_structures {

    class BinarySearchTree {
    public:
        BinarySearchTree() = default;

        // Zugriff (read-only) auf den Baum
        const BinaryTree& tree() const { return _tree; }

        // Root-Operationen (delegiert)
        BinaryTreeNodeSharedPtr get_root() const;
        void create_root(BinaryTreeNode::data_t data);
        void set_root(const BinaryTreeNodeSharedPtr& new_root_node);

        // Traversierung (delegiert)
        void print_preorder() const;
        void print_inorder() const;
        void print_postorder() const;
        void print_levelorder() const;

        // BST-spezifisch
        void insert(BinaryTreeNode::data_t data);
        bool find(BinaryTreeNode::data_t data) const;
        void remove(BinaryTreeNode::data_t data);

    private:
        BinaryTree _tree;
    };

}