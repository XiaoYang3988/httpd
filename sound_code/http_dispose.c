#include "http_dispose.h"
   
/*
**  http请求返回处理
**
*/
/* http请求中有未知的请求方法，返回 code:501 */
static void unimplemented(int client) {
    
    char buf[1024] = {};

    sprintf(buf, "HTTP/1.1 501 Method Not Implemented\r\n");
    send(client, buf, strlen(buf), 0);

    sprintf(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);

    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);

    //sprintf(buf, "Connection: close\r\n");
    //send(client, buf, sizeof(buf), 0);

    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);

    sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
    send(client, buf, strlen(buf), 0);

    sprintf(buf, "</TITLE></HEAD>\r\n");
    send(client, buf, strlen(buf), 0);

    sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
    send(client, buf, strlen(buf), 0);

    sprintf(buf, "</BODY></HTML>\r\n");
    send(client, buf, strlen(buf), 0);
}
/* http请求文件不存在，返回code:404 */
static void not_found(int client) {

    char buf[1024] = {};

    sprintf(buf, "HTTP/1.1 404 NOT FOUND\r\n");
    send(client, buf, strlen(buf), 0);

    sprintf(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);

    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);

    //sprintf(buf, "Connection: close\r\n");
    //send(client, buf, sizeof(buf), 0);

    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);

    sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
    send(client, buf, strlen(buf), 0);

    sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
    send(client, buf, strlen(buf), 0);

    sprintf(buf, "your request because the resource specified\r\n");
    send(client, buf, strlen(buf), 0);

    sprintf(buf, "is unavailable or nonexistent.\r\n");
    send(client, buf, strlen(buf), 0);

    sprintf(buf, "</BODY></HTML>\r\n");
    send(client, buf, strlen(buf), 0);
}
/* http请求错误，返回code:400 */
static void bad_request(int client)
{
    char buf[1024] = {};  

    sprintf(buf, "HTTP/1.1 400 BAD REQUEST\r\n");
    send(client, buf, sizeof(buf), 0);

    sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, sizeof(buf), 0);

    //sprintf(buf, "Connection: close\r\n");
    //send(client, buf, sizeof(buf), 0);

    sprintf(buf, "\r\n");
    send(client, buf, sizeof(buf), 0);

    sprintf(buf, "<P>Your browser sent a bad request, ");
    send(client, buf, sizeof(buf), 0);

    sprintf(buf, "such as a POST without a Content-Length.\r\n");
    send(client, buf, sizeof(buf), 0);
}
/* 读取文件并发送到客户端 */
void binary_system_cat(int client, FILE *resource, size_t length) {

    int8_t  *data = NULL;

    data = Calloc(length, sizeof(int8_t));
    if(data == NULL) {
        err_sys("binary_system_cat error");
        return;
    }

    fread(data, length, 1, resource);
    if(ferror(resource) ) {
        err_sys("binary_system_cat fread error");
    }

    send(client, data, length, 0);
    free(data);
}
/*
**  http连续字符安全处理
**
*/
/* 只能拷贝连续字符内存 */
static void *__http_str_memcpy(int8_t *destination, const int8_t *firstly, size_t destination_max_len, size_t copy_num) {

    if(copy_num > strlen(firstly)) {
        return NULL;
    }
        
    if(copy_num > destination_max_len - 1) {
        return NULL;
    }

    return memcpy(destination, firstly, copy_num);
}
/* 连续字符内存是否相同的比较, 忽略字母大小写 */
static bool __http_str_strncasecmp(const int8_t *str, const int8_t *str_1, size_t num) {

    /* 比较地址连续长度较小的字符内存的简易方法 */
    size_t start = 0;

    if(num > strlen(str) || num > strlen(str_1)) {
        err_alert("__http_str_strncasecmp Incorrect view and modification of memory");
        return false;
    }

    while(start < num) {

        int8_t c_str = *(str + start);
        int8_t c_str_1 = *(str_1 + start);

        /* 大写英文字母判断 */
        if(isalpha(c_str) != 0 && isalpha(c_str_1) != 0) {

            if(isupper(c_str) != 0) {
                c_str = c_str + 32;
            }            

            if(isupper(c_str_1) != 0) {
                c_str_1 = c_str_1 + 32;
            }  
        }

        if(c_str != c_str_1) {
            return false;
        }

        start += 1;
    }
    
    return true;
}

