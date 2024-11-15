/*
 * Navn: Lucas Ta Jensen
 * Email: ltje24@student.aau.dk
 * Gruppe: 2
 * Studieretning: Cyber- og Computerteknologi
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MIN_DICE 5
#define MAX_DICE 50
#define DICE_SIDES 6
#define SCOREBOARD_SIZE 16
#define SMALL_STRAIGHT_POINTS 15
#define LARGE_STRAIGHT_POINTS 20
#define YATZE_POINTS 50
#define BONUS 50

// prototypes -----------------------------------------------------

int roll_die();
int *roll_multiple_dice(int n);

void handle_rolls(int number_of_dice, int *point_array, int *total_points);
void handle_roll(int number_of_dice, int *point_array, int *total_points, int current_point_index, int (*check_function)(int*, int, int), int roll_argument);
void print_score_board(int *point_array, int total_points);


int check_for_matching_dice(int *dice_array, int array_size, int die_to_match);
int check_for_pairs(int *dice_array, int array_size, int required_pairs);
int check_for_sets(int *dice_array, int array_size, int required_set_size);
int check_for_straights(int *dice_array, int array_size, int start_number);
int check_for_full_house(int *dice_array, int array_size, int blank);
int check_for_yatzy(int *dice_array, int array_size, int blank);

void clear_input();
int array_includes(int *array, int array_size, int value, bool find_all);
void print_array(int *array, int array_size);

// main ----------------------------------------------------------

int main() {
    int n;
    char play_again = 'y';

    // setup
    srand(time(NULL));

    while (play_again == 'y' || play_again == 'Y'){
        printf("Welcome to Yatzy (kind of) !!\n\n");
        printf("Enter how many dice you would like to play with (has to be 5 or more):\n");
        while (!scanf("%i", &n) || n < MIN_DICE || n > MAX_DICE) {
            printf("Invalid Input. Enter an int equal to 5 or more:\n");
            clear_input();
        }

        // roll handling
        int point_array[SCOREBOARD_SIZE];
        int total_points = 0;

        handle_rolls(n, point_array, &total_points);
        print_score_board(point_array, total_points);


        printf("Would you like to play again? Enter 'y' to play again, enter any other key to exit:\n");
        clear_input();

        scanf("%c", &play_again);
    }

    printf("\nThanks for playing ^~^\n");
    return EXIT_SUCCESS;
}

// dice rolling -----------------------------------------------------

// rolls a number between 1 and 6
int roll_die() {
    return (rand() % DICE_SIDES) + 1;
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

    // rolls for matching value (one for each side of the dice)
    for (int i = current_point_index; i < DICE_SIDES; i++) {
        handle_roll(number_of_dice, point_array, total_points, current_point_index, check_for_matching_dice, i + 1);
        current_point_index++;
    }

    if (*total_points > BONUS) {
        point_array[current_point_index] = BONUS;
        *total_points += BONUS;
    }
    current_point_index++;

    // rolls for pairs
    for (int i = 0; i < 2; i++) {
        handle_roll(number_of_dice, point_array, total_points, current_point_index, check_for_pairs, i + 1);
        current_point_index++;
    }

    // rolls for sets
    for (int i = 0; i < 2; i++) {
        handle_roll(number_of_dice, point_array, total_points, current_point_index, check_for_sets, i + 3);
        current_point_index++;
    }

    // rolls for straights
    for (int i = 0; i < 2; i++) {
        handle_roll(number_of_dice, point_array, total_points, current_point_index, check_for_straights, i + 1);
        current_point_index++;
    }

    // rolls for full house
    handle_roll(number_of_dice, point_array, total_points, current_point_index, check_for_full_house, 0);
    current_point_index++;

    // rolls for yatzy
    handle_roll(number_of_dice, point_array, total_points, current_point_index, check_for_yatzy, 0);
}

// handles a roll based on a number of different parameters. used to minimize repetition
void handle_roll(int number_of_dice, int *point_array, int *total_points, int current_point_index, int (*check_function)(int*, int, int), int roll_argument) {
    int *dice_roll = roll_multiple_dice(number_of_dice);
    int points_received = check_function(dice_roll, number_of_dice, roll_argument);

    point_array[current_point_index] = points_received;
    *total_points += points_received;

    free(dice_roll);
}

// prints the scoreboard based on an array of points
void print_score_board(int *point_array, int total_points) {
    printf("\n~~ Score Board ~~\n");
    int current_point_index = 0;

    for (int i = 0; i < DICE_SIDES; i++) {
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
    printf(" Large Straight: %i\n", point_array[current_point_index]);
    current_point_index++;
    printf(" Full House: %i\n", point_array[current_point_index]);
    current_point_index++;
    printf( " Yatzy: %i\n", point_array[current_point_index]);

    printf("\n~~ Total Points: %i ~~\n\n", total_points);
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

    int points = 0;
    int found_pairs = 0;

    // checks each number for pairs starting from the highest. if a pair is found add the sum of them to the points.
    // exit the loop when the required amount of pairs has been found
    for (int i = DICE_SIDES; i > 0; i--) {
        if (array_includes(dice_array, array_size, i, true) >= 2) {
            points += i * 2;
            found_pairs++;
        }

        if (found_pairs >= required_pairs) {
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
    for (int i = DICE_SIDES; i > 0; i--) {
        if (array_includes(dice_array, array_size, i, true) >= required_set_size) {
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
        printf(" Small straight: ");
    } else {
        printf(" Large straight: ");
    }
    print_array(dice_array, array_size);

    int points = 0;

    // checks for every number required for a straight. if a number is not found exit the loop.
    for (int i = start_number; i < start_number + 5; i++) {
        if (!array_includes(dice_array, array_size, i, false)) {
            break;
        }

        // if we are still in the loop in the last iteration, this means that all required values for a straight has been rolled.
        // then the points are set depending on if its a small or a large straight
        if (i == start_number + 4) {
            points = start_number == 1 ? SMALL_STRAIGHT_POINTS : LARGE_STRAIGHT_POINTS;
        }
    }

    printf(" -- %i\n", points);
    return points;
}

// handles the dice rolls for full house. the blank argument is there to make it compatible with the handle_roll function
int check_for_full_house(int *dice_array, int array_size, int blank) {
    printf(" Full House: ");
    print_array(dice_array, array_size);

    int points;
    int set_value;

    // checks if there is a set for every number starting from the highest.
    // if a set is found, set the points to the sum of the dice and save the value used for sets.
    for (int i = DICE_SIDES; i > 0; i--) {
        if (array_includes(dice_array, array_size, i, true) >= 3) {
            points = i * 3;
            set_value = i;
            break;
        }

        // if no set is found after the last value, return 0 points
        if (i == 1) {
            printf(" -- %i\n", 0);
            return 0;
        }
    }

    // for each number check if there are 5 or more of this value (in this case it can be used even if the value is used in a set, as there is both a set and a pair),
    // or if the current number is not the number used for a set, check if there are 2 or more (in which case there is just a pair)
    for (int i = DICE_SIDES; i > 0; i--) {
        int dice_amount = array_includes(dice_array, array_size, i, true);
        if (dice_amount >= 5 || (i != set_value && dice_amount >=2)) {
            points += (i * 2);
            break;
        }

        // if no pair is found after last value, return 0 points
        if (i == 1) {
            printf(" -- %i\n", 0);
            return 0;
        }
    }

    printf(" -- %i\n", points);
    return points;
}

// handles the dice roll for yatze. the blank argument is there to make it compatible with the handle_roll function
int check_for_yatzy(int *dice_array, int array_size, int blank) {
    printf(" Yatzy: ");
    print_array(dice_array, array_size);

    int points = 0;

    // checks every value if there are 5 of them, in which case a yatzy has been rolled
    for (int i = DICE_SIDES; i > 0; i--) {
        if (array_includes(dice_array, array_size, i, true) >= 5) {
            points = YATZE_POINTS;
            break;
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