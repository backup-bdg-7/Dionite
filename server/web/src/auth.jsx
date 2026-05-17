import React, { createContext, useContext, useEffect, useState } from 'react';
import axios from 'axios';

const API = import.meta.env.VITE_API_URL || 'http://localhost:4000';
const Ctx = createContext(null);

export function AuthProvider({ children }) {
  const [user, setUser] = useState(null);

  useEffect(() => {
    const tok = localStorage.getItem('dionite_admin_tok');
    if (!tok) return;
    axios.get(`${API}/api/auth/me`, { headers: { Authorization: `Bearer ${tok}` } })
      .then((r) => { if (r.data.role === 'admin') setUser(r.data); })
      .catch(() => localStorage.removeItem('dionite_admin_tok'));
  }, []);

  const login = async (email, password) => {
    const r = await axios.post(`${API}/api/auth/login`, { email, password });
    if (r.data.role !== 'admin') throw new Error('Not an admin account.');
    localStorage.setItem('dionite_admin_tok', r.data.access_token);
    setUser(r.data);
  };
  const logout = () => { localStorage.removeItem('dionite_admin_tok'); setUser(null); };

  const apiClient = axios.create({ baseURL: API });
  apiClient.interceptors.request.use((cfg) => {
    const tok = localStorage.getItem('dionite_admin_tok');
    if (tok) cfg.headers.Authorization = `Bearer ${tok}`;
    return cfg;
  });

  return <Ctx.Provider value={{ user, login, logout, api: apiClient }}>{children}</Ctx.Provider>;
}

export const useAuth = () => useContext(Ctx);
