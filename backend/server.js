const express = require('express');
const cors = require('cors');
const { exec } = require('child_process');
const path = require('path');
const fs = require('fs');
require('dotenv').config();

const app = express();
app.use(cors());
app.use(express.json());

// Path to C++ Modules
const MODULES_DIR = path.join(__dirname, '..');

// Helper to compile and run C++ code
function runCpp(sourcePath, inputStr) {
    return new Promise((resolve, reject) => {
        const exeName = process.platform === 'win32' ? 'temp.exe' : './temp.out';
        const compileCmd = `g++ "${sourcePath}" -o ${exeName}`;
        
        exec(compileCmd, (err, stdout, stderr) => {
            if (err) return reject(`Compile Error: ${stderr}`);
            
            const child = exec(exeName, (runErr, runOut, runErrOut) => {
                if (runErr) return reject(`Runtime Error: ${runErrOut}`);
                resolve(runOut);
            });
            
            child.stdin.write(inputStr + '\n');
            child.stdin.end();
        });
    });
}

// Module 1 Parsers
function parseGauss(output) {
    const history = [];
    const iterRegex = /Iter\s+\d+:\s+([\d\.\-\s]+)/g;
    let match;
    while ((match = iterRegex.exec(output)) !== null) {
        history.push({ x: match[1].trim().split(/\s+/).map(Number) });
    }
    
    const solution = [];
    const solRegex = /x\d+\s*=\s*([\d\.\-]+)/g;
    while ((match = solRegex.exec(output)) !== null) {
        solution.push(Number(match[1]));
    }
    
    return { history, solution, raw: output };
}

app.post('/api/solve/m1_jacobi', async (req, res) => {
    const { n, matrix, maxIter, tol } = req.body;
    // Flat string of inputs: n, A and b elements, maxIter, tol
    const inputStr = `${n} ${matrix.flat().join(' ')} ${maxIter} ${tol}`;
    try {
        const out = await runCpp(path.join(MODULES_DIR, 'Module_1', 'GaussJacobi.cpp'), inputStr);
        res.json(parseGauss(out));
    } catch (e) {
        res.status(500).json({ error: e });
    }
});

// Same for Seidel and Jordan
app.post('/api/solve/m1_seidel', async (req, res) => {
    const { n, matrix, maxIter, tol } = req.body;
    const inputStr = `${n} ${matrix.flat().join(' ')} ${maxIter} ${tol}`;
    try {
        const out = await runCpp(path.join(MODULES_DIR, 'Module_1', 'GaussSeidel.cpp'), inputStr);
        res.json(parseGauss(out));
    } catch (e) {
        res.status(500).json({ error: e });
    }
});

app.post('/api/solve/m1_jordan', async (req, res) => {
    const { n, matrix } = req.body;
    const inputStr = `${n} ${matrix.flat().join(' ')}`;
    try {
        const out = await runCpp(path.join(MODULES_DIR, 'Module_1', 'GaussJordan.cpp'), inputStr);
        // Jordan has different output format
        const history = [];
        const stepBlocks = out.split(/Step \d+:/).slice(1);
        stepBlocks.forEach(block => {
            const lines = block.trim().split('\n').filter(l => l.trim());
            const mat = lines.map(line => line.trim().split(/\s+/).map(Number));
            // Ensure we don't accidentally parse the "Final Result" text
            if(mat.length === n) history.push({ matrix: mat });
        });
        
        const solution = [];
        const solRegex = /x\d+\s*=\s*([\d\.\-]+)/g;
        let match;
        while ((match = solRegex.exec(out)) !== null) {
            solution.push(Number(match[1]));
        }
        res.json({ history, solution, raw: out });
    } catch (e) {
        res.status(500).json({ error: e });
    }
});

const PORT = 3001;
app.listen(PORT, () => console.log(`Backend running on http://localhost:${PORT}`));
