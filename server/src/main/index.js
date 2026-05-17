// ============================================================================
// Dionite Server — entry point (Express REST + WebSocket + PostgreSQL)
// ============================================================================
require('dotenv').config();
const express = require('express');
const cors = require('cors');
const helmet = require('helmet');
const morgan = require('morgan');
const http = require('http');

const { initDb, pool } = require('../database/models');
const authRoutes = require('../auth/routes');
const gameRoutes = require('../game/routes');
const charRoutes = require('../game/characters');
const adminRoutes = require('../admin/routes');
const matchmakingRoutes = require('../matchmaking/routes');
const { attachWs } = require('../matchmaking/ghosts');

const PORT = process.env.PORT || 4000;

async function main() {
  await initDb(pool);

  const app = express();
  app.set('trust proxy', 1);
  app.use(helmet());
  app.use(cors({ origin: process.env.FRONTEND_URL || '*' }));
  app.use(express.json({ limit: '2mb' }));
  app.use(morgan('combined'));

  app.get('/api/health', (_, res) => res.json({ ok: true, ts: Date.now() }));
  app.use('/api/auth', authRoutes);
  app.use('/api', gameRoutes);
  app.use('/api', charRoutes);
  app.use('/api/admin', adminRoutes);
  app.use('/api/match', matchmakingRoutes);

  app.use((err, req, res, _next) => {
    console.error('[error]', err);
    res.status(err.status || 500).json({ error: err.message || 'Internal Error' });
  });

  const server = http.createServer(app);
  attachWs(server);
  server.listen(PORT, () => console.log(`Dionite server listening on :${PORT}`));
}

main().catch((e) => { console.error('Fatal boot:', e); process.exit(1); });
