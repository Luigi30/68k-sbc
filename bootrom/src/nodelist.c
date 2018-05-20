#include "nodelist.h"

void LIST_Init(List *list, uint8_t type)
{
  //An empty list loops back on itself.
  list->lh_Head = (struct Node *)((long)list + 4);
  list->lh_Tail = NULL;
  list->lh_TailPred = (struct Node *)((long)list);
  list->lh_Type = type;
}
