/**
 * @copyright (c) 2024, MacRsh
 *
 * @date 2024-03-15    MacRsh    First version
 */

#include "mr_object.h"
#include <string.h>

/* Statically initializes an object */
#define OBJECT_INIT(_object, _name)                                            \
    {                                                                          \
        .name = (_name),                                                       \
        .list = {.next = &(_object).list, .prev = &(_object).list},            \
        .clist                                                                 \
            = {.next = &(_object).clist,                                       \
               .prev = &(_object).clist }                                      \
    }

/* Root object */
static struct mr_object root_object = OBJECT_INIT(root_object, "root");

/* This function is used to check if the list is inited */
static inline int list_is_inited(struct mr_object_list *list)
{
    return ((list->next != NULL) && (list->prev != NULL));
}

/* This function is used to init the list */
static inline void list_init(struct mr_object_list *list)
{
    list->next = list;
    list->prev = list;
}

/* This function is used to insert the node before the list */
static inline void list_insert_before(struct mr_object_list *list,
                                      struct mr_object_list *node)
{
    list->prev->next = node;
    node->prev = list->prev;
    list->prev = node;
    node->next = list;
}

/* This function is used to remove the node from the list */
static inline void list_remove(struct mr_object_list *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;
    node->next = node->prev = node;
}

/* This function is used to find the object from the parent */
static struct mr_object *object_find_from_parent(struct mr_object *parent,
                                                 const char *name)
{
#define CONTAIN_OF(pointer, type, member)                                      \
    ((type *)((char *)(pointer) - (size_t)(&((type *)0)->member)))

    for (struct mr_object_list *list = parent->clist.next;
         list != &parent->clist; list = list->next)
    {
        struct mr_object *object = CONTAIN_OF(list, struct mr_object, list);
        if (strncmp(name, object->name, MR_CFG_OBJECT_NAME_MAX_LEN) == 0)
        {
            return object;
        }
    }
    return NULL;
}

/* This function is used to register the object to the parent */
static int object_register_to_parent(struct mr_object *object, const char *name,
                                     struct mr_object *parent)
{
    /* Check if the object exists */
    if (object_find_from_parent(parent, name) != NULL)
    {
        return -1;
    }

    /* Initialize the list */
    if (!list_is_inited(&object->list))
    {
        list_init(&object->list);
    }
    if (!list_is_inited(&object->clist))
    {
        list_init(&object->clist);
    }

    /* Register the object */
    strncpy(object->name, name, MR_CFG_OBJECT_NAME_MAX_LEN);
    list_insert_before(&parent->clist, &object->list);
    object->parent = parent;
#define OBJECT_MAGIC (0xdeadbeef)
    object->magic = OBJECT_MAGIC;
    return 0;
}

/* This function is used to find the next parent from the path */
static struct mr_object *next_parent_find_from_path(struct mr_object *parent,
                                                    const char **path)
{
#define BOUND(_val, _min, _max)                                                \
    ((_val) < (_min) ? (_min) : ((_val) > (_max) ? (_max) : (_val)))

    if ((*path)[0] == '/')
    {
        (*path)++;
    }

    const char *next_path = strchr((*path), '/');
    if (next_path != NULL)
    {
        char name[MR_CFG_OBJECT_NAME_MAX_LEN + 1] = {0};

        /* Get the object name */
        size_t len = BOUND(next_path - (*path), 0, MR_CFG_OBJECT_NAME_MAX_LEN);
        strncpy(name, *path, len);
        name[len] = '\0';
        *path = next_path + 1;

        return object_find_from_parent(parent, name);
    } else
    {
        return NULL;
    }
}

/* This function is used to register the object by path */
static int object_register_by_path(struct mr_object *parent,
                                   struct mr_object *object, const char *path)
{
    struct mr_object *next_parent = next_parent_find_from_path(parent, &path);
    if (next_parent != NULL)
    {
        return object_register_by_path(next_parent, object, path);
    } else
    {
        return object_register_to_parent(object, path, parent);
    }
}

/* This function is used to find the object by path */
static struct mr_object *object_find_by_path(struct mr_object *parent,
                                             const char *path)
{
    struct mr_object *next_parent = next_parent_find_from_path(parent, &path);
    if (next_parent != NULL)
    {
        return object_find_by_path(next_parent, path);
    } else
    {
        return object_find_from_parent(parent, path);
    }
}

/* This function is used to print the tree of an object */
static void object_tree_print(struct mr_object *parent, int level)
{
    if (level == 0)
    {
        MR_OBJECT_PRINTF("|--- %-*s", MR_CFG_OBJECT_NAME_MAX_LEN, parent->name);
    } else
    {
        MR_OBJECT_PRINTF("%*s|--- %-*s", level, " ", MR_CFG_OBJECT_NAME_MAX_LEN,
                         parent->name);
    }
    MR_OBJECT_PRINTF("\r\n");
    for (struct mr_object_list *child = parent->clist.next;
         child != &parent->clist; child = child->next)
    {
        struct mr_object *object = CONTAIN_OF(child, struct mr_object, list);
        object_tree_print(object, level + 5);
    }
}

/**
 * @brief This function find an object by path.
 *
 * @param path The path of the object.
 *
 * @return The pointer of the object, or NULL if not found.
 */
struct mr_object *mr_object_find(const char *path)
{
    MR_OBJECT_ASSERT(path != NULL);

    return object_find_by_path(&root_object, path);
}

/**
 * @brief This function register an object by path.
 *
 * @param object The pointer of the object.
 * @param path The path of the object.
 *
 * @return 0 if success, -1 if failed.
 */
int mr_object_register(struct mr_object *object, const char *path)
{
    MR_OBJECT_ASSERT(object != NULL && path != NULL);
    MR_OBJECT_ASSERT(object->magic != OBJECT_MAGIC);

    MR_OBJECT_DISABLE_INTERRUPT();
    int ret = object_register_by_path(&root_object, object, path);
    MR_OBJECT_ENABLE_INTERRUPT();
    return ret;
}

/**
 * @brief This function unregister an object.
 *
 * @param object The pointer of the object.
 *
 * @return 0 if success, -1 if failed.
 */
int mr_object_unregister(struct mr_object *object)
{
    MR_OBJECT_ASSERT(object != NULL);
    MR_OBJECT_ASSERT(object->magic == OBJECT_MAGIC);

    MR_OBJECT_DISABLE_INTERRUPT();
    list_remove(&object->list);
    object->magic = 0;
    object->parent = NULL;
    MR_OBJECT_ENABLE_INTERRUPT();
    return 0;
}

/**
 * @brief This function get the parent of an object.
 *
 * @param object The pointer of the object.
 *
 * @return The pointer of the parent, or NULL if not found.
 */
struct mr_object *mr_object_parent_get(struct mr_object *object)
{
    MR_OBJECT_ASSERT(object != NULL);
    MR_OBJECT_ASSERT(object->magic == OBJECT_MAGIC);

    return object->parent;
}

/**
 * @brief This function print the tree of an object.
 *
 * @param object The pointer of the object.
 *
 * @note If object is NULL, it will print the tree of the root object.
 */
void mr_object_tree_print(struct mr_object *object)
{
    if (object == NULL)
    {
        object = &root_object;
    }
    object_tree_print(object, 0);
}
