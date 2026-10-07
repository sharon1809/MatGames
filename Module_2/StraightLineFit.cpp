#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// --- Model: Core Math Logic ---
class StraightLineFitSolver {
public:
    struct Result {
        double a; // Intercept
        double b; // Slope
        vector<double> predicted_y;
    };

    static Result solve(const vector<double>& x, const vector<double>& y) {
        int n = x.size();
        double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;

        for (int i = 0; i < n; ++i) {
            sum_x += x[i];
            sum_y += y[i];
            sum_xy += x[i] * y[i];
            sum_x2 += x[i] * x[i];
        }

        Result result;
        // Solving normal equations:
        // n*a + sum_x*b = sum_y
        // sum_x*a + sum_x2*b = sum_xy
        
        double denominator = (n * sum_x2 - sum_x * sum_x);
        if (denominator == 0) {
            // Cannot fit (e.g. all x are the same)
            result.a = 0;
            result.b = 0;
            return result;
        }

        result.a = (sum_y * sum_x2 - sum_x * sum_xy) / denominator;
        result.b = (n * sum_xy - sum_x * sum_y) / denominator;

        for (int i = 0; i < n; ++i) {
            result.predicted_y.push_back(result.a + result.b * x[i]);
        }

        return result;
    }
};

// --- View/Controller: Terminal Interface ---
void runTerminalSimulation() {
    cout << "=== Straight Line Fitting (y = a + bx) Simulator ===\n";
    cout << "Enter the number of data points: ";
    int n;
    if (!(cin >> n) || n < 2) {
        cout << "Need at least 2 points.\n";
        return;
    }

    vector<double> x(n), y(n);
    cout << "Enter the points (x y):\n";
    for (int i = 0; i < n; ++i) {
        cin >> x[i] >> y[i];
    }

    auto result = StraightLineFitSolver::solve(x, y);

    cout << "\n--- Final Result ---\n";
    cout << fixed << setprecision(4);
    cout << "Equation of the line: y = " << result.a << " + " << result.b << "x\n\n";

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