/*
**  http协议处理
**
*/
static int8_t *__get_http_head(int32_t client_fd) {

    int8_t  *http_head = NULL;
    size_t  head_len = (2<<6);
    size_t  num_len = 0;

    bool    judge_2 = false;
    bool    judge_1 = false;
    bool    judge = false;

    http_head = Calloc(head_len, sizeof(int8_t));

    while(1) {
        
        int8_t  __char = '\0';
        int32_t status = Recv(client_fd, &__char, 1, 0);

        if(num_len == HTTP_HEAD_MAX - 1) {
            free(http_head);
            http_head = NULL;
            break;       
        }

        if(num_len == head_len) {
            head_len = head_len * 2;
            http_head = Realloc(http_head, head_len);
        }

        if (status == 0) {
            free(http_head);
            http_head = NULL;
            break;
        }

        http_head[num_len] = __char;
        
        if(judge == false) {
            if(__char == '\r') {
                judge = true;
                num_len += 1;
                continue;
            }
        }

        if(judge_1 == false && judge == true) {
            if(__char == '\n') {
                judge_1 = true;
                num_len += 1;
                continue;
            }else {
                judge == false;
                num_len += 1;
                continue;
            }
        }

        if(judge_2 == false && judge_1 == true && judge == true) {
            if(__char == '\r') {
                judge_2 = true;
                num_len += 1;
                continue;
            }else {
                judge_1 = false;
                judge = false;
                num_len += 1;
                continue;
            }
        }

        if(judge_2 == true && judge_1 == true && judge == true) {
            if(__char == '\n') {
                http_head[num_len + 1] = '\0'; 
                break;
            }else {
                judge = false;
                judge_1 = false;
                judge_2 = false;
                num_len += 1;
                continue;
            }
        }

        num_len += 1;
    }

    return http_head;
}
static int8_t *__get_http_data(int32_t client_fd, size_t length) {

    if(length == 0) {
        return NULL;
    }

    int8_t  *http_data = Calloc(length + 1, sizeof(int8_t));
    int32_t status = Recv(client_fd, http_data, (length * sizeof(int8_t)), 0);

    if (status == 0) {
        free(http_data);
        http_data = NULL;
    }    

    return http_data;
}
static struct __http_request_line __get_http_request_line(int8_t *http_head) {

    struct __http_request_line request_line = {};
    int8_t  *blank_1 = NULL; 
    int8_t  *blank = NULL; 
    int8_t  *r = NULL;

    blank = strstr(http_head, " ");
    r = strstr(http_head, "\r\n");
    blank_1 = strstr(blank + 1, " ");

    /* 检查请求行格式是否正确 */
    if(blank == NULL || blank_1 == NULL || r == NULL) {
        err_alert("http request line Invalid format");
        return request_line;
    }    

    int8_t  *method_begin = http_head;
    int8_t  *method_end = blank;

    int8_t  *request_url_begin = blank + 1;
    int8_t  *request_url_end = blank_1;

    int8_t  *versions_begin = blank_1 + 1;
    int8_t  *versions_end = r;     


    if(abs(method_begin - method_end) + 1 < METHOD_MAX) {
        memcpy(&request_line.method[0], method_begin, abs(method_begin - method_end));
    }    

    if(abs(request_url_begin - request_url_end) + 1 < REQUEST_URL_MAX) {
        memcpy(&request_line.request_url[0], request_url_begin, abs(request_url_begin - request_url_end));
    }    

    if(abs(versions_begin - versions_end) + 1 < VERSIONS_MAX) {
        memcpy(&request_line.versions[0], versions_begin, abs(versions_begin - versions_end));
    }    

