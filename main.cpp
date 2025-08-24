#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <float.h>

const double eps = DBL_EPSILON;
const int inf_roots = 1e9;     //enum
enum {
    no_input = -1,
    no_roots = 0,
    one_root = 1,
    two_roots = 2,
};

char compare_doubles (double n1, double n2) {
    return fabs(n1 - n2) < eps;
}
    //разбить на три функции

int Solve_Linear_Equation (double coef_b, double coef_c,
                            double* x1) {
    *x1 = -coef_c / coef_b;
    return one_root;
}

int Solve_Square_Equation (double coef_a, double coef_b, double coef_c,
                                        double* x1, double* x2) {
    double discriminant = coef_b * coef_b - 4 * coef_a * coef_c;
    if (compare_doubles(discriminant, 0)) {
            *x1 = -coef_b / (2 * coef_a);
            return one_root;
        }
        else if (discriminant < 0) {
            return no_roots;
        }
        else {
            *x1 = (-coef_b + sqrt(discriminant)) / (2 * coef_a);
            *x2 = (-coef_b - sqrt(discriminant)) / (2 * coef_a);

            return two_roots;
        }
}

int Solve (double coef_a, double coef_b, double coef_c,
                                double* x1, double* x2)    {
    if (isnan(coef_a) || isnan(coef_b) || isnan(coef_c)) {
        return no_input;
    }
    if (compare_doubles(coef_a, 0)) {
        if (compare_doubles(coef_b, 0)) {
            if (compare_doubles(coef_c, 0)) {
                return inf_roots;
            }
            else {
                return no_roots;
            }
        }
        else {
            return Solve_Linear_Equation(coef_b, coef_c, x1);
        }
    }
    else {
        return Solve_Square_Equation(coef_a, coef_b, coef_c, x1, x2);
    }
}

void input(double &coef_a, double &coef_b, double &coef_c) {
    while (scanf("%lf %lf %lf", &coef_a, &coef_b, &coef_c) != 3) {
        char ch = getchar();
        while (ch != '\n' && ch != EOF)
            ch = getchar();
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
                    while (ans1 != '\n' && ans != EOF)
                        ans1 = getchar();
                    goto incorrect_input;
                }
            }
            else {
                while (ans != '\n' && ans != EOF)
                    ans = getchar();
                goto incorrect_input;
            }

    }
}

void output(int num_of_roots, double &x1, double &x2) {
    switch(num_of_roots) {
        case inf_roots:
            printf("Infinite number of roots\n");
            break;
        case no_roots:
            printf("No roots\n");
            break;
        case one_root:
            printf("One root: %lf\n", x1);
            break;
        case two_roots:
            printf("Two roots: %lf %lf\n", x1, x2);
            break;
        case no_input:
            printf("Program terminated");
            break;
        default:
            printf("Error: switch reached impossible case");      //
    }
}


int main () {
    double coef_a = NAN, coef_b = NAN, coef_c = NAN;     // rename     and add nan
    double x1 = NAN, x2 = NAN;
    printf("Solves quadratic equation ax^2 + bx + c = 0\n");
    printf("Enter coefficients a, b, c:\n");
    input(coef_a, coef_b, coef_c);
    int count_roots = Solve(coef_a, coef_b, coef_c, &x1, &x2);
    output(count_roots, x1, x2);
    return 0;
}
