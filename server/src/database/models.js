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
    CREATE TABLE IF NOT EXISTS characters (
      id BIGSERIAL PRIMARY KEY,
      user_id BIGINT NOT NULL REFERENCES users(id) ON DELETE CASCADE,
      name TEXT NOT NULL,
      class_id TEXT NOT NULL CHECK (class_id IN ('crusader','necromancer','sorcerer','ranger','monk')),
      hardcore BOOLEAN NOT NULL DEFAULT FALSE,
      dead BOOLEAN NOT NULL DEFAULT FALSE,
      level INT NOT NULL DEFAULT 1,
      paragon_level INT NOT NULL DEFAULT 0,
      spire_best_floor INT NOT NULL DEFAULT 0,
      play_seconds INT NOT NULL DEFAULT 0,
      last_biome TEXT NOT NULL DEFAULT 'verdant_wilds',
      blob JSONB NOT NULL DEFAULT '{}',
      created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
      updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
      UNIQUE (user_id, name)
    );
    CREATE INDEX IF NOT EXISTS characters_user ON characters(user_id);
  `);
}

module.exports = { pool, initDb };
