#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// ---------------------------------------------------------
// EDIT THIS FUNCTION FOR YOUR SPECIFIC PROBLEM
// Example: dy/dx = x + y
double f(double x, double y) {
    return x + y; 
}
// ---------------------------------------------------------

// --- Model: Core Math Logic ---
class AdamBashforthSolver {
public:
    struct Step {
        double x, y;
        bool is_predicted_and_corrected; // true for AB steps, false for RK4 initial steps
    };
    struct Result {
        vector<Step> history;
    };

    // Helper: RK4 step to generate initial values
    static double rk4_step(double x, double y, double h, double (*func)(double, double)) {
        double k1 = h * func(x, y);
        double k2 = h * func(x + h / 2.0, y + k1 / 2.0);
        double k3 = h * func(x + h / 2.0, y + k2 / 2.0);
        double k4 = h * func(x + h, y + k3);
        return y + (k1 + 2 * k2 + 2 * k3 + k4) / 6.0;
    }

    static Result solve(double x0, double y0, double target_x, double h, double (*func)(double, double)) {
        Result result;
        vector<double> x_vals, y_vals, f_vals;

        // 1. Generate the first 4 points (index 0, 1, 2, 3) using RK4
        x_vals.push_back(x0);
        y_vals.push_back(y0);
        f_vals.push_back(func(x0, y0));
        result.history.push_back({x0, y0, false});

        for (int i = 1; i <= 3; ++i) {
            double next_x = x_vals[i-1] + h;
            double next_y = rk4_step(x_vals[i-1], y_vals[i-1], h, func);
            
            x_vals.push_back(next_x);
            y_vals.push_back(next_y);
            f_vals.push_back(func(next_x, next_y));
            result.history.push_back({next_x, next_y, false});
        }

        // 2. Use Adam-Bashforth Predictor-Corrector for the rest
        int n = 3;
        while (x_vals[n] < target_x - 1e-9) {
            double next_x = x_vals[n] + h;
            
            // Predictor (Adam-Bashforth 4-step)
            double y_pred = y_vals[n] + (h / 24.0) * (55.0 * f_vals[n] - 59.0 * f_vals[n-1] + 37.0 * f_vals[n-2] - 9.0 * f_vals[n-3]);
            double f_pred = func(next_x, y_pred);

            // Corrector (Adam-Moulton 4-step)
            double y_corr = y_vals[n] + (h / 24.0) * (9.0 * f_pred + 19.0 * f_vals[n] - 5.0 * f_vals[n-1] + f_vals[n-2]);
            double f_corr = func(next_x, y_corr);

            x_vals.push_back(next_x);
            y_vals.push_back(y_corr);
            f_vals.push_back(f_corr);
            result.history.push_back({next_x, y_corr, true});

            n++;
        }
        
        return result;
    }
};

// --- View/Controller: Terminal Interface ---
void runTerminalSimulation() {
    cout << "=== Adam-Bashforth Predictor-Corrector Simulator ===\n";
    cout << "NOTE: The function f(x, y) = dy/dx is defined in the source code.\n";
    cout << "Please edit the 'double f(double x, double y)' function at the top of the file to match your coursework problem before compiling.\n";
    cout << "This simulator automatically uses RK4 to generate the 3 starting points required for Adam-Bashforth!\n\n";

    double x0, y0, target_x, h;
    cout << "Enter initial x (x0): ";
    if (!(cin >> x0)) return;
    cout << "Enter initial y (y0): ";
    cin >> y0;
    cout << "Enter target x to evaluate: ";
    cin >> target_x;
    cout << "Enter step size (h): ";
    cin >> h;

    auto result = AdamBashforthSolver::solve(x0, y0, target_x, h, f);

    cout << "\n--- Calculation Steps ---\n";
    cout << fixed << setprecision(6);
    cout << setw(10) << "x" << setw(15) << "y" << setw(20) << "Method Used" << "\n";
    
    for (const auto& step : result.history) {
        cout << setw(10) << step.x << setw(15) << step.y 
             << setw(20) << (step.is_predicted_and_corrected ? "Adam-Bashforth" : "RK4 (Initialization)") << "\n";
    }

    cout << "\n--- Final Result ---\n";
    cout << "y(" << target_x << ") is approximately " << result.history.back().y << "\n";
}

int main() {
    runTerminalSimulation();
    return 0;
}
