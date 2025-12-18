#include <iostream>
#include <data_structures/binary_tree.hpp>

int main() {

    // Create a binary tree
    data_structures::BinaryTree bt;

    // Create root
    bt.create_root(1);
    const auto root = bt.get_root();

    // Create left subtree
    root->create_left_child(2);
    const auto left = root->get_left_child();
    left->create_left_child(4);
    left->create_right_child(5);

    // Create right subtree
    root->create_right_child(3);
    const auto right = root->get_right_child();
    right->create_left_child(6);
    right->create_right_child(7);

    std::cout << "Tree structure:" << std::endl;
    std::cout << "         1" << std::endl;
    std::cout << "       /   \\" << std::endl;
    std::cout << "      2     3" << std::endl;
    std::cout << "     / \\   / \\" << std::endl;
    std::cout << "    4   5 6   7" << std::endl << std::endl;

    // Display traversals
    bt.print_preorder();
    bt.print_inorder();
    bt.print_postorder();
    bt.print_levelorder();


    return 0;
}
