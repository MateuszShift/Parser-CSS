#pragma once
#include <iostream>

template <class T>
class listNode
{
public:
    T *data;
    listNode<T> *next;
    listNode<T> *previous;

    listNode(T *data)
    {
        this->data = data;
        next = nullptr;
        previous = nullptr;
    }
    ~listNode() {}

    listNode<T> &operator=(const listNode<T> &other)
    {
        if (this != &other)
        {
            data = other.data;
            next = other.next;
            previous = other.previous;
        }
        return *this;
    }

    bool operator==(const listNode<T> &other) const
    {
        return data == other.data;
    }

    bool operator==(const char *name) const
    {
        return *data == name; 
    }

    bool operator==(const int id) const
    {
        return *data == id; 
    }

    friend std::ostream &operator<<(std::ostream &out, const listNode<T> * node)
    {
        out << node->data;
        return out;
    }
};

template <class T>
class DoublyLinkedList
{
private:
    int size;

public:
    listNode<T> *head;
    listNode<T> *tail;

public:
    DoublyLinkedList()
    {
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    ~DoublyLinkedList()
    {
        listNode<T> *curr = head;
        listNode<T> *temp;
        while (curr != nullptr)
        {
            temp = curr;
            curr = curr->next;
            delete temp;
        }
    }

    listNode<T> *add(T *data)
    {
        listNode<T> *newNode = new listNode<T>(data);

        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            newNode->previous = tail;
            tail = newNode;
        }
        size++;
        return newNode;
    }

    void remove(listNode<T> *node)
    {
        if (node == nullptr)
        {
            return;
        }
        if (node->previous == nullptr)
        {
            head = node->next;
        }
        else
        {
            node->previous->next = node->next;
        }
        if (node->next == nullptr)
        {
            tail = node->previous;
        }
        else
        {
            node->next->previous = node->previous;
        }
        delete node;
        size--;
    }
    int getSize(){
        return size;
    }
    int getSize(const char* name){
        listNode<T> *node = head;
        int count = 0;
        while (node)
        {
            if (*(node->data) == name)
            {
                count++;
            }
            node = node->next;
        }
        return count;
    }

    int getSize(const int num){
        listNode<T> *node = head;
        int count = 0;
        while (node)
        {
            if (*(node->data) == num)
            {
                count++;
            }
            node = node->next;
        }
        return count;
    }


    listNode<T> *find(T *data)
    {
        listNode<T> *node = head;

        while (node)
        {

            if (*(node->data) == *data)
            {
                return node;
            }
            node = node->next;
        }
        return NULL;
    }

    listNode<T> *find(const char *name)
    {
        listNode<T> *node = head;

        while (node)
        {
            if (*(node->data) == name)
            {
                return node;
            }
            node = node->next;
        }
        return NULL;
    }

    listNode<T> *find(const int id)
    {
        listNode<T> *node = head;

        while (node)
        {
            if (*(node->data) == id)
            {
                return node;
            }
            node = node->next;
        }
        return NULL;
    }

    listNode<T>* findIndex(int idx){
        listNode<T>* node = head;

        while(node && idx){
            idx--;            
            node = node->next;
        }
        return node;
    }

    listNode<T> *findBackwards(const char *name)
    {
        listNode<T> *node = tail;

        while (node)
        {
            if (*(node->data) == name)
            {
                return node;
            }
            node = node->previous;
        }
        return NULL;
    }

    listNode<T>* findBackwards(const int index){
        listNode<T>* node = tail;
        while(node){
            if(*(node->data) == index){
                return node;
            }
            node = node->previous;
        }
        return NULL;
    }

    void printList()
    {
        listNode<T> *curr = head;
        while (curr != nullptr)
        {
            std::cout << curr->data << " ";
            curr = curr->next;
        }
    }
    bool operator==(const DoublyLinkedList<T> &other)
    {
        return true;
    }

    friend std::ostream &operator<<(std::ostream &out, const DoublyLinkedList<T> *list)
    {
        listNode<T> *node = list->head;

        while (node)
        {
            out << node;
            node = node->next;
        }
        return out;
    }
};
