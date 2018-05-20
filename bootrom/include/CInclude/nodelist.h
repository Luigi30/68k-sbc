#ifndef NODE_H
#define NODE_H

#include <stdint.h>
#include <string.h>

typedef struct
{
  struct MinNode *mln_Succ;
  struct MinNode *mln_Pred;
} MinNode;

typedef struct
{
  struct Node    *ln_Succ;
  struct Node    *ln_Pred;
  uint8_t  ln_Type;
  int8_t   ln_Pri;
  uint8_t *ln_Name;
} Node;

typedef struct
{
  MinNode *mlh_Head;
  MinNode *mlh_Tail;
  MinNode *mlh_TailPred;
} MinList;

typedef struct
{
  Node   *lh_Head;
  Node   *lh_Tail;
  Node   *lh_TailPred;
  uint8_t lh_Type;
  uint8_t lh_Pad;
} List;

void LIST_Init(List *list, uint8_t type);

extern void LIST_AddHead(__reg("a0") List *list, __reg("a1") Node *node);
extern void LIST_AddTail(__reg("a0") List *list, __reg("a1") Node *node);

extern Node *LIST_RemHead(__reg("a0") List *list);
extern Node *LIST_RemTail(__reg("a0") List *list);
extern Node *LIST_Remove(__reg("a1") Node *node);

#endif
