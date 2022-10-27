#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <inttypes.h>
#include <assert.h>

#define YELLOW "<font color=Gold>"
#define RED    "<font color=DarkRed>"
#define ORANGE "<font color=DarkOrange>"
#define GREEN  "<font color=LimeGreen>"
#define BLUE   "<font color=MediumBlue>"
#define OLIVE  "<font color=Olive>"
#define BLACK  "<font color=Black>"
#define CANCEL "</font>"
#define USUAL  BLACK

#define LOG_FILE "log.html"

/*________________________________USER_FUNCTION_DECLARATIONS_________________________________*/

/**
*   @brief Prints message in LOG_FILE.
*
*   @param fmt [in] - printf format
*
*   @return nothing
*/
void log_message       (const char *fmt, ...);

/**
*   @brief Prints error-message in LOG_FILE. Before the message prints "ERROR: ".
*
*   @param fmt [in] - printf format
*
*   @return nothing
*/
void log_error         (const char *fmt, ...);

/**
*   @brief Prints warning-message in LOG_FILE. Before the message prints "WARNING: ".
*
*   @param fmt [in] - printf format
*
*   @return nothing
*/
void log_warning       (const char *fmt, ...);

/**
*   @brief Prints string in LOG_FILE even if it is "nullptr" or "poison".
*
*   @param str_name [in] - name of string to print
*   @param str      [in] - pointer to the first byte of string to print
*   @param len      [in] - minimal length of segment occupied by string
*
*   @return nothing
*/
void log_char_ptr      (const char *str_name, const char *str);

void log_param_place   (const char   *file,
                        const char   *func,
                        const int32_t line);

/*______________________________ADDITIONAL_FUNCTION_DECLARATIONS_____________________________*/

/**
*   @brief Opens log-file. Ckecks if opening is OK and in this case prints message in the log-file.
*   @brief Uses atexit() to call LOG_STREAM_CLOSE() after program end.
*
*   @return 1 if checking is OK. Does abort() if an ERROR found.
*/
static int LOG_STREAM_OPEN  ();

/**
*   @brief Closes log-file. Called by using atexit().
*
*   @return 1 if closing is OK. Does abort() if an ERROR found.
*/
static void LOG_STREAM_CLOSE ();

/*___________________________________________________________________________________________*/

static FILE *LOG_STREAM            = nullptr;
static int  _OPEN_CLOSE_LOG_STREAM = LOG_STREAM_OPEN();

static int LOG_STREAM_OPEN()
{
    LOG_STREAM = fopen(LOG_FILE, "w");

    assert (LOG_STREAM != nullptr);
    setvbuf(LOG_STREAM,   nullptr, _IONBF, 0);
    fprintf(LOG_STREAM, "<pre>\n""\"%s\" OPENING IS OK\n\n", LOG_FILE);

    atexit(LOG_STREAM_CLOSE);
    return 1;
}

static void LOG_STREAM_CLOSE()
{
    assert (LOG_STREAM != nullptr);

    fprintf(LOG_STREAM, "\n\n\"%s\" CLOSING IS OK\n\n", LOG_FILE);
    fclose (LOG_STREAM);
}

/*___________________________________________________________________________________________*/

#define log_place()                                                                 \
        log_message("\n"                                                            \
                    "    FILE: %s\n"                                                \
                    "FUNCTION: %s\n"                                                \
                    "    LINE: %d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__)

void log_param_place(const char   *file,
                     const char   *func,
                     const int32_t line)
{
    assert(file != nullptr);
    assert(func != nullptr);

    log_message("\n"
                "    FILE: %s\n"
                "FUNCTION: %s\n"
                "    LINE: %d\n", file, func, line);
}

void log_message(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    vfprintf(LOG_STREAM, fmt, ap);
}

void log_char_ptr(const char *str_name, const char *str)
{
    assert(str_name != nullptr);

    if    (str == nullptr)  log_message("%s: " BLUE  "nullptr\n" CANCEL, str_name);
    else                    log_message("%s: " USUAL "\"%s\"\n"  CANCEL, str_name, str);
}

void log_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    fprintf (LOG_STREAM, RED "ERROR: ");
    vfprintf(LOG_STREAM, fmt, ap);
    fprintf (LOG_STREAM, CANCEL);
}

void log_warning(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    fprintf (LOG_STREAM, ORANGE "WARNING: ");
    vfprintf(LOG_STREAM, fmt, ap);
    fprintf (LOG_STREAM, CANCEL);
}
