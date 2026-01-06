
#include <random>
#include <vector>
#include <iostream>
#include <data_structures/binarySearchTree.hpp>

// Dies ist ein naiver Test (der nicht vollständig prüft ob ihre Implementierung korrekt ist).
void naiver_test() {
  // Hier erstellen wir eine zufällige Folge von Zahlen
  std::random_device rand_device;
  std::mt19937 rand_engine(rand_device());
  std::uniform_int_distribution<int> dist(100, 999);
  std::vector<int> zahlenfolge(20);
  for (int& x : zahlenfolge) {
    x = dist(rand_engine);
  }
  
  // Nutzen Sie diese Folge, um ihren binären Suchbaum aufzubauen.
  // ...
  data_structures::BinarySearchTree bst;
  for (const int& x : zahlenfolge) {
    bst.insert(x);
  }
  bst.insert(42);
  
  // Prüfen Sie, ob ihr binärer Suchbaum die Elemente im Inorderdurchlauf aufsteigend ausgeben.
  // ...
  std::cout << "Inorder Traversal of BST: ";
  bst.print_inorder();
  std::cout << std::endl;
  
  
  // Prüfen Sie, ob die Zahl 42 in Ihrem Baum enthalten ist.
  // ...
  if (constexpr int search_value = 42; bst.find(search_value)) {
    std::cout << search_value << " is found in the BST." << std::endl;
  } else {
    std::cout << search_value << " is not found in the BST." << std::endl;
  }
}


int main() {
  naiver_test();
}
