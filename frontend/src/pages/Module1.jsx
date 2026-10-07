import { useState } from 'react';
import { motion } from 'framer-motion';
import GlassCard from '../components/GlassCard.jsx';
import { LineChart, Line, XAxis, YAxis, Tooltip, ResponsiveContainer } from 'recharts';

export default function Module1() {
  const [method, setMethod] = useState('m1_jacobi');
  const [n, setN] = useState(3);
  const [maxIter, setMaxIter] = useState(50);
  const [tol, setTol] = useState(0.0001);
  const [matrix, setMatrix] = useState([
    [4, 1, 1, 6],
    [1, 5, 2, 8],
    [1, 2, 6, 9]
  ]);
  const [result, setResult] = useState(null);
  const [loading, setLoading] = useState(false);

  const handleSolve = async () => {
    setLoading(true);
    try {
      const res = await fetch(`http://localhost:3001/api/solve/${method}`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ n, matrix, maxIter, tol })
      });
      const data = await res.json();
      setResult(data);
    } catch (e) {
      console.error(e);
    }
    setLoading(false);
  };

  const chartData = result?.history?.map((step, i) => {
    const obj = { iteration: i };
    if(step.x) step.x.forEach((val, idx) => obj[`x${idx+1}`] = val);
    return obj;
  });

  const colors = ['#00f0ff', '#8b5cf6', '#ec4899', '#facc15'];

  return (
    <div className="container mx-auto px-4 py-10 flex gap-8 relative z-10">
      {/* Left: Input */}
      <div className="w-1/3">
        <GlassCard>
          <h2 className="text-2xl font-display mb-6">Linear Systems</h2>
          <select 
            className="w-full bg-[rgba(0,0,0,0.3)] border border-cardBorder rounded-lg p-3 text-white mb-6 outline-none focus:border-cyan-400"
            value={method} onChange={e => setMethod(e.target.value)}
          >
            <option value="m1_jacobi">Gauss-Jacobi</option>
            <option value="m1_seidel">Gauss-Seidel</option>
            <option value="m1_jordan">Gauss-Jordan</option>
          </select>

          <div className="space-y-4 text-sm text-gray-300">
            <div>
              <label className="block mb-1">Variables (n)</label>
              <input type="number" value={n} onChange={e => setN(parseInt(e.target.value))} className="w-full bg-[rgba(0,0,0,0.3)] border border-cardBorder rounded-lg p-2 text-white" />
            </div>
            
            <div className="overflow-x-auto py-2">
              <label className="block mb-2">Augmented Matrix</label>
              <div className="space-y-2">
                {matrix.map((row, i) => (
                  <div key={i} className="flex gap-2">
                    {row.map((val, j) => (
                      <input 
                        key={j} type="number" value={val}
                        onChange={(e) => {
                          const newM = [...matrix];
                          newM[i][j] = parseFloat(e.target.value) || 0;
                          setMatrix(newM);
                        }}
                        className="w-12 bg-[rgba(0,0,0,0.3)] border border-cardBorder rounded-lg p-2 text-center text-white font-mono"
                      />
                    ))}
                  </div>
                ))}
              </div>
            </div>

            {method !== 'm1_jordan' && (
              <div className="flex gap-4">
                <div className="w-1/2">
                  <label className="block mb-1">Max Iterations</label>
                  <input type="number" value={maxIter} onChange={e => setMaxIter(e.target.value)} className="w-full bg-[rgba(0,0,0,0.3)] border border-cardBorder rounded-lg p-2 text-white" />
                </div>
                <div className="w-1/2">
                  <label className="block mb-1">Tolerance</label>
                  <input type="number" step="0.0001" value={tol} onChange={e => setTol(e.target.value)} className="w-full bg-[rgba(0,0,0,0.3)] border border-cardBorder rounded-lg p-2 text-white" />
                </div>
              </div>
            )}

            <button onClick={handleSolve} className="w-full mt-6 bg-gradient-to-r from-cyan-500 to-blue-500 hover:from-cyan-400 hover:to-blue-400 text-white font-semibold py-3 px-4 rounded-lg transition-all shadow-[0_0_20px_rgba(0,240,255,0.3)] hover:shadow-[0_0_30px_rgba(0,240,255,0.5)]">
              {loading ? 'Computing...' : 'Solve'}
            </button>
          </div>
        </GlassCard>
      </div>

      {/* Right: Output */}
      <div className="w-2/3 flex flex-col gap-6">
        {result && (
          <motion.div initial={{opacity:0, x:20}} animate={{opacity:1, x:0}}>
            <GlassCard className="h-64 mb-6">
              <h3 className="text-lg font-display mb-4 text-gray-300">Convergence Graph</h3>
              {method !== 'm1_jordan' ? (
                <ResponsiveContainer width="100%" height="100%">
                  <LineChart data={chartData}>
                    <XAxis dataKey="iteration" stroke="#666" />
                    <YAxis stroke="#666" />
                    <Tooltip contentStyle={{backgroundColor: '#040b16', border: '1px solid #333'}} />
                    {Array.from({length: n}).map((_, i) => (
                      <Line key={i} type="monotone" dataKey={`x${i+1}`} stroke={colors[i]} strokeWidth={2} dot={false} />
                    ))}
                  </LineChart>
                </ResponsiveContainer>
              ) : (
                <div className="flex items-center justify-center h-full text-gray-500">Graph not applicable for direct methods.</div>
              )}
            </GlassCard>

            <GlassCard>
              <h3 className="text-lg font-display mb-4 text-gray-300">Final Solution</h3>
              <div className="flex gap-4">
                {result.solution?.map((val, i) => (
                  <div key={i} className="bg-[rgba(0,0,0,0.3)] border border-cardBorder rounded-lg p-4 flex-1 text-center">
                    <div className="text-sm text-cyan-400 mb-1 font-mono">x{i+1}</div>
                    <div className="text-2xl font-mono">{val.toFixed(4)}</div>
                  </div>
                ))}
              </div>
            </GlassCard>
          </motion.div>
        )}
      </div>
    </div>
  );
}
