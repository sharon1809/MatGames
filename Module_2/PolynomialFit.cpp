#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

// --- Model: Core Math Logic ---
class PolynomialFitSolver {
public:
    struct Result {
        double a, b, c; // y = a + bx + cx^2
        vector<double> predicted_y;
        bool success;
    };

    static Result solve(const vector<double>& x, const vector<double>& y) {
        int n = x.size();
        Result result;
        result.success = true;

        if (n < 3) {
            result.success = false;
            return result;
        }

        double sum_x = 0, sum_x2 = 0, sum_x3 = 0, sum_x4 = 0;
        double sum_y = 0, sum_xy = 0, sum_x2y = 0;

        for (int i = 0; i < n; ++i) {
            double x2 = x[i] * x[i];
            sum_x += x[i];
            sum_x2 += x2;
            sum_x3 += x2 * x[i];
            sum_x4 += x2 * x2;
            sum_y += y[i];
            sum_xy += x[i] * y[i];
            sum_x2y += x2 * y[i];
        }

        // Augmented matrix for normal equations:
        // [n      sum_x  sum_x2 | sum_y  ]
        // [sum_x  sum_x2 sum_x3 | sum_xy ]
        // [sum_x2 sum_x3 sum_x4 | sum_x2y]
        vector<vector<double>> matrix = {
            {(double)n, sum_x, sum_x2, sum_y},
            {sum_x, sum_x2, sum_x3, sum_xy},
            {sum_x2, sum_x3, sum_x4, sum_x2y}
        };

        // Gaussian elimination to solve the 3x3 system
        for (int i = 0; i < 3; ++i) {
            // Pivoting
            int max_row = i;
            for (int k = i + 1; k < 3; ++k) {
                if (abs(matrix[k][i]) > abs(matrix[max_row][i])) {
                    max_row = k;
                }
            }
            swap(matrix[i], matrix[max_row]);

            if (abs(matrix[i][i]) < 1e-9) {
                result.success = false; // Singular
                return result;
            }

            double pivot = matrix[i][i];
            for (int j = i; j <= 3; ++j) {
                matrix[i][j] /= pivot;
            }

            for (int k = 0; k < 3; ++k) {
                if (k != i) {
                    double factor = matrix[k][i];
                    for (int j = i; j <= 3; ++j) {
                        matrix[k][j] -= factor * matrix[i][j];
                    }
                }
            }
        }

        result.a = matrix[0][3];
        result.b = matrix[1][3];
        result.c = matrix[2][3];

        for (int i = 0; i < n; ++i) {
            result.predicted_y.push_back(result.a + result.b * x[i] + result.c * x[i] * x[i]);
        }

        return result;
    }
};

// --- View/Controller: Terminal Interface ---
void runTerminalSimulation() {
    cout << "=== Second Degree Polynomial Fitting (y = a + bx + cx^2) Simulator ===\n";
    cout << "Enter the number of data points: ";
    int n;
    if (!(cin >> n) || n < 3) {
        cout << "Need at least 3 points.\n";
        return;
    }

    vector<double> x(n), y(n);
    cout << "Enter the points (x y):\n";
    for (int i = 0; i < n; ++i) {
        cin >> x[i] >> y[i];
    }

    auto result = PolynomialFitSolver::solve(x, y);

    cout << "\n--- Final Result ---\n";
    if (!result.success) {
        cout << "Failed to fit the polynomial (singular matrix).\n";
        return;
    }

    cout << fixed << setprecision(4);
    cout << "Equation: y = " << result.a << " + " << result.b << "x + " << result.c << "x^2\n\n";

    cout << "--- Data vs Prediction ---\n";
    cout << setw(10) << "x" << setw(15) << "y (actual)" << setw(15) << "y (predicted)" << "\n";
    for (int i = 0; i < n; ++i) {
        cout << setw(10) << x[i] << setw(15) << y[i] << setw(15) << result.predicted_y[i] << "\n";
    }
}

int main() {
    runTerminalSimulation();
    return 0;
}
