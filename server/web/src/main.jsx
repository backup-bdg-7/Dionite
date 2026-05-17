import React from 'react';
import ReactDOM from 'react-dom/client';
import { BrowserRouter, Routes, Route, Link, Navigate } from 'react-router-dom';
import { AuthProvider, useAuth } from './auth.jsx';
import Login from './pages/Login.jsx';
import Dashboard from './pages/Dashboard.jsx';
import Players from './pages/Players.jsx';
import Leaderboard from './pages/Leaderboard.jsx';
import Items from './pages/Items.jsx';

function Shell() {
  const { user, logout } = useAuth();
  if (!user) return <Navigate to="/login" />;
  return (
    <div style={{ display: 'flex', height: '100vh' }}>
      <nav style={{ width: 220, background: '#0c0b10', padding: 20, borderRight: '1px solid #3a3632' }}>
        <h1 style={{ color: '#a5854c', fontSize: 18, margin: 0 }}>DIONITE</h1>
        <p style={{ color: '#6e6862', fontSize: 11, marginTop: 4 }}>Game Master Console</p>
        <ul style={{ listStyle: 'none', padding: 0, marginTop: 20 }}>
          <li style={{ marginBottom: 8 }}><Link to="/" style={{ color: '#e4dcd3' }}>Dashboard</Link></li>
          <li style={{ marginBottom: 8 }}><Link to="/players" style={{ color: '#e4dcd3' }}>Players</Link></li>
          <li style={{ marginBottom: 8 }}><Link to="/leaderboard" style={{ color: '#e4dcd3' }}>Leaderboard</Link></li>
          <li style={{ marginBottom: 8 }}><Link to="/items" style={{ color: '#e4dcd3' }}>Items</Link></li>
        </ul>
        <button className="btn" onClick={logout} style={{ marginTop: 30 }}>Logout</button>
      </nav>
      <main style={{ flex: 1, overflow: 'auto', padding: 24 }}>
        <Routes>
          <Route path="/" element={<Dashboard />} />
          <Route path="/players" element={<Players />} />
          <Route path="/leaderboard" element={<Leaderboard />} />
          <Route path="/items" element={<Items />} />
        </Routes>
      </main>
    </div>
  );
}

function App() {
  return (
    <AuthProvider>
      <BrowserRouter>
        <Routes>
          <Route path="/login" element={<Login />} />
          <Route path="/*" element={<Shell />} />
        </Routes>
      </BrowserRouter>
    </AuthProvider>
  );
}

ReactDOM.createRoot(document.getElementById('root')).render(<App />);
