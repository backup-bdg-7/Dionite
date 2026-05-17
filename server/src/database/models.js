// ============================================================================
// Dionite Server — PostgreSQL pool + table bootstrap
// ============================================================================
const { Pool } = require('pg');

const pool = new Pool({
  connectionString: process.env.DATABASE_URL || 'postgres://dionite:dionite@localhost:5432/dionite',
  max: 20,
  idleTimeoutMillis: 30000,
});

async function initDb(p) {
  await p.query(`
    CREATE TABLE IF NOT EXISTS users (
      id BIGSERIAL PRIMARY KEY,
      email TEXT UNIQUE NOT NULL,
      password_hash TEXT,
      apple_subject TEXT UNIQUE,
      name TEXT,
      role TEXT NOT NULL DEFAULT 'player',
      created_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
    );
    CREATE TABLE IF NOT EXISTS saves (
      user_id BIGINT PRIMARY KEY REFERENCES users(id) ON DELETE CASCADE,
      blob JSONB NOT NULL DEFAULT '{}',
      updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
    );
    CREATE TABLE IF NOT EXISTS spire_runs (
      id BIGSERIAL PRIMARY KEY,
      user_id BIGINT NOT NULL REFERENCES users(id) ON DELETE CASCADE,
      floor INT NOT NULL,
      score INT NOT NULL,
      biome TEXT NOT NULL,
      time_sec REAL NOT NULL,
      created_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
    );
    CREATE INDEX IF NOT EXISTS spire_runs_floor_score ON spire_runs(floor DESC, score DESC);
    CREATE TABLE IF NOT EXISTS ghost_records (
      id BIGSERIAL PRIMARY KEY,
      user_id BIGINT NOT NULL REFERENCES users(id) ON DELETE CASCADE,
      floor INT NOT NULL,
      score INT NOT NULL,
      frames JSONB NOT NULL,
      created_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
    );
    CREATE INDEX IF NOT EXISTS ghost_records_floor ON ghost_records(floor);
  `);
}

module.exports = { pool, initDb };
