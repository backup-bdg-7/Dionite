-- 003_cosmetics.sql — Cosmetic catalog, ownership, transmog loadouts, shop ledger.
-- Designed for PostgreSQL 16. Run after 002_characters.sql.

-- All known cosmetic IDs (server-of-record). The client mirrors this from /api/shop/listing.
CREATE TABLE IF NOT EXISTS cosmetic_catalog (
    id              TEXT PRIMARY KEY,
    name            TEXT        NOT NULL,
    slot            TEXT        NOT NULL,
    rarity          TEXT        NOT NULL DEFAULT 'Common',
    tier            TEXT        NOT NULL DEFAULT 'Bronze',
    price_coins     INTEGER     NOT NULL DEFAULT 0,
    price_shards    INTEGER     NOT NULL DEFAULT 0,
    season_number   INTEGER     NOT NULL DEFAULT 0,
    class_lock      TEXT,
    event_only      BOOLEAN     NOT NULL DEFAULT FALSE,
    metadata        JSONB       NOT NULL DEFAULT '{}'::jsonb,
    created_at      TIMESTAMPTZ NOT NULL DEFAULT now()
);

CREATE INDEX IF NOT EXISTS idx_cosmetic_slot   ON cosmetic_catalog(slot);
CREATE INDEX IF NOT EXISTS idx_cosmetic_season ON cosmetic_catalog(season_number);

-- Account-wide unlocks: looting a unique piece unlocks its appearance forever.
CREATE TABLE IF NOT EXISTS cosmetic_unlocks (
    user_id         BIGINT REFERENCES users(id) ON DELETE CASCADE,
    cosmetic_id     TEXT   REFERENCES cosmetic_catalog(id) ON DELETE CASCADE,
    source          TEXT   NOT NULL DEFAULT 'loot',          -- 'loot' | 'purchase' | 'pass' | 'gift'
    acquired_at     TIMESTAMPTZ NOT NULL DEFAULT now(),
    PRIMARY KEY (user_id, cosmetic_id)
);

-- Per-character transmog loadout (slot → cosmetic_id JSON + 4 dye channels).
CREATE TABLE IF NOT EXISTS character_transmog (
    character_id    BIGINT PRIMARY KEY REFERENCES characters(id) ON DELETE CASCADE,
    appearance      JSONB NOT NULL DEFAULT '{}'::jsonb,
    dyes            JSONB NOT NULL DEFAULT '{}'::jsonb,
    updated_at      TIMESTAMPTZ NOT NULL DEFAULT now()
);

-- Player currencies (gold drops in-game; coins are premium / IAP-purchased; shards earned).
CREATE TABLE IF NOT EXISTS player_wallet (
    user_id         BIGINT PRIMARY KEY REFERENCES users(id) ON DELETE CASCADE,
    gold            BIGINT  NOT NULL DEFAULT 0,
    coins           BIGINT  NOT NULL DEFAULT 0,
    shards          BIGINT  NOT NULL DEFAULT 0,
    updated_at      TIMESTAMPTZ NOT NULL DEFAULT now()
);

-- Storefront ledger — every coin/shard movement (audit + anti-fraud).
CREATE TABLE IF NOT EXISTS shop_transactions (
    id              BIGSERIAL PRIMARY KEY,
    user_id         BIGINT REFERENCES users(id) ON DELETE CASCADE,
    cosmetic_id     TEXT   REFERENCES cosmetic_catalog(id),
    coins_delta     BIGINT NOT NULL DEFAULT 0,
    shards_delta    BIGINT NOT NULL DEFAULT 0,
    iap_receipt     TEXT,                                       -- Apple/Google receipt JSON
    reason          TEXT   NOT NULL,                           -- 'purchase' | 'refund' | 'reward' | 'gift'
    created_at      TIMESTAMPTZ NOT NULL DEFAULT now()
);
CREATE INDEX IF NOT EXISTS idx_shop_tx_user ON shop_transactions(user_id, created_at DESC);

-- Featured rotation (server publishes to /api/shop/listing).
CREATE TABLE IF NOT EXISTS shop_listings (
    cosmetic_id          TEXT PRIMARY KEY REFERENCES cosmetic_catalog(id) ON DELETE CASCADE,
    price_coins          INTEGER NOT NULL DEFAULT 0,
    price_shards         INTEGER NOT NULL DEFAULT 0,
    discount_percent     INTEGER NOT NULL DEFAULT 0,
    featured             BOOLEAN NOT NULL DEFAULT FALSE,
    featured_until       TIMESTAMPTZ,
    bundle_ids           TEXT[] NOT NULL DEFAULT '{}'
);

-- Season pass.
CREATE TABLE IF NOT EXISTS season_pass (
    season              INTEGER PRIMARY KEY,
    name                TEXT NOT NULL,
    starts_at           TIMESTAMPTZ NOT NULL,
    ends_at             TIMESTAMPTZ NOT NULL,
    free_track          JSONB NOT NULL DEFAULT '[]'::jsonb,   -- [{tier:1, rewards:["id1","id2"], xp:1000}]
    premium_track       JSONB NOT NULL DEFAULT '[]'::jsonb
);

CREATE TABLE IF NOT EXISTS player_season_progress (
    user_id             BIGINT REFERENCES users(id) ON DELETE CASCADE,
    season              INTEGER REFERENCES season_pass(season) ON DELETE CASCADE,
    xp                  BIGINT  NOT NULL DEFAULT 0,
    tier                INTEGER NOT NULL DEFAULT 0,
    premium_owned       BOOLEAN NOT NULL DEFAULT FALSE,
    claimed_free        INTEGER[] NOT NULL DEFAULT '{}',
    claimed_premium     INTEGER[] NOT NULL DEFAULT '{}',
    PRIMARY KEY (user_id, season)
);
