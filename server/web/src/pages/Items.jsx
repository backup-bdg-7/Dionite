import React, { useEffect, useState } from 'react';

export default function Items() {
  const [data, setData] = useState(null);
  useEffect(() => {
    fetch('/data/loot_tables.json').then((r) => r.json()).then(setData).catch(() => {
      // Fallback: read inline (in real deployment, copy /app/assets/data into public/data).
      setData({ rarities: [], weapons: [], affixes: [] });
    });
  }, []);
  if (!data) return <p>Loading…</p>;
  return (
    <div>
      <h2 style={{ color: '#a5854c' }}>Item Templates & Affixes</h2>
      <h3 style={{ color: '#a5854c', marginTop: 24 }}>Weapons</h3>
      <table>
        <thead><tr><th>ID</th><th>Name</th><th>Kind</th><th>Damage</th><th>Fire Rate</th><th>Magazine</th></tr></thead>
        <tbody>{(data.weapons || []).map((w) => (
          <tr key={w.id}><td>{w.id}</td><td>{w.name}</td><td>{w.kind}</td><td>{w.dmg}</td><td>{w.fireRate}</td><td>{w.magazine}</td></tr>
        ))}</tbody>
      </table>
      <h3 style={{ color: '#a5854c', marginTop: 24 }}>Affixes</h3>
      <table>
        <thead><tr><th>ID</th><th>Name</th><th>Stat</th><th>Amount</th></tr></thead>
        <tbody>{(data.affixes || []).map((a) => (
          <tr key={a.id}><td>{a.id}</td><td>{a.name}</td><td>{a.stat}</td><td>{a.amt}</td></tr>
        ))}</tbody>
      </table>
    </div>
  );
}
