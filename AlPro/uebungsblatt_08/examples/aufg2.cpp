#include <data_structures/binary_tree.hpp>
#include <iostream>

int main() {
  using namespace data_structures;

  std::cout << "Erstelle Binärbaum:" << std::endl;
  // Root
  const BinaryTree tree(42);
  const BinaryTreeNodeSharedptr root = tree.get_root();

  // Ebene 1: Kinder von 42
  root->set_left_child(11);
  root->set_right_child(22);

  // Ebene 2 links: Kinder von 11
  const BinaryTreeNodeSharedptr node_11 = root->get_left_child();
  node_11->set_left_child(9);
  node_11->set_right_child(0);

  // Ebene 3 links: Kinder von 9
  const BinaryTreeNodeSharedptr node_9 = node_11->get_left_child();
  node_9->set_left_child(-6);
  node_9->set_right_child(22);

  // Ebene 2 rechts: rechter Teilbaum unter 22
  const BinaryTreeNodeSharedptr node_22 = root->get_right_child();
  node_22->set_right_child(544);

  // Ebene 3 rechts: Kinder von 544 (links 11)
  const BinaryTreeNodeSharedptr node_544 = node_22->get_right_child();
  node_544->set_left_child(11);

  std::cout << std::endl;
  // Baum ausgeben
  tree.print();

  return 0;
}
