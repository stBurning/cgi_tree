#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <windows.h>
#include "list/list.h"
#include "cgi/cgi.h"
#include "tree/tree.h"


using namespace std;
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
//

struct group{
    char* group_id;
    list<student*>* lst = new list<student*>;
};

void show_form(){
    cout<<"<form action='tree.cgi' method='post'>\n"
          "<select name=\"sort\">\n"
          "  <option>По фамилии</option>\n"
          "  <option>По среднему баллу</option>\n"
          "</select>"
          "<input name=\"btn\" class=\"submitbtn\" type=\"submit\" title=\"Отправить\">\n"
          "</form>\n"<<endl;
}
void add_to_groups(list<group>* groups, student* curr_stud){
    /**Добавление студента в список его группы
     * - если такой группы нет, создается новая**/
    for(int i = 0; i < groups->get_size(); i++){
        group g = groups->get(i);
        if(!strcmp(curr_stud->group, g.group_id)){
            g.lst->append(curr_stud);
            return;
        }
    }//Если ни одна группа не подошла, создаем новую и добавляем в список групп
    group new_group;
    new_group.group_id = curr_stud->group;
    new_group.lst->append(curr_stud);
    groups->append(new_group);

}
void read_file(list<group>*& groups){
    /**Считываем данные о студентах из файла
     * - добавляем студентов в списки их групп**/
    ifstream file("data/data");
    if(file.is_open()){
        while (!file.eof()){
            student* curr_stud = new student;
            file >> curr_stud->surname;
            file >> curr_stud->name;
            file >> curr_stud->group;
            file >> curr_stud->mark;
            add_to_groups(groups, curr_stud);
        }
    } else{
        throw "Не удалось считать файл";
    }
}
void build_trees(list<node*>*& trees, list<group>* groups, compare_type ctype){
    /**По спискам групп строим деревья**/
    //TODO Добавить возможность выбора параметра сортировки
    for(int i = 0; i < groups->get_size();i++){
        group g = groups->get(i);
        node* tree = nullptr;
        for(int j = 0; j < g.lst->get_size(); j++){
            add(tree, g.lst->get(j), ctype);
        }
        trees->append(tree);
    }

}
void show_tree(node* tree){
    if (!tree) return;
    int h = tree->height;
    int col = (1 << h) - 1;
    elem* q = nullptr;
    create_queue(q, tree);
    elem* qfirst = q;
    int lvl = 0;
    while (q){
        lvl++;
        int cells = 0;
        int empt_c = (1 << (h-lvl))-1;
        cout << "<tr>\n";
        while (cells<col) {
            if (cells) {
                cout << "<td>&nbsp;</td>\n";
                cells++;
            }
            for (int j = 0; j < empt_c; j++) {
                cells++;
                cout << "<td>&nbsp;</td>\n";
            }
            cells++;
            cout << "<td style='border:1px solid black;'>";
            if (q->uzel){
                    student* stud = q->uzel->val;
                    cout<<stud->surname<<" "<<stud->name<<" "<<stud->mark<<"\n"<<endl;
            }
            else cout << "-";
            cout << "</td>";
            q = q->next;
            for (int j = 0; j < empt_c; j++) {
                cells++;
                cout << "<td>&nbsp;</td>\n";
            }
        }
        cout << "</tr>\n";
    }
    delete_queue(qfirst);
}

void show_content(){
    char* data;
    get_form_data(data);//Получаем данные формы
    char* sort;
    get_user_value(sort, data, "sort");//Получаем критерий сортировки
    if(data){
        list<group>* groups;
        if(!strcmp(sort,"По фамилии")){
            list<group>* groups = new list<group>;
            list<node*>* trees = new list<node*>;
            read_file(groups);
            build_trees(trees,groups, SURNAME);
            for(int i = 0; i < trees->get_size(); i++){
                node* tree = trees->get(i);
                cout<<"Группа:"<<tree->val->group<<endl;
                cout << "<table class = 'content' style='border: 1px solid black;'>";
                show_tree(tree);
                cout << "</table>";
                cout<<""<<endl;
            }
        }else if(!strcmp(sort,"По среднему баллу")){
            list<group>* groups = new list<group>;
            list<node*>* trees = new list<node*>;
            read_file(groups);
            build_trees(trees,groups, VALUE);
            for(int i = 0; i < trees->get_size(); i++){
                node* tree = trees->get(i);
                cout<<"Группа:"<<tree->val->group<<endl;
                cout << "<table class = 'content' style='border: 1px solid black;'>";
                show_tree(tree);
                cout << "</table>";
                cout<<""<<endl;
            }
        } else{
            //TODO Add exception
        }
        groups->clear();
        delete data;
    }
    delete sort;
}


int main() {

    cout << "Content-type: text/html; charset=utf-8\n\n";
    ifstream f("tree.html");
    if (f.is_open()){
        char *buf = new char[65536];
        while (!f.eof()) {
            f.getline(buf, 65536);
            if (strcmp(buf, "<!--main_content-->") == 0) {
                show_form();
                if(!is_get()){
                    show_content();
                }
            } else cout << buf << endl;
        }
        delete[] buf;
        f.close();
    }

    return 0;
}