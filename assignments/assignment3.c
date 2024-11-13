#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void print_numbers(double xs[], int length);
void print_points(double xs[], double ys[], int length);
void dists(double xs[], double ys[], int length, double ds[]);

double calculate_length(int x1, int x2, int y1, int y2);

int main() {
    double xs[] = {0,7,3,8,1};
    double ys[] = {4,0,3,3,5};
    int length = 5;
    printf("xs:\n");
    print_numbers(xs, length);
    printf("\nys:\n");
    print_numbers(ys, length);
    printf("\nPoints:\n");
    print_points(xs, ys, length);
    double ds[length-1];
    dists(ys,xs,length,ds);
    printf("\nDists:\n");
    print_numbers(ds,length-1);

    return EXIT_SUCCESS;
}

void print_numbers(double xs[], int length) {
    for (int i = 0; i < length; i++) {
        printf("%lf\n", xs[i]);
    }
}

void print_points(double xs[], double ys[], int length) {
    for (int i = 0; i < length; i++) {
        printf("(%lf, %lf)\n", xs[i], ys[i]);
    }
}

// Calculates the total length of the lines based on their points
void dists(double xs[], double ys[], int length, double ds[]) {
    for (int i = 0; i <= length; i++) {
        double line_length = calculate_length(xs[i], xs[i + 1], ys[i], ys[i + 1]);
        ds[i] = line_length;
    }
}

// Calculates the distance between two points
double calculate_length(int x1, int x2, int y1, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}
