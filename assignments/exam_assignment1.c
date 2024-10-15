#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BINARY_OPERATORS "+-*/^"
#define UNARY_OPERATORS "#%!q"
#define EXIT_CHARACTER 'q'

double run_calculator();

void clear_input();
int scan_data(char* operator, double* operand);
void handle_invalid_input(char* error_message, char* operator, double* operand);
void do_next_op(double* acc, char operator, double operand);

void welcome_message();


int main() {
    double final_result = run_calculator();

    printf("Ended Calculations\nFinal Result: %lf", final_result);
    return EXIT_SUCCESS;
}

// Encapsulated the entire calculator and dialogue into its own function because the assignment told me to do that.
double run_calculator() {
    // Initialisation ------------------------------------------
    double acc, current_operand;
    char current_operator = '0';

    welcome_message();

    printf("Enter Initial Number:\n");

    while (!scanf(" %lf", &acc)) {
        printf("Invalid Input: Enter a number (double)\nEnter Initial Number:\n");
        clear_input();
    }

    // Calculator ---------------------------------------------
    while (current_operator != EXIT_CHARACTER) {
        scan_data(&current_operator, &current_operand);

        do_next_op(&acc, current_operator, current_operand);

        printf("Current Result: %lf\n\n", acc);
    }

    return acc;
}

// This function is used to clear the user inputs.
// This is necessary because if the user inputs multiple characters when supposed to enter one, the rest will buffer for the next scan and mess it up (bad)
void clear_input() {
    while ((getchar()) != '\n');
}

// Handles the input of the operator and the operand
int scan_data(char* operator, double* operand) {
    printf("Enter operator:");

    // Scans for operator and validates if it's a char
    if (!scanf(" %c", operator)) {
        handle_invalid_input("Invalid Input: Operator must be a single character (char)", operator, operand);
        return EXIT_FAILURE;
    }

    clear_input();

    // Checks if the operator is a unary operator (by checking if it's in the unary operator string), in which case nothing else has to be done.
    if (strchr(UNARY_OPERATORS, *operator) != NULL) {
        return EXIT_SUCCESS;
    }

    // Checks if the operator is a binary operator, in which case the user needs to input an operand as well
    if (strchr(BINARY_OPERATORS, *operator) != NULL) {
        printf("Enter operand:");

        // Keeps asking the user for an operand until it's a valid input
        while (!scanf(" %lf", operand)) {
            handle_invalid_input("Invalid Input: Operand must be a number (double)", operator, operand);
            clear_input();
            printf("Enter a different operand:");

        }
        return EXIT_SUCCESS;

    }

    // If the operator is neither among the binary or unary ones, it's not a valid operator
    handle_invalid_input("Invalid Input: Entered Operator is not among accepted operators", operator, operand);
    return EXIT_FAILURE;

}

// Called if the user inputs an invalid input
void handle_invalid_input(char* error_message, char* operator, double* operand) {
    printf("%s\n\n", error_message);
    *operator = '0';
    *operand = 0.0;
}

void do_next_op(double* acc, char operator, double operand) {
    switch (operator) {
        case '+':
            *acc += operand;
            break;
        case '-':
            *acc -= operand;
            break;
        case '*':
            *acc *= operand;
            break;
        case '/':
            *acc /= operand;
            break;
        case '^':
            *acc = pow(*acc, operand);
            break;
        case '#':
            *acc = sqrt(*acc);
            break;
        case '%':
            *acc *= -1;
            break;
        case '!':
            *acc = 1 / *acc;
            break;
        case '0': case EXIT_CHARACTER:
            break;
        default:
            printf("Something went wrong :c\n");
            break;
    }
}

void welcome_message() {
    printf("  _   __                  _____            __  _____       __            __       __               ______ __  ___\n");
    printf(" | | / /___  ____ __ __  / ___/___  ___   / / / ___/___ _ / /____ __ __ / /___ _ / /_ ___   ____  /_  __//  |/  /\n");
    printf(" | |/ // -_)/ __// // / / /__ / _ \\/ _ \\ / / / /__ / _ `// // __// // // // _ `// __// _ \\ / __/   / /  / /|_/ / \n");
    printf(" |___/ \\__//_/   \\_, /  \\___/ \\___/\\___//_/  \\___/ \\_,_//_/ \\__/ \\_,_//_/ \\_,_/ \\__/ \\___//_/     /_/  /_/  /_/  \n");
    printf("                /___/                                                                                                \n");
    printf("\nAvailable binary operators:\n  + Adds the operand to the result\n  - Subtracts the operand from the result\n  * Multiplies the operand with the result\n  / Divides the operand with the result\n  ^ Takes the result to the power of the operand\n");
    printf("\nAvailable unary operators:\n  # Takes the square root of the result\n  %% Flips the sign of the result\n  ! Divides 1 with the result\n");
    printf("\nTo exit the program enter 'q' as the operator\n\n");
}