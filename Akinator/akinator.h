#ifndef AKINATOR_H
#define AKINATOR_H

typedef char tree_elem_t [100];

typedef struct node
{
    tree_elem_t data;

    node_t* next;
    node_t* previous;
} node_t;

enum TreeError
{
    kDoneTree     = 0,
    kCantPushTree = 1,
};

#endif // AKINATOR_H
