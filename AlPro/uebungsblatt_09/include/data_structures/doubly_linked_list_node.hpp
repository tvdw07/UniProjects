#pragma once

#include <memory>

namespace data_structures {

// A DoublyLinkedListNode stores its sucessor and predecessor
class DoublyLinkedListNode{
public:
  DoublyLinkedListNode(int data);

// We model the Node in such a way, that it is comprehensible to a student, that is still
// learning C++ and Object Oriented Modeling and that has not yet learned Iterators.
// To this end, we model the Node as "data storage object", therefore, all its members are public.
// Note that, in a large project, that approach is (most probably) a weak choice.
public:
  std::shared_ptr<DoublyLinkedListNode> next;
  std::weak_ptr<DoublyLinkedListNode> prev;

  int data_;
};

// In the end, the DoublyLinkedListNode are dynamically organized on the heap.
// Therefore, we use Shared Pointer for the organization of DoublyLinkedListNode.
// It will turn out, that shared_ptr is the best way because we want
// to iterate through the nodes (i.e., we want multiple references at once).

// The type "std::shared_ptr<DoublyLinkedListNode>" is abbriviated with "DListNodeSharedPtr"
using DListNodeSharedPtr = std::shared_ptr<DoublyLinkedListNode>;

// The type "std::weak_ptr<DoublyLinkedListNode>" is abbriviated with "DListNodeWeakPtr"
using DListNodeWeakPtr   = std::weak_ptr<DoublyLinkedListNode>;

}
