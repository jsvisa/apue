#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARN 2
#define LOG_ERROR 3

void logFile(const char *path, char* procName, int level, int sid, char *fmt, ... ) {
	int len;
	int fd;
	time_t t;
	va_list ap;
	struct tm tm;
	char fname[128] ;
	char newfmt[512], line[1024];
	const char *levStr[4] = {"DEBUG", "INFO", "WARN", "ERROR"};

	if( (level < 0) || (level > 4)) {
		va_end( ap );
		return;
	}

	time(&t);
	va_start(ap, fmt);
	localtime_r( &t, &tm );

	sprintf(fname, "%s/%s-%02d%02d%02d.log",
			path, procName, tm.tm_year+1900-2000, tm.tm_mon+1, tm.tm_mday);
	sprintf(newfmt, "[%s]%02d:%02d:%02d [%d]: %s\n",
			levStr[level], tm.tm_hour, tm.tm_min, tm.tm_sec, sid, fmt);
	len = vsnprintf( line, sizeof( line ), newfmt, ap);
	va_end( ap );

	fd=open(fname, O_CREAT|O_RDWR|O_APPEND, S_IRUSR|S_IWUSR);
	if(fd < 0)
		return;
	write(fd, line, len);
	close(fd);
}


#define PROC_NAME "my_prog"
int main(int argc, char **argv) {
	logFile("./", PROC_NAME, LOG_DEBUG, getpid(), "Program starting..., pid is %d", (int)getpid());
	return 0;
}
