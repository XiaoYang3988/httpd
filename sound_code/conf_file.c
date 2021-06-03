#include "conf_file.h"

static struct __conf_str __conf_file_read(const int8_t *path) {

    struct __conf_str   file = {};
    struct stat statbuf = {};  
    FILE    *fp = NULL;

    //err_msg("__conf_file_read path: %s", path);
    if(stat(path, &statbuf) != 0) {
        err_sys("%s file does not exist", path);
    }

    if(statbuf.st_size > CONF_FILE_MAX_SIZE) {
        err_sys("file data greater than CONF_FILE_MAX_SIZE", path);
    }

    fp = fopen(path, "r");
    fread(&file.data[0], CONF_FILE_MAX_SIZE, 1, fp);
    if(ferror(fp) ) {
        err_sys("fread error");
    }

    fclose(fp);
    return file;
}
static struct __conf_data_line __get_conf_str_name_line(const struct __conf_str *conf, const int8_t *name) {

    struct __conf_data_line line_data = {};
    int8_t  *head = NULL;
    int8_t  *middle = NULL;
    int8_t  *trail = NULL;
    int32_t  str_len = 0;
    
    head = strstr(&conf->data[0], name);
    middle = strstr(head, "=");
    trail = strstr(head, "\n");
    if(head == NULL && middle == NULL && trail == NULL) {
        err_sys("file data format error");
    }

    str_len = abs(trail - middle) + 1;
    if(str_len > CONF_FILE_LINE_MAX) {
        err_sys("file data each line byts greater than CONF_FILE_LINE_MAX");
    }

    memcpy(&line_data.data[0], middle + 1, str_len - 1);
    return line_data;
}

/*
** 函数作用: 读取blogs.conf配置文件的文本数据
** 参数：
**      无
** 返回值：
**      conf_file: struct __conf_file
*/
file_path get_path(const int8_t *name, const int8_t *relative_path) {

    int8_t  func_path[] = __FILE__;
    file_path   path = {};
    int8_t  *head = NULL;
    size_t  copy_len = 0;

    if(name == NULL || relative_path == NULL)
        return path;

    //err_msg("get_path __FILE__: %s", __FILE__);
    head = strstr(&func_path[0], name);
    copy_len = strlen(__FILE__) - strlen(head);
    
    if(head == NULL) {
        err_quit("macro CURRENT_FOLDER_NAME error");
    }

    if(copy_len + strlen(CONF_DIR_AND_FILE_NAME) + 1 > LINUX_PATH_MAX_ADD_FILE_NAME_MAX) {
        err_quit("macro LINUX_PATH_MAX_ADD_FILE_NAME_MAX error");
    }

    memcpy(&path.data[0], &func_path[0], copy_len);
    memcpy(&path.data[0] + copy_len, relative_path, strlen(relative_path));
    
    return path;
}

/*
** 函数作用: 读取blogs.conf配置文件的文本数据
** 参数：
**      无
** 返回值：
**      conf_file: struct __conf_file
*/
conf_file return_conf_file() {
    
    struct __conf_str   conf_str = {};
    conf_file   conf = {};
    file_path   conf_path = {};  
    int64_t     num = 0;

    /* 获取配置文件路径 */
    conf_path = get_path(CURRENT_FOLDER_NAME, CONF_DIR_AND_FILE_NAME);
    /* 读取配置文件内的文本数据 */
    conf_str = __conf_file_read(&conf_path.data[0]);

    /* 将字符数据内所有的换行符替换成空格符 */
    for (size_t i = 0; i < strlen(&conf_str.data[0]); i++) {
        if(conf_str.data[i] == '\r')
            conf_str.data[i] = ' ';
    }
    
    /* 解析字符数据转换为 conf_file 配置文件结构体 */
    {
        /* 获取端口号 */
        struct __conf_data_line line_data = __get_conf_str_name_line(&conf_str, PORT);
        int64_t num = atoi(&line_data.data[0]);

        if(num > UINT16_MAX) {
            conf.port = 0;
        }else {
            conf.port = num;
        }    
    }

    return conf;
}