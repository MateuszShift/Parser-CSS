#include <iostream>
#include "section.h"

#ifndef __NODE_H__
#define __NODE_H__

#define T 17

class Node
{
public:
    int index;
    int id;
    Section *sections[T];

public:
    Node(int id)
    {
        this->index = 0;
        this->id = id;
        for(int i = 0; i < T; i++)
        {
            sections[i] = NULL;
        }
    }

    ~Node() {
    }

    int getCount() { return index; }

    Section *addSection()
    {
        if (index < T)
        {
            sections[index] = new Section(); 
            index++;
            return sections[index - 1];
        }
        return NULL;
    }

    Attribute *findAttribute(const char *attrName)
    {
        Attribute *att = NULL;
        if (index > 0)
        {
            int idx = index;

            do
            {
                att = sections[idx - 1]->findAttribute(attrName);
                idx--;
            } while (idx && att == NULL);
        }
        return att;
    }

    Selector *findSelector(const char *selName)
    {
        Selector *sel = NULL;
        if (index > 0)
        {
            int idx = index;

            do
            {
                sel = sections[idx - 1]->findSelector(selName);
                idx--;
            } while (idx && sel == NULL);
        }
        return sel;
    }

    Attribute *findAttributeInSelector(const char *selName, const char *attrName)
    {
        if (index > 0)
        {
            int idx = index;

            do
            {
                if (sections[idx - 1]->findSelector(selName))
                {
                    Attribute * att = sections[idx - 1]->findAttribute(attrName);
                    if (att)
                    {
                        return att;
                    }
                }
                idx--;
            } while (idx);
        }
        return NULL;
    }

    bool deleteSection(const int secIndex)
    {
        int secId = (secIndex-1) % T;
        int nodeId = (secIndex-1) / T;
        if (nodeId == this->id)
        {
            if (secId >= 0 && secId < this->index) 
            {
                delete this->sections[secId];
                while (secId < (T - 1))
                {
                    this->sections[secId] = this->sections[secId + 1];
                    secId++;
                }
                this->sections[T - 1] = NULL;
                index--; 
                return true;
            }
        }
        return false;
    }

    bool checkIfSecEmpty(const int secIdx){
        Section* sec = this->sections[secIdx];
        if(sec->findIfEmptyAttr(secIdx)){
            return true;
        }
        return false;
    }

    Section *popSection()
    {
        Section *sec = this->sections[0];
        if (sec)
        {
            int secId = 0;
            while (secId < (T - 1))
            {
                this->sections[secId] = this->sections[secId + 1];
                secId++;
            }
            this->sections[T - 1] = NULL;
            index--;
            return sec;
        }
        return NULL;
    }

    bool pushSection(Section* sec){
        if(this->index == T-1){
            this->sections[T-1] = sec;
            index++;
            return true;
        }
        return false;
    }


    Section *findSection(const int secIndex)
    {
        int secId = (secIndex - 1) % T;
        int nodeId = (secIndex - 1) / T;
        if (nodeId == this->id)
        {
            if (secId >= 0 && secId < this->index)
            {
                return this->sections[secId];
            }
        }
        return NULL;
    }

    int countSelectors(const char *name)
    {
        int idx = 0;
        int count = 0;
        while (idx < index)
        {
            count += sections[idx]->countSelectors(name);
            idx++;
        }
        return count;
    }

    int countAttributes(const char *name)
    {
        int idx = 0;
        int count = 0;
        while (idx < index)
        {
            count += sections[idx]->countAttributes(name);
            idx++;
        }
        return count;
    }

    bool addAttribute(const char *attributeName, const char *attributeValue)
    {
        return false;
    }

    bool operator==(const Node &node)
    {
        return false;
    }

    bool operator==(const int node)
    {
        return false;
    }

    bool operator==(const char *node)
    {
        return false;
    }

    friend std::ostream &operator<<(std::ostream &out, const Node *node)
    {
        if (node)
        {
            int id = 0;
            out << "Node: " << node->id << "\n";
            while (id < node->index)
            {
                out << node->sections[id];
                id++;
            }
        }
        return out;
    }
};

#endif