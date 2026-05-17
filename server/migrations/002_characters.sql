-- Dionite — Multi-character migration. Adds Diablo-style character roster.
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

-- Drop the singleton `saves` constraint and migrate row data into characters
-- (optional, can be done manually). Old `saves` table remains for legacy migration.
