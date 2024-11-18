#include "log.h"

static const char *level_strings[] = {
  "DEBUG",
  "INFO",
  "WARNING",
  "ERROR",
  "CRITICAL"
};

void lx_log(LogLevel level, const char *fmt, ...)
{
  if (level < LOG_LVL_INFO)
  {
    // handle debug
    return;
  }
 
  va_list args;
  va_start(args, fmt);

  // get the current time
  time_t t = time(NULL);
  struct tm *tm_info = localtime(&t);
  char time_buffer[26];
  strftime(time_buffer, sizeof(time_buffer), LX_LOG_TIME_FMT, tm_info);

  fprintf(stderr, "[%s] [%s] ", time_buffer, level_strings[level]);
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "\n");

  va_end(args);
}
