#include <iostream>

#ifndef __ATTRIBUTE_H__
#define __ATTRIBUTE_H__

extern char *StrDup(const char *str);

class Attribute
{
private:
  int counterAttribute;
  char *attributeName;
  char *attributeValue;

public:
  Attribute(const char *attributeName, const char *attributeValue)
  {
    this->attributeName = StrDup(attributeName);
    this->attributeValue = StrDup(attributeValue);
    counterAttribute++;
  }
  ~Attribute()
  {
    free(attributeName);
    free(attributeValue);
  }

  int getCounterAttribute()
  {
    return counterAttribute;
  }

  char *getAttributeName()
  {
    return attributeName;
  }

  char *getAttributeValue()
  {
    return attributeValue;
  }

  bool operator==(const Attribute &otherAttribute)
  {
    return strcmp(attributeName, otherAttribute.attributeName) == 0;
  }

  bool operator==(const char *attrName)
  {
    return strcmp(attributeName, attrName) == 0;
  }

  bool operator==(const int attr)
  {
    return false;
  }

  Attribute &operator=(const char *attValue)
  {
    free(attributeValue);
    attributeValue = StrDup(attValue);
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &out, const Attribute *node)
  {
    if (node)
    {
      out << node->attributeValue;
    }
    return out;
  }
};

#endif
