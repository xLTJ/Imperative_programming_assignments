/*
 * Navn: Lucas Ta Jensen
 * Email: ltje24@student.aau.dk
 * Gruppe: 2
 * Studieretning: Cyber- og Computerteknologi
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// !! IMPORTANT !!
// makes sure that this points to the correct location of the file
#define FILE_NAME "../assignments/exam_assignment3/kampe-2024-2025.txt"

// constants for lengths of different things in the data. they have to be at least 1 larger than the actual limit to account for the null-string at the end
// if any of these are changed, also modify the sscan function in populate_array
#define DATA_LINE_LENGTH 64
#define MAX_TEAM_LENGTH 4
#define DAY_LENGTH 4
#define TIME_LENGTH 6
#define DATE_LENGTH 6

#define MATCH_AMOUNT 90
#define TEAM_AMOUNT 12

// structs ---------------------------------------------------------------------------------------------------------

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

typedef struct Team {
    char name[MAX_TEAM_LENGTH];
    int points;
    int goals_by_team;
    int goals_against_team;
} Team;


// prototypes ----------------------------------------------------------------------------------------------------

int populate_match_array(Match matches[MATCH_AMOUNT], const char *filename);
int add_match_to_team_data(Match match, Team teams[TEAM_AMOUNT]);
int is_team_empty(Team team);
int index_of_team(char team_name[MAX_TEAM_LENGTH], Team teams[TEAM_AMOUNT]);
int add_team(char team_name[MAX_TEAM_LENGTH], Team teams[TEAM_AMOUNT]);

// Main -----------------------------------------------------------------------------------------------------------

int main() {
    Match matches[MATCH_AMOUNT];
    Team teams[TEAM_AMOUNT] = {0};

    populate_match_array(matches, FILE_NAME);

    for (int i = 0; i < MATCH_AMOUNT; i++) {
        add_match_to_team_data(matches[i], teams);
    }

    return EXIT_SUCCESS;
}


int populate_match_array(Match matches[MATCH_AMOUNT], const char *filename) {
    FILE *data = fopen(filename, "r");
    if (data == NULL) {
        fclose(data);
        printf("Error opening file: %s\n", filename);
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
            printf("Error reading data from line: %i (file: %s)\n", row + 1, filename);
            return -1;
        }

        row++;
    }

    fclose(data);
    return 0;
}


int add_match_to_team_data(Match match, Team teams[TEAM_AMOUNT]) {
    int home_team_index = index_of_team(match.home_team, teams);
    int away_team_index = index_of_team(match.away_team, teams);

    // the index -1 is returned if the team could not be found in the array, so in that case we add the missing team(s)
    if (home_team_index == -1) {
        home_team_index = add_team(match.home_team, teams);
    }

    if (away_team_index == -1) {
        away_team_index = add_team(match.away_team, teams);
    }

    // if the returned index from add_team is -1, this means the array is full
    if (home_team_index == -1 || away_team_index == -1) {
        printf("Error, team array is full, cannot add team\n");
        return -1;
    }

    // adding goals
    teams[home_team_index].goals_by_team += match.home_team_goals;
    teams[home_team_index].goals_against_team += match.away_team_goals;
    teams[away_team_index].goals_by_team += match.away_team_goals;
    teams[away_team_index].goals_against_team += match.home_team_goals;

    // adding points based on who won
    if (match.home_team_goals > match.away_team_goals) {
        teams[home_team_index].points += 3;
    } else if (match.home_team_goals < match.away_team_goals) {
        teams[away_team_index].points += 3;
    } else {
        teams[home_team_index].points++;
        teams[away_team_index].points++;
    }
}


int is_team_empty(Team team) {
    return team.name[0] == '\000';
}


int index_of_team(char team_name[MAX_TEAM_LENGTH], Team teams[TEAM_AMOUNT]) {
    for (int i = 0; i < TEAM_AMOUNT; i++) {
        if (strcmp(team_name, teams[i].name) == 0) {
            return i;
        }
    }

    return -1; // team not found in array
}


int add_team(char team_name[MAX_TEAM_LENGTH], Team teams[TEAM_AMOUNT]) {
    for (int i = 0; i < TEAM_AMOUNT; i++) {
        if (is_team_empty(teams[i])) {
            strcpy(teams[i].name, team_name);
            return i;
        }
    }

    return -1; // array is full
}