    return request_line;
}
/* 通用获取http请求头函数 */
static int8_t *__get_http_request_head(const int8_t *http_head ,const int8_t *request_head_name) {

    int8_t  *data = NULL;

    int8_t  *head = NULL;
    int8_t  *colon = NULL;
    int8_t  *end = NULL;

    size_t  data_length = 0;

    head = strstr(http_head, request_head_name);
    if(head == NULL) {
        return data;
    }
    end = strstr(head, HTTP_LINE_BREAK);
    if(end == NULL) {
        return data;
    }
    colon = strstr(http_head, HTTP_REQUEST_SEPARATOR) + 1;
    data_length = abs(colon - end);
    data = Calloc(data_length + 1, sizeof(int8_t));
    memcpy(data, colon, data_length);

    return data;
}
static size_t __get_http_request_accept_language(int8_t *http_head) {

    int8_t  *request_name_data = NULL;
    size_t  data_length = 0;

    request_name_data = __get_http_request_head(http_head, HTTP_DATA_LENGTH_NAME);

    if(request_name_data != NULL) {
        data_length = strtoul(request_name_data, NULL, 10);
    }

    free(request_name_data);
    return data_length;
}
/* 返回请求类型的枚举类型 */
static enum __request_type __get_request_type_enum(const int8_t *head) {

    {  
        /* 未优化的字符匹配，过多的函数调用 */
    }    

    if(__http_str_strncasecmp(head, "GET\0", strlen("GET\0")) == true) 
        return GET;

    if(__http_str_strncasecmp(head, "POST\0", strlen("POST\0")) == true) 
        return POST;    

    if(__http_str_strncasecmp(head, "HEAD\0", strlen("HEAD\0")) == true) 
        return HEAD;

    if(__http_str_strncasecmp(head, "PUT\0", strlen("PUT\0")) == true) 
        return PUT;    

    if(__http_str_strncasecmp(head, "DELETE\0", strlen("DELETE\0")) == true) 
        return DELETE;

    if(__http_str_strncasecmp(head, "CONNECT\0", strlen("CONNECT\0")) == true) 
        return CONNECT;    

    if(__http_str_strncasecmp(head, "OPTIONS\0", strlen("OPTIONS\0")) == true) 
        return OPTIONS;

    if(__http_str_strncasecmp(head, "TRACE\0", strlen("TRACE\0")) == true) 
        return TRACE;    

    if(__http_str_strncasecmp(head, "PATCH\0", strlen("PATCH\0")) == true) 
        return PATCH;
  
    return NO_REQUEST_TYPE;
}
/* 返回文件的类型 */
static enum __file_type __get_file_type_enum(const int8_t *file_path) {

    {  
        /* 未优化的字符匹配，过多的函数调用 */
    }   

    if (strstr(file_path, ".html") != NULL) {
        return HTML;
    }

    if (strstr(file_path, ".gif") != NULL) {
        return GIF;
    }

    if (strstr(file_path, ".png") != NULL) {
        return PNG;
    }

    if (strstr(file_path, ".jpg") != NULL) {
        return JPG;
    }

    if (strstr(file_path, ".mp3") != NULL) {
        return MP3;
    }

    if (strstr(file_path, ".mp4") != NULL) {
        return MP4;
    }   

    if (strstr(file_path, ".ico") != NULL) {
        return ICO;
    }   

    if (strstr(file_path, ".txt") != NULL) {
        return TEXT;
    } 

