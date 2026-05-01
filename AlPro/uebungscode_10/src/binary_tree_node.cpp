#include <data_structures/binary_tree_node.hpp>
#include <utility>

namespace data_structures {

// Konstruktor
// Bei der Objekterzeugung werden zuerst die Membervariablen initialisiert,
// dabei wird das Member _parent mit dem übergegeben Wert parent initialisiert,
// das Member _left_child wird mit dem Wert nullptr initialisiert (d.h., es ist noch kein linkes Kind vorhanden),
// das Member _right_child wird mit dem Wert nullptr initialisiert (d.h., es ist noch kein rechted Kind vorhanden) und
// das Member _data wird mit dem übergebenen Wert data initialisiert.
// Anschließend wird der Funktionscode des Konstruktors ausgeführt, dieser ist hier leer,
// da wir neben der Initialisierung nichts weiteres tun wollen.
BinaryTreeNode::BinaryTreeNode(const data_t data, BinaryTreeNodeWeakPtr parent)
  : _parent(std::move(parent)),
    _left_child(nullptr),
    _right_child(nullptr),
    _data(data)
{  
}
 
const BinaryTreeNode::data_t BinaryTreeNode::get_data() const {
  return _data;
}
 
const BinaryTreeNode::data_t BinaryTreeNode::get_left_child_data() const {
  return _left_child->get_data();
}

const BinaryTreeNode::data_t BinaryTreeNode::get_right_child_data() const {
  return _right_child->get_data();
}

void BinaryTreeNode::create_left_child (data_t data) {
  // Hier wird die Funktion weak_from_this verwendet, siehe auch Vorlesungsfolien.
  _left_child = std::make_shared<BinaryTreeNode>(data, weak_from_this());
}

void BinaryTreeNode::create_right_child (data_t data) {
  // Hier wird die Funktion weak_from_this verwendet, siehe auch Vorlesungsfolien.
  _right_child = std::make_shared<BinaryTreeNode>(data, weak_from_this());
}

BinaryTreeNode::BinaryTreeNodeWeakPtr BinaryTreeNode::get_parent() {
  return _parent;
}

BinaryTreeNode::BinaryTreeNodeSharedPtr BinaryTreeNode::get_left_child() {
  return _left_child;
}

BinaryTreeNode::BinaryTreeNodeSharedPtr BinaryTreeNode::get_right_child() {
  return _right_child;
}

void BinaryTreeNode::set_left_child(const BinaryTreeNodeSharedPtr &left_child) {
  _left_child = left_child;
}

void BinaryTreeNode::set_right_child(const BinaryTreeNodeSharedPtr &right_child) {
  _right_child = right_child;
}

void BinaryTreeNode::del_left_child() {
  _left_child = nullptr;
}

void BinaryTreeNode::del_right_child() {
  _right_child = nullptr;
}

}
