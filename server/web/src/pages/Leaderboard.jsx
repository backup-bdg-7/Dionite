import React, { useEffect, useState } from 'react';
import { useAuth } from '../auth.jsx';

export default function Leaderboard() {
  const { api } = useAuth();
  const [rows, setRows] = useState([]);
  useEffect(() => { api.get('/api/spire/leaderboard').then((r) => setRows(r.data)); }, []);
  return (
    <div>
      <h2 style={{ color: '#a5854c' }}>Infinity Spire Leaderboard</h2>
      <table>
        <thead><tr><th>#</th><th>Player</th><th>Floor</th><th>Score</th><th>Biome</th><th>Time</th></tr></thead>
        <tbody>
          {rows.map((r, i) => (
            <tr key={i}>
              <td>{i + 1}</td><td>{r.name}</td><td style={{ color: '#fbbf24' }}>{r.floor}</td>
              <td>{r.score}</td><td>{r.biome}</td><td>{Number(r.time_sec).toFixed(1)}s</td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}
