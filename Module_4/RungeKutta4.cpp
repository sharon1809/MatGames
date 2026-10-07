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
class RK4Solver {
public:
    struct Step {
        double x, y, k1, k2, k3, k4;
    };
    struct Result {
        vector<Step> history;
    };

    static Result solve(double x0, double y0, double target_x, double h, double (*func)(double, double)) {
        Result result;
        double x = x0;
        double y = y0;
        
        // Initial state doesn't have k values computed from a previous step
        result.history.push_back({x, y, 0, 0, 0, 0});

        while (x < target_x - 1e-9) {
            double k1 = h * func(x, y);
            double k2 = h * func(x + h / 2.0, y + k1 / 2.0);
            double k3 = h * func(x + h / 2.0, y + k2 / 2.0);
            double k4 = h * func(x + h, y + k3);

            y = y + (k1 + 2 * k2 + 2 * k3 + k4) / 6.0;
            x = x + h;

            result.history.push_back({x, y, k1, k2, k3, k4});
        }
        
        return result;
    }
};

// --- View/Controller: Terminal Interface ---
void runTerminalSimulation() {
    cout << "=== Runge-Kutta 4th Order (RK4) Simulator ===\n";
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

    auto result = RK4Solver::solve(x0, y0, target_x, h, f);

    cout << "\n--- Calculation Steps ---\n";
    cout << fixed << setprecision(6);
    cout << setw(10) << "x" << setw(15) << "y" << setw(12) << "k1" << setw(12) << "k2" << setw(12) << "k3" << setw(12) << "k4" << "\n";
    
    for (size_t i = 1; i < result.history.size(); ++i) { // Skip i=0 as it's the starting point
        const auto& step = result.history[i];
        cout << setw(10) << step.x << setw(15) << step.y 
             << setw(12) << step.k1 << setw(12) << step.k2 
             << setw(12) << step.k3 << setw(12) << step.k4 << "\n";
    }

    cout << "\n--- Final Result ---\n";
    cout << "y(" << target_x << ") is approximately " << result.history.back().y << "\n";
}

int main() {
    runTerminalSimulation();
    return 0;
}
