#include <iostream>
#include "section.h"
#include "node.h"
#include "dlList.h"

#ifndef __CSS_H__
#define __CSS_H__

class CSS
{
    int id;

public:
    DoublyLinkedList<Node> *nodes;

public:
    CSS()
    {
        this->id = 0;
        this->nodes = new DoublyLinkedList<Node>();
        this->addNode();
    }

    ~CSS()
    {
        delete this->nodes;
    }

    Node *addNode()
    {
        Node *node = new Node(id);
        this->nodes->add(node);
        id++;
        return node;
    }

    Section *addSection()
    {
        if (this->nodes->getSize() == 0)
        {
            this->addNode();
        }
        Section *sec = this->nodes->tail->data->addSection();
        if (sec == NULL)
        {
            this->addNode();
            sec = this->nodes->tail->data->addSection();
        }
        return sec;
    }

    Attribute *findAttribute(const char *attrName)
    {
        listNode<Node> *node = this->nodes->tail;
        Attribute *att = NULL;
        while (node && att == NULL)
        {
            att = node->data->findAttribute(attrName);
            node = node->previous;
        }
        return att;
    }

    Selector *findSelector(const char *selName)
    {
        listNode<Node> *node = this->nodes->tail;
        Selector *sel = NULL;
        while (node && sel == NULL)
        {
            sel = node->data->findSelector(selName);
            node = node->previous;
        }
        return sel;
    }

    Section *findSection(const int secIndex)
    {

        listNode<Node> *node = this->nodes->findIndex(((secIndex - 1) / T));
        if (node)
        {
            return node->data->findSection(secIndex);
        }

        return NULL;
    }

    bool deleteSection(const int secIndex)
    {
        listNode<Node> *node = this->nodes->findIndex((secIndex - 1) / T);
        if (node)
        {
            if (node->data->deleteSection(secIndex))
            {
                if (node->data->getCount() == 0)
                {
                    this->nodes->remove(node);
                    this->id--;
                }
                else
                {
                    while (node && node->next)
                    {
                        Section *sec = node->next->data->popSection();
                        if (sec == NULL)
                        {
                            return false;
                        }
                        if (node->next->data->getCount() == 0)
                        {
                            this->nodes->remove(node->next);
                            this->id--;
                        }
                        if (node->data->pushSection(sec) == false)
                        {
                            return false;
                        }
                        node = node->next;
                    }
                }
                return true;
            }
        }

        return false;
    }

    bool checkIfSecEmpty(const int secIdx)
    {
        listNode<Node> *node = this->nodes->findIndex(((secIdx - 1) / T));
        if (node->data->checkIfSecEmpty(secIdx))
        {
            return true;
        }
        return false;
    }

    bool deleteAttributeInSection(const int secId, const char *attName)
    {
        Section *sec = this->findSection(secId);
        if (sec)
        {

            if (sec->deleteAttribute(attName))
            {
                if (sec->getAttributeCount() == 0)
                {
                    this->deleteSection(secId);
                }
                return true;
            }
        }
        return false;
    }

    Attribute *findAttributeInSelector(const char *selName, const char *attrName)
    {
        listNode<Node> *node = this->nodes->tail;
        Attribute *att = NULL;
        while (node && att == NULL)
        {
            att = node->data->findAttributeInSelector(selName, attrName);
            node = node->previous;
        }
        return att;
    }

    int countSections()
    {
        listNode<Node> *node = this->nodes->tail;
        int count = 0;
        while (node)
        {
            count += node->data->getCount();
            node = node->previous;
        }
        return count;
    }

    int countSelectors(const char *name)
    {
        listNode<Node> *node = this->nodes->tail;
        int count = 0;
        while (node)
        {
            count += node->data->countSelectors(name);
            node = node->previous;
        }
        return count;
    }

    int countAttributes(const char *name)
    {
        listNode<Node> *node = this->nodes->head;
        int count = 0;
        while (node)
        {
            count += node->data->countAttributes(name);
            node = node->next;
        }
        return count;
    }

    Node *findNode(const int id)
    {
        listNode<Node> *node = this->nodes->find(id);
        if (node)
        {
            return node->data;
        }
        return NULL;
    }

    friend std::ostream &operator<<(std::ostream &out, const CSS *css)
    {
        out << "CSS:\n"
            << css->nodes << "\n";
        return out;
    }
};

#endif