    return TEXT;
}  
/* 获取url中的查询符字符 */
static int8_t *__get_inquire_char(int8_t *url) {

    if(url == NULL) {
        return NULL;
    }

    int8_t  *inquire_url = NULL;

    int8_t  *bgein = NULL;
    int8_t  *end = NULL;

    if (strstr(url, HTTP_INQUIRE_SEPARATOR) != NULL) {
        bgein = strstr(url, HTTP_INQUIRE_SEPARATOR) + 1;
    } else {
        return NULL;
    }

    end = bgein + strlen(bgein);

    inquire_url = Calloc(abs(bgein - end) + 1, sizeof(int8_t));
    memcpy(inquire_url, bgein, abs(bgein - end));

    return inquire_url;
}
/* 获取文件绝对路径 */
static file_path __get_file_absolute_path(int8_t *url) {

    file_path   path = {};

    if(url == NULL) {
        return path;
    }

    path = get_path(CURRENT_FOLDER_NAME, HTTPD_RELATIVE_PATH);

    int8_t  *file_path_begin = url;
    int8_t  *file_path_end = NULL;

    if(strstr(url, HTTP_INQUIRE_SEPARATOR) != NULL) {
        file_path_end = strstr(url, HTTP_INQUIRE_SEPARATOR);
    }else {
        file_path_end = file_path_begin + strlen(file_path_begin);
    }
    
    if(__http_str_strncasecmp(url, "/", strlen("/")) == true && strlen(url) == 1) {
        if(strlen("/index.html") + strlen(&path.data[0]) + 1 > LINUX_PATH_MAX_ADD_FILE_NAME_MAX) {
            err_alert("/index.html error");
            file_path error_path = {};
            return error_path;            
        } 
        strcpy(path.data + strlen(&path.data[0]), "/index.html");
    }else {     
        if(__http_str_memcpy(path.data + strlen(&path.data[0]), file_path_begin, LINUX_PATH_MAX_ADD_FILE_NAME_MAX - strlen(&path.data[0]), abs(file_path_end - file_path_begin)) == NULL) {
            err_alert("url:%s error", url);
            file_path error_path = {};
            return error_path;             
        }
    }

    return path;
}
/* cgi程序处理 */
static void __execute_cgi(int client_fd, const char *file_name, const char *query_string) {

    if(query_string == NULL) {
        err_alert("__execute_cgi: query_string is zero");
        bad_request(client_fd);
        return;
    }

    int8_t  buf[1024] = {};
    int8_t  *argv[] = { query_string, NULL };
    struct stat statbuf = {}; 
    int8_t  *envp[] = { NULL };

    /* 检查文件是否存在 */
    if(stat(file_name, &statbuf) != 0) {
        err_alert("__execute_cgi statbuf: %s ", file_name);
        not_found(client_fd);
        return;  
    }  

    /* 检查是不是路径文件 */
    if (S_ISDIR(statbuf.st_mode)) {
        err_alert("__execute_cgi statbuf.st_mode: %s ", file_name);
        not_found(client_fd);
        return;          
    }

    strcpy(buf, "HTTP/1.1 200 OK\r\n");
    send(client_fd, buf, strlen(buf), 0);

    strcpy(buf, SERVER_STRING);
    send(client_fd, buf, strlen(buf), 0);    

    /* 子进程所运行的函数块 */ 
    /* 没有遵守cgi协议规范 */
    if (Fork() == 0) { 
        /* 设置cgi环境变量 */
        //setenv("QUERY_STRING", query_string, 1); 
        /* 将标准输出重定向到客户端 */
        Dup2(client_fd, STDOUT_FILENO);         
        /* 运行CGI程序 */
        Execve(file_name, argv, envp); 
    }
    /* 等待子进程中断或结束 */
    Wait(NULL); 
}
/* 普通文件处理 */
static void __serve_file(int client_fd, const char *file_name) {

    enum __file_type file_type = TEXT;
    struct stat statbuf = {}; 
    FILE *file = NULL;

    /* 检查文件是否存在 */
    if(stat(file_name, &statbuf) != 0) {
        err_alert("__serve_file statbuf: %s ", file_name);
        not_found(client_fd);
        return;  
    }  

    /* 检查是不是路径文件 */
    if (S_ISDIR(statbuf.st_mode)) {
        err_alert("__serve_file statbuf.st_mode: %s ", file_name);
        not_found(client_fd);
        return;          
    }

    file = fopen(file_name, "r");
    if(file == NULL) {
        err_alert("__serve_file fopen: %s ", file_name);
        not_found(client_fd);
        return;          
    }

    file_type = __get_file_type_enum(file_name);

    switch (file_type) {
        case HTML:
            {
                char buf[1024] = {};

                strcpy(buf, "HTTP/1.1 200 OK\r\n");
                send(client_fd, buf, strlen(buf), 0);

                strcpy(buf, SERVER_STRING);
                send(client_fd, buf, strlen(buf), 0);

                sprintf(buf, "content-type: text/html;charset=utf-8\r\n");
                send(client_fd, buf, strlen(buf), 0);

                sprintf(buf, "content-length: %d\r\n", statbuf.st_size);
                send(client_fd, buf, strlen(buf), 0);

                strcpy(buf, "\r\n");
                send(client_fd, buf, strlen(buf), 0);         

                binary_system_cat(client_fd, file, statbuf.st_size);    
            }
            break;
        case GIF:
            {
                char buf[1024] = {};

                strcpy(buf, "HTTP/1.1 200 OK\r\n");
                send(client_fd, buf, strlen(buf), 0);

                strcpy(buf, SERVER_STRING);
                send(client_fd, buf, strlen(buf), 0);

                sprintf(buf, "content-length: %d\r\n", statbuf.st_size);
                send(client_fd, buf, strlen(buf), 0);

                sprintf(buf, "content-type: image/gif\r\n");
                send(client_fd, buf, strlen(buf), 0);

                strcpy(buf, "\r\n");
                send(client_fd, buf, strlen(buf), 0);      

                binary_system_cat(client_fd, file, statbuf.st_size);    
            }
            break;  
        case JPG:
            {
                char buf[1024] = {};

                strcpy(buf, "HTTP/1.1 200 OK\r\n");
                send(client_fd, buf, strlen(buf), 0);

                strcpy(buf, SERVER_STRING);
                send(client_fd, buf, strlen(buf), 0);

                sprintf(buf, "accept-ranges: bytes\r\n");
                send(client_fd, buf, strlen(buf), 0);

                sprintf(buf, "content-length: %d\r\n", statbuf.st_size);
                send(client_fd, buf, strlen(buf), 0);

                sprintf(buf, "content-type: image/jpeg\r\n");
                send(client_fd, buf, strlen(buf), 0);

                strcpy(buf, "\r\n");
                send(client_fd, buf, strlen(buf), 0); 

                binary_system_cat(client_fd, file, statbuf.st_size);        
            }
            break;
        case PNG:
            {
                char buf[1024] = {};

                strcpy(buf, "HTTP/1.1 200 OK\r\n");
                send(client_fd, buf, strlen(buf), 0);

                strcpy(buf, SERVER_STRING);
                send(client_fd, buf, strlen(buf), 0);

                sprintf(buf, "accept-ranges: bytes\r\n");
                send(client_fd, buf, strlen(buf), 0);

                sprintf(buf, "content-length: %d\r\n", statbuf.st_size);
                send(client_fd, buf, strlen(buf), 0);

                sprintf(buf, "content-type: image/png\r\n");
                send(client_fd, buf, strlen(buf), 0);

                strcpy(buf, "\r\n");
                send(client_fd, buf, strlen(buf), 0); 

                binary_system_cat(client_fd, file, statbuf.st_size);             
            }
            break; 
        case ICO:
            {
                char buf[1024] = {};

                strcpy(buf, "HTTP/1.1 200 OK\r\n");
                send(client_fd, buf, strlen(buf), 0);

                strcpy(buf, SERVER_STRING);
                send(client_fd, buf, strlen(buf), 0);

                sprintf(buf, "content-length: %d\r\n", statbuf.st_size);
                send(client_fd, buf, strlen(buf), 0);

                sprintf(buf, "content-type: image/x-icon\r\n");
                send(client_fd, buf, strlen(buf), 0);

                strcpy(buf, "\r\n");
                send(client_fd, buf, strlen(buf), 0); 

                binary_system_cat(client_fd, file, statbuf.st_size);           
            }
            break;
        case MP4:
            /* code */
            break;
        case MP3:
            /* code */
            break; 
        case TEXT:
            {
                char buf[1024] = {};

                strcpy(buf, "HTTP/1.1 200 OK\r\n");
                send(client_fd, buf, strlen(buf), 0);

                strcpy(buf, SERVER_STRING);
                send(client_fd, buf, strlen(buf), 0);

                sprintf(buf, "content-type: text/plain\r\n");
                send(client_fd, buf, strlen(buf), 0);

                sprintf(buf, "content-length: %d\r\n", statbuf.st_size);
                send(client_fd, buf, strlen(buf), 0);

                strcpy(buf, "\r\n");
                send(client_fd, buf, strlen(buf), 0);    

                binary_system_cat(client_fd, file, statbuf.st_size);      
            }
            break;
    }

    fclose(file);
}

