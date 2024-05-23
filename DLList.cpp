#include "DLList.h"

template<typename Object>
DLList<Object>::Node::Node(const Object &d, Node *p, Node *n)
        : data{d}, prev{p}, next{n} {}

template<typename Object>
DLList<Object>::Node::Node(Object &&d, Node *p, Node *n)
        : data{std::move(d)}, prev{p}, next{n} {}

template<typename Object>
DLList<Object>::const_iterator::const_iterator() : current{nullptr} {}

template<typename Object>
const Object &DLList<Object>::const_iterator::operator*() const {
    return retrieve();
}

template<typename Object>
typename DLList<Object>::const_iterator &DLList<Object>::const_iterator::operator++() {
    current = current->next;
    return *this;
}

template<typename Object>
typename DLList<Object>::const_iterator DLList<Object>::const_iterator::operator++(int) {
    const_iterator old = *this;
    ++(*this);
    return old;
}

template<typename Object>
bool DLList<Object>::const_iterator::operator==(const const_iterator &rhs) const {
    return current == rhs.current;
}

template<typename Object>
bool DLList<Object>::const_iterator::operator!=(const const_iterator &rhs) const {
    return !(*this == rhs);
}

template<typename Object>
Object &DLList<Object>::const_iterator::retrieve() const {
    return current->data;
}

template<typename Object>
DLList<Object>::const_iterator::const_iterator(Node *p) : current{p} {}

template<typename Object>
DLList<Object>::iterator::iterator() {}

template<typename Object>
Object &DLList<Object>::iterator::operator*() {
    return const_iterator::retrieve();
}

template<typename Object>
const Object &DLList<Object>::iterator::operator*() const {
    return const_iterator::operator*();
}

template<typename Object>
typename DLList<Object>::iterator &DLList<Object>::iterator::operator++() {
    this->current = this->current->next;
    return *this;
}

template<typename Object>
typename DLList<Object>::iterator &DLList<Object>::iterator::operator--() {
    this->current = this->current->prev;
    return *this;
}

template<typename Object>
typename DLList<Object>::iterator DLList<Object>::iterator::operator++(int) {
    iterator old = *this;
    ++(*this);
    return old;
}

template<typename Object>
typename DLList<Object>::iterator DLList<Object>::iterator::operator--(int) {
    iterator old = *this;
    --(*this);
    return old;
}

template<typename Object>
typename DLList<Object>::iterator DLList<Object>::iterator::operator+(int steps) const {
    iterator new_itr = *this;
    for(int i = 0; i < steps; ++i) {
        ++new_itr;
    }
    return new_itr;
}

template<typename Object>
DLList<Object>::iterator::iterator(Node *p) : const_iterator{p} {}

template<typename Object>
DLList<Object>::DLList() : theSize{0}, head{new Node}, tail{new Node} {
    head->next = tail;
    tail->prev = head;
}

template<typename Object>
DLList<Object>::DLList(std::initializer_list<Object> initList) : DLList() {
    for(const auto &item : initList) {
        push_back(item);
    }
}

template<typename Object>
DLList<Object>::~DLList() {
    clear();
    delete head;
    delete tail;
}

template<typename Object>
DLList<Object>::DLList(const DLList &rhs) : DLList() {
    for(auto &item : rhs) {
        push_back(item);
    }
}

template<typename Object>
DLList<Object> &DLList<Object>::operator=(const DLList &rhs) {
    DLList copy = rhs;
    std::swap(*this, copy);
    return *this;
}

template<typename Object>
DLList<Object>::DLList(DLList &&rhs) : theSize{rhs.theSize}, head{rhs.head}, tail{rhs.tail} {
    rhs.theSize = 0;
    rhs.head = nullptr;
    rhs.tail = nullptr;
}

template<typename Object>
DLList<Object> &DLList<Object>::operator=(DLList &&rhs) {
    std::swap(theSize, rhs.theSize);
    std::swap(head, rhs.head);
    std::swap(tail, rhs.tail);
    return *this;
}

template<typename Object>
typename DLList<Object>::iterator DLList<Object>::begin() {
    return {head->next};
}

template<typename Object>
typename DLList<Object>::const_iterator DLList<Object>::begin() const {
    return {head->next};
}

template<typename Object>
typename DLList<Object>::iterator DLList<Object>::end() {
    return {tail};
}

template<typename Object>
typename DLList<Object>::const_iterator DLList<Object>::end() const {
    return {tail};
}

template<typename Object>
int DLList<Object>::size() const {
    return theSize;
}

template<typename Object>
bool DLList<Object>::empty() const {
    return size() == 0;
}

template<typename Object>
void DLList<Object>::clear() {
    while(!empty()) {
        pop_front();
    }
}

template<typename Object>
Object &DLList<Object>::front() {
    return *begin();
}

template<typename Object>
const Object &DLList<Object>::front() const {
    return *begin();
}

template<typename Object>
Object &DLList<Object>::back() {
    return *(--end());
}

template<typename Object>
const Object &DLList<Object>::back() const {
    return *(--end());
}

template<typename Object>
void DLList<Object>::push_front(const Object &x) {
    insert(begin(), x);
}

template<typename Object>
void DLList<Object>::push_front(Object &&x) {
    insert(begin(), std::move(x));
}

template<typename Object>
void DLList<Object>::push_back(const Object &x) {
    insert(end(), x);
}

template<typename Object>
void DLList<Object>::push_back(Object &&x) {
    insert(end(), std::move(x));
}

template<typename Object>
void DLList<Object>::pop_front() {
    erase(begin());
}

template<typename Object>
void DLList<Object>::pop_back() {
    erase(--end());
}

template<typename Object>
typename DLList<Object>::iterator DLList<Object>::insert(iterator itr, const Object &x) {
    Node *p = itr.current;
    theSize++;
    return {p->prev = p->prev->next = new Node{x, p->prev, p}};
}

template<typename Object>
typename DLList<Object>::iterator DLList<Object>::insert(iterator itr, Object &&x) {
    Node *p = itr.current;
    theSize++;
    return {p->prev = p->prev->next = new Node{std::move(x), p->prev, p}};
}

template<typename Object>
void DLList<Object>::insert(int pos, const Object &x) {
    insert(get_iterator(pos), x);
}

template<typename Object>
void DLList<Object>::insert(int pos, Object &&x) {
    insert(get_iterator(pos), std::move(x));
}

template<typename Object>
typename DLList<Object>::iterator DLList<Object>::erase(iterator itr) {
    Node *p = itr.current;
    iterator retVal(p->next);
    p->prev->next = p->next;
    p->next->prev = p->prev;
    delete p;
    theSize--;
    return retVal;
}

template<typename Object>
void DLList<Object>::erase(int pos) {
    erase(get_iterator(pos));
}

template<typename Object>
typename DLList<Object>::iterator DLList<Object>::erase(iterator from, iterator to) {
    for(iterator itr = from; itr != to;) {
        itr = erase(itr);
    }
    return to;
}

template<typename Object>
void DLList<Object>::print() const {
    for(const auto &item : *this) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

template<typename Object>
void DLList<Object>::init() {
    theSize = 0;
    head = new Node;
    tail = new Node;
    head->next = tail;
    tail->prev = head;
}

template<typename Object>
typename DLList<Object>::iterator DLList<Object>::get_iterator(int pos) {
    iterator itr = begin();
    for(int i = 0; i < pos; ++i) {
        ++itr;
    }
    return itr;
}
