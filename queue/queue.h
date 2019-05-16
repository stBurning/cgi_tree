#ifndef CGI_TREE_QUEUE_H
#define CGI_TREE_QUEUE_H
#include "../tree/tree.h"
//Очередь для обхода дерева в ширину
struct elem{
    elem* next = nullptr;
    node* uzel;
};
elem* add(elem*& q, elem* qlast, node* uzel){
    elem* new_el = new elem;
    new_el->uzel = uzel;
    if (!q) {
        q = new_el;
        return q;
    } else {
        if (qlast) qlast->next = new_el;
        return new_el;
    }
}
void create_queue(elem*& q, node* tree){
    if (!tree) return;
    add(q, nullptr, tree);
    int cnt = (1 << tree->height) - 1;
    elem* curr = q;
    elem* last = q;
    int curr_cnt = 1;
    while (curr_cnt<cnt){
        last = add(
                q,
                last,
                (curr->uzel)?curr->uzel->left:nullptr
        );
        last = add(
                q,
                last,
                (curr->uzel)?curr->uzel->right:nullptr
        );
        curr = curr->next;
        curr_cnt+=2;
    }
}
void delete_queue(elem*& q){
    auto* c = q;
    while (c){
        elem* old = c;
        c = c->next;
        delete old;
    }
    q = nullptr;
}

#endif //CGI_TREE_QUEUE_H
