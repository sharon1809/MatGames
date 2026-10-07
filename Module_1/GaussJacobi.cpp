#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// --- Model: Core Math Logic ---
class GaussJacobiSolver {
public:
    struct Result {
        vector<double> solution;
        int iterations;
        bool converged;
        vector<vector<double>> iteration_history; // Useful for UI/Gamification
    };

    static Result solve(const vector<vector<double>>& A, const vector<double>& b, int max_iterations, double tolerance) {
        int n = A.size();
        vector<double> x(n, 0.0); // Initial guess
        vector<double> x_new(n, 0.0);
        Result result;
        result.converged = false;
        
        for (int iter = 0; iter < max_iterations; ++iter) {
            result.iteration_history.push_back(x);
            
            for (int i = 0; i < n; ++i) {
                double sum = 0.0;
                for (int j = 0; j < n; ++j) {
                    if (i != j) {
                        sum += A[i][j] * x[j];
                    }
                }
                x_new[i] = (b[i] - sum) / A[i][i];
            }

            // Check for convergence
            double max_diff = 0.0;
            for (int i = 0; i < n; ++i) {
                max_diff = max(max_diff, abs(x_new[i] - x[i]));
            }
            
            x = x_new;
            
            if (max_diff < tolerance) {
                result.converged = true;
                result.iterations = iter + 1;
                result.solution = x;
                result.iteration_history.push_back(x);
                return result;
            }
        }
        
        result.iterations = max_iterations;
        result.solution = x;
        return result;
    }
};

// --- View/Controller: Terminal Interface ---
void runTerminalSimulation() {
    cout << "=== Gauss-Jacobi Method Simulator ===\n";
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

    int max_iter;
    double tol;
    cout << "Enter max iterations: ";
    cin >> max_iter;
    cout << "Enter tolerance (e.g., 0.0001): ";
    cin >> tol;

    auto result = GaussJacobiSolver::solve(A, b, max_iter, tol);

    cout << "\n--- Iteration History ---\n";
    for (size_t i = 0; i < result.iteration_history.size(); ++i) {
        cout << "Iter " << setw(2) << i << ": ";
        for (double val : result.iteration_history[i]) {
            cout << setw(10) << val << " ";
        }
        cout << "\n";
    }

    cout << "\n--- Final Result ---\n";
    if (result.converged) {
        cout << "Converged in " << result.iterations << " iterations.\n";
    } else {
        cout << "Did not converge within " << max_iter << " iterations.\n";
    }

    cout << "Solution: \n";
    for (int i = 0; i < n; ++i) {
        cout << "x" << i + 1 << " = " << result.solution[i] << "\n";
    }
}

int main() {
    runTerminalSimulation();
    return 0;
}
