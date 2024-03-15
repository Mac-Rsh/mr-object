/**
 * @copyright (c) 2024, MacRsh
 *
 * @date 2024-03-15    MacRsh    First version
 */

#ifndef _MR_OBJECT_H
#define _MR_OBJECT_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @addtogroup Object
 * @{
 */

/* Object name max length(configurable) */
#define MR_CFG_OBJECT_NAME_MAX_LEN      (16)

/* Assert */
#define MR_OBJECT_ASSERT(_ex)
/* Interrupt */
#define MR_OBJECT_DISABLE_INTERRUPT()
#define MR_OBJECT_ENABLE_INTERRUPT()
/* Printf */
#define MR_OBJECT_PRINTF(_fmt, ...)

/**
 * @brief Object structure.
 */
struct mr_object
{
    uint32_t magic;                                         /**< Magic number */
#ifndef MR_CFG_OBJECT_NAME_MAX_LEN
#define MR_CFG_OBJECT_NAME_MAX_LEN      (16)
#endif /* MR_CFG_OBJECT_NAME_MAX_LEN */
    char name[MR_CFG_OBJECT_NAME_MAX_LEN];                  /**< Name */
    struct mr_object_list
    {
        struct mr_object_list *next;
        struct mr_object_list *prev;
    } list, clist;                                          /**< List */
    struct mr_object *parent;                               /**< Parent */
};

struct mr_object *mr_object_find(const char *path);
int mr_object_register(struct mr_object *object, const char *path);
int mr_object_unregister(struct mr_object *object);
struct mr_object *mr_object_parent_get(struct mr_object *object);
void mr_object_tree_print(struct mr_object *object);
/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MR_OBJECT_H */
