#include <stdio.h>

// defining how many of each time unit it takes to get to the next one. very helpful in case the orbit or rotation of the earth changes so we can adjust the values easily.
#define seconds_per_minute 60
#define minutes_per_hour 60
#define hours_per_day 24
#define days_per_week 7

/*
 * for the calculation we start with the lowest unit and divide it with the amount needed to get to the next unit.
 * as we are dealing with ints it will round down, so for instance 119 seconds becomes 1 minute.
 * then we use the modulo operator to calculate how much there is left after the conversion and store that in the variable.
 * in the 119 seconds example we will have 59 seconds left.
 * and then we just continue doing this with the next unit and so on we get until the format is correct.
 */

int main() {
    int seconds, minutes, hours, days, weeks;

    printf("enter a number of seconds:\n");
    scanf("%d", &seconds);

    minutes = seconds / seconds_per_minute;
    seconds %= seconds_per_minute;

    hours = minutes / minutes_per_hour;
    minutes %= minutes_per_hour;

    days = hours / hours_per_day;
    hours %= hours_per_day;

    weeks = days / days_per_week;
    days %= days_per_week;

    printf("weeks: %d\ndays: %d\nhours: %d\nminutes: %d\nseconds: %d", weeks, days, hours, minutes, seconds);

    return 0;
}

