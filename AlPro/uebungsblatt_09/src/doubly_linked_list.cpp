#include <data_structures/doubly_linked_list.hpp>
#include <iostream>

namespace data_structures {
    // Konstruktor
    DoublyLinkedList::DoublyLinkedList()
        : head(nullptr),
          tail(nullptr) {
    }

    // Get the first / last Node of the current List
    DListNodeSharedPtr DoublyLinkedList::get_first() const {
        return head;
    }

    DListNodeSharedPtr DoublyLinkedList::get_last() const {
        return tail;
    }

    // Get the next / previous Node of the current List
    DListNodeSharedPtr DoublyLinkedList::next(const DListNodeSharedPtr &n) const {
        if (n == nullptr) {
            return nullptr;
        }
        return n->next;
    }

    DListNodeSharedPtr DoublyLinkedList::prev(const DListNodeSharedPtr &n) const {
        if (n == nullptr) {
            return nullptr;
        }
        return n->prev.lock();
    }

    // Insert and delete Nodes
    // Füge ein Element am Anfang der Liste ein
    DListNodeSharedPtr DoublyLinkedList::insert_front(int x) {
        auto new_node = std::make_shared<DoublyLinkedListNode>(x);

        if (head == nullptr) {
            // Liste ist leer
            head = new_node;
            tail = new_node;
        } else {
            // Verbinde den neuen Node mit dem aktuellen head
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }

        return new_node;
    }

    // Füge ein Element nach einem Vorgänger ein
    DListNodeSharedPtr DoublyLinkedList::insert_after(const DListNodeSharedPtr &pred, int x) {
        if (pred == nullptr) {
            return nullptr;
        }

        auto new_node = std::make_shared<DoublyLinkedListNode>(x);

        // Verbinde den neuen Node nach pred
        new_node->next = pred->next;
        new_node->prev = pred;

        // Aktualisiere den nachfolgenden Node
        if (pred->next != nullptr) {
            pred->next->prev = new_node;
        } else {
            // pred war der tail
            tail = new_node;
        }

        // Aktualisiere pred
        pred->next = new_node;

        return new_node;
    }

    // Entferne einen Node
    DListNodeSharedPtr DoublyLinkedList::remove(const DListNodeSharedPtr &n) {
        if (n == nullptr) {
            return nullptr;
        }

        DListNodeSharedPtr next_node = n->next;

        // Aktualisiere die Vorgänger-Verbindung
        const DListNodeSharedPtr prev_node = n->prev.lock();
        if (prev_node != nullptr) {
            prev_node->next = next_node;
        } else {
            // n ist der head
            head = next_node;
        }

        // Aktualisiere die Nachfolger-Verbindung
        if (next_node != nullptr) {
            next_node->prev = prev_node;
        } else {
            // n ist der tail
            tail = prev_node;
        }

        // Trenne die zu löschenden Node
        n->next = nullptr;
        n->prev.reset();

        return next_node;
    }

    // Print the current List
    void DoublyLinkedList::print() const {
        std::cout << "Liste (vorwaerts): ";
        DListNodeSharedPtr current = head;
        while (current != nullptr) {
            std::cout << current->data_ << " ";
            current = current->next;
        }
        std::cout << std::endl;

        std::cout << "Liste (rueckwaerts): ";
        current = tail;
        while (current != nullptr) {
            std::cout << current->data_ << " ";
            current = current->prev.lock();
        }
        std::cout << std::endl;
    }
}
