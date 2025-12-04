#include <data_structures/list.hpp>
#include <iostream>

int main() {
  using namespace data_structures;

  // Erstelle eine neue Liste
  List list;

  std::cout << "1. Leere Liste erstellt:" << std::endl;
  list.print();
  std::cout << std::endl;

  // Füge Elemente am Anfang hinzu
  std::cout << "2. Füge Elemente am Anfang hinzu (10, 20, 30):" << std::endl;
  list.insert_front(10);
  list.insert_front(20);
  list.insert_front(30);
  list.print();
  std::cout << std::endl;

  // Füge Element nach dem ersten Element hinzu
  std::cout << "3. Füge 25 nach dem ersten Element hinzu:" << std::endl;
  const ListNodeptr first = list.get_first();
  list.insert_after(first, 25);
  list.print();
  std::cout << std::endl;

  // Iteriere durch die Liste
  std::cout << "4. Iteriere durch die Liste:" << std::endl;
  ListNodeptr current = list.get_first();
  int index = 0;
  while (current != nullptr) {
    std::cout << "   Element " << index << ": " << current->_data << std::endl;
    current = list.next(current);
    index++;
  }
  std::cout << std::endl;

  // Entferne das erste Element
  std::cout << "5. Entferne das erste Element:" << std::endl;
  const ListNodeptr removed = list.remove_front();
  std::cout << "   Entferntes Element: " << removed->_data << std::endl;
  list.print();
  std::cout << std::endl;


  // Zeige Größe
  std::cout << "7. Finale Größe der Liste: " << list.size() << std::endl;

  return 0;
}
