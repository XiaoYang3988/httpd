#ifndef __log
#define __log

#include    "unpv_config.h"

#include	<stdarg.h>		/* ANSI C header file */
#include	<syslog.h>		/* for syslog() */
#include    <stdio.h>
#include    <error.h>
#include    <errno.h>
#include    <unistd.h>
#include    <stdlib.h>
#include    <string.h>
#include	<inttypes.h>

#define OPENLOG_LOGOPT    LOG_CONS
#define OPENLOG_FACILITY    0
#define PROGRAM_NAME    "httpd"    /* 日志名称 */
#define	MAXLINE		4096	/* max text line length */
#define	BUFFSIZE	8192	/* buffer size for reads and writes */

void	 err_dump(const int8_t *, ...);
void	 err_msg(const int8_t *, ...);
void	 err_quit(const int8_t *, ...);
void	 err_ret(const int8_t *, ...);
void	 err_sys(const int8_t *, ...);
void     err_debug(const int8_t *, ...);
void     err_alert(const int8_t *, ...);
#endif