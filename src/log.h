#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <inttypes.h>
#include <assert.h>

#define YELLOW "<font color=Gold>"
#define RED    "<font color=DarkRed>"
#define GREEN  "<font color=LimeGreen>"
#define BLUE   "<font color=MediumBlue>"
#define OLIVE  "<font color=Olive>"
#define BLACK  "<font color=Black>"
#define CANCEL "</font>"
#define USUAL  BLACK

#ifndef LOG_FILE
#define LOG_FILE "log.html"
#endif

/*___________________________FUNCTION_DECLARATION___________________________*/

static void log_message      (const char *fmt, ...);
static void log_error        (const char *fmt, ...);
static void log_char_ptr     (const char *str_name, const char         *str, const char   *poison, const uint8_t len);
static void log_int64_t      (const char *num_name, const int64_t num_value, const int64_t poison, const uint8_t len);

static int  OPEN_LOG_STREAM  ();
static void CLOSE_LOG_STREAM ();

/*__________________________________________________________________________*/


static FILE *LOG_STREAM = nullptr;

/**
*   @brief Opens log-file. Ckecks if opening is OK and in this case prints message in the log-file.
*   @brief Uses atexit() to call CLOSE_LOG_STREAM() after program end.
*
*   @return 1 if checking is OK. Does abort() if an ERROR found.
*/

int OPEN_LOG_STREAM()
{
    LOG_STREAM = fopen(LOG_FILE, "w");

    assert (LOG_STREAM != nullptr);

    setvbuf(LOG_STREAM,   nullptr, _IONBF, 0);

    fprintf(LOG_STREAM, "<pre>\n""\"%s\" OPENING IS OK\n\n", LOG_FILE);

    atexit(CLOSE_LOG_STREAM);
    return 1;
}

/**
*   @brief Closes log-file. Called by using atexit().
*
*   @return 1 if closing is OK. Does abort() if an ERROR found.
*/

void CLOSE_LOG_STREAM()
{
    assert (LOG_STREAM != nullptr);

    fprintf(LOG_STREAM, "\n\n\"%s\" CLOSING IS OK\n\n", LOG_FILE);
    fclose (LOG_STREAM);
}

static int _OPEN_CLOSE_LOG_STREAM = OPEN_LOG_STREAM();

#define log_place()                                                                 \
        log_message("    FILE: %s\n"                                                \
                    "FUNCTION: %s\n"                                                \
                    "    LINE: %d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__)

/**
*   @brief Prints message in LOG_FILE.
*
*   @param fmt [in] - printf format
*
*   @return nothing
*/

void log_message(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    vfprintf(LOG_STREAM, fmt, ap);
    //fprintf (LOG_STREAM, "</font>");
}

/**
*   @brief Prints string in LOG_FILE even if it is "nullptr" or "poison".
*
*   @param str_name [in] - name of string to print
*   @param str      [in] - pointer to the first byte of string to print
*   @param poison   [in] - poison (pointer to the first byte)-value of string to print
*   @param len      [in] - minimal length of segment occupied by string
*
*   @return nothing
*/

void log_char_ptr(const char *str_name, const char *str, const char *poison, const uint8_t len)
{
    assert(str_name != nullptr);

    if      (str == nullptr)  log_message("%-*s : " BLUE   "\"nullptr\"\n" CANCEL, len, str_name);
    else if (str ==  poison)  log_message("%-*s : " OLIVE  "\"POISON\"\n"  CANCEL, len, str_name);
    else                      log_message("%-*s : " USUAL  "\"%s\"\n"      CANCEL, len, str_name, str);
}

/**
*   @brief Prints "int64_t" in LOG_FILE even if it is "poison".
*
*   @param num_name  [in] - name of number to print
*   @param num_value [in] - value of number to print
*   @param poison    [in] - poison value of number to print
*   @param len       [in] - minimal length of segment occupied by string
*
*   @return nothing
*/

void log_int64_t(const char *num_name, const int64_t num_value, const int64_t poison, const uint8_t len)
{
    assert(num_name != nullptr);

    if (num_value == poison) log_message("%-*s = " OLIVE  "\"POISON\"\n" CANCEL, len, num_name);
    else                     log_message("%-*s = %lld\n"                       , len, num_name, num_value);
}

/**
*   @brief Prints error-message in LOG_FILE. Before the message prints "ERROR: ".
*
*   @param fmt [in] - printf format
*
*   @return nothing
*/

void log_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    fprintf (LOG_STREAM, RED "ERROR: ");
    vfprintf(LOG_STREAM, fmt, ap);
    fprintf (LOG_STREAM, CANCEL);
}

#endif //LOG_H