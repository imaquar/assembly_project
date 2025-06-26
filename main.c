#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>

typedef double (*afunc)(double);

// Declaration of functions from the funcs.asm file
extern double f1(double);
extern double f2(double);
extern double f3(double);

// Declaration of three additional functions 
// from the funcs.asm file (for tests)
extern double f11(double);
extern double f22(double);
extern double f33(double);

// Variables for counting iterations
int root_iterations = 0;
int integral_iterations = 0;

// Function for calculating the root by the chord method
double root(afunc f, afunc g, double a, double b, double eps1) {

    const int max_iterations = 1000; // Variable for max number of iterations
    double c = NAN; // Intersection point
    root_iterations = 0; // Counter of iterations
    
    // F(x) = f(x) - g(x) on the boundaries of the segment
    double F_a = f(a) - g(a);
    double F_b = f(b) - g(b);
    // Check if F(a) and F(b) have different signs
    if (F_a * F_b >= 0) {
        fprintf(stderr, "Function does not change sign on the boundaries of the segment\n");
        return NAN;
    }

    // Check the sign of the first derivative (monotonicity), i.e. F_b > F_a => function increases, otherwise decreases
    int is_increasing = (F_b > F_a); // Flag, responsible for whether the function increases or decreases

    // Check the sign of the second derivative(convexity), i.e. compare the value of the function at the midpoint with the value of the chord
    double mid = (a + b) / 2; // Midpoint of the segment [a, b]
    double F_mid = f(mid) - g(mid); // Difference of functions at the midpoint
    double chord_mid = (F_a + F_b) / 2; // Chord at the midpoint
    int is_above = (F_mid > chord_mid); // Flag, responsible for whether the function is above the chord
    // Sign of the product of derivatives F'*F'', i.e. if the function increases and is above the chord or 
    // the function decreases and is below the chord, then F'*F'' > 0, i.e. the same signs of derivatives
    int same_sign_of_derivatives = (is_increasing && is_above) || (!is_increasing && !is_above);

    // Main loop
    while (root_iterations < max_iterations) {
        root_iterations++;
        // Calculate the values of the function on our boundaries
        F_a = f(a) - g(a);
        F_b = f(b) - g(b);
        // Calculate the intersection point of the chord with the abscissa axis:
        c = (a * F_b - b * F_a) / (F_b - F_a);
        double F_c = f(c) - g(c);
        // Check the exit conditions:
        // the value of the function is close to zero |F(c)| < eps1 or
        // the length of the segment is less than the accuracy |b-a| <= eps1
        if (fabs(F_c) < eps1 || fabs(b - a) <= eps1) {
            break;
        }
        // Update the boundaries that depend on the signs of derivatives
        if (same_sign_of_derivatives) {
            // F'*F'' > 0 left approximation
            double F_c_eps = f(c + eps1) - g(c + eps1); // Check eps neighborhood to the right of c
            if (F_c * F_c_eps < 0) {
                break;
            }
            a = c;
        } else {
            // F'*F'' < 0 right approximation
            double F_c_eps = f(c - eps1) - g(c - eps1); // Check eps neighborhood to the left of c
            if (F_c * F_c_eps < 0) {
                break;
            }
            b = c;
        }
    }
    // Check for a large number of iterations
    if (root_iterations >= max_iterations) {
        fprintf(stderr, "The number of iterations is greater than the maximum allowed\n");
    }
    
    return c;
}

// Function for calculating the integral by the rectangle method
double integral(afunc f, double a, double b, double eps2) {

    integral_iterations = 0; // Counter of iterations
    int n = 10; // Initial number of divisions n0 = 10
    double prev_integral = 0.0; // Previous value of the integral In
    double current_integral = 0.0; // Current value of the integral I2n
    double h, x, sum;
    // Case when the limits of integration are given in reverse order
    int sign = 1;
    if (a > b) {
        double temp = a;
        a = b;
        b = temp;
        sign = -1;
    }
    
    // Calculate the integral with the initial number of divisions
    h = (b - a) / n; // Step of integration
    sum = 0.0;
    for (int i = 0; i < n; i++) {
        x = a + (i + 0.5) * h;
        sum += f(x);
    }
    current_integral = sum * h; // The formula of rectangles
    integral_iterations++;
    // Main loop with doubling the number of divisions
    while (integral_iterations < 20) {
        prev_integral = current_integral; // Saving the previous value of In
        n *= 2; // Doubling the number of divisions
        h = (b - a) / n; // New step
        sum = 0.0; // Sum for the number of divisions
        for (int i = 0; i < n; i++) {
            x = a + (i + 0.5) * h;
            sum += f(x);
        }
        current_integral = sum * h; // New value of the integral I2n
        integral_iterations++;
        // Check the Runge condition |In-I2n| < 3*eps2
        if (integral_iterations > 1 && fabs(current_integral - prev_integral) < 3 * eps2) {
            break;
        }
    }
    
    return (sign * current_integral);
}