/*
** 函数作用: 处理已连接的http请求
** 参数：
**      client_fd：已连接的套接口描述符
** 返回值：
**      无
*/
void http_accept_dispose(int32_t client_fd) {
    
    /* 协议头 */
    int8_t  *http_head = NULL;
    /* 协议头中的请求行 */
    struct __http_request_line request_line = {};
    /* 文件的绝对路径 */
    file_path  file_path = {};
    /* 请求类型 */
    enum __request_type request_type = NO_REQUEST_TYPE;

    http_head = __get_http_head(client_fd);
    request_line = __get_http_request_line(http_head);
    file_path = __get_file_absolute_path(&request_line.request_url[0]);

    /* 获取请求类型 */
    request_type = __get_request_type_enum(&request_line.method[0]);
    
    /* 不支持保持tcp的链接状态 */
    switch (request_type) {
        case GET:
            {   
                
                int8_t  *get_inquire_char = __get_inquire_char(&request_line.request_url[0]);

                if(get_inquire_char != NULL) {
                    __execute_cgi(client_fd, &file_path.data[0], get_inquire_char);
                }else {
                    __serve_file(client_fd, &file_path.data[0]);
                }
                
                free(get_inquire_char);
                Close(client_fd);  
            }
            break;
        case POST:
            {   
                /* http数据段长度 */
                size_t  http_data_length = __get_http_request_accept_language(http_head);
                /* http数据段 */
                int8_t  *http_data = NULL;
                if(http_data_length == 0) {
                    err_alert("post data is zero");
                    bad_request(client_fd);
                    Close(client_fd);
                    break;
                }
                http_data = __get_http_data(client_fd, http_data_length);
                __execute_cgi(client_fd, &file_path.data[0], http_data);

                free(http_data);
                Close(client_fd);                      
            }
            break;            
        case HEAD:
            err_alert("%s request type Has not been implemented", &request_line.method[0]);
            unimplemented(client_fd);
            Close(client_fd);   
            break;
        case PUT:
            err_alert("%s request type Has not been implemented", &request_line.method[0]);
            unimplemented(client_fd);
            Close(client_fd);   
            break;       
        case DELETE:
            err_alert("%s request type Has not been implemented", &request_line.method[0]);
            unimplemented(client_fd);
            Close(client_fd);   
            break;
        case CONNECT:
            err_alert("%s request type Has not been implemented", &request_line.method[0]);
            unimplemented(client_fd);
            Close(client_fd);   
            break;       
        case OPTIONS:
            err_alert("%s request type Has not been implemented", &request_line.method[0]);
            unimplemented(client_fd);
            Close(client_fd);   
            break;
        case TRACE:
            err_alert("%s request type Has not been implemented", &request_line.method[0]);
            unimplemented(client_fd);
            Close(client_fd);   
            break;       
        case PATCH:
            err_alert("%s request type Has not been implemented", &request_line.method[0]);
            unimplemented(client_fd);
            Close(client_fd);   
            break;
        case NO_REQUEST_TYPE:
            err_alert("%s no request type", &request_line.method[0]);
            bad_request(client_fd);
            Close(client_fd);   
            break;       
    /* switch end */
    }    

    free(http_head);
    return;
}