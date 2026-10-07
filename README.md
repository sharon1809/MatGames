# MatGames

## Architecture
- **Backend**: Node.js/Express. It parses inputs, feeds them directly into your existing C++ executables using standard input (cin), and extracts the outputs. Your C++ remains 100% untouched.
- **Frontend**: React + Vite + Tailwind CSS + Framer Motion. Provides a premium, glassmorphic UI.

## Setup Instructions

### 1. Run the Backend API
Open a terminal in the `backend` folder and run:
```bash
npm install
npm start
```
*The backend runs on port 3001.*

### 2. Run the Frontend UI
Open a NEW terminal in the `frontend` folder and run:
```bash
npm install
npm run dev
```
*The frontend runs on port 5173. Open http://localhost:5173 in your browser.*
