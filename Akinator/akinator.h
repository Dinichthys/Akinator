#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>
#include <stdlib.h>

#include "../Stack/stack.h"

static const size_t kCommandTerminalDefLen  = 400;
static const size_t kCommandTerminalDumpLen = 200;
static const size_t kLenFlag                = 100;
static const size_t kLenFileName            = 100;
static const size_t kLenElement             = 100;
static const size_t kLenAnswer              = 10;
static const size_t kDepthStack             = 10;

static const char kRunFlag        [kLenFlag] = "Отгадывать";
static const char kCreateFlag     [kLenFlag] = "Создать";
static const char kDefinitionFlag [kLenFlag] = "Определение";
static const char kComparisonFlag [kLenFlag] = "Сравнение";
static const char kWriteFlag      [kLenFlag] = "Запись";
static const char kExitFlag       [kLenFlag] = "Выход";

typedef char tree_elem_t [kLenElement];

typedef struct node
{
    tree_elem_t data;

    node* left;
    node* right;

    char answer [kLenAnswer];
} node_t;

enum TreeErrorAkin
{
    kDoneTreeAkin               = 0,

    kCantCtorTreeAkin           = 1,

    kCantReadNameOfFileAkin     = 2,

    kCantCallocInputBuffer      = 3,
    kCantOpenDataBase           = 4,
    kCantReadDataBaseAkin       = 5,

    kRunTimeErrorAkin           = 6,

    kCantPushTreeAkin           = 7,

    kCantDumpTreeAkin           = 8,

    kCantReadAnswerAddElem      = 9,
    kCantReadAnswerReplay       = 10,

    kCantReadSubjectAkin        = 11,

    kDidntFoundSubject          = 12,

    kCantPrintDefinition        = 13,
    kCantMakeDefinition         = 14,
    kCantCtorStackDefinition    = 15,
    kCantDtorStackDefinition    = 16,

    kCantPrintComparison        = 17,
    kCantMakeComparison         = 18,
    kCantCtorStackComparison    = 19,
    kCantDtorStackComparison    = 20,

    kNoAnswerInStack            = 21,
    kNoDataInStack              = 22,

    kInvalidFlagAkin            = 23,
};

enum TreeResult
{
    kFalseResult             = 0,
    kTrueResult              = 1,
    kCantFindAnswerForResult = 2,
    kInvalAnswer             = 3,
};

enum TreeErrorAkin ParseFlags      (node_t* const root);
enum TreeErrorAkin RunAkinatorFlag (node_t* const root);
enum TreeResult    GetAnswer       (void);
enum TreeErrorAkin CreateFlag      (node_t* const root);
enum TreeErrorAkin WriteFlag       (const node_t* const root);
enum TreeErrorAkin DefinitionFlag  (node_t* const root);
enum TreeErrorAkin ComparisonFlag  (node_t* const root);
enum TreeErrorAkin TreeCtorAkin    (node_t* const node, const tree_elem_t data);
enum TreeErrorAkin TreeDtorAkin    (node_t* const root);
enum TreeErrorAkin PushNodeAkin    (node_t* const node, const tree_elem_t element, const bool right);
enum TreeErrorAkin DumpTreeAkin    (const node_t* const root);
const char* EnumToStr              (const enum TreeErrorAkin error);

#endif // AKINATOR_H
