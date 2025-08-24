#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <float.h>
#include <stdlib.h>  //for NULL

const double eps = DBL_EPSILON;
const int inf_roots = 1e9;
const int num_of_tests = 10;

enum cases {
    no_input = -1,
    no_roots = 0,
    one_root = 1,
    two_roots = 2,
};

//cleans the input stream up to the next line
void flushline () {
    char c = '\0';
    while ((c = getchar()) != '\n' && c != EOF);
}

char equal (double n1, double n2) {
    assert(isfinite(n1));
    assert(isfinite(n2));
    return fabs(n1 - n2) < eps;
}

int solve_linear_equation (double coef_b, double coef_c,
                            double* x1) {
    if (equal(coef_b, 0)) {
        if (equal(coef_c, 0)) {
            return inf_roots;
        }
        else {
            return no_roots;
        }
    }
    else {
        *x1 = -coef_c / coef_b;
        return one_root;
    }
}

int solve_square_equation (double *coefs, double *roots) {
    double discriminant = coefs[1] * coefs[1] - 4 * coefs[0] * coefs[2];
    if (equal(discriminant, 0)) {
        roots[0] = -coefs[1] / (2 * coefs[0]);
        return one_root;
    }
    else if (discriminant < 0) {
        return no_roots;
    }
    else {
        roots[0] = (-coefs[1] + sqrt(discriminant)) / (2 * coefs[0]);
        roots[1] = (-coefs[1] - sqrt(discriminant)) / (2 * coefs[0]);
        return two_roots;
    }
}

int solve (double *coefs, double *roots) {
    if (isnan(coefs[0]) || isnan(coefs[1]) || isnan(coefs[2])) {
        return no_input;
    }
    assert(roots[0] != NULL);
    assert(roots[1] != NULL);
    if (equal(coefs[0], 0)) {
        return solve_linear_equation(coefs[1], coefs[2], &roots[0]);
    }
    else {
        return solve_square_equation(coefs, roots);
    }
}


int one_test_solve(double *test, int test_num) {
    double roots[] = {NAN, NAN};
    double coefs[] = {test[0], test[1], test[2]};          // 'test' array should have the following form:
    int count_roots = solve(coefs, roots);                // {coef_a, coef_b, coef_c, x1, x2}
    if (!(equal(roots[0], test[3]) && equal(roots[1], test[4]))) {
        printf("Test #%d failed: solve() -> %lg %lg (should be %lg %lg)\n", test_num, roots[0], roots[1], test[3], test[4]);
    }
}

int run_test_solve() {
    double tests[num_of_tests][5] = {
        {1, 5, 6, -2, -3},
        {1, -2, 1, 1, NAN},
        {0, 1, -1, 1, NAN},
        {1, 1, -2, -2, 1},
        {0, 0, 0, NAN, NAN},
        {0, 0, 1, NAN, NAN},
        {1, -6, 5, 1, 5},
        {1, -10, 24, 4, 6},
        {1, -8, 15, 3, 5},
        {0, 10, -1, 0.1, NAN}
    };
    int failed_tests = 0;
    for (int i = 0; i < num_of_tests; ++i) {
        failed_tests += one_test_solve(tests[i], i);
    }
    return failed_tests;
}


void input(double *coefs) {
    assert(coefs[0] != NULL);
    assert(coefs[1] != NULL);
    assert(coefs[2] != NULL);
    while (scanf("%lf %lf %lf", &coefs[0], &coefs[1], &coefs[2]) != 3) {
        flushline();
        incorrect_input:
            printf("Incorrect input\n");
            printf("Still want to continue? [y/n]\n");
                char ans = getchar();
            if (ans == 'n') {
                break;
            }
            else if (ans == 'y') {
                char ans1 = getchar();
                if (ans1 == '\n' || ans1 == EOF) {
                    printf("Enter coefficients a, b, c:\n");
                }
                else {
                    flushline();
                    goto incorrect_input;
                }
            }
            else {
                flushline();
                goto incorrect_input;
            }

    }
}

void output(int num_of_roots, double *roots) {
    switch(num_of_roots) {
        case inf_roots:
            printf("Infinite number of roots\n");
            break;
        case no_roots:
            printf("No roots\n");
            break;
        case one_root:
            printf("One root: %lf\n", roots[0]);
            break;
        case two_roots:
            printf("Two roots: %lf %lf\n", roots[0], roots[1]);
            break;
        case no_input:
            printf("Program terminated\n");
            break;
        default:
            printf("Error: switch reached impossible case\n");
    }
}

int main () {
    double coefs[] = {NAN, NAN, NAN};
    double roots[] = {NAN, NAN};
    printf("Solves quadratic equation ax^2 + bx + c = 0\n");
    printf("Enter coefficients a, b, c:\n");
    input(coefs);
    assert(isfinite(coefs[0]));
    assert(isfinite(coefs[1]));
    assert(isfinite(coefs[2]));
    int count_roots = solve(coefs, roots);
    output(count_roots, roots);
    //printf("Failed tests: %d\n", run_test_solve());
    return 0;
}
