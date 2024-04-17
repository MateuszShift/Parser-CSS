#include <iostream>
#include "dlList.h"

#ifndef __SELECTOR_H__
#define __SELECTOR_H__

extern char *StrDup(const char *str);

class Selector
{

private:

  int counterSelector;
  char *selectorName;

public:
  Selector(const char *name)
  {
    this->selectorName = StrDup(name);
    counterSelector++;
  }

  ~Selector()
  {
    free(this->selectorName);
  }

  int getCounterSelector()
  {
    return counterSelector;
  }

  char *getName()
  {
    return selectorName;
  }

  bool operator==(const Selector &otherSelector)
  {
    return strcmp(selectorName, otherSelector.selectorName) == 0;
  }

  bool operator==(const char *selName)
  {
    return strcmp(selectorName, selName) == 0;
  }

  bool operator==(const int sel)
  {
    return false;
  }

  Selector &operator=(const Selector &otherSelector)
  {
    free(selectorName);
    selectorName = StrDup(otherSelector.selectorName);
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &out, const Selector *sel)
  {
    if (sel)
    {
      out << sel->selectorName;
    }
    return out;
  }
};

#endif
