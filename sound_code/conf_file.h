#ifndef __file_io
#define __file_io

#include    "log.h"

#include    <stdlib.h>
#include    <string.h>
#include	<inttypes.h>
#include    <sys/stat.h>  

/* 配置文件大小的最大值 */
#define CONF_FILE_MAX_SIZE 4096
/* windos最大文件名大小加路径名大小 */
#define WIN_PATH_MAX_ADD_FILE_NAME_MAX 260
/* linux最大文件名大小加路径名大小 */
#define LINUX_PATH_MAX_ADD_FILE_NAME_MAX 4351
/* 配置文件名称 */
#define CONF_DIR_AND_FILE_NAME "conf/httpd.conf"
/* 当前文夹名称 */
#define CURRENT_FOLDER_NAME "sound_code"
/* 配置文件每行字符最大数量 */
#define CONF_FILE_LINE_MAX 128
/* 端口配置名称 */
#define PORT "port"
/* 分割符号 */
#define SOLIT_STR "/n"

/* 配置文件属性别名 */
typedef struct __conf_file conf_file;
/* 文件路径别名 */
typedef struct __file_path file_path;

/* 配置文件属性 */
struct __conf_file {
    uint16_t port;
};

/* 配置文件字符内容 */
struct __conf_str {
    int8_t data[CONF_FILE_MAX_SIZE];
};

/* 文件路径加文件名 */
struct __file_path {
    int8_t data[LINUX_PATH_MAX_ADD_FILE_NAME_MAX];
};

/* 配置文件每行字符数组 */
struct __conf_data_line {
    int8_t data[CONF_FILE_LINE_MAX];
};

/*
** 函数作用: 读取blogs.conf配置文件的文本数据
** 参数：
**      无
** 返回值：
**      conf_file: struct __conf_file
*/
conf_file return_conf_file();
/*
** 函数作用: 读取blogs.conf配置文件的文本数据
** 参数：
**      无
** 返回值：
**      conf_file: struct __conf_file
*/
file_path get_path();

#endif