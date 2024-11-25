/*
 * Navn: Lucas Ta Jensen
 * Email: ltje24@student.aau.dk
 * Gruppe: 2
 * Studieretning: Cyber- og Computerteknologi
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// contants for lengths of different things in the data.
// if any of these are changed, also modify the sscan function in populate_array
#define DATA_LINE_LENGTH 64
#define MAX_TEAM_LENGTH 3
#define DAY_LENGTH 3
#define TIME_LENGTH 5
#define DATE_LENGTH 5

#define MATCH_AMOUNT 90
#define FILE_NAME "../assignments/exam_assignment3/kampe-2024-2025.txt"

typedef struct Match {
    char day[DAY_LENGTH];
    char date[DATE_LENGTH];
    char time[TIME_LENGTH];
    char home_team[MAX_TEAM_LENGTH];
    char away_team[MAX_TEAM_LENGTH];
    int home_team_goals;
    int away_team_goals;
    int spectators;
} Match;

int populate_array(Match matches[MATCH_AMOUNT], const char *filename);

// Main -----------------------------------------------------------------------------------------------------------

int main() {
    Match matches[MATCH_AMOUNT];

    populate_array(matches, FILE_NAME);

    return EXIT_SUCCESS;
}


int populate_array(Match matches[MATCH_AMOUNT], const char *filename) {
    FILE *data = fopen(filename, "r");
    if (data == NULL) {
        fclose(data);
        printf("Error opening file: %s", filename);
        return -1;
    }

    char line[DATA_LINE_LENGTH];
    int row = 0;

    while (fgets(line, sizeof(line), data) && row < MATCH_AMOUNT) {
        int handled_variables = sscanf(line, "%3s %5s %5s %[^ ] - %[^ ] %d - %d %d",
               matches[row].day, matches[row].date, matches[row].time, matches[row].home_team, matches[row].away_team,
               &matches[row].home_team_goals, &matches[row].away_team_goals, &matches[row].spectators);

        if (handled_variables != 8) {
            fclose(data);
            printf("Error reading data from line: %i (file: %s)", row + 1, filename);
            return -2;
        }

        row++;
    }

    fclose(data);


    return 0;
}