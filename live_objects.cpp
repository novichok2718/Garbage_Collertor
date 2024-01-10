#include <stdlib.h>
#include <string.h>
#include "os_mem.h"
#include "LinkedList.h"
#define MAX_LINKS_NUM 16
#define MAX_LENGTH_NAME 32

struct Object
{
    char* name;
    int8_t is_free;
    int8_t is_root;
    LinkedList* links;
}typedef Object;

Object** mm = NULL;
int SIZE = 0;

// main functions prototype
int create(int size, int num_pages);
int destroy();
int create_object(const char* name);
int destroy_object(const char* name);
void print_objects();
int set_root(const char* name);
int link(const char* object1_name, const char* object2_name);
void collect_live_objects(void);
void setup_memory_manager(memory_manager_t* mm);

// Helper functions prototype
int8_t is_contains(char* name);

int main()
{
    memory_manager_t mm;
    setup_memory_manager(&mm);
/*    mm.create(1000, 2);// expected: 1, passed
    mm.destroy();// expected: 1, passed
    mm.destroy();// expected: 0, passed
    mm.create(1000, 1);// expected: 1, passed
    mm.create(1000, 3);// expected: 0, passed
    mm.destroy();// expected: 1, passed*/
    
/*    mm.create(2, 1) != 0; //expected: 1, passed
    mm.create_object("o1");// expected: 1, passed
    mm.create_object("o2"); //expected: 1, passed
    mm.destroy_object("abc"); //expected: 0, passed
    mm.destroy_object("o1"); //expected: 1, passed
    mm.destroy_object("o1"); //expected: 0, passed
    mm.destroy_object("o2"); //expected: 1, passed
    mm.create_object("o1"); //expected: 1, passed
    mm.destroy_object("o1"); //expected: 1, passed
    mm.create_object("o1"); //expected: 1, passed
    mm.create_object("o1"); //expected: 0, passed
    mm.create_object("o2"); //expected: 1, passed
    mm.create_object("o3"); //expected: 0, passed
    mm.destroy_object("o1"); //expected: 1, passed
    mm.destroy(); //expected: 1, passed*/

    mm.create(10, 1) != 0; //expected: 1, passed
    mm.create_object("o9");// expected: 1, passed
    mm.create_object("o1");// expected: 1, passed
    mm.create_object("o2");// expected: 1, passed
    mm.create_object("o3");// expected: 1, passed
    mm.create_object("o4");// expected: 1, passed
    mm.create_object("o5");// expected: 1, passed
    mm.create_object("o6");// expected: 1, passed
    mm.create_object("o7");// expected: 1, passed
    mm.create_object("o8");// expected: 1, passed
    mm.create_object("o0");// expected: 1, passed
    mm.set_root("abc");// expected: 0, passed
    mm.set_root("o1");// expected: 1, passed
    mm.set_root("o2");// expected: 1, passed
    mm.link("o1", "abc");// expected: 0, passed
    mm.link("o1", "o3"); //expected: 1, passed
    mm.destroy_object("o1");// expected: 1, passed
    mm.link("o1", "o4"); //expected: 0, passed
    mm.link("o2", "o4"); //expected: 1, passed
    mm.link("o4", "o2"); //expected: 1, passed
    mm.link("o5", "o6"); //expected: 1, passed
    mm.destroy(); //expected: 1, passed
    return 0;
}

// Implementation of helper functions
int8_t is_contains(char* name)
{
    int flag = 0;
    for (int i = 0; i != SIZE; ++i)
    {
        if (!mm[i]->is_free && !strcmp(mm[i]->name, name))
        {
            flag = 1;
        }
    }
    free(name);
    return flag;
}

// Implementation of main functions
void setup_memory_manager(memory_manager_t* mm)
{
    mm->create = create;
    mm->destroy = destroy;
    mm->create_object = create_object;
    mm->destroy_object = destroy_object;
    mm->print_objects = print_objects;
    mm->set_root = set_root;
    mm->link = link;
    mm->collect_live_objects = collect_live_objects;
}

int create(int size, int num_pages)
{
    if (!mm && size > 0)
    {
        mm = (Object**)malloc(sizeof(Object*) * size);
        SIZE = size;
        for (int i = 0; i != size; ++i)
        {
            mm[i] = (Object*)malloc(sizeof(Object));
            mm[i]->is_free = 1;
            mm[i]->is_root = 0;
            mm[i]->name = NULL;
            mm[i]->links = NULL;
        }
        return 1;
    }
    return 0;
}

int destroy()
{
    if (mm)
    {
        for (int i = 0; i != SIZE; ++i)
        {
            if (!mm[i]->is_free)
            {
                deleteLinkedLinst(&mm[i]->links);
                free(mm[i]->name);
            }
            free(mm[i]);
            mm[i] = NULL;
        }
        free(mm);
        mm = NULL;
        return 1;
    }
    return 0;
}

int create_object(const char* name)
{
    if (mm && name)
    {
        for (int i = 0; i != SIZE; ++i)
        {
            if (mm[i]->is_free && !is_contains(strdup(name)))
            {
                mm[i]->name = strdup(name);
                mm[i]->links = createLinkedList();
                mm[i]->is_free = 0;
                return 1;
            }
        }
    }
    return 0;
}

int destroy_object(const char* name)
{
    if (mm && name)
    {
        for (int i = 0; i != SIZE; ++i)
        {
            if (!mm[i]->is_free && !strcmp(name, mm[i]->name))
            {
                deleteLinkedLinst(&mm[i]->links);
                free(mm[i]->name);
                mm[i]->name = NULL;
                mm[i]->is_free = 1;
                return 1;
            }
        }
    }
    return 0;
}

void print_objects()
{
    for (int i = 0; i < SIZE; ++i)
    {
        if (!mm[i]->is_free)
        {
            printf("%s \n", mm[i]->name);
        }
    }
}

int set_root(const char* name)
{
    if (mm && name)
    {
        for (int i = 0; i != SIZE; ++i)
        {
            if (!strcmp(name, mm[i]->name))
            {
                mm[i]->is_root = 1;
                return 1;
            }
        }
    }
    return 0;
}

int link(const char* object1_name, const char* object2_name)
{
    if (mm && object1_name && object2_name)
    {
        for (int i = 0; i != SIZE; ++i)
        {
            if (!mm[i]->is_free && !strcmp(object1_name, mm[i]->name) && is_contains(strdup(object2_name)))
            {
                pushBack(mm[i]->links, object2_name);
                return 1;
            }
        }
    }
    return 0;
}

void collect_live_objects(void)
{

}

