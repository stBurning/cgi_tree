#ifndef CGI_CGI_H
#define CGI_CGI_H

#ifdef __cplusplus
extern "C"{
#endif

#include <sec_api/stdlib_s.h>
errno_t getenv_s(
        size_t *ret_required_buf_size,
        char *buf,
        size_t buf_size_in_bytes,
        const char *name
);

#ifdef __cplusplus
}
#endif

bool is_get();
bool is_post();
void get_form_data(char*&data);
void get_user_value(char*& out_value,const char* data,const char*key_name);
size_t get_content_length();
void str_decode(char*&dec_str,const char*erc_str);
#endif //CGI_CGI_H
