import { motion } from 'framer-motion';

export default function GlassCard({ children, className = '', onClick }) {
  return (
    <motion.div 
      whileHover={{ y: -5, boxShadow: '0 10px 40px rgba(0, 240, 255, 0.15)' }}
      className={`glass rounded-2xl p-6 transition-colors hover:bg-[rgba(255,255,255,0.05)] ${className}`}
      onClick={onClick}
    >
      {children}
    </motion.div>
  );
}
