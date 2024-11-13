/*
 * Navn: Lucas Ta Jensen
 * Email: ltje24@student.aau.dk
 * Gruppe: 2
 * Studieretning: Cyber- og Computerteknologi
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_DICE 5
#define BONUS 50
#define SCOREBOARD_SIZE 16

// prototypes -----------------------------------------------------

int roll_die();
int *roll_multiple_dice(int n);

void handle_rolls(int number_of_dice, int *point_array, int *total_points);
void print_score_board(int *point_array, int total_points);
int check_for_matching_dice(int *dice_array, int array_size, int die_to_match);
int check_for_pairs(int *dice_array, int array_size, int required_pairs);
int check_for_sets(int *dice_array, int array_size, int required_set_size);
int check_for_straights(int *dice_array, int array_size, int start_number);

void clear_input();
int array_includes(int *array, int array_size, int value, bool find_all);
void print_array(int *array, int array_size);
int array_highest_value(int *array, int array_size, bool remove_largest_number);

// main ----------------------------------------------------------

int main() {
    int n;

    // setup
    srand(time(NULL));

    printf("Welcome to Yatze (kind of) !!\n\n");
    printf("Enter how many dice you would like to play with (has to be 5 or more):\n");
    while (!scanf("%i", &n) || n < MIN_DICE) {
        printf("Invalid Input. Enter an int equal to 5 or more:\n");
        clear_input();
    }

    // roll handling
    int point_array[SCOREBOARD_SIZE];
    int total_points = 0;

    handle_rolls(n, point_array, &total_points);
    print_score_board(point_array, total_points);
}

// dice rolling -----------------------------------------------------

// rolls a number between 1 and 6
int roll_die() {
    return (rand() % 6) + 1;
}

// rolls n dice, and returns the result in an array.
int *roll_multiple_dice(int n) {
    int *dice_array = malloc(n * sizeof(int));


    for (int i = 0; i < n; i++) {
        dice_array[i] = roll_die();
    }

    return dice_array;
}

// point counting -----------------------------------------------------

// calculates all the different rolls
void handle_rolls(int number_of_dice, int *point_array, int *total_points) {
    printf("\n~~ Rolling Dice ~~\n");
    int current_point_index = 0;

    // rolls for matching value
    for (int i = current_point_index; i < 6; i++) {
        int *dice_roll = roll_multiple_dice(number_of_dice);
        int points_received = check_for_matching_dice(dice_roll, number_of_dice, i + 1);
        point_array[current_point_index] = points_received;
        *total_points += points_received;

        free(dice_roll);
        current_point_index++;
    }

    if (*total_points > BONUS) {
        point_array[current_point_index] = BONUS;
        *total_points += BONUS;
    }
    current_point_index++;

    // rolls for pairs
    for (int i = 0; i < 2; i++) {
        int *dice_roll = roll_multiple_dice(number_of_dice);
        int points_received = check_for_pairs(dice_roll, number_of_dice, i + 1);
        point_array[current_point_index] = points_received;
        *total_points += points_received;

        free(dice_roll);
        current_point_index++;
    }

    // rolls for sets
    for (int i = 0; i < 2; i++) {
        int *dice_roll = roll_multiple_dice(number_of_dice);
        int points_received = check_for_sets(dice_roll, number_of_dice, i + 3);
        point_array[current_point_index] = points_received;
        *total_points += points_received;

        free(dice_roll);
        current_point_index++;
    }

    // rolls for straights
    for (int i = 0; i < 2; i++) {
        int *dice_roll = roll_multiple_dice(number_of_dice);
        int points_received = check_for_straights(dice_roll, number_of_dice, i + 1);
        point_array[current_point_index] = points_received;
        *total_points += points_received;

        free(dice_roll);
        current_point_index++;
    }
}

// prints the scoreboard based on an array of points
void print_score_board(int *point_array, int total_points) {
    printf("\n~~ Score Board ~~\n");
    int current_point_index = 0;

    for (int i = 0; i < 6; i++) {
        printf(" %i's: %i\n", i + 1, point_array[current_point_index]);
        current_point_index++;
    }

    printf(" Bonus: %i\n\n", point_array[current_point_index]);
    current_point_index++;

    for (int i = 0; i < 2; i++) {
        printf(" %i pair(s): %i\n", i + 1, point_array[current_point_index]);
        current_point_index++;
    }

    for (int i = 0; i < 2; i++) {
        printf(" %i of a kind: %i\n", i + 3, point_array[current_point_index]);
        current_point_index++;
    }

    printf(" Small straight: %i\n", point_array[current_point_index]);
    current_point_index++;
    printf(" Large Straight: %i", point_array[current_point_index]);
    current_point_index++;

    printf("\n~~ Total Points: %i ~~", total_points);
}

// handles the dice roll for when you want to roll a specific value, and returns the awarded points
int check_for_matching_dice(int *dice_array, int array_size, int die_to_match) {
    printf(" %i's: ", die_to_match);
    print_array(dice_array, array_size);

    int matching_dice_amount = array_includes(dice_array, array_size, die_to_match, true);

    // the roll must include 5 dice, so if there are more than 5 matching dice these will not be counted
    if (matching_dice_amount > MIN_DICE) {
        matching_dice_amount = MIN_DICE;
    }

    printf(" -- %i\n", matching_dice_amount * die_to_match);
    return matching_dice_amount * die_to_match;
}

// handles the dice rolls for when you want to roll a specific amount of pairs, and returns the awarded points
int check_for_pairs(int *dice_array, int array_size, int required_pairs) {
    printf(" %i pairs: ", required_pairs);
    print_array(dice_array, array_size);

    int pair_array[6];

    // checks how many pairs there is of each number, and adds that to pair_array
    for (int i = 0; i < 6; i++) {
        pair_array[i] = array_includes(dice_array, array_size, i + 1, true) / 2;
    }

    int current_pair_amount = 0;
    int points = 0;

    // adds points per pair from the highest number to the lowest number. exists when the required number of pairs is met
    for (int i = 5; i >= 0; i--) {

        // if there are more pairs of the number than remaining number of required pairs, only take the required pairs (also the parentheses are just for readability)
        int pairs_to_take = (pair_array[i] > (required_pairs - current_pair_amount)) ? (required_pairs - current_pair_amount) : (pair_array[i]);

        points += pairs_to_take * (i + 1) * 2;
        current_pair_amount += pair_array[i];

        if (current_pair_amount >= required_pairs) {
            break;
        }
    }

    printf(" -- %i\n", points);
    return points;
}

// handles the dice rolls for when you want to roll a set of a specific size, and returns the awarded points
int check_for_sets(int *dice_array, int array_size, int required_set_size) {
    printf(" %i of a kind: ", required_set_size);
    print_array(dice_array, array_size);

    int points = 0;

    // checks if there is a set for every number starting from the highest. if a set is found, set the points to the sum of the dice and exit the loop
    for (int i = 6; i > 0; i--) {
        if (array_includes(dice_array, array_size, i) >= required_set_size, true) {
            points = i * required_set_size;
            break;
        }
    }

    printf(" -- %i\n", points);
    return points;
}

// handles the dice rolls for when you want to roll for a type of straight (either 1-5 for small or 2-6 for large).
// both types are 5 numbers from the start number, so the start number is the only thing we need to keep in mind
int check_for_straights(int *dice_array, int array_size, int start_number) {
    if (start_number == 1) {
        printf("Small straight: ");
    } else {
        printf("Large straight: ");
    }

    int points;

    // checks for every number required for a straight. if a number is not found exit the loop.
    for (int i = start_number; i < start_number + 5; i++) {
        if (!array_includes(dice_array, array_size, i, false)) {
            break;
        }

        // if we are still in the loop in the last iteration, this means that all required values for a straight has been rolled
        // then the points are set depending on if its a small or a large straight
        if (i == start_number + 4) {
            points = start_number == 1 ? 15 : 20;
        }
    }

    printf(" -- %i\n", points);
    return points;
}

// helper functions --------------------------------------------------

// clears user input to prevent input buffer overflow
void clear_input() {
    while ((getchar()) != '\n');
}

// takes an array and a value, checks how many instances of said value is in the array if find_all is true. if find_all is false the function just figures out if the value exists in the array.
int array_includes(int *array, int array_size, int value, bool find_all) {
    int count = 0;

    for (int i = 0; i < array_size; i++) {
        if (array[i] == value) {
            count++;

            // if find_all is false exit the loop as continuing is unnecessary
            if (!find_all) {
                break;
            }
        }
    }

    return count;
}

// prints the contents of an array
void print_array(int *array, int array_size) {
    printf("[ ");
    for (int i = 0; i < array_size; i++) {
        printf("%i ", array[i]);
    }
    printf("]");
}

// returns the largest value in an array. is also able to remove this value from the array if needed.
int array_highest_value(int *array, int array_size, bool remove_largest_number) {
    int largest_value = 0;
    int largest_value_index = 0;

    for (int i = 0; i < array_size; i++) {
        if (array[i] > largest_value) {
            largest_value = array[i];
            largest_value_index = i;
        }
    }

    if (remove_largest_number) {
        array[largest_value_index] = -1;
    }

    return largest_value;
}