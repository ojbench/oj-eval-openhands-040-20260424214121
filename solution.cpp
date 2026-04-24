#include <iostream>
#include <memory>
#include <optional>

// Implementation based on the provided template in README

template <typename T>
class DoublyLinkedList {
private:
    // Node structure definition
    struct Node {
        T value;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;

        Node(const T& val) : value(val), next(nullptr) {}
    };

    std::shared_ptr<Node> head;
    std::weak_ptr<Node> tail;
    size_t length;

public:
    // Iterator class
    class iterator {
    private:
        std::shared_ptr<Node> current;

    public:
        iterator(std::shared_ptr<Node> node) : current(node) {}

        // Dereference operator to get value
        T& operator*() {
            // 注：对空迭代器调用此函数是未定义行为，测试数据中不会出现
            return current->value;
        }

        // Post-increment operator
        iterator operator++(int) {
            // 注：对空迭代器调用此函数是未定义行为，测试数据中不会出现
            iterator tmp = *this;
            if (current) current = current->next;
            return tmp;
        }

        // Equality operators
        bool operator==(const iterator& other) const {
            // 注：比较属于不同容器的非空迭代器是未定义行为，测试数据中不会出现
            return current.get() == other.current.get();
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    // Constructor
    DoublyLinkedList() : head(nullptr), length(0) {}

    // No destructor needed, memory managed by shared_ptr

    // Check if list is empty
    bool empty() const {
        return length == 0;
    }

    // Get the size of the list
    size_t size() const {
        return length;
    }

    // Add element to the front
    void push_front(const T& value) {
        auto node = std::make_shared<Node>(value);
        node->next = head;
        if (head) {
            head->prev = node;
        } else {
            // list was empty, tail becomes this node
            tail = node;
        }
        head = node;
        ++length;
    }

    // Add element to the back
    void push_back(const T& value) {
        if (!head) {
            push_front(value);
            return;
        }
        auto last = tail.lock();
        auto node = std::make_shared<Node>(value);
        if (last) {
            last->next = node;
            node->prev = last;
        } else {
            // Should not happen if list not empty, but guard anyway
            head->prev.reset();
        }
        tail = node;
        ++length;
    }

    // Remove and return the first element
    std::optional<T> pop_front() {
        if (!head) {
            return std::nullopt;
        }
        T ret = std::move(head->value);
        auto next = head->next;
        if (next) {
            next->prev.reset();
            head = next;
            --length;
        } else {
            // only one element
            head.reset();
            tail.reset();
            length = 0;
        }
        if (length == 0) {
            head.reset();
            tail.reset();
        }
        return ret;
    }

    // Remove and return the last element
    std::optional<T> pop_back() {
        if (empty()) {
            return std::nullopt;
        }
        auto last = tail.lock();
        if (!last) {
            // Fallback: only head exists
            T val = std::move(head->value);
            head.reset();
            tail.reset();
            length = 0;
            return val;
        }
        T ret = std::move(last->value);
        auto prev = last->prev.lock();
        if (prev) {
            prev->next.reset();
            tail = prev;
            --length;
        } else {
            // last was the only node
            head.reset();
            tail.reset();
            length = 0;
        }
        if (length == 0) {
            head.reset();
            tail.reset();
        }
        return ret;
    }

    // Iterators
    iterator begin() {
        return iterator(head);
    }

    iterator end() {
        return iterator(nullptr);
    }
};
