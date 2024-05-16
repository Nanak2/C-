template <typename T>
class Stack {
private:
    class Node {
    public:
        T value;
        Node *next;
    };
    Node * head;

public:
    Stack();
    ~Stack();
    void push(T v);
    T pop();
    bool isEmpty();
};

template<typename T> Stack<T>::Stack()
{
    head = nullptr;
}

template<typename T> Stack<T>::~Stack()
{
    while (head != nullptr)
    {
        Node *t = head->next;
        delete head;
        head = t;
    }
}

template <typename T> void Stack<T>::push(T v)
{
    Node *p = new Node;
    p->value = v;
    p->next = head;
    head = p;
}

template<typename T> T Stack<T>::pop()
{
    T tVal = head->value;
    Node *t = head;
    head = head->next;
    delete t;
    return tVal;
}

template <typename T> bool Stack<T>::isEmpty()
{
    return head == nullptr;
}