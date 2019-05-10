#ifndef TREE_TEMPL_LIST_H
#define TREE_TEMPL_LIST_H

#include <iostream>
using namespace std;
/**List - односвязный список
 *  __ Начало списка задается next = nullptr - указателем __**/
template <class T>struct list{


private:T value;//Значение
private:list* next;//Указатель на след. элемент
private:unsigned int size = 0;

public:
    list(){//Конструктор пустого списка
        value;
        next = nullptr;
    }

    void append(T add_item){//Функция добавления элемента в конец списка
        auto * new_element = new list();
        new_element->value = add_item;
        if(this->next){
            list<T>* curr = this;
            while (curr->next){
                curr = curr->next;
            }
            curr->next = new_element;
        } else{
            this->next = new_element;
        }
        size++;
    }

    T get(int pos){
        if(pos < size){
            list<T>* curr = this;
            for(int i = 0; i <= pos; i++) curr = curr->next;
            return curr->value;
        } else throw "Index out of range";
    }

    void removeAll(T item){
        //Удаление из списка всех элементов по значению
        list<T>*curr = this->next;
        list<T>*prev = this;
        while (curr){
            if(curr->value == item){
                prev->next = curr->next;
                curr = curr->next;
                size--;
            } else{
                prev = curr;
                curr = curr->next;
            }
        }

    }
    void print_trace(){

        if(this->next){
            cout<<this->next->value<<" ";
            list<T>* curr = this->next;
            while(curr->next){
                cout<<curr->next->value<<" ";
                curr = curr->next;
            }
            cout<<'\n';
        } else{
            cout<<"list is empty"<<endl;
        }
    }
    int get_size(){
        return size;
    }
    void clear(){//Очистка списка
        if(this->next){
            list<T>* curr = this->next;
            while(curr->next){
                list<T>* prev = curr;
                curr = curr->next;
                delete prev;
            }
            this->next = nullptr;
            size = 0;
        }
    }
};


#endif //TREE_TEMPL_LIST_H
