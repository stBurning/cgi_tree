#ifndef INC_2019_03_20_2_TREE_H
#define INC_2019_03_20_2_TREE_H

#include "../list/list.h"
#include <string>


struct student{

    char* surname = new char[40];
    char* name = new char[40];
    char* group = new char[10];
    float mark;

};
/**Функции сравнения для сортировки дерева**/


int compare_str(student* const x, student* const y){
    int surname_equal = strcmp(x->surname, y->surname);
    if(surname_equal){
        return surname_equal;
    } else{
        if(strcmp(x->name, y->name)){
            return strcmp(x->name, y->name);
        } else{
            if(x->mark > y->mark) return 1;
            else if(x->mark == y->mark) return 0;
            else return -1;
        }

    }
}
int compare_int(student* const x, student* const y){
    if(x->mark > y->mark) return 1;
    else if(x->mark == y->mark) return  compare_str(x, y);
    else return -1;
}

using namespace std;



struct node{
    student* val;
    int height = 1;
    node* left = nullptr;
    node* right = nullptr;
};

enum compare_type{
    SURNAME,
    VALUE
};

void add(node*& root, student *x);
void drop(node*& tree);
int get_height(node* elem);
void balance(node*& tree);
void small_left_rotate(node*& root);
void big_left_rotate(node*& tree);
void small_right_rotate(node*& root);
void big_right_rotate(node*& tree);


void add(node*& root, node* elem, compare_type ctype){
    if (!elem) return;
    if (!root) root = elem;
    else {
        if (ctype == SURNAME) {
            if (compare_str(root->val, elem->val) == 1) {
                add(root->left, elem, SURNAME);
            } else {
                if (compare_str(root->val, elem->val) == 0) {
                    //Полностью равны
                } else {
                    add(root->right, elem, SURNAME);
                }
            }
            balance(root);
            root->height = get_height(root);
        }
        if (ctype == VALUE) {
            if (compare_int(root->val, elem->val) == 1) {
                add(root->left, elem, VALUE);
            } else {
                if (compare_int(root->val, elem->val) == 0) {
                    //Полностью раны
                } else {
                    add(root->right, elem, VALUE);
                }
            }
            balance(root);
            root->height = get_height(root);
        }
    }
}
void add(node*& root, student* x, compare_type ctype){
    node* new_el = new node;
    new_el->val = x;
    add(root, new_el, ctype);
}

void drop(node*& tree){
    if (tree){
        drop(tree->left);
        drop(tree->right);
        delete tree;
        tree = nullptr;
    }
}

node* get_parent(node* tree, node* elem){
    if (!tree || !elem || tree==elem) return nullptr;
    if (
            tree->left == elem ||
            tree->right == elem
            ) return tree;
    if (compare_int(elem->val, tree->val))//TODO Добавить сортировку по фамилии и имени
        return get_parent(tree->left, elem);
    else return get_parent(tree->right, elem);
}


int get_height(node* elem){
    if (!elem) return 0;
    int lh = (elem->left) ? elem->left->height : 0;
    int rh = (elem->right) ? elem->right->height : 0;
    return 1 + ((lh>=rh) ? lh : rh);
}

void balance(node*& tree){
    if (!tree) return;
    int lh = get_height(tree->left);
    int rh = get_height(tree->right);
    if (abs(lh-rh)>=2){
        if (rh>lh){//Левый поворот
            if (
                    get_height(tree->right->left)<
                    get_height(tree->right->right)
                    ){
                //малый поворот
                small_left_rotate(tree);
            } else {
                //большой поворот
                big_left_rotate(tree);
            }
        } else { //Правый поворот
            if (
                    get_height(tree->left->right)<
                    get_height(tree->left->left)
                    ){
                //малый поворот
                small_right_rotate(tree);
            } else {
                //большой поворот
                big_right_rotate(tree);
            }
        }
    }
}

void small_left_rotate(node*& root){
    if (!root) return;
    node* r = root->right;
    root->right = r->left;
    r->left = root;
    root = r;
    if (root->left) root->left->height = get_height(root->left);
}

void big_left_rotate(node*& tree){
    small_right_rotate(tree->right);
    small_left_rotate(tree);
}

void small_right_rotate(node*& root){
    if (!root) return;
    node* r = root->left;
    root->left = r->right;
    r->right = root;
    root = r;
    if (root->right) root->right->height = get_height(root->right);
}

void big_right_rotate(node*& tree){
    small_left_rotate(tree->left);
    small_right_rotate(tree);
}
#endif //INC_2019_03_20_2_TREE_H
