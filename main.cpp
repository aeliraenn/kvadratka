#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <float.h>
#include <stdlib.h>  //for NULL

const double eps = DBL_EPSILON;
const int inf_roots = 1e9;
const int num_of_tests = 10;

enum cases {
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
        roots[0] = (-coefs[1] - sqrt(discriminant)) / (2 * coefs[0]);
        roots[1] = (-coefs[1] + sqrt(discriminant)) / (2 * coefs[0]);
        return two_roots;
    }
}

int solve (double *coefs, double *roots) {
    assert(isfinite(coefs[0]));
    assert(isfinite(coefs[1]));
    assert(isfinite(coefs[2]));
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
    double roots[] = {123, 123}; //123 -- used instead of NAN as a base value for testing purposes
    double coefs[] = {test[0], test[1], test[2]};          // 'test' array should have the following form:
    int count_roots = solve(coefs, roots);                // {coef_a, coef_b, coef_c, x1, x2}
    if (!(equal(roots[0], test[3]) && equal(roots[1], test[4]))) {
        printf("\033[31mTest #%d failed: solve() -> %lg %lg (should be %lg %lg)\033[0m\n", test_num, roots[0], roots[1], test[3], test[4]);
        return 1;
    }
    else {
        return 0;
    }
}

int run_test_solve() {
    double tests[num_of_tests][5] = {
        {1, 5, 6, -3, -2},
        {1, -2, 1, 1, 123},
        {0, 1, -1, 1, 123},
        {1, 1, -2, -2, 1},
        {0, 0, 0, 123, 123},
        {0, 0, 1, 123, 123},
        {1, -6, 5, 1, 5},
        {1, -10, 24, 4, 6},
        {1, -8, 15, 3, 5},
        {0, 10, -1, 0.1, 123}
    };
    int failed_tests = 0;
    for (int i = 0; i < num_of_tests; ++i) {
        failed_tests += one_test_solve(tests[i], i);
    }
    return failed_tests;
}

int input(double *coefs) {
    assert(coefs[0] != NULL);
    assert(coefs[1] != NULL);
    assert(coefs[2] != NULL);
    input_coefficients:
        printf("Enter coefficients a, b, c:\n");
        if (scanf("%lf %lf %lf", &coefs[0], &coefs[1], &coefs[2]) != 3) {
            flushline();
            goto incorrect_input;
        }
        else {
            return 1;
        }
    incorrect_input:
        printf("Incorrect input\n");
        printf("Still want to continue? \033[33m[y/n]\033[0m\n");
        char ans = getchar();
        flushline();
        if (ans == 'n') {
            return 0;
        }
        else if (ans == 'y') {
            goto input_coefficients;
        }
        else {
            goto incorrect_input;
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
        default:
            printf("Error: switch reached impossible case\n");
    }
}

int loop () {
    ask_for_input:
        printf("Want me to solve another one? \033[33m[y/n]\033[0m\n");
        flushline();
        char ans = getchar();
        if (ans == 'y') {
            return 1;
        }
        else if (ans == 'n') {
            return 0;
        }
        else {
            printf("Incorrect input\n");
            goto ask_for_input;
        }
}

int main () {
    /*
    //testing
    int failed_tests = run_test_solve();
    if (failed_tests > 0) {
        printf("\033[31mFailed tests: %d\033[0m\n", failed_tests);
    }
    else {
        printf("\033[32mAll tests passed!:)\033[0m\n");
    }
    */
    double coefs[] = {NAN, NAN, NAN};
    double roots[] = {NAN, NAN};
    int program_running = 1;
    printf("Solves quadratic equation ax^2 + bx + c = 0\n");
    while (program_running) {
        program_running = input(coefs);
        if (!program_running) {
            printf("End of program. Goodbye.\n");
            break;
        }
        assert(isfinite(coefs[0]));
        assert(isfinite(coefs[1]));
        assert(isfinite(coefs[2]));
        int count_roots = solve(coefs, roots);
        output(count_roots, roots);
        program_running = loop();
    }
    return 0;
}
