#pragma once

#include <memory>

#include <data_structures/doubly_linked_list_node.hpp>

namespace data_structures {

// Anforderungen:
// Container
//   size()
// 
//  HIER DIE ANFORDERUNGSLISTE VERVOLLSTÄNDIGEN
//  SIEHE AUCH ZETTEL 8 -> include/data_structures/list.hpp
//
//
class DoublyLinkedList{
public:
  DoublyLinkedList();
  //
  // FEHLENDE FUNKTIONEN DER ANFORDERUNGSLISTE HIER ERGÄNZEN
  //

  // Get the first / last Node of the current List
  DListNodeSharedPtr get_first() const;
  DListNodeSharedPtr get_last() const;

  // Get the next / previous Node of the current List
  DListNodeSharedPtr next(const DListNodeSharedPtr& n) const;
  DListNodeSharedPtr prev(const DListNodeSharedPtr& n) const;

  // Insert and delete Nodes
  DListNodeSharedPtr insert_front(int x);
  DListNodeSharedPtr insert_after(const DListNodeSharedPtr& pred, int x);
  DListNodeSharedPtr remove(const DListNodeSharedPtr& n);

  // Print the current List
  void print() const;

private:
  DListNodeSharedPtr head;
  DListNodeSharedPtr tail;
};

}
