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
    element_t *entry = NULL, *safe = NULL;
    // cppcheck-suppress unusedLabel
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

    int str_len = strlen(s) + 1;
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

    int str_len = strlen(s) + 1;
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
    sp[bufsize - 1] = '\0';
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
    sp[bufsize - 1] = '\0';
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

    struct list_head *slow = head->next, *fast = slow->next;
    element_t *dlt;
    for (; fast->next != head && fast->next->next != head;
         fast = fast->next->next) {
        slow = slow->next;
    }
    slow = slow->next;
    dlt = list_entry(slow, typeof(*dlt), list);
    list_del(slow);
    q_release_element(dlt);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head) || list_is_singular(head))
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
    if (!head || list_empty(head) || list_is_singular(head))
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
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    struct list_head *cur = head;
    do {
        struct list_head *tmp;
        tmp = cur->next;
        cur->next = cur->prev;
        cur->prev = tmp;
        cur = tmp;
    } while (cur != head);
    return;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    int round = q_size(head) / k;
    struct list_head *cur = head->next, *prev_tail, *new_tail;
    for (int i = 0; i < round; i++) {
        prev_tail = cur->prev;
        new_tail = cur;
        cur->prev = cur->next;
        cur = cur->next;
        for (int j = 1; j < (k - 1); j++) {
            struct list_head *tmp = cur->next;
            cur->next = cur->prev;
            cur->prev = tmp;
            cur = tmp;
        }
        new_tail->next = cur->next;
        cur->next->prev = new_tail;
        cur->next = cur->prev;
        prev_tail->next = cur;
        cur->prev = prev_tail;
        cur = new_tail->next;
    }
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    struct list_head less, greater;
    element_t *node = NULL, *safe = NULL;
    element_t *pivot = list_first_entry(head, element_t, list);
    list_del(&pivot->list);
    INIT_LIST_HEAD(&less);
    INIT_LIST_HEAD(&greater);
    // cppcheck-suppress unusedLabel
    list_for_each_entry_safe (node, safe, head, list) {
        if (strcmp(node->value, pivot->value) < 0) {
            list_del(&node->list);
            list_add_tail(&node->list, &less);
        } else {
            list_del(&node->list);
            list_add_tail(&node->list, &greater);
        }
    }
    q_sort(&less, descend);
    q_sort(&greater, descend);
    list_add(&pivot->list, head);
    if (descend) {
        list_splice(&greater, head);
        list_splice_tail(&less, head);
    } else {
        list_splice(&less, head);
        list_splice_tail(&greater, head);
    }
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head) || list_is_singular(head))
        return 0;
    char *max = list_entry(head->next, element_t, list)->value;
    element_t *entry, *safe;
    int count = 0;
    // cppcheck-suppress unusedLabel
    list_for_each_entry_safe (entry, safe, head, list) {
        if (strcmp(entry->value, max) < 0) {
            list_del(&entry->list);
            q_release_element(entry);
        } else {
            max = entry->value;
            count++;
        }
    }
    return count;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;
    struct list_head *max = head->prev;
    int count = 1;
    while (max != head) {
        if (max->prev == head)
            break;
        element_t *cur = list_entry(max, element_t, list);
        element_t *pre = list_entry(max->prev, element_t, list);
        if (strcmp(cur->value, pre->value) > 0) {
            list_del(&pre->list);
            q_release_element(pre);
        } else {
            max = max->prev;
            count++;
        }
    }
    return count;
}

static inline void merge_two_list(struct list_head *head1,
                                  struct list_head *head2,
                                  bool descend)
{
    if (!head1 || !head2 || list_empty(head2))
        return;
    if (list_empty(head1)) {
        list_splice_init(head2, head1);
        return;
    }
    struct list_head *q1 = head1->next, *q2 = head2->next;
    for (; q1 != head1 && q2 != head2;) {
        element_t *e1 = list_entry(q1, element_t, list);
        element_t *e2 = list_entry(q2, element_t, list);
        if ((!descend && strcmp(e2->value, e1->value) < 0) ||
            (descend && strcmp(e2->value, e1->value) > 0)) {
            struct list_head *next_q2 = q2->next;
            list_del(q2);
            list_add(q2, q1->prev);
            q2 = next_q2;
        } else
            q1 = q1->next;
    }
    if (!list_empty(head2))
        list_splice_tail_init(head2, head1);
}


/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || list_empty(head) || list_is_singular(head))
        return q_size(list_entry(head->next, queue_contex_t, chain)->q);
    struct list_head finish;
    INIT_LIST_HEAD(&finish);
    while (!list_is_singular(head)) {
        struct list_head *cur = head->next;
        while (cur != head && cur->next != head) {
            queue_contex_t *qc1 = list_entry(cur, queue_contex_t, chain);
            queue_contex_t *qc2 = list_entry(cur->next, queue_contex_t, chain);
            merge_two_list(qc1->q, qc2->q, descend);
            list_del(&qc2->chain);
            list_add(&qc2->chain, &finish);
            cur = cur->next;
        }
    }
    list_splice_tail_init(&finish, head);
    return q_size(list_entry(head->next, queue_contex_t, chain)->q);
}
