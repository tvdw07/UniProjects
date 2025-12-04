#pragma once

#include <memory>

namespace data_structures {

// A ListNode stores its succesor and some data.
class ListNode{
public:
  ListNode(int data);

// We model the ListNode in such a way, that it is comprehensible to a student, that is still
// learning C++ and Object Oriented Modeling and that has not yet learned Iterators.
// To this end, we model the ListNode as "data storage object", therefore, all its members are public.
// Note that, in a large project, that approach is (most probably) a weak choice.
public:
  std::shared_ptr<ListNode> next;
  int _data;
};

// In the end, the ListNodes are dynamically organized on the heap.
// Therefore, we use Shared Pointer for the organization of ListNodes.
// It will turn out, that shared_ptr is the best way because we want
// to iterate through the nodes (i.e., we want multiple references at once).

// The type "std::shared_ptr<ListNode>" is abbriviated with "ListNodeptr"
using ListNodeptr = std::shared_ptr<ListNode>;

}
