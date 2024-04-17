#include <iostream>
#include "dlList.h"
#include "selectors.h"
#include "attributes.h"

#ifndef __SECTION_H__
#define __SECTION_H__

class Section
{
  DoublyLinkedList<Selector> *selectors;
  DoublyLinkedList<Attribute> *attributes;

public:
  Section()
  {
    this->selectors = new DoublyLinkedList<Selector>();
    this->attributes = new DoublyLinkedList<Attribute>();
  }


  ~Section()
  {
    delete this->selectors;
    delete this->attributes;
  }

  Attribute *findAttribute(const char *attrName)
  {
    listNode<Attribute> *att = this->attributes->findBackwards(attrName);
    if (att)
    {
      return att->data;
    }
    return NULL;
  }

  bool findIfEmptyAttr(const int idx){
    if(this->attributes->head == NULL){
      return true;
    }
    return false;
  }

  Selector *findSelector(const char *selName)
  {
    listNode<Selector> *sel = this->selectors->findBackwards(selName);
    if (sel)
    {
      return sel->data;
    }
    return NULL;
  }

  Selector* findSelectorByIndex(const int index){
    listNode<Selector>*sel = this->selectors->findIndex(index-1);
    if(sel){
      return sel->data;
    }
    return NULL;
    
  }

  Selector* findSelectors(const int selIndex){
    listNode<Selector>* selector = this->selectors->findBackwards(selIndex);
    if(selector){
      return selector->data;
    }
    return NULL;
  }

  Attribute* printAttValue(const char* name){
    listNode<Attribute> * attribute = this->attributes->findBackwards(name);
    if(attribute){
      return attribute->data;
    }
    return NULL;
  }

  bool addAttribute(const char *attributeName, const char *attributeValue)
  {
    Attribute *att = this->findAttribute(attributeName);
    if (att == NULL)
    {
      Attribute *att = new Attribute(attributeName, attributeValue);
      this->attributes->add(att);
    }
    else
    {
      *att = attributeValue;
    }
    return true;
  }

  bool updateAttribute(const char *attributeName, const char *attributeValue)
  {
    Attribute *att = this->findAttribute(attributeName);
    if (att != NULL)
    {
      *att = attributeValue;
      return true;
    }
    return false;
  }

  int getAttributeCount(){
    return this->attributes->getSize();
  }

  bool addSelector(const char *name)
  {
    if (this->findSelector(name) == NULL)
    {
      Selector *sel = new Selector(name);
      this->selectors->add(sel);
      return true;
    }
    return false;
  }

  int countSelectors(){
    return this->selectors->getSize();
  }

  int countSelectors(const char* selName){
    return this->selectors->getSize(selName);
  }

  int countAttributes(const char* name){
    return this->attributes->getSize(name);
  }

  int countAttributes(){
    return this->attributes->getSize();
  }

  bool deleteAttribute(const char *attributeName)
  {
    listNode<Attribute> *att = this->attributes->find(attributeName);
    if (att != NULL)
    {
      Attribute *data = att->data;
      this->attributes->remove(att);
      delete data;
      return true;
    }
    return false;
  }

  bool deleteSelector(const char *selName)
  {
    listNode<Selector> *selec = this->selectors->find(selName);
    if (selec != NULL)
    {
      Selector *data = selec->data;
      this->selectors->remove(selec);
      delete data;
      return true;
    }
    return false;
  }
  friend std::ostream &operator<<(std::ostream &out, const Section *sect)
  {
    if (sect)
    {
      out << "Section: ";
      out << "Selectors:"
          << sect->selectors;
      out << "Attributes:"
          << sect->attributes;
    }
    return out;
  }
};

#endif