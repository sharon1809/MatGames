#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// --- Model: Core Math Logic ---
class NewtonsBackwardSolver {
public:
    struct Result {
        double target_x;
        double interpolated_y;
        double p;
        vector<vector<double>> diff_table; 
    };

    static Result solve(const vector<double>& x, const vector<double>& y, double target_x) {
        int n = x.size();
        Result result;
        result.target_x = target_x;
        
        // Build backward difference table
        vector<vector<double>> table(n, vector<double>(n, 0.0));
        for (int i = 0; i < n; ++i) {
            table[i][0] = y[i];
        }

        for (int j = 1; j < n; ++j) {
            for (int i = n - 1; i >= j; --i) {
                table[i][j] = table[i][j-1] - table[i-1][j-1];
            }
        }
        
        result.diff_table = table;

        // Calculate p
        double h = x[1] - x[0];
        double p = (target_x - x[n-1]) / h;
        result.p = p;

        // Apply formula: y = y_n + p * nabla y_n + p(p+1)/2! * nabla^2 y_n + ...
        double sum = table[n-1][0];
        double p_term = p;
        double fact = 1;

        for (int j = 1; j < n; ++j) {
            sum += (p_term * table[n-1][j]) / fact;
            p_term *= (p + j);
            fact *= (j + 1);
        }

        result.interpolated_y = sum;
        return result;
    }
};

// --- View/Controller: Terminal Interface ---
void runTerminalSimulation() {
    cout << "=== Newton's Backward Interpolation Simulator ===\n";
    cout << "Enter the number of data points: ";
    int n;
    if (!(cin >> n) || n < 2) {
        cout << "Need at least 2 points.\n";
        return;
    }

    vector<double> x(n), y(n);
    cout << "Enter the points (x y), making sure x values are equally spaced:\n";
    for (int i = 0; i < n; ++i) {
        cin >> x[i] >> y[i];
    }

    cout << "Enter the x value you want to interpolate (usually near the end): ";
    double target_x;
    cin >> target_x;

    auto result = NewtonsBackwardSolver::solve(x, y, target_x);

    cout << "\n--- Backward Difference Table ---\n";
    cout << fixed << setprecision(4);
    for (int i = 0; i < n; ++i) {
        cout << setw(10) << x[i];
        for (int j = 0; j <= i; ++j) {
            cout << setw(12) << result.diff_table[i][j];
        }
        cout << "\n";
    }

    cout << "\n--- Final Result ---\n";
    cout << "Calculated p = " << result.p << "\n";
    cout << "Interpolated value at x = " << target_x << " is y = " << result.interpolated_y << "\n";
}

int main() {
    runTerminalSimulation();
    return 0;
}
