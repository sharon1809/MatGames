// --- Data Structure for Topics ---
const topics = [
    { id: 'm1_jacobi', module: 'Module 1', name: 'Gauss-Jacobi Method', type: 'system_solver' },
    { id: 'm1_seidel', module: 'Module 1', name: 'Gauss-Seidel Method', type: 'system_solver' },
    { id: 'm1_jordan', module: 'Module 1', name: 'Gauss-Jordan Method', type: 'direct_solver' }
];

let currentTopic = null;
let currentChart = null;
let simulationSteps = [];
let currentStepIndex = 0;

// --- Initialization ---
document.addEventListener('DOMContentLoaded', () => {
    buildSidebar();
    setupEventListeners();
});

function buildSidebar() {
    const nav = document.getElementById('module-nav');
    let currentModule = '';
    
    topics.forEach(t => {
        if (t.module !== currentModule) {
            const modTitle = document.createElement('div');
            modTitle.className = 'nav-module-title';
            modTitle.textContent = t.module;
            modTitle.style.marginTop = currentModule === '' ? '0' : '1.5rem';
            nav.appendChild(modTitle);
            currentModule = t.module;
        }
        
        const el = document.createElement('div');
        el.className = 'nav-topic';
        el.textContent = t.name;
        el.dataset.id = t.id;
        el.onclick = () => selectTopic(t.id);
        nav.appendChild(el);
    });
}

function selectTopic(id) {
    document.querySelectorAll('.nav-topic').forEach(el => el.classList.remove('active'));
    document.querySelector(`.nav-topic[data-id="${id}"]`).classList.add('active');
    
    currentTopic = topics.find(t => t.id === id);
    document.getElementById('current-topic-title').textContent = currentTopic.name;
    document.getElementById('input-panel').style.display = 'flex';
    document.getElementById('viz-panel').style.display = 'none';
    
    buildInputs(currentTopic.type);
}

function buildInputs(type) {
    const container = document.getElementById('dynamic-inputs');
    container.innerHTML = '';
    
    if (type === 'system_solver' || type === 'direct_solver') {
        container.innerHTML = `
            <div class="input-group">
                <label>Number of Variables (n)</label>
                <input type="number" id="var-count" value="3" min="2" max="5">
            </div>
            <div class="input-group" id="matrix-container">
                <label>Augmented Matrix (A | b)</label>
                <div id="matrix-inputs"></div>
            </div>
        `;
        
        if (type === 'system_solver') {
            container.innerHTML += `
                <div class="input-group">
                    <label>Max Iterations</label>
                    <input type="number" id="max-iter" value="50">
                </div>
                <div class="input-group">
                    <label>Tolerance</label>
                    <input type="number" id="tol" value="0.0001" step="0.0001">
                </div>
            `;
        }
        
        const varInput = document.getElementById('var-count');
        varInput.onchange = generateMatrixInputs;
        generateMatrixInputs();
    }
}

function generateMatrixInputs() {
    const n = parseInt(document.getElementById('var-count').value);
    const container = document.getElementById('matrix-inputs');
    container.innerHTML = '';
    
    // Default diagonal dominant matrix for good convergence
    const defaults = [
        [4, 1, 1, 6],
        [1, 5, 2, 8],
        [1, 2, 6, 9]
    ];

    for (let i = 0; i < n; i++) {
        const row = document.createElement('div');
        row.className = 'matrix-input-row fade-in';
        for (let j = 0; j <= n; j++) {
            const input = document.createElement('input');
            input.type = 'number';
            input.id = `mat-${i}-${j}`;
            input.value = (i < defaults.length && j <= defaults[0].length-1) ? defaults[i][j] : (i === j ? 4 : 1);
            row.appendChild(input);
        }
        container.appendChild(row);
    }
}

function setupEventListeners() {
    document.getElementById('solve-btn').onclick = runSimulation;
    document.getElementById('prev-step-btn').onclick = () => showStep(currentStepIndex - 1);
    document.getElementById('next-step-btn').onclick = () => showStep(currentStepIndex + 1);
}

// --- Math Logic & Simulation ---

function runSimulation() {
    document.getElementById('viz-panel').style.display = 'flex';
    document.getElementById('viz-panel').classList.add('fade-in');
    
    if (currentTopic.id === 'm1_jacobi') simulateGaussJacobi();
    else if (currentTopic.id === 'm1_seidel') simulateGaussSeidel();
    else if (currentTopic.id === 'm1_jordan') simulateGaussJordan();
}

function simulateGaussJacobi() {
    const { A, b, n, maxIter, tol } = getMatrixInputs();
    let x = new Array(n).fill(0);
    simulationSteps = [{ step: 0, x: [...x], desc: "Initial Guess" }];
    
    for (let iter = 1; iter <= maxIter; iter++) {
        let x_new = new Array(n).fill(0);
        let max_diff = 0;
        
        for (let i = 0; i < n; i++) {
            let sum = 0;
            for (let j = 0; j < n; j++) {
                if (i !== j) sum += A[i][j] * x[j];
            }
            x_new[i] = (b[i] - sum) / A[i][i];
            max_diff = Math.max(max_diff, Math.abs(x_new[i] - x[i]));
        }
        
        x = [...x_new];
        simulationSteps.push({ step: iter, x: [...x], diff: max_diff, desc: `Iteration ${iter} | Max Diff = ${max_diff.toFixed(6)}` });
        
        if (max_diff < tol) break;
    }
    
    setupVisualization(true);
}

