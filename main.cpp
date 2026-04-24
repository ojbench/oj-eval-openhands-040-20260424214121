#include <bits/stdc++.h>
using namespace std;

// Include solution template here via header-style inclusion
// We will include the implementation by compiling both files together.

template <typename T>
class DoublyLinkedList {
private:
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
    class iterator {
    private:
        std::shared_ptr<Node> current;
    public:
        iterator(std::shared_ptr<Node> node) : current(node) {}
        T& operator*() { return current->value; }
        iterator operator++(int) { iterator tmp = *this; if (current) current = current->next; return tmp; }
        bool operator==(const iterator& other) const { return current.get() == other.current.get(); }
        bool operator!=(const iterator& other) const { return !(*this == other); }
    };
    DoublyLinkedList() : head(nullptr), length(0) {}
    bool empty() const { return length == 0; }
    size_t size() const { return length; }
    void push_front(const T& value) {
        auto node = std::make_shared<Node>(value);
        node->next = head;
        if (head) head->prev = node; else tail = node;
        head = node; ++length;
    }
    void push_back(const T& value) {
        if (!head) { push_front(value); return; }
        auto last = tail.lock();
        auto node = std::make_shared<Node>(value);
        if (last) { last->next = node; node->prev = last; }
        tail = node; ++length;
    }
    std::optional<T> pop_front() {
        if (!head) return std::nullopt;
        T ret = std::move(head->value);
        auto nxt = head->next;
        if (nxt) { nxt->prev.reset(); head = nxt; --length; }
        else { head.reset(); tail.reset(); length = 0; }
        if (length == 0) { head.reset(); tail.reset(); }
        return ret;
    }
    std::optional<T> pop_back() {
        if (empty()) return std::nullopt;
        auto last = tail.lock();
        if (!last) { T val = std::move(head->value); head.reset(); tail.reset(); length = 0; return val; }
        T ret = std::move(last->value);
        auto prev = last->prev.lock();
        if (prev) { prev->next.reset(); tail = prev; --length; }
        else { head.reset(); tail.reset(); length = 0; }
        if (length == 0) { head.reset(); tail.reset(); }
        return ret;
    }
    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // Simple interactive driver based on input commands:
    // Format:
    // n commands, each command:
    // PF x -> push_front x
    // PB x -> push_back x
    // POF -> pop_front, print value or NONE
    // POB -> pop_back, print value or NONE
    // SZ -> print size
    // IT -> iterate and print all values separated by spaces (then newline)

    int q; if(!(cin>>q)) return 0;
    DoublyLinkedList<long long> dl;
    while(q--){
        string op; cin>>op;
        if(op=="PF"){ long long x; cin>>x; dl.push_front(x); }
        else if(op=="PB"){ long long x; cin>>x; dl.push_back(x); }
        else if(op=="POF"){ auto v=dl.pop_front(); if(v) cout<<*v<<"\n"; else cout<<"NONE\n"; }
        else if(op=="POB"){ auto v=dl.pop_back(); if(v) cout<<*v<<"\n"; else cout<<"NONE\n"; }
        else if(op=="SZ"){ cout<<dl.size()<<"\n"; }
        else if(op=="IT"){ bool first=true; for(auto it=dl.begin(); it!=dl.end(); it++){ if(!first) cout<<' '; first=false; cout<<*it; } cout<<"\n"; }
    }
    return 0;
}
