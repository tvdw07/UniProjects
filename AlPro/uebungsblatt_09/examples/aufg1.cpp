#include <data_structures/doubly_linked_list.hpp>
#include <iostream>

int main() {
  using data_structures::DoublyLinkedList;

  DoublyLinkedList list;

  std::cout << "Test 1: insert_front\n";
  // Füge Elemente am Anfang der Liste ein
  const auto node5 = list.insert_front(5);
  auto node3 = list.insert_front(3);
  auto node7 = list.insert_front(7);
  list.print();
  std::cout << '\n';

  std::cout << "Test 2: insert_after\n";
  const auto node4 = list.insert_after(node5, 4);
  list.print();
  std::cout << '\n';

  std::cout << "Test 3: get_first / get_last\n";
  // Gib erstes und letztes Element aus
  if (const auto first = list.get_first()) {
    std::cout << "Erstes Element: " << first->data_ << '\n';
  }
  if (const auto last = list.get_last()) {
    std::cout << "Letztes Element: " << last->data_ << '\n';
  }
  std::cout << '\n';

  std::cout << "Test 4: Navigation\n";
  // Navigiere durch die Liste
  if (const auto first = list.get_first()) {
    if (const auto n = list.next(first)) {
      std::cout << "next(first): " << n->data_ << '\n';
    }
  }
  std::cout << '\n';

  std::cout << "Test 5: remove(4)\n";
  list.remove(node4);
  list.print();
  std::cout << '\n';

  std::cout << "Test 6: weitere insert_after\n";
  const auto node10 = list.insert_after(list.get_first(), 10);
  const auto node1  = list.insert_after(list.get_last(), 1);
  list.print();
  std::cout << '\n';

  std::cout << "Test 7: remove first\n";
  list.remove(node10);
  list.print();
  std::cout << '\n';

  std::cout << "Test 8: remove last\n";
  list.remove(node1);
  list.print();
  std::cout << '\n';

  return 0;
}
