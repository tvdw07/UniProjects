#include <data_structures/list.hpp>
#include <iostream>

namespace data_structures {

List::List() : head(nullptr), num_elements(0) {}

uint32_t List::size() {
  return num_elements;
}

ListNodeptr List::get_first() {
  return head;
}

ListNodeptr List::next(const ListNodeptr& n) const {
  if (n == nullptr) {
    return nullptr;
  }
  return n->next;
}

ListNodeptr List::insert_front(const int x) {
  ListNodeptr new_node = std::make_shared<ListNode>(x); // Create a new node.
  new_node->next = head; // Point the new node to the current head.
  head = new_node; // Update the head to the new node.
  num_elements++; // Increment the element count.
  return new_node;
}

ListNodeptr List::insert_after(const ListNodeptr& pred, const int x) {
  if (pred == nullptr) {
    return nullptr; // Cannot insert after a null node.
  }
  //IDE schlägt vor, hier "auto" zu verwenden, was ist der Vorteil?
  ListNodeptr new_node = std::make_shared<ListNode>(x); // Create a new node.
  new_node->next = pred->next; // Link the new node to the successor of pred.
  pred->next = new_node; // Link pred to the new node.
  num_elements++; // Increment the element count.
  return new_node;
}

ListNodeptr List::remove_front() {
  if (head == nullptr) {
    return nullptr; // List is empty, nothing to remove.
  }
  ListNodeptr removed = head; // Store the current head.
  head = head->next; // Update the head to the next node.
  num_elements--; // Decrement the element count.
  return removed;
}

ListNodeptr List::remove_after(const ListNodeptr& pred) {
  if (pred == nullptr || pred->next == nullptr) {
    return nullptr; // Cannot remove if pred is null or has no successor.
  }
  ListNodeptr removed = pred->next; // Store the node to be removed.
  pred->next = removed->next; // Link pred to the successor of the removed node.
  num_elements--; // Decrement the element count.
  return removed;
}

void List::print() const {
  std::cout << "List (size=" << num_elements << "): ";
  ListNodeptr current = head; // Start from the head.
  while (current != nullptr) {
    std::cout << current->_data; // Print the data of the current node.
    if (current->next != nullptr) {
      std::cout << " -> "; // Print an arrow if there is a next node.
    }
    current = current->next; // Move to the next node.
  }
  std::cout << std::endl;
}

}
