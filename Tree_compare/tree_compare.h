#ifndef TREE_COMPARE_H
#define TREE_COMPARE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const size_t kCommandTerminalDumpLen = 200;

typedef struct node
{
    void* data;

    node* more;
    node* less_or_equal;

    size_t size;
} node_t;

enum TreeError
{
    kDoneTree     = 0,
    kCantCtorTree = 1,
    kCantPushTree = 2,
    kCantDumpTree = 3,
};

enum TreeError TreeCtor (node_t* const node, void* const data, const size_t size_elem);
enum TreeError TreeDtor (node_t* const root);
enum TreeError PushNode (node_t* const node, void* const element, int compare (void* first, void* second));
enum TreeError DumpTree (const node_t* const root, void PrintData (void* const data, FILE* const dump_file));

#endif // TREE_COMPARE_H