function simulateGaussSeidel() {
    const { A, b, n, maxIter, tol } = getMatrixInputs();
    let x = new Array(n).fill(0);
    simulationSteps = [{ step: 0, x: [...x], desc: "Initial Guess" }];
    
    for (let iter = 1; iter <= maxIter; iter++) {
        let max_diff = 0;
        let x_old = [...x];
        
        for (let i = 0; i < n; i++) {
            let sum = 0;
            for (let j = 0; j < n; j++) {
                if (i !== j) sum += A[i][j] * x[j]; // uses updated x directly
            }
            let x_new = (b[i] - sum) / A[i][i];
            max_diff = Math.max(max_diff, Math.abs(x_new - x_old[i]));
            x[i] = x_new;
        }
        
        simulationSteps.push({ step: iter, x: [...x], diff: max_diff, desc: `Iteration ${iter} | Max Diff = ${max_diff.toFixed(6)}` });
        if (max_diff < tol) break;
    }
    
    setupVisualization(true);
}

function simulateGaussJordan() {
    const { A, b, n } = getMatrixInputs();
    let aug = A.map((row, i) => [...row, b[i]]);
    simulationSteps = [{ step: 0, matrix: JSON.parse(JSON.stringify(aug)), desc: "Initial Augmented Matrix" }];
    
    let stepCount = 1;
    for (let i = 0; i < n; i++) {
        // Pivot
        let pivot = aug[i][i];
        for (let j = 0; j <= n; j++) aug[i][j] /= pivot;
        simulationSteps.push({ step: stepCount++, matrix: JSON.parse(JSON.stringify(aug)), desc: `Normalize Row R${i+1}` });

        for (let k = 0; k < n; k++) {
            if (k !== i) {
                let factor = aug[k][i];
                for (let j = 0; j <= n; j++) aug[k][j] -= factor * aug[i][j];
            }
        }
        simulationSteps.push({ step: stepCount++, matrix: JSON.parse(JSON.stringify(aug)), desc: `Eliminate Column ${i+1}` });
    }
    
    setupVisualization(false);
}

function getMatrixInputs() {
    const n = parseInt(document.getElementById('var-count').value);
    const A = [], b = [];
    for(let i=0; i<n; i++) {
        A.push([]);
        for(let j=0; j<n; j++) {
            A[i].push(parseFloat(document.getElementById(`mat-${i}-${j}`).value));
        }
        b.push(parseFloat(document.getElementById(`mat-${i}-${n}`).value));
    }
    const maxIter = document.getElementById('max-iter') ? parseInt(document.getElementById('max-iter').value) : 0;
    const tol = document.getElementById('tol') ? parseFloat(document.getElementById('tol').value) : 0;
    return { A, b, n, maxIter, tol };
}

// --- UI Updates ---

function setupVisualization(showChart) {
    currentStepIndex = 0;
    document.getElementById('chart-wrapper').style.display = showChart ? 'block' : 'none';
    
    if (showChart) {
        drawChart();
    }
    showStep(0);
}

function showStep(index) {
    if (index < 0 || index >= simulationSteps.length) return;
    currentStepIndex = index;
    
    const step = simulationSteps[index];
    const display = document.getElementById('step-display');
    
    document.getElementById('step-counter').textContent = `Step ${index} / ${simulationSteps.length - 1}`;
    document.getElementById('prev-step-btn').disabled = index === 0;
    document.getElementById('next-step-btn').disabled = index === simulationSteps.length - 1;
    
    let html = `<strong>${step.desc}</strong>\n\n`;
    
    if (step.x) { // Iterative methods
        html += step.x.map((val, i) => `x${i+1} = ${val.toFixed(6)}`).join('\n');
    } else if (step.matrix) { // Matrix methods
        html += step.matrix.map(row => 
            row.map(v => v.toFixed(3).padStart(8, ' ')).join('  ')
        ).join('\n');
    }
    
    display.innerHTML = html;
}

function drawChart() {
    const ctx = document.getElementById('resultChart').getContext('2d');
    if (currentChart) currentChart.destroy();
    
    const labels = simulationSteps.map(s => s.step);
    const n = simulationSteps[0].x.length;
    const datasets = [];
    
    const colors = ['#00f0ff', '#ff0055', '#bbff00', '#ff00ff', '#00ff88'];
    
    for (let i = 0; i < n; i++) {
        datasets.push({
            label: `x${i+1}`,
            data: simulationSteps.map(s => s.x[i]),
            borderColor: colors[i % colors.length],
            backgroundColor: colors[i % colors.length] + '22',
            borderWidth: 2,
            tension: 0.3,
            fill: true
        });
    }

    currentChart = new Chart(ctx, {
        type: 'line',
        data: { labels, datasets },
        options: {
            responsive: true, maintainAspectRatio: false,
            plugins: {
                legend: { labels: { color: '#fff' } }
            },
            scales: {
                x: { ticks: { color: '#aaa' }, grid: { color: '#333' }, title: {display: true, text: 'Iteration', color:'#fff'} },
                y: { ticks: { color: '#aaa' }, grid: { color: '#333' } }
            }
        }
    });
}
