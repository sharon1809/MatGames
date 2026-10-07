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
class EulerSolver {
public:
    struct Step {
        double x, y;
    };
    struct Result {
        vector<Step> history;
    };

    static Result solve(double x0, double y0, double target_x, double h, double (*func)(double, double)) {
        Result result;
        double x = x0;
        double y = y0;
        result.history.push_back({x, y});

        // We use a small epsilon for floating point comparison
        while (x < target_x - 1e-9) {
            y = y + h * func(x, y);
            x = x + h;
            result.history.push_back({x, y});
        }
        
        return result;
    }
};

// --- View/Controller: Terminal Interface ---
void runTerminalSimulation() {
    cout << "=== Euler's Method Simulator ===\n";
    cout << "NOTE: The function f(x, y) = dy/dx is defined in the source code.\n";
    cout << "Please edit the 'double f(double x, double y)' function at the top of the file to match your coursework problem before compiling.\n\n";

    double x0, y0, target_x, h;
    cout << "Enter initial x (x0): ";
    if (!(cin >> x0)) return;
    cout << "Enter initial y (y0): ";
    cin >> y0;
    cout << "Enter target x to evaluate: ";
    cin >> target_x;
    cout << "Enter step size (h): ";
    cin >> h;

    auto result = EulerSolver::solve(x0, y0, target_x, h, f);

    cout << "\n--- Calculation Steps ---\n";
    cout << fixed << setprecision(6);
    cout << setw(10) << "x" << setw(15) << "y" << "\n";
    for (const auto& step : result.history) {
        cout << setw(10) << step.x << setw(15) << step.y << "\n";
    }

    cout << "\n--- Final Result ---\n";
    cout << "y(" << target_x << ") is approximately " << result.history.back().y << "\n";
}

int main() {
    runTerminalSimulation();
    return 0;
}
