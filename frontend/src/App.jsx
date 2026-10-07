import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import Home from './pages/Home.jsx';
import Module1 from './pages/Module1.jsx';

export default function App() {
  return (
    <Router>
      <div className="min-h-screen relative">
        <div className="bg-orb orb-1"></div>
        <div className="bg-orb orb-2"></div>
        <div className="bg-orb orb-3"></div>
        <div className="math-symbol" style={{top: '10%', left: '80%'}}>∫</div>
        <div className="math-symbol" style={{top: '60%', left: '10%'}}>Σ</div>
        
        <Routes>
          <Route path="/" element={<Home />} />
          <Route path="/module1" element={<Module1 />} />
        </Routes>
      </div>
    </Router>
  );
}
