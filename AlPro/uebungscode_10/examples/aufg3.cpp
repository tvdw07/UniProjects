#include <iostream>

#include "data_structures/hashtable.hpp"

int main() {
  data_structures::Hashtable ht;

  ht.insert("Alice");
  ht.insert("Bob");
  ht.insert("Charlie");
  ht.insert("Alice"); // Duplikat

  ht.print();
  std::cout << "Size: " << ht.size() << "\n";

  std::cout << "Find Bob: " << ht.find("Bob") << "\n";
  ht.remove("Bob");
  std::cout << "Find Bob after remove: " << ht.find("Bob") << "\n";

  ht.print();
}
