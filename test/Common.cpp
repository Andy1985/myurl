#include <sys/time.h>
#include <string.h>
#include "Common.h"

int my_itoa (char *to, unsigned int from)
{
    return sprintf (to, "%u", from);
}

unsigned int my_atoi (const char *to)
{
    return (unsigned int) strtoul (to, NULL, 10);
}

string GetCurrentTime ()
{
    char current_time[32] = { 0 };
    time_t now = time (NULL);
    struct tm *ptr = localtime (&now);

    strftime (current_time, 32, "%Y-%m-%d %H:%M:%S", ptr);

    return string (current_time);
}

string GetCurrentDate ()
{
    char current_time[32] = { 0 };
    time_t now = time (NULL);
    struct tm *ptr = localtime (&now);

    strftime (current_time, 32, "%Y-%m-%d", ptr);

    return string (current_time);
}
