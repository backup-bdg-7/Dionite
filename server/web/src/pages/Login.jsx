import React, { useState } from 'react';
import { useAuth } from '../auth.jsx';
import { useNavigate } from 'react-router-dom';

export default function Login() {
  const { login } = useAuth();
  const [email, setEmail] = useState('admin@dionite.game');
  const [password, setPassword] = useState('admin123');
  const [err, setErr] = useState('');
  const nav = useNavigate();

  return (
    <div style={{ display: 'flex', height: '100vh', alignItems: 'center', justifyContent: 'center', background: '#050505' }}>
      <div className="panel" style={{ width: 380 }}>
        <h2 style={{ color: '#a5854c', textAlign: 'center', margin: 0 }}>Dionite</h2>
        <p style={{ textAlign: 'center', color: '#6e6862', fontSize: 11 }}>Game Master Console</p>
        <input value={email} onChange={(e) => setEmail(e.target.value)} placeholder="Email" style={{ marginTop: 12 }} />
        <input value={password} onChange={(e) => setPassword(e.target.value)} placeholder="Password" type="password" style={{ marginTop: 8 }} />
        {err && <div style={{ color: '#e11d48', fontSize: 12, marginTop: 8 }}>{err}</div>}
        <button className="btn" style={{ marginTop: 12, width: '100%' }}
          onClick={async () => {
            setErr('');
            try { await login(email, password); nav('/'); }
            catch (e) { setErr(e.response?.data?.error || e.message); }
          }}>
          Sign In
        </button>
      </div>
    </div>
  );
}
