#include "message.h"

#if defined(__unix__)
#define WARNING_COLOR "\033[0;33m"
#define ERROR_COLOR "\033[0;31m"
#define RESET "\033[0m"
#endif

void regular_message(const char* msg) {
#ifdef DEBUG
  time_t current_time;
  struct tm *timeinfo;
  char time_str[128];

  time(&current_time);
  timeinfo = localtime(&current_time);
  strftime(time_str, 128, "%A %B %d %Y %X", timeinfo);

  fprintf(stdout, "[%s]: %s\n", time_str, msg);
#endif
}

void warning_message(const char* msg) {
#ifdef DEBUG
  time_t current_time;
  struct tm *timeinfo;
  char time_str[128];

  time(&current_time);
  timeinfo = localtime(&current_time);
  strftime(time_str, 128, "%A %B %d %Y %X", timeinfo);

  #if defined(__unix__)
    fprintf(stdout, WARNING_COLOR"[%s]: WARNING: %s\n", time_str, msg);
    fprintf(stdout, RESET"");
  #else
    fprintf(stdout, "[%s]: WARNING: %s\n", time_str, msg);
  #endif
#endif
}

void error_message(const char* msg) {
#ifdef DEBUG
  time_t current_time;
  struct tm *timeinfo;
  char time_str[128];

  time(&current_time);
  timeinfo = localtime(&current_time);
  strftime(time_str, 128, "%A %B %d %Y %X", timeinfo);

  #if defined(__unix__)
    fprintf(stderr, ERROR_COLOR"[%s]: ERROR: %s\n", time_str, msg);
    fprintf(stdout, RESET"");
  #else
    fprintf(stderr, "[%s]: ERROR: %s\n", time_str, msg);
  #endif
#endif
}

#if defined(__unix__)
#undef WARNING_COLOR
#undef ERROR_COLOR
#undef RESET
#endif
