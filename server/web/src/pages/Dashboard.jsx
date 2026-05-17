import React, { useEffect, useState } from 'react';
import { useAuth } from '../auth.jsx';

export default function Dashboard() {
  const { api } = useAuth();
  const [stats, setStats] = useState({});
  useEffect(() => { api.get('/api/admin/stats').then((r) => setStats(r.data)); }, []);
  return (
    <div>
      <h2 style={{ color: '#a5854c' }}>Dashboard</h2>
      <div style={{ display: 'grid', gridTemplateColumns: 'repeat(3, 1fr)', gap: 16, marginTop: 20 }}>
        {['users', 'saves', 'spire_runs'].map((k) => (
          <div key={k} className="panel">
            <div style={{ color: '#6e6862', fontSize: 11, textTransform: 'uppercase' }}>{k}</div>
            <div style={{ fontSize: 36, color: '#a5854c', marginTop: 8 }}>{stats[k] ?? '-'}</div>
          </div>
        ))}
      </div>
    </div>
  );
}
