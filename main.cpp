#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <float.h>
#include <stdlib.h>  //for NULL
#include <conio.h>

#ifdef _WIN32
#include <windows.h> //for sleep() on Windows
#else
#include <unistd.h> //for usleep() on Linux, etc.
#endif

const double eps = DBL_EPSILON;
const int num_of_tests = 1;
const int delay = 50;

enum colors {
    GREEN,   //32m
    YELLOW,  //33m
    RED,  //31m
    BLUE, //34m
    WHITE //37m
};

enum compare_cases {
    LESS = -1,
    EQUAL = 0,
    MORE = 1
};

enum cases {
    no_roots = 0,
    one_root = 1,
    two_roots = 2,
    inf_roots = -1
};

// more files

struct Coefs {
    double a;
    double b;
    double c;
};

struct Roots {
    double x1;
    double x2;
    int num_of_roots;
};

struct TestData {
    Coefs coefs;
    Roots roots;
};

// add struct for coefs and roots with n_roots

void start (double *coefs, double *roots);
void output (int num_of_roots, double *roots);
void continue_solving (double *coefs, double *roots);
void input (double *coefs, double *roots);
void flushline ();
char compare_doubles (double n1, double n2, int compare_case);
int solve_linear_equation(double coef_b, double coef_c, double *x1);
int solve_square_equation(double *coefs, double *roots);
int solve (double *coefs, double *roots);
bool check_root_linear (double *coefs, double x);
bool check_roots_square (double *coefs, double *roots);
int one_test_solve(TestData *test, int test_num);
void run_test_solve();
void ms_sleep(int milliseconds);
void type_string(const char *text);
void type_int(int num);
void type_double(double num);
void start_loading();

void ms_sleep(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
     usleep((useconds_t)milliseconds * 1000);
#endif
}

void type_string(const char *text) {
    assert(text != NULL);

    for (int i = 0; i < strlen(text); i++) {
        putchar(text[i]);
        fflush(stdout);
        ms_sleep(delay);
    }
}

void type_int(int num) {
    char str_num[20] = {};
    sprintf(str_num, "%d", num);
    type_string(str_num);

}

void type_double(double num) {
    assert(isfinite(num));

    char str_num[30] = {};
    sprintf(str_num, "%lg", num);
    type_string(str_num);

}

void start_loading() {
    ms_sleep(500);
    type_string("Please wait, loading program... \n");
    ms_sleep(500);
    for (int i = 0; i <= 100; i++) {
        ms_sleep(100);
        printf("Loading: \033[33m[%d%%]\033[0m\r", i);
        if (i == 33) {
            type_string("Oof, I need to get a rest...");
            ms_sleep(3000);
            type_string(" To continue, \033[33mpress any key\033[0m (please, don't).\r");
            _getch();
            printf("\x1b[2K");
            ms_sleep(1500);
            type_string("Alright...\r");
            ms_sleep(2000);
            printf("\x1b[2K");
            type_string("Loading: \r");
        }
    }

    //printf("\x1b[2K"); //clears the current line in output buffer
    type_string("\033[32mProgram is loaded! :)\033[0m\a\n");
}

void flushline () {
    char c = '\0';
    while ((c = getchar()) != '\n' && c != EOF);
    //cleans the input stream up to the next line
}

char compare_doubles (double n1, double n2, int compare_case) {
    assert(isfinite(n1));
    assert(isfinite(n2));

    switch (compare_case) {
        case LESS:
            return ((n1 - n2) < eps);
        case EQUAL:
            return (fabs(n1 - n2) < eps);
        case MORE:
            return ((n1 - n2) > eps);
        default:
            printf("Error: compare_doubles didn't get a specifier\n");
    }
}

