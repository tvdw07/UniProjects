#pragma once

#include <memory>

#include <data_structures/list_node.hpp>

namespace data_structures{

  // Anforderungen:
  // Container
  //   size()
  // iterierbar
  //   get_first()
  //   next()
  // sequentiell
  //   verträglieche insert/remove-Operationen
  // einfach verkettete Liste
  //   iterator-Reihenfolge = sequentielle Reihenfolge
  //   insert_front(data)
  //   insert_after(node, data)
  //   remove_front()
  //   remove_after(node)

class List{
public:
  List();

  // Number of elements
  uint32_t size();

  // Get the first / next Node of the current List
  ListNodeptr get_first();
  ListNodeptr next(const ListNodeptr& n) const;

  // Insert and delete Nodes
  ListNodeptr insert_front(const int x);
  ListNodeptr insert_after(const ListNodeptr& pred, const int x);
  ListNodeptr remove_front();
  ListNodeptr remove_after(const ListNodeptr& pred);

  // Print the current List
  void print() const;

private:
  ListNodeptr head;
  uint32_t num_elements;
};

}
