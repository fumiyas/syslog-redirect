/*
 * Permission to use, copy, modify, and distribute this software
 * for any purpose and without fee is hereby granted. The author
 * disclaims all warranties with regard to this software.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syslog.h>

static int Level = LOG_WARNING;
static const char *Ident = NULL;

static const char *Pid = NULL;
static const char *Facility = NULL;

static const char *PrintLevelP = NULL;
static const char *FlushLogP = NULL;

static const char *facility_string(int facility)
{
  if (facility == LOG_AUTH) {
    return "AUTH";
#ifdef LOG_AUTHPRIV
  } else if (facility == LOG_AUTHPRIV) {
    return "AUTHPRIV";
#endif
  } else if (facility == LOG_CRON) {
    return "CRON";
  } else if (facility == LOG_DAEMON) {
    return "DAEMON";
  } else if (facility == LOG_FTP) {
    return "FTP";
  } else if (facility == LOG_KERN) {
    return "KERN";
  } else if (facility == LOG_LOCAL0) {
    return "LOCAL0";
  } else if (facility == LOG_LOCAL1) {
    return "LOCAL1";
  } else if (facility == LOG_LOCAL2) {
    return "LOCAL2";
  } else if (facility == LOG_LOCAL3) {
    return "LOCAL3";
  } else if (facility == LOG_LOCAL4) {
    return "LOCAL4";
  } else if (facility == LOG_LOCAL5) {
    return "LOCAL5";
  } else if (facility == LOG_LOCAL6) {
    return "LOCAL6";
  } else if (facility == LOG_LOCAL7) {
    return "LOCAL7";
  } else if (facility == LOG_LPR) {
    return "LPR";
  } else if (facility == LOG_MAIL) {
    return "MAIL";
  } else if (facility == LOG_NEWS) {
    return "NEWS";
  } else if (facility == LOG_SYSLOG) {
    return "SYSLOG";
  } else if (facility == LOG_USER) {
    return "USER";
  } else if (facility == LOG_UUCP) {
    return "UUCP";
  }

  return NULL;
}

static const char *level_string(int level)
{
  if (level == LOG_EMERG) {
    return "EMERG";
  } else if (level == LOG_ALERT) {
    return "ALERT";
  } else if (level == LOG_CRIT) {
    return "CRITICAL";
  } else if (level == LOG_ERR) {
    return "ERROR";
  } else if (level == LOG_WARNING) {
    return "WARNING";
  } else if (level == LOG_NOTICE) {
    return "NOTICE";
  } else if (level == LOG_INFO) {
    return "INFO";
  } else if (level == LOG_DEBUG) {
    return "DEBUG";
  }

  return "UNKNOWN";
}

static void init(const char *ident, int option, int facility)
{
  const char *level_env = getenv("SYSLOG_REDIRECT_LOG_LEVEL");
  if (level_env) {
    // FIXME: Support string to integer conversion (e.g.: "warning" -> 4)
    Level = atoi(level_env);
  }

  if (getenv("SYSLOG_REDIRECT_PRINT_IDENT")) {
    Ident = ident;
  }

  if (getenv("SYSLOG_REDIRECT_PRINT_PID")) {
    // FIXME
  }
  if (getenv("SYSLOG_REDIRECT_PRINT_FACILITY")) {
    Facility = facility_string(facility);
  }
  

  PrintLevelP = getenv("SYSLOG_REDIRECT_PRINT_LEVEL");
  FlushLogP = getenv("SYSLOG_REDIRECT_FLUSH_LOG");
}

void openlog(const char *ident, int option, int facility)
{
  init(ident, option, facility);
}

void syslog(int priority, const char *format, ...)
{
  va_list arg_ptr;

  if ((priority & LOG_PRIMASK) > Level) {
    return;
  }

  if (Ident) {
    fputs(Ident, stderr);
    fputs(": ", stderr);
  }
  if (Pid) {
    fputs(Pid, stderr);
    fputs(": ", stderr);
  }
  if (Facility) {
    if (priority & LOG_FACMASK) {
      fputs(facility_string(priority & LOG_FACMASK), stderr);
    } else {
      fputs(Facility, stderr);
    }
    fputs(": ", stderr);
  }
  if (PrintLevelP) {
    fputs(level_string(priority & LOG_PRIMASK), stderr);
    fputs(": ", stderr);
  }

  va_start(arg_ptr, format);
  (void)vfprintf(stderr, format, arg_ptr);
  va_end(arg_ptr);

  if (FlushLogP) {
    fflush(stderr);
  }
}
