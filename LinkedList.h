#include <stdlib.h>
#include <string.h>

struct ListItem
{
    char* name;
    ListItem* next;
    ListItem* prev;
}typedef ListItem;

struct LinkedList
{
    int size;
    ListItem* head;
    ListItem* tail;
}typedef LinkedList;

LinkedList* createLinkedList()
{
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void deleteLinkedLinst(LinkedList** list)
{
    ListItem *head = (*list)->head;
    ListItem* next = NULL;
    while (head)
    {
        next = head->next;
        free(head->name);
        head->name = NULL;
        free(head);
        head = next;
    }
    free(*list);
    *list = NULL;
}

void pushBack(LinkedList* list, const char* name)
{
    ListItem* item = (ListItem*)malloc(sizeof(ListItem));
    item->name = strdup(name);
    item->next = NULL;
    item->prev = list->tail;
    if (list->tail)
    {
        list->tail->next = item;
    }
    list->tail = item;
    if (!list->head)
    {
        list->head = item;
    }
    ++list->size;
}

ListItem* popBack(LinkedList* list)
{
    ListItem* item = list->tail;
    list->tail = list->tail->prev;
    if (list->tail)
    {
        list->tail->next = NULL;
    }
    if (item == list->head)
    {
        list->head = NULL;
    }
    return item;
}
