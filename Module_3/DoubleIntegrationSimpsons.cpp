#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// --- Model: Core Math Logic ---
class DoubleSimpsonsSolver {
public:
    struct Result {
        double integral;
        bool success;
        string message;
        vector<double> inner_integrals; // Integrals along the x-axis for each y
    };

    static Result solve(const vector<vector<double>>& z, double hx, double hy) {
        Result result;
        int m = z.size() - 1;    // number of intervals along y
        if (m <= 0) return {0, false, "Invalid grid", {}};
        int n = z[0].size() - 1; // number of intervals along x

        if (n % 2 != 0 || m % 2 != 0) {
            result.success = false;
            result.message = "Number of intervals in both x and y directions must be even for Simpson's 1/3 rule.";
            result.integral = 0;
            return result;
        }

        result.success = true;

        // Helper function for 1D Simpson's 1/3
        auto simpsons1D = [](const vector<double>& vals, double h) {
            int len = vals.size() - 1;
            double sum = vals[0] + vals[len];
            for (int i = 1; i < len; i += 2) sum += 4 * vals[i];
            for (int i = 2; i < len; i += 2) sum += 2 * vals[i];
            return (h / 3.0) * sum;
        };

        // 1. Integrate along x for each row (y)
        for (int i = 0; i <= m; ++i) {
            double row_integral = simpsons1D(z[i], hx);
            result.inner_integrals.push_back(row_integral);
        }

        // 2. Integrate the resulting values along y
        result.integral = simpsons1D(result.inner_integrals, hy);

        return result;
    }
};

// --- View/Controller: Terminal Interface ---
void runTerminalSimulation() {
    cout << "=== Double Integration (Simpson's 1/3 Rule) Simulator ===\n";
    
    int n, m;
    cout << "Enter the number of intervals for x (n, must be even): ";
    if (!(cin >> n)) return;
    cout << "Enter the number of intervals for y (m, must be even): ";
    if (!(cin >> m)) return;

    if (n % 2 != 0 || m % 2 != 0) {
        cout << "Error: Both n and m must be even!\n";
        return;
    }

    double hx, hy;
    cout << "Enter step size for x (hx): ";
    cin >> hx;
    cout << "Enter step size for y (hy): ";
    cin >> hy;

    int rows = m + 1;
    int cols = n + 1;
    vector<vector<double>> z(rows, vector<double>(cols));

    cout << "Enter the grid of function values z = f(x,y):\n";
    cout << "Enter row by row (each row corresponds to a specific y value, going through all x values)\n";
    for (int i = 0; i < rows; ++i) {
        cout << "Row " << i << " (" << cols << " values): ";
        for (int j = 0; j < cols; ++j) {
            cin >> z[i][j];
        }
    }

    auto result = DoubleSimpsonsSolver::solve(z, hx, hy);

    if (result.success) {
        cout << "\n--- Calculation Steps ---\n";
        cout << fixed << setprecision(4);
        cout << "Integrals along x for each y:\n";
        for (size_t i = 0; i < result.inner_integrals.size(); ++i) {
            cout << "I_x(y_" << i << ") = " << result.inner_integrals[i] << "\n";
        }

        cout << "\n--- Final Result ---\n";
        cout << "The approximate double integral is: " << result.integral << "\n";
    } else {
        cout << "\nError: " << result.message << "\n";
    }
}

int main() {
    runTerminalSimulation();
    return 0;
}
