#include <utility>
#include <iostream>

template <typename Object>
class DLList {
private:
    struct Node {
        Object data;
        Node *prev;
        Node *next;

        Node(const Object &d = Object{}, Node *p = nullptr, Node *n = nullptr);
        Node(Object &&d, Node *p = nullptr, Node *n = nullptr);
    };

public:
    class const_iterator{
    public:
        const_iterator();
        const Object &operator*() const;
        const_iterator &operator++();
        const_iterator operator++ (int);
        bool operator== (const const_iterator& rhs) const;
        bool operator!= (const const_iterator& rhs) const;

    protected:
        Node* current;
        Object& retrieve() const;
        const_iterator(Node *p);

        friend class DLList<Object>;
    };

    class iterator : public const_iterator {
    public:
        iterator();
        Object& operator*();
        const Object& operator*() const;
        iterator & operator++ ();
        iterator &operator--();
        iterator operator++ (int);
        iterator operator-- (int);
        iterator operator+ (int steps) const;

    protected:
        iterator(Node *p);

        friend class DLList<Object>;
    };

public:
    DLList();
    DLList(std::initializer_list<Object> initList);
    ~DLList();
    DLList(const DLList &rhs);
    DLList &operator=(const DLList &rhs);
    DLList(DLList &&rhs);
    DLList &operator=(DLList &&rhs);

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    int size() const;
    bool empty() const;

    void clear();

    Object &front();
    const Object &front() const;
    Object &back();
    const Object &back() const;

    void push_front(const Object &x);
    void push_front(Object &&x);
    void push_back(const Object &x);
    void push_back(Object &&x);

    void pop_front();
    void pop_back();

    iterator insert(iterator itr, const Object &x);
    iterator insert(iterator itr, Object &&x);

    void insert(int pos, const Object &x);
    void insert(int pos, Object &&x);

    iterator erase(iterator itr);
    void erase(int pos);
    iterator erase(iterator from, iterator to);

    void print() const;

private:
    int theSize;
    Node *head;
    Node *tail;

    void init();
    iterator get_iterator(int pos);
};