#include <time.h>

// some code used:
// https://stackoverflow.com/questions/17167949/how-to-use-timer-in-c

void Wait(int duration_in_milliseconds)
{
    int milliseconds_now = clock() * 1000 / CLOCKS_PER_SEC;
    int milliseconds_end = milliseconds_now + duration_in_milliseconds;
    while(milliseconds_now < milliseconds_end)
        milliseconds_now = clock() * 1000 / CLOCKS_PER_SEC;
}