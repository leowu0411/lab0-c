#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *ret = malloc(sizeof(struct list_head));
    if (ret)
        INIT_LIST_HEAD(ret);
    return ret;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;
    element_t *entry = NULL, *safe;
    list_for_each_entry_safe (entry, safe, head, list) {
        q_release_element(entry);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *node = malloc(sizeof(element_t));
    if (!node)
        return false;

    int str_len = sizeof(s) + 1;
    node->value = malloc(str_len * sizeof(char));
    if (!node->value) {
        free(node);
        return false;
    }

    strncpy(node->value, s, str_len);
    list_add(&node->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *node = malloc(sizeof(element_t));
    if (!node)
        return false;

    int str_len = sizeof(s) + 1;
    node->value = malloc(str_len * sizeof(char));
    if (!node->value) {
        free(node);
        return false;
    }

    strncpy(node->value, s, str_len);
    list_add_tail(&node->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *node;
    node = list_entry(head->next, typeof(*node), list);
    if (!(node->value) || !sp)
        return node;

    strncpy(sp, node->value, bufsize - 1);
    sp[bufsize] = '\0';
    list_del_init(&node->list);
    return node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *node;
    node = list_entry(head->prev, typeof(*node), list);
    if (!(node->value) || !sp)
        return node;

    strncpy(sp, node->value, bufsize - 1);
    sp[bufsize] = '\0';
    list_del_init(&node->list);
    return node;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head || !head->next)
        return 0;
    int num = 0;
    struct list_head *ptr;
    for (ptr = head->next; ptr != head; ptr = ptr->next) {
        num++;
    }
    return num;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;

    struct list_head **fest = &head->next, **slow;
    element_t *dlt;
    if (!(*fest))
        return NULL;
    for (slow = &head; (*fest)->next && (*fest)->next->next;
         fest = &(*fest)->next->next) {
        slow = &(*slow)->next;
    }
    slow = &(*slow)->next;
    dlt = list_entry(*slow, typeof(*dlt), list);
    *slow = (*slow)->next;
    q_release_element(dlt);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head))
        return false;
    struct list_head *ptr = head->next;
    element_t *cur = list_entry(ptr, element_t, list),
              *nxt = list_entry(ptr->next, element_t, list);
    while (&cur->list != head && &nxt->list != head) {
        if (strcmp(cur->value, nxt->value) == 0) {
            while (&nxt->list != head && strcmp(cur->value, nxt->value) == 0) {
                list_del(ptr);
                ptr = &nxt->list;
                q_release_element(cur);
                cur = list_entry(ptr, element_t, list);
                nxt = list_entry(ptr->next, element_t, list);
            }
            list_del(ptr);
            q_release_element(cur);
        }
        ptr = &nxt->list;
        cur = list_entry(ptr, element_t, list);
        nxt = list_entry(ptr->next, element_t, list);
    }
    return true;
}
/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head))
        return;
    struct list_head *slow = head->next, *fast = slow->next;
    while (slow != head && fast != head) {
        slow->prev->next = fast;
        fast->next->prev = slow;
        slow->next = fast->next;
        fast->next = slow;
        fast->prev = slow->prev;
        slow->prev = fast;

        slow = slow->next;
        fast = slow->next;
    }
    return;
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
