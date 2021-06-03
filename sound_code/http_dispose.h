#ifndef __http_dispose
#define __http_dispose

#include    "wrap_unix.h"
#include    "conf_file.h"
#include    "http_type.h"
#include    "socket.h"
#include    "log.h"

#include    <stdbool.h>
#include    <ctype.h>
#include    <stdlib.h>

#define SERVER_STRING   "Server: httpd/0.1.0\r\n"
#define HTTPD_RELATIVE_PATH "httpd"
#define HTTP_HEAD_MAX   (2<<12)
#define HTTP_DATA_LENGTH_NAME   "accept_language:"
#define HTTP_LINE_BREAK "\r\n"
#define HTTP_REQUEST_SEPARATOR  ":"
#define HTTP_INQUIRE_SEPARATOR  "?"

/*
** 请求行长度
*/
/* 版本字符串最大字符长度 */
#define VERSIONS_MAX 16
/* 请求url最大字符长度 */
#define REQUEST_URL_MAX 128
/* 请求最大字符长度 */
#define METHOD_MAX 32

/* 文件类型 */
enum __file_type {
    HTML = 1, GIF, JPG, PNG, MP4, TEXT, MP3, ICO,
};
/* 请求类型 */
enum __request_type {
    GET = 1, POST, HEAD, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH, NO_REQUEST_TYPE,
};

/* http请求行 */
struct __http_request_line {
    int8_t  method[METHOD_MAX];
    int8_t  request_url[REQUEST_URL_MAX];
    int8_t  versions[VERSIONS_MAX];
};



/*
** 函数作用: 处理已连接的http请求
** 参数：
**      client_fd：已连接的套接口描述符
** 返回值：
**      无
*/
void http_accept_dispose(int32_t client_fd);
#endif