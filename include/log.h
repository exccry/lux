#pragma once

#include <stdarg.h>
#include <time.h>

#include "internal.h"

#define LX_LOG_TIME_FMT "%d-%m-%Y %H:%M:%S"

typedef enum
{
  LOG_LVL_DEBUG,
  LOG_LVL_INFO,
  LOG_LVL_WARNING,
  LOG_LVL_ERROR,
  LOG_LVL_CRITICAL,
} LogLevel;

void lx_log(LogLevel level, const char *fmt, ...);
