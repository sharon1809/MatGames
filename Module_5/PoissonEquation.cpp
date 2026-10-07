#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

// --- Model: Core Math Logic ---
class PoissonSolver {
public:
    struct Result {
        vector<vector<double>> grid;
        int iterations;
        bool converged;
    };

    static Result solve(vector<vector<double>> grid, const vector<vector<double>>& f_vals, double h, int max_iter, double tol) {
        int rows = grid.size();
        int cols = grid[0].size();
        Result result;
        result.converged = false;
        double h2 = h * h;

        for (int iter = 0; iter < max_iter; ++iter) {
            double max_diff = 0.0;

            for (int i = 1; i < rows - 1; ++i) {
                for (int j = 1; j < cols - 1; ++j) {
                    double old_val = grid[i][j];
                    // Formula: u(i,j) = (u(i-1,j) + u(i+1,j) + u(i,j-1) + u(i,j+1) - h^2 * f(x_i, y_j)) / 4
                    grid[i][j] = 0.25 * (grid[i-1][j] + grid[i+1][j] + grid[i][j-1] + grid[i][j+1] - h2 * f_vals[i][j]);
                    max_diff = max(max_diff, abs(grid[i][j] - old_val));
                }
            }

            if (max_diff < tol) {
                result.converged = true;
                result.iterations = iter + 1;
                result.grid = grid;
                return result;
            }
        }

        result.iterations = max_iter;
        result.grid = grid;
        return result;
    }
};

// --- View/Controller: Terminal Interface ---
void runTerminalSimulation() {
    cout << "=== Poisson Equation Simulator ===\n";
    cout << "Equation: u_xx + u_yy = f(x,y)\n";
    
    int rows, cols;
    cout << "Enter the grid size (rows cols): ";
    if (!(cin >> rows >> cols) || rows < 3 || cols < 3) {
        cout << "Grid must be at least 3x3 to have interior points.\n";
        return;
    }

    double h;
    cout << "Enter step size (h): ";
    cin >> h;

    vector<vector<double>> grid(rows, vector<double>(cols, 0.0));
    vector<vector<double>> f_vals(rows, vector<double>(cols, 0.0));

    cout << "Enter boundary conditions for u:\n";
    cout << "Top row (" << cols << " values): ";
    for (int j = 0; j < cols; ++j) cin >> grid[0][j];
    
    cout << "Bottom row (" << cols << " values): ";
    for (int j = 0; j < cols; ++j) cin >> grid[rows - 1][j];
    
    cout << "Left column (excluding corners, " << rows - 2 << " values): ";
    for (int i = 1; i < rows - 1; ++i) cin >> grid[i][0];
    
    cout << "Right column (excluding corners, " << rows - 2 << " values): ";
    for (int i = 1; i < rows - 1; ++i) cin >> grid[i][cols - 1];

    cout << "Enter initial guess for interior points of u (e.g., 0): ";
    double guess;
    cin >> guess;
    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            grid[i][j] = guess;
        }
    }

    cout << "Enter the values for f(x,y) at the interior grid points.\n";
    for (int i = 1; i < rows - 1; ++i) {
        cout << "Row " << i << " (" << cols - 2 << " interior values): ";
        for (int j = 1; j < cols - 1; ++j) {
            cin >> f_vals[i][j];
        }
    }

    int max_iter;
    double tol;
    cout << "Enter max iterations: ";
    cin >> max_iter;
    cout << "Enter tolerance (e.g., 0.0001): ";
    cin >> tol;

    auto result = PoissonSolver::solve(grid, f_vals, h, max_iter, tol);

    cout << "\n--- Final Result ---\n";
    if (result.converged) {
        cout << "Converged in " << result.iterations << " iterations.\n";
    } else {
        cout << "Did not converge within " << max_iter << " iterations.\n";
    }

    cout << "Final Grid:\n";
    cout << fixed << setprecision(4);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << setw(10) << result.grid[i][j] << " ";
        }
        cout << "\n";
    }
}

int main() {
    runTerminalSimulation();
    return 0;
}