// Function for testing root
void test_root(int func1, int func2, double a, double b, double eps, double r) {

    // Definition of function f
    afunc f; 
    if (func1 == 1) {
        f = f1;
    } else if (func1 == 2) {
        f = f2;
    } else if (func1 == 3) {
        f = f3;
    } else if (func1 == 11) {
        f = f11;
    } else if (func1 == 22) {
        f = f22;
    } else {
        f = f33;
    }
    // Definition of function g
    afunc g;
    if (func2 == 1) {
        g = f1;
    } else if (func2 == 2) {
        g = f2;
    } else if (func2 == 3) {
        g = f3;
    } else if (func2 == 11) {
        g = f11;
    } else if (func2 == 22) {
        g = f22;
    } else{
        g = f33;
    }
    
    double result = root(f, g, a, b, eps); // Our result
    double abs_error = fabs(r - result); // Absolute error
    double rel_error = abs_error / fabs(r); // Relative error
    
    printf("%.5lf %.5lf %.5lf\n", result, abs_error, rel_error);
}

// Function for testing integral
void test_integral(int func, double a, double b, double eps, double r) {
    
    // Definition of function f
    afunc f;
    if (func == 1) {
        f = f1;
    } else if (func == 2) {
        f = f2;
    } else if (func == 3) {
        f = f3;
    } else if (func == 11) {
        f = f11;
    } else if (func == 22) {
        f = f22;
    } else {
        f = f33;
    }
    
    double result = integral(f, a, b, eps); // Our result
    double abs_error = fabs(r - result); // Absolute error
    double rel_error = abs_error / fabs(r); // Relative error
    
    printf("%.5lf %.5lf %.5lf\n", result, abs_error, rel_error);
}

// Function for calculating the difference of functions f1 and f2
double f1_minus_f2(double x) {
    return f1(x) - f2(x);
}
// Function for calculating the difference of functions f1 and f3
double f1_minus_f3(double x) {
    return f1(x) - f3(x);
}

// Function for displaying --help and -h
void print_help() {
    printf("You need to write: ./integral <option>\n");
    printf("List of options:\n");
    printf("-h, --help - Show all possible command line keys\n");
    printf("-r, --root - Show the abscissas of the intersection points of curves\n");
    printf("-i, --iterations - Show the number of iterations\n");
    printf("-R, --test-root F1:F2:A:B:E:R - Testing the root function\n");
    printf("-I, --test-integral F:A:B:E:R - Testing the integral function\n");
    printf("If there are no options, the program will output the area\n");
}

int main(int argc, char *argv[]) {
    int opt; // For the processed option
    int show_roots = 0; // Flag for displaying the intersection points
    int show_iterations = 0; // Flag for displaying the number of iterations
    
    // Array of structures for long options
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'}, {"root", no_argument, 0, 'r'},
        {"iterations", no_argument, 0, 'i'}, {"test-root", required_argument, 0, 'R'},
        {"test-integral", required_argument, 0, 'I'}, {0, 0, 0, 0}};
    // Loop for processing command line arguments
    while ((opt = getopt_long(argc, argv, "hriR:I:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'h': //--help or -h
                print_help();
                return 0;
            case 'r': //--root or -r
                show_roots = 1;
                break;
            case 'i': //--iterations or -i
                show_iterations = 1;
                break;
            case 'R': { //-R or --test-root
                int f1, f2;
                double a, b, eps, r;
                if (sscanf(optarg, "%d:%d:%lf:%lf:%lf:%lf", &f1, &f2, &a, &b, &eps, &r) != 6) {
                    fprintf(stderr, "Incorrect format for --test-root\n");
                    return 1;
                }
                test_root(f1, f2, a, b, eps, r);
                return 0;
            }
            case 'I': { //-I or --test-integral
                int f;
                double a, b, eps, r;
                if (sscanf(optarg, "%d:%lf:%lf:%lf:%lf", &f, &a, &b, &eps, &r) != 5) {
                    fprintf(stderr, "Incorrect format for --test-integral\n");
                    return 1;
                }
                test_integral(f, a, b, eps, r);
                return 0;
            }
            default: // Unknown option
                fprintf(stderr, "Unknown option\n");
                return 1;
        }
    }
    
    double eps1 = 0.0001;
    
    // Finding the intersection points of functions
    double x12 = root(f1, f2, 1.8, 2.2, eps1);
    double x23 = root(f2, f3, 0.0, 1.0, eps1);
    double x13 = root(f1, f3, -0.5, 0.0, eps1);
    // Flag --root or -r for displaying the intersection points
    if (show_roots) {
        printf("Intersection points:\n");
        printf("f1 и f2: %.5lf\n", x12);
        printf("f2 и f3: %.5lf\n", x23);
        printf("f1 и f3: %.5lf\n", x13);
        return 0;
    }
    // Flag --iterations or -i for displaying the number of iterations
    if (show_iterations) {
        printf("Iterations for finding the roots:\n");
        root(f1, f2, 1.8, 2.2, eps1);
        printf("f1 и f2: %d\n", root_iterations);
        root(f2, f3, 0.0, 1.0, eps1);
        printf("f2 и f3: %d\n", root_iterations);
        root(f1, f3, -0.5, 0.0, eps1);
        printf("f1 и f3: %d\n", root_iterations);
        return 0;
    }
    
    // Calculating the area
    double area = 0.0;

    // Integral (f1-f3) from x13 to x23
    area += integral((f1_minus_f3), x13, x23, eps1);
    // Integral (f1-f2) from x23 to x12
    area += integral((f1_minus_f2), x23, x12, eps1);
    
    printf("Area: %lf\n", area);
    
    return 0;
}