int solve_linear_equation (double coef_b, double coef_c,
                            double* x1) {
    assert(isfinite(coef_b));
    assert(isfinite(coef_c));
    assert(x1 != NULL);

    if (compare_doubles(coef_b, 0, EQUAL)) {
        if (compare_doubles(coef_c, 0, EQUAL)) {
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
    assert(coefs != NULL);
    assert(isfinite(coefs[0]));
    assert(isfinite(coefs[1]));
    assert(isfinite(coefs[2]));
    assert(roots != NULL);

    if (compare_doubles(coefs[2], 0, EQUAL)) {
        solve_linear_equation(coefs[0], coefs[1], &roots[0]);
        roots[1] = 0;
        return two_roots;
    }
    else {
        double discriminant = coefs[1] * coefs[1] - 4 * coefs[0] * coefs[2];

        if (compare_doubles(discriminant, 0, EQUAL)) {
            roots[0] = roots[1] = -coefs[1] / (2 * coefs[0]);
            return two_roots;
        }
        else if (compare_doubles(discriminant, 0, LESS)) {
            return no_roots;
        }
        else {
            roots[0] = (-coefs[1] - sqrt(discriminant)) / (2 * coefs[0]);
            roots[1] = (-coefs[1] + sqrt(discriminant)) / (2 * coefs[0]);
            return two_roots;
        }
    }
}

int solve (double *coefs, double *roots) {
    assert(coefs != NULL);
    assert(isfinite(coefs[0]));
    assert(isfinite(coefs[1]));
    assert(isfinite(coefs[2]));
    assert(roots != NULL);

    if (compare_doubles(coefs[0], 0, EQUAL)) {
        return solve_linear_equation(coefs[1], coefs[2], &roots[0]);
    }
    else {
        return solve_square_equation(coefs, roots);
    }
}

bool check_root_linear (double *coefs, double x) {
    assert(coefs != NULL);
    assert(isfinite(coefs[1]));
    assert(isfinite(coefs[2]));
    assert(isfinite(x));

    return compare_doubles(coefs[1] * x + coefs[2], 0, EQUAL);
}

bool check_roots_square (double *coefs, double *roots) {
    assert(coefs != NULL);
    assert(isfinite(coefs[0]));
    assert(isfinite(coefs[1]));
    assert(isfinite(coefs[2]));
    assert(roots != NULL);

    return (compare_doubles(coefs[0] * roots[0] * roots[0] + coefs[1] * roots[0] + coefs[2], 0, EQUAL) &&
    compare_doubles(coefs[0] * roots[1] * roots[1] + coefs[1] * roots[1] + coefs[2], 0, EQUAL));
}

int one_test_solve(TestData *test, int test_num) {
     assert(test != NULL);
     assert(isfinite(test->coefs.a));
     assert(isfinite(test->coefs.b));
     assert(isfinite(test->coefs.c));

    double coefs[] = {test->coefs.a, test->coefs.b, test->coefs.c};
    double roots[2] = {};
    int num_of_roots = solve(coefs, roots);

    switch (num_of_roots) {
        case no_roots:
            if (num_of_roots != test->roots.num_of_roots) {
                printf("\033[31mTest #%d failed: solve() -> %d roots (should not be any)\033[0m\n", test_num, num_of_roots, test->roots.num_of_roots);
                return 1;
            }
            else {
                return 0;
            }
        case inf_roots:
            if (num_of_roots != test->roots.num_of_roots) {
                printf("\033[31mTest #%d failed: solve() -> %d roots (should be infinite number of roots)\033[0m\n", test_num, num_of_roots, test->roots.num_of_roots);
                return 1;
            }
            else {
                return 0;
            }
        case one_root:
            if (!(num_of_roots == test->roots.num_of_roots && check_root_linear(coefs, roots[0]))) {
                printf("\033[31mTest #%d failed: solve() -> %lg (number of roots = %d, should be %d)\033[0m\n", test_num, roots[0], num_of_roots, test->roots.num_of_roots);
                return 1;
            }
            else {
                return 0;
            }
        case two_roots:
            if (!(num_of_roots == test->roots.num_of_roots && check_roots_square(coefs, roots))) {
                printf("\033[31mTest #%d failed: solve() -> %lg %lg (number of roots = %d, should be %d)\033[0m\n", test_num, roots[0], roots[1], num_of_roots, test->roots.num_of_roots);
                return 1;
            }
            else {
                return 0;
            }
        default:
            printf("Error");
            return 0;
            break;

    }
}

void run_test_solve() {
    TestData tests[num_of_tests] = {
        {.coefs = {1, 5, 6}, .roots = {-2, -3, 2}}};
         /*
        {.coef_a = 1, .coef_b = -2, .coef_c = 1, .num_of_roots = 2},
        {.coef_a = 0, .coef_b = 1, .coef_c = -1, .num_of_roots = 1},
        {.coef_a = 1, .coef_b = 1, .coef_c = -2, .num_of_roots = 2},
        {.coef_a = 0, .coef_b = 0, .coef_c = 0, .num_of_roots = -1},
        {.coef_a = 0, .coef_b = 0, .coef_c = 1, .num_of_roots = 0},
        {.coef_a = 1, .coef_b = -6, .coef_c = 5, .num_of_roots = 2},
        {.coef_a = 1, .coef_b = -10, .coef_c = 24, .num_of_roots = 2},
        {.coef_a = 1, .coef_b = -8, .coef_c = 15, .num_of_roots = 2},
        {.coef_a = 100, .coef_b = 10, .coef_c = -1, .num_of_roots = 2},
        {.coef_a = 0, .coef_b = 0, .coef_c = -10000000, .num_of_roots = 0},
        {.coef_a = 1, .coef_b = 0, .coef_c = 0, .num_of_roots = 2}
    };
    */

    int failed_tests = 0;
    for (int i = 0; i < num_of_tests; ++i) {
        failed_tests += one_test_solve(&tests[i], i);
    }
    if (failed_tests > 0) {
        type_string("\033[31mFailed tests: %d\033[0m\n");
        type_int(failed_tests);
    }
    else {
        type_string("\033[32mAll tests passed. Hooray!\033[0m\n");
    }
}

void start (double *coefs, double *roots) {
    assert(coefs != NULL);
    assert(roots != NULL);

    type_string("Want to run tests (\033[33mt\033[0m) or solver (\033[33ms\033[0m)? \033[33m[t/s]\033[0m\n");
    char ans = getchar();
    while (!(ans == 't' || ans == 's')) {
        flushline();
        type_string("Incorrect input. Please, type either \033[33m't'\033[0m or \033[33m's'\033[0m.\n");
        ans = getchar();
        char ans1 = getchar();
        if (ans1 != '\n' && ans1 != EOF) {
            ans = '$';
        }
    }
    if (ans == 't') {
        run_test_solve();
        flushline();
        type_string("Want to run the solver (\033[33ms\033[0m) or end (\033[33me\033[0m) the program? \033[33m[s/e]\033[0m\n");
        char ans = getchar();
        while (!(ans == 's' || ans == 'e')) {
            flushline();
            type_string("Incorrect input. Please, type either \033[33m's'\033[0m or \033[33m'e'\033[0m.\n");
            ans = getchar();
            char ans1 = getchar();
            if (ans1 != '\n' && ans1 != EOF) {
                ans = '$';
            }
        }
        if (ans == 's') {
            input(coefs, roots);
            int count_roots = solve(coefs, roots);
            output(count_roots, roots);
            continue_solving(coefs, roots);
        }
        else {
            type_string("End of program. Goodbye.\n");
        }

    }
    else {
        input(coefs, roots);
        int count_roots = solve(coefs, roots);
        output(count_roots, roots);
        continue_solving(coefs, roots);
    }
}

void input(double *coefs, double *roots) {
    assert(coefs != NULL);
    assert(roots != NULL);
    assert(isfinite(coefs[0]));
    assert(isfinite(coefs[1]));
    assert(isfinite(coefs[2]));

    type_string("Enter coefficients a, b, c:\n");
    while (scanf("%lf %lf %lf", &coefs[0], &coefs[1], &coefs[2]) != 3) {
        flushline();
        type_string("Incorrect input. Enter coefficients a, b, c:\n");
    }

}

void output(int num_of_roots, double *roots) {
    assert(roots != NULL);

    switch(num_of_roots) {
        case inf_roots:
            type_string("Infinite number of roots\n");
            break;
        case no_roots:
            type_string("No roots\n");
            break;
        case one_root:
            type_string("One root: ");
            type_double(roots[0]);
            printf("\n");
            break;
        case two_roots:
            type_string("Two roots: ");
            type_double(roots[0]);
            type_string(" and ");
            type_double(roots[1]);
            printf("\n");
            break;
        default:
            type_string("Error: switch reached impossible case\n");
    }
}

void continue_solving (double *coefs, double *roots) {
    type_string("Want me to continue the program? \033[33m[y/n]\033[0m\n");
    flushline();
    char ans = getchar();
    while (!(ans == 'y' || ans == 'n')) {
        flushline();
        type_string("Incorrect input. Please, type either \033[33m'y'\033[0m or \033[33m'n'\033[0m.\n");
        ans = getchar();
    }
    if (ans == 'y') {
        flushline();
        start(coefs, roots);
    }
    else {
        type_string("End of program. Goodbye.\n");
    }
}

int main () {
    type_string("Solves quadratic equation ax^2 + bx + c = 0\n");
    start_loading();
    Roots rroots = {};
    Coefs ccoefs {};
    double coefs[3] = {};
    double roots[] = {NAN, NAN};
    start(coefs, roots);

    return 0;
}
