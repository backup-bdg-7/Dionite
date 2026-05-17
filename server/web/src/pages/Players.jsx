import React, { useEffect, useState } from 'react';
import { useAuth } from '../auth.jsx';

export default function Players() {
  const { api } = useAuth();
  const [players, setPlayers] = useState([]);
  const refresh = () => api.get('/api/admin/players').then((r) => setPlayers(r.data));
  useEffect(() => { refresh(); }, []);

  return (
    <div>
      <h2 style={{ color: '#a5854c' }}>Players</h2>
      <table>
        <thead><tr><th>ID</th><th>Email</th><th>Name</th><th>Role</th><th>Lvl</th><th>Gold</th><th>Spire</th><th></th></tr></thead>
        <tbody>
          {players.map((p) => (
            <tr key={p.id}>
              <td>{p.id}</td><td>{p.email}</td><td>{p.name}</td>
              <td>{p.role}</td><td>{p.level}</td><td>{p.gold}</td><td>{p.spire_best_floor}</td>
              <td>
                <button className="btn" onClick={async () => {
                  await api.post(`/api/admin/players/${p.id}/grant_gold`, { amount: 1000 }); refresh();
                }}>+1000g</button>
                <button className="btn" style={{ marginLeft: 6, borderColor: '#e11d48' }} onClick={async () => {
                  if (!confirm('Delete player?')) return;
                  await api.delete(`/api/admin/players/${p.id}`); refresh();
                }}>Delete</button>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}
