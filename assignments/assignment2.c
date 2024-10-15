#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
    int n;
    int c;

    // Input -------------------------------------------------------

    // Takes input for n and checks if its valid
    printf("Enter the value of n (must be positive integer):\n");

    if (!scanf("%d",&n) || n < 1) {
        printf("Invalid Input for 'n'");
        return EXIT_FAILURE;
    };

    // Checks if the input is over the integer limit, as then the value of n gets set to that, and the result is not accurate.
    // (unless user for some reason entered the exact value of the integer limit, in which case that sucks i guess, u cant do that)
    if (n >= INT_MAX) {
        printf("value of n is over integer limit, please enter a number below %d", INT_MAX);
        return EXIT_FAILURE;
    }

    // Takes input for c and checks if its valid
    printf("Enter the value of c (must be an integer between 0-9):\n");

    if (!scanf("%d", &c) || c < 0 || c > 9) {
        printf("Invalid Input for 'c'");
        return EXIT_FAILURE;
    };

    // Calculations --------------------------------------------------------------

    /*
     * For calculating we use a for-loop for iterating through every number from 1 to n.
     * Inside the for loop we have a nested while-loop to iterate through every digit in the current number.
     * Here we first get the last digit using the modulo operator, and then compares it to the value of c.
     * If they are the same, the digit counter increases by 1, and we break out of loop (as we just need to find how many numbers has the digit c).
     * If they are *not* the same, we remove the last digit by dividing the current number by 10, and the loops keeps running until there are no more digits.
     */

    // Declaring variable to count the occurrence of c in each number
    int digit_counter = 0;

    // Iterates through every number from 1 to n
    for (int i = 1; i <= n; i++) {
        int temporary_number = i;

        // Iterates through every digit to see if c is a digit in the number. If c is found, break out of loop and increase counter.
        while (temporary_number > 1) {
            int last_digit = temporary_number % 10;

            if (last_digit == c) {
                digit_counter++;
                break;
            }

            temporary_number /= 10;
        }
    }

    // Output --------------------------------------------------------------------

    if (digit_counter > 0) {
        printf("\nThe digit '%d' is in %d numbers between 1 - %d", c, digit_counter, n);

    } else {
        printf("The digit '%d' is not in any number between 1 - %d", n, c);

    }

    return EXIT_SUCCESS;
}