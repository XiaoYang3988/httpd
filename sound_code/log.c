#include "log.h"

static void	err_doit(int32_t, int32_t, const int8_t *, va_list);

/* Nonfatal error related to system call
 * Print32_t message and return */

void
err_ret(const int8_t *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, LOG_INFO, fmt, ap);
	va_end(ap);
	return;
}

/* Fatal error related to system call
 * Print32_t message and terminate */

void
err_sys(const int8_t *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(1);
}

/* Fatal error related to system call
 * Print32_t message, dump core, and terminate */

void
err_dump(const int8_t *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
	abort();		/* dump core and terminate */
	//exit(1);		/* shouldn't get here */
}

/* Nonfatal error unrelated to system call
 * Print32_t message and return */

void
err_msg(const int8_t *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, LOG_INFO, fmt, ap);
	va_end(ap);
	return;
}

/* Fatal error unrelated to system call
 * Print32_t message and terminate */

void
err_quit(const int8_t *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(1);
}

/* Fatal error unrelated to system call
 * Print32_t debug and terminate */

void
err_debug(const int8_t *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, LOG_DEBUG, fmt, ap);
	va_end(ap);
	return;
}

/* Fatal error unrelated to system call
 * Print32_t alert and terminate */

void
err_alert(const int8_t *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, LOG_ALERT, fmt, ap);
	va_end(ap);
	return;
}

/* Print32_t message and return to caller
 * Caller specifies "errnoflag" and "level" */

static void
err_doit(int32_t errnoflag, int32_t level, const int8_t *fmt, va_list ap)
{
	int32_t		errno_save, n;
	int8_t	buf[MAXLINE + 1];

	errno_save = errno;		/* value caller might want print32_ted */
#ifdef	HAVE_VSNPRINTF
	vsnprintf(buf, MAXLINE, fmt, ap);	/* safe */
#else
	vsprintf(buf, fmt, ap);					/* not safe */
#endif
	n = strlen(buf);
	if (errnoflag)
		snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
	strcat(buf, "\n");
    openlog(PROGRAM_NAME, OPENLOG_LOGOPT, 0);
	syslog(level, buf);
	return;
}
