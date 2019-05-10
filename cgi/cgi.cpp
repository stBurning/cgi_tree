#include "cgi.h"
#include <stdlib.h>
#include <string.h>
#include <cstdio>

bool is_get(){
    size_t realsize;
    char*value = new char[5];
    getenv_s(&realsize,value,5,"REQUEST_METHOD");
    bool res=!strcmp(value,"GET");
    delete []value;
    return res;
}

bool is_post(){
    size_t realsize;
    char*value = new char[5];
    getenv_s(&realsize,value,5,"REQUEST_METHOD");
    bool res=!strcmp(value,"POST");
    delete []value;
    return res;
}
size_t get_content_length(){

    char* value = new char[20];
    size_t realsize;
    getenv_s(&realsize, value, 20, "CONTENT_LENGTH");
    size_t size=0;
    if(realsize){
        sscanf_s(value,"%d",&size);
    }
    delete []value;
    return size;
}

void get_form_data(char*&data){

    if(is_get()){
        size_t realsize;
        char*value=new char[10240];
        getenv_s(&realsize,value,10240,"QUERY_STRING");
        data=new char[realsize+1];
        strcpy_s(data,realsize+1,value);
    }else {
        size_t buf_size=get_content_length()+1;
        data=new char[buf_size];
        fread(data,sizeof(char),buf_size,stdin);
        data[buf_size-1]=0;
    }
}
void get_user_value(char*& out_key, const char* data, const char* key_name){

    out_key = nullptr;
    if(data == nullptr || strlen(data) == 0) return;
    char* d = strdup(data);
    char* tmp = d;
    char* cnt;
    while(char* part = strtok_s(tmp,"&",&cnt)){
        tmp = nullptr;
        char* val;
        char* key = strtok_s(part,"=",&val);
        if(strcmp(key, key_name) == 0){
            out_key = strdup(val);
            str_decode(out_key, out_key);
            delete[]d;
            return;
        }
    }
}

void str_decode(char*& dec_str,const char* erc_str){

    char* res = new char[strlen(erc_str)+1];
    int i = 0, j = 0;
    while(erc_str[i]){
        if(erc_str[i] == '+') res[j]=' ';
        else if(erc_str[i] == '%'){
            char ch[3] = {erc_str[i+1],erc_str[i+2],0};
            int c;
            sscanf_s(ch,"%X",&c);
            res[j]=c;
            i+=2;
        }else {
            res[j] = erc_str[i];
        }
        j++;
        i++;
    }
    res[j]=0;
    dec_str = new char[strlen(res)+1];
    strcpy(dec_str,res);
    delete[]res;
}