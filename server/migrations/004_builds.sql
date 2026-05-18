-- 004_builds.sql — Build-export + Gallery + Achievements + Anti-cheat audit.
-- Lets players share character builds (skill/paragon/loot snapshots) with the world.

CREATE TABLE IF NOT EXISTS build_exports (
    id              BIGSERIAL PRIMARY KEY,
    owner_user_id   BIGINT REFERENCES users(id) ON DELETE CASCADE,
    character_id    BIGINT REFERENCES characters(id) ON DELETE SET NULL,
    slug            TEXT UNIQUE NOT NULL,
    title           TEXT NOT NULL,
    class_id        TEXT NOT NULL,
    level           INTEGER NOT NULL,
    paragon_level   INTEGER NOT NULL DEFAULT 0,
    snapshot        JSONB NOT NULL,        -- {skills:[...], paragon:[...], gear:[...], stats:{...}}
    description     TEXT,
    youtube_url     TEXT,
    is_public       BOOLEAN NOT NULL DEFAULT TRUE,
    upvotes         INTEGER NOT NULL DEFAULT 0,
    views           BIGINT  NOT NULL DEFAULT 0,
    created_at      TIMESTAMPTZ NOT NULL DEFAULT now(),
    updated_at      TIMESTAMPTZ NOT NULL DEFAULT now()
);
CREATE INDEX IF NOT EXISTS idx_build_class    ON build_exports(class_id, upvotes DESC);
CREATE INDEX IF NOT EXISTS idx_build_recent   ON build_exports(created_at DESC);

CREATE TABLE IF NOT EXISTS build_upvotes (
    user_id     BIGINT REFERENCES users(id) ON DELETE CASCADE,
    build_id    BIGINT REFERENCES build_exports(id) ON DELETE CASCADE,
    voted_at    TIMESTAMPTZ NOT NULL DEFAULT now(),
    PRIMARY KEY (user_id, build_id)
);

CREATE TABLE IF NOT EXISTS achievements (
    id              TEXT PRIMARY KEY,
    name            TEXT NOT NULL,
    description     TEXT,
    points          INTEGER NOT NULL DEFAULT 10,
    rewards         JSONB   NOT NULL DEFAULT '[]'::jsonb,   -- cosmetic ids
    metadata        JSONB   NOT NULL DEFAULT '{}'::jsonb
);

CREATE TABLE IF NOT EXISTS player_achievements (
    user_id         BIGINT REFERENCES users(id) ON DELETE CASCADE,
    achievement_id  TEXT   REFERENCES achievements(id) ON DELETE CASCADE,
    progress        INTEGER NOT NULL DEFAULT 0,
    target          INTEGER NOT NULL DEFAULT 1,
    unlocked_at     TIMESTAMPTZ,
    PRIMARY KEY (user_id, achievement_id)
);

-- Anti-cheat audit — every suspicious save delta logged for review.
CREATE TABLE IF NOT EXISTS anticheat_flags (
    id              BIGSERIAL PRIMARY KEY,
    user_id         BIGINT REFERENCES users(id) ON DELETE CASCADE,
    character_id    BIGINT REFERENCES characters(id) ON DELETE SET NULL,
    reason          TEXT NOT NULL,
    severity        SMALLINT NOT NULL DEFAULT 1,        -- 1 watch, 2 review, 3 ban
    evidence        JSONB,
    created_at      TIMESTAMPTZ NOT NULL DEFAULT now()
);
CREATE INDEX IF NOT EXISTS idx_anticheat_user ON anticheat_flags(user_id, created_at DESC);
