#ifndef BASE_LINKED_LISTS_H

#define SLLStackPushN(h, n, next) (h == 0) ? (h = n) : (n->next = h, h = n)
#define SLLStackPush(h, n) SLLStackPushN(h, n, next)

#define BASE_LINKED_LISTS_H
#endif
