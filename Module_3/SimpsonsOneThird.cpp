#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// --- Model: Core Math Logic ---
class SimpsonsOneThirdSolver {
public:
    struct Result {
        double integral;
        bool success;
        string message;
    };

    static Result solve(const vector<double>& y, double h) {
        Result result;
        int n = y.size() - 1; // number of intervals

        if (n % 2 != 0) {
            result.success = false;
            result.message = "Number of intervals (n) must be even for Simpson's 1/3 rule.";
            result.integral = 0;
            return result;
        }

        double sum_ends = y[0] + y[n];
        double sum_odd = 0;
        double sum_even = 0;

        for (int i = 1; i < n; i += 2) {
            sum_odd += y[i];
        }

        for (int i = 2; i < n; i += 2) {
            sum_even += y[i];
        }

        result.integral = (h / 3.0) * (sum_ends + 4 * sum_odd + 2 * sum_even);
        result.success = true;
        return result;
    }
};

// --- View/Controller: Terminal Interface ---
void runTerminalSimulation() {
    cout << "=== Simpson's 1/3 Rule Simulator ===\n";
    cout << "Enter the number of intervals (must be even): ";
    int n;
    if (!(cin >> n)) return;

    if (n % 2 != 0) {
        cout << "Error: Number of intervals must be even!\n";
        return;
    }

    int num_points = n + 1;
    vector<double> y(num_points);
    
    cout << "Enter the step size (h): ";
    double h;
    cin >> h;

    cout << "Enter the " << num_points << " y values (y_0 to y_n):\n";
    for (int i = 0; i < num_points; ++i) {
        cin >> y[i];
    }

    auto result = SimpsonsOneThirdSolver::solve(y, h);

    cout << "\n--- Final Result ---\n";
    if (result.success) {
        cout << fixed << setprecision(4);
        cout << "The approximate integral is: " << result.integral << "\n";
    } else {
        cout << "Calculation failed: " << result.message << "\n";
    }
}

int main() {
    runTerminalSimulation();
    return 0;
}
