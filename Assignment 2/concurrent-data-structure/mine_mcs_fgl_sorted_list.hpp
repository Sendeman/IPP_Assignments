#ifndef lacpp_sorted_list_hpp
#define lacpp_sorted_list_hpp lacpp_sorted_list_hpp
#include <atomic>

/*
    Heavily inspired on chapter 7.5.3, The MCS Queue Lock,
    from The Art of Multiprocessor Programming, pages 155
*/

struct QNode
{
    std::atomic_bool locked;
    std::atomic<QNode *>next;
    QNode()
    {
        locked = true;
        next = nullptr;
    }
};

struct McsLock
{
    std::atomic<QNode *> threadNode;
    std::atomic<QNode *> tail;

    McsLock()
    {
        threadNode.store(nullptr);
        tail.store(nullptr);
    }

    void lock()
    {
        QNode *qnode = new QNode();
        QNode *prev = tail.exchange(qnode);
        if (prev)
        {
            qnode->locked = true;
            prev->next = qnode;
            while (qnode->locked)
            {
            }
        }
        threadNode.store(qnode);
    }

    void unlock()
    {
        if (!threadNode.load()->next)
        {
            QNode *p = threadNode.load();
            if (tail.compare_exchange_strong(p, nullptr))
            {
                return;
            }
            while (!threadNode.load()->next)
            {
            }
        }
        threadNode.load()->next.load()->locked = false;
    }
};

/* a sorted list implementation by David Klaftenegger, 2015
 * please report bugs or suggest improvements to david.klaftenegger@it.uu.se
 */

/*
    Heavily inspired on chapter 9.5, Fine-Grained Synchronization,
    from The Art of Multiprocessor Programming, pages 202 and 203
*/

/* struct for list nodes */
template <typename T>
struct node
{
    T value;
    node<T> *next;
    McsLock nodeLock;
};

/* Concurrent sorted singly-linked list (std course-graining) */
template <typename T>
class sorted_list
{
    node<T> *first = nullptr;

public:
    /* default implementations:
     * default constructor
     * copy constructor (note: shallow copy)
     * move constructor
     * copy assignment operator (note: shallow copy)
     * move assignment operator
     *
     * The first is required due to the others,
     * which are explicitly listed due to the rule of five.
     */
    sorted_list() = default;
    sorted_list(const sorted_list<T> &other) = default;
    sorted_list(sorted_list<T> &&other) = default;
    sorted_list<T> &operator=(const sorted_list<T> &other) = default;
    sorted_list<T> &operator=(sorted_list<T> &&other) = default;
    ~sorted_list()
    {
        while (first != nullptr)
        {
            remove(first->value);
        }
    }

    /* insert v into the list */
    void insert(T v)
    {
        if (first)
            first->nodeLock.lock();
        node<T> *pred = first;
        node<T> *succ = pred ? pred->next : nullptr;
        if (succ)
            succ->nodeLock.lock();
        while (succ != nullptr && succ->value < v)
        {
            pred->nodeLock.unlock();
            pred = succ;
            succ = succ->next;
            if (succ)
                succ->nodeLock.lock();
        }

        /* construct new node */
        node<T> *current = new node<T>();
        current->value = v;

        /* insert new node between pred and succ */
        current->next = succ;
        if (succ)
            succ->nodeLock.unlock();
        if (pred == nullptr)
        {
            first = current;
        }
        else
        {
            pred->next = current;
            pred->nodeLock.unlock();
        }
    }

    void remove(T v)
    {
        if (first)
            first->nodeLock.lock();
        node<T> *pred = first;
        node<T> *current = pred ? pred->next : nullptr;
        if (current)
            current->nodeLock.lock();
        while (current != nullptr && current->value < v)
        {
            pred->nodeLock.unlock();
            pred = current;
            current = current->next;
            if (current)
                current->nodeLock.lock();
        }
        if (current == nullptr || current->value != v)
        {
            if (current)
                current->nodeLock.unlock();
            if (pred)
                pred->nodeLock.unlock();
            return;
        }
        /* remove current */
        if (pred == nullptr)
        {
            first = current->next;
        }
        else
        {
            pred->next = current->next;
            pred->nodeLock.unlock();
        }
        current->nodeLock.unlock();
        delete current;
    }

    /* count elements with value v in the list. */
    std::size_t count(T v)
    {
        std::size_t cnt = 0;
        if (first)
            first->nodeLock.lock();
        node<T> *pred = first;
        node<T> *current = pred ? pred->next : nullptr;
        if (current)
            current->nodeLock.lock();
        while (current != nullptr && current->value <= v)
        {
            pred->nodeLock.unlock();
            pred = current;
            current = current->next;
            if (current)
                current->nodeLock.lock();
        }
        while (current != nullptr && current->value == v)
        {
            pred->nodeLock.unlock();
            pred = current;
            current = current->next;
            if (current)
                current->nodeLock.lock();
            cnt++;
        }
        if (pred)
            pred->nodeLock.unlock();
        if (current)
            current->nodeLock.unlock();
        return cnt;
    }
};

#endif // lacpp_sorted_list_hpp
