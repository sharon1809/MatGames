#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// --- Model: Core Math Logic ---
class LagrangeInterpolationSolver {
public:
    struct Result {
        double target_x;
        double interpolated_y;
        vector<double> l_terms; // The L_i(x) terms for each point
    };

    static Result interpolate(const vector<double>& x, const vector<double>& y, double target_x) {
        int n = x.size();
        Result result;
        result.target_x = target_x;
        result.interpolated_y = 0.0;
        
        for (int i = 0; i < n; ++i) {
            double l_i = 1.0;
            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    l_i *= (target_x - x[j]) / (x[i] - x[j]);
                }
            }
            result.l_terms.push_back(l_i);
            result.interpolated_y += y[i] * l_i;
        }

        return result;
    }
};

// --- View/Controller: Terminal Interface ---
void runTerminalSimulation() {
    cout << "=== Lagrange's Interpolation Simulator ===\n";
    cout << "Enter the number of known data points: ";
    int n;
    if (!(cin >> n) || n < 2) {
        cout << "Need at least 2 points.\n";
        return;
    }

    vector<double> x(n), y(n);
    cout << "Enter the known points (x y):\n";
    for (int i = 0; i < n; ++i) {
        cin >> x[i] >> y[i];
    }

    cout << "Enter the x value you want to interpolate: ";
    double target_x;
    cin >> target_x;

    auto result = LagrangeInterpolationSolver::interpolate(x, y, target_x);

    cout << "\n--- Calculation Steps (L terms) ---\n";
    cout << fixed << setprecision(4);
    for (size_t i = 0; i < result.l_terms.size(); ++i) {
        cout << "L_" << i << "(" << target_x << ") = " << result.l_terms[i] 
             << "  -> y_" << i << " * L_" << i << " = " << y[i] * result.l_terms[i] << "\n";
    }

    cout << "\n--- Final Result ---\n";
    cout << "Interpolated value at x = " << target_x << " is y = " << result.interpolated_y << "\n";
}

int main() {
    runTerminalSimulation();
    return 0;
}
