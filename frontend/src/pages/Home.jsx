import { motion } from 'framer-motion';
import { useNavigate } from 'react-router-dom';
import GlassCard from '../components/GlassCard.jsx';

const modules = [
  { id: 'm1', title: 'Module 1: Linear Systems', desc: 'Gauss-Jacobi, Seidel, Jordan', path: '/module1' },
  { id: 'm2', title: 'Module 2: Interpolation', desc: 'Straight Line, Polynomial, Lagrange', path: '/module2' },
  { id: 'm3', title: 'Module 3: Integration', desc: 'Simpson\\'s 1/3, Newton\\'s Backward', path: '/module3' },
  { id: 'm4', title: 'Module 4: ODEs', desc: 'Euler, RK4, Adam-Bashforth', path: '/module4' },
  { id: 'm5', title: 'Module 5: PDEs', desc: 'Liebmann, Poisson, Wave', path: '/module5' },
];

export default function Home() {
  const nav = useNavigate();
  return (
    <div className="container mx-auto px-4 py-20 relative z-10">
      <motion.div initial={{ opacity: 0, y: 20 }} animate={{ opacity: 1, y: 0 }} className="text-center mb-16">
        <h1 className="text-6xl font-display font-bold tracking-tight mb-4">
          Mat<span className="text-transparent bg-clip-text bg-gradient-to-r from-cyan-400 to-violet-500">Games</span>
        </h1>
        <p className="text-xl text-gray-400 font-sans max-w-2xl mx-auto">
          Interactive numerical methods playground. Visualize computations step-by-step.
        </p>
      </motion.div>

      <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6 max-w-5xl mx-auto">
        {modules.map((m, i) => (
          <motion.div key={m.id} initial={{ opacity: 0, y: 20 }} animate={{ opacity: 1, y: 0 }} transition={{ delay: i * 0.1 }}>
            <GlassCard className="cursor-pointer h-full" onClick={() => nav(m.path)}>
              <h3 className="text-xl font-display font-semibold mb-2">{m.title}</h3>
              <p className="text-gray-400 text-sm">{m.desc}</p>
            </GlassCard>
          </motion.div>
        ))}
      </div>
    </div>
  );
}
