#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

// --- Model: Core Math Logic ---
class WaveEquationSolver {
public:
    struct Result {
        vector<vector<double>> grid; // grid[time][space]
    };

    // Solves u_tt = c^2 * u_xx
    static Result solve(double c, double h, double k, int time_steps, const vector<double>& initial_u, const vector<double>& initial_du_dt) {
        int n = initial_u.size();
        Result result;
        
        // grid[j][i] is u(x_i, t_j)
        vector<vector<double>> u(time_steps, vector<double>(n, 0.0));
        
        double lambda = (c * k) / h;
        double lambda2 = lambda * lambda;

        // Step 1: Initial condition (t = 0)
        for (int i = 0; i < n; ++i) {
            u[0][i] = initial_u[i];
        }

        // Keep boundary conditions constant for all time steps
        for (int j = 0; j < time_steps; ++j) {
            u[j][0] = initial_u[0];
            u[j][n - 1] = initial_u[n - 1];
        }

        if (time_steps > 1) {
            // Step 2: First time step using initial velocity condition (t = 1)
            for (int i = 1; i < n - 1; ++i) {
                u[1][i] = (1.0 - lambda2) * u[0][i] + 
                          0.5 * lambda2 * (u[0][i-1] + u[0][i+1]) + 
                          k * initial_du_dt[i];
            }
        }

        // Step 3: Subsequent time steps
        for (int j = 1; j < time_steps - 1; ++j) {
            for (int i = 1; i < n - 1; ++i) {
                u[j+1][i] = 2.0 * (1.0 - lambda2) * u[j][i] + 
                            lambda2 * (u[j][i-1] + u[j][i+1]) - 
                            u[j-1][i];
            }
        }

        result.grid = u;
        return result;
    }
};

// --- View/Controller: Terminal Interface ---
void runTerminalSimulation() {
    cout << "=== Wave Equation Simulator ===\n";
    cout << "Equation: u_tt = c^2 * u_xx\n";
    
    double c, h, k;
    cout << "Enter wave speed (c): ";
    if (!(cin >> c)) return;
    cout << "Enter space step size (h): ";
    cin >> h;
    cout << "Enter time step size (k): ";
    cin >> k;

    cout << "Lambda = " << (c * k) / h << "\n";
    if ((c * k) / h > 1.0) {
        cout << "Warning: Lambda > 1. The solution may be unstable!\n";
    }

    int space_nodes;
    cout << "Enter number of spatial points (including boundaries): ";
    cin >> space_nodes;
    if (space_nodes < 3) {
        cout << "Need at least 3 spatial points.\n";
        return;
    }

    int time_steps;
    cout << "Enter number of time steps to compute: ";
    cin >> time_steps;

    vector<double> initial_u(space_nodes);
    cout << "Enter the initial positions u(x, 0) at all " << space_nodes << " points (including boundaries):\n";
    for (int i = 0; i < space_nodes; ++i) {
        cin >> initial_u[i];
    }

    vector<double> initial_du_dt(space_nodes);
    cout << "Enter the initial velocities u_t(x, 0) at the interior points (" << space_nodes - 2 << " values):\n";
    initial_du_dt[0] = 0; // Boundary velocity
    for (int i = 1; i < space_nodes - 1; ++i) {
        cin >> initial_du_dt[i];
    }
    initial_du_dt[space_nodes - 1] = 0; // Boundary velocity

    auto result = WaveEquationSolver::solve(c, h, k, time_steps, initial_u, initial_du_dt);

    cout << "\n--- Final Result (Wave Grid) ---\n";
    cout << "Row = time step, Column = spatial point\n";
    cout << fixed << setprecision(4);
    for (int j = 0; j < time_steps; ++j) {
        cout << "t=" << setw(3) << j << ": ";
        for (int i = 0; i < space_nodes; ++i) {
            cout << setw(10) << result.grid[j][i] << " ";
        }
        cout << "\n";
    }
}

int main() {
    runTerminalSimulation();
    return 0;
}
