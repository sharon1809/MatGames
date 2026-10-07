#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// --- Model: Core Math Logic ---
class GaussJordanSolver {
public:
    struct Result {
        vector<double> solution;
        bool success;
        vector<vector<vector<double>>> step_history; // Store augmented matrix after each pivot for UI
    };

    static Result solve(vector<vector<double>> A, vector<double> b) {
        int n = A.size();
        Result result;
        result.success = true;

        // Create augmented matrix [A | b]
        vector<vector<double>> aug(n, vector<double>(n + 1));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                aug[i][j] = A[i][j];
            }
            aug[i][n] = b[i];
        }
        
        result.step_history.push_back(aug);

        for (int i = 0; i < n; ++i) {
            // Partial pivoting
            int max_row = i;
            for (int k = i + 1; k < n; ++k) {
                if (abs(aug[k][i]) > abs(aug[max_row][i])) {
                    max_row = k;
                }
            }

            // Swap rows
            if (max_row != i) {
                swap(aug[i], aug[max_row]);
                result.step_history.push_back(aug);
            }

            // Check for singular matrix
            if (abs(aug[i][i]) < 1e-9) {
                result.success = false;
                return result;
            }

            // Make the diagonal element 1
            double pivot = aug[i][i];
            for (int j = 0; j <= n; ++j) {
                aug[i][j] /= pivot;
            }

            // Make all other elements in current column 0
            for (int k = 0; k < n; ++k) {
                if (k != i) {
                    double factor = aug[k][i];
                    for (int j = 0; j <= n; ++j) {
                        aug[k][j] -= factor * aug[i][j];
                    }
                }
            }
            result.step_history.push_back(aug);
        }

        // Extract solution
        result.solution.resize(n);
        for (int i = 0; i < n; ++i) {
            result.solution[i] = aug[i][n];
        }

        return result;
    }
};

// --- View/Controller: Terminal Interface ---
void printMatrix(const vector<vector<double>>& matrix) {
    for (const auto& row : matrix) {
        for (double val : row) {
            cout << setw(10) << val << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void runTerminalSimulation() {
    cout << "=== Gauss-Jordan Elimination Simulator ===\n";
    cout << "Enter the number of variables: ";
    int n;
    if (!(cin >> n)) return;

    vector<vector<double>> A(n, vector<double>(n));
    vector<double> b(n);

    cout << "Enter the augmented matrix coefficients (A | b):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> A[i][j];
        }
        cin >> b[i];
    }

    auto result = GaussJordanSolver::solve(A, b);

    cout << "\n--- Elimination Steps ---\n";
    for (size_t i = 0; i < result.step_history.size(); ++i) {
        cout << "Step " << i << ":\n";
        printMatrix(result.step_history[i]);
    }

    cout << "--- Final Result ---\n";
    if (!result.success) {
        cout << "The system has no unique solution (singular matrix).\n";
    } else {
        cout << "Solution: \n";
        for (int i = 0; i < n; ++i) {
            cout << "x" << i + 1 << " = " << result.solution[i] << "\n";
        }
    }
}

int main() {
    runTerminalSimulation();
    return 0;
}
