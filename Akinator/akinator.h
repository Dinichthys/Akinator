#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const size_t kCommandTerminalDumpLen = 200;
static const size_t kLenElement = 100;
static const size_t kLenAnswer = 10;

typedef char tree_elem_t [kLenElement];

typedef struct node
{
    tree_elem_t data;

    node* left;
    node* right;
} node_t;

enum TreeErrorAkin
{
    kDoneTreeAkin     = 0,
    kCantCtorTreeAkin = 1,
    kCantPushTreeAkin = 2,
    kCantDumpTreeAkin = 3,
    kRunTimeErrorAkin = 4,
    kCantReadAnswer   = 5,
};

enum TreeResult
{
    kFalseResult             = 0,
    kTrueResult              = 1,
    kCantFindAnswerForResult = 2,
    kInvalAnswer             = 3,
};

enum TreeErrorAkin RunAkinator (node_t* const root);
enum TreeErrorAkin TreeCtorAkin (node_t* const node, const tree_elem_t data);
enum TreeErrorAkin TreeDtorAkin (node_t* const root);
enum TreeErrorAkin PushNodeAkin (node_t* const node, const tree_elem_t element, const bool right);
enum TreeErrorAkin DumpTreeAkin (const node_t* const root);

#endif // AKINATOR_H
