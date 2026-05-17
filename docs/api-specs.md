# Dionite — API Specification

Base URL: `https://api.dionite.game/api` (or `http://localhost:4000/api` for dev).
All authenticated endpoints require `Authorization: Bearer <access_token>`.

## Auth

### POST `/auth/register`
```json
{ "email": "you@example.com", "password": "secret123", "name": "Optional" }
```
Response: `{ "id":1, "email":"…", "name":"…", "role":"player", "access_token":"…", "refresh_token":"…" }`

### POST `/auth/login`
Body: `{ "email":"…", "password":"…" }` — same response.

### POST `/auth/refresh`
Body: `{ "refresh_token":"…" }` → `{ "access_token":"…" }`

### GET `/auth/me`
Returns the current user object.

### POST `/auth/apple`
Body: `{ "identity_token":"…", "auth_code":"…" }` — verified against Apple, returns the same token bundle.

## Player Save (legacy single save — superseded by /characters)

### GET `/save`  *(auth)*
Returns the player's full save blob.

### PUT `/save`  *(auth)*
Stores the entire save blob (server overwrites; client owns the schema).
Shape:
```json
{
  "level": 1, "xp": 0, "gold": 100, "soul_fragments": 0,
  "skill_points": 0, "spent_skills": { "w_vigor1": 1 },
  "health_max": 100, "mana_max": 50,
  "inventory": [], "equipped": {}, "bank": [],
  "runes_owned": { "rune_sol": 2 }, "gems_owned": { "gem_ruby": 1 },
  "spire_best_floor": 12, "campaign_progress": 3,
  "unlocked_biomes": ["verdant_wilds","ashen_wastes"],
  "play_seconds": 4815, "last_biome": "verdant_wilds"
}
```

### POST `/save/reset` *(auth)*
Wipes the save back to default.

## Characters (multi-character roster, Diablo-style)

### GET `/characters` *(auth)*
List all characters for the signed-in user. Each row contains:
```json
{ "id": 12, "name": "Verith", "class_id": "crusader", "level": 56,
  "paragon_level": 12, "last_biome": "ashen_wastes", "spire_best_floor": 24,
  "play_seconds": 41020, "hardcore": false, "dead": false,
  "updated_at": "2026-05-17T14:00:00Z" }
```

### POST `/characters` *(auth)*
Create a new character. Body:
```json
{ "name": "Verith", "class_id": "crusader", "hardcore": false,
  "cosmetic": { "body": "default_body", "head": "default_head", "tint": "#A5854C", "sigil": "sigil_doom" } }
```
- `class_id` ∈ `crusader | necromancer | sorcerer | ranger | monk`
- Returns the full save blob for the new character.
- Max **6 characters per user**.

### GET `/characters/:id` *(auth)*
Returns the full save blob for the character (must be owned).

### PUT `/characters/:id` *(auth)*
Replaces the full save blob. Backend mirrors `level`, `paragon_level`,
`spire_best_floor`, `play_seconds`, `last_biome`, `dead` into top-level
columns for fast roster queries.

### DELETE `/characters/:id` *(auth)*
Deletes the character.

## Infinity Spire

### POST `/spire/run`  *(auth)*
Body: `{ "floor": 12, "score": 4500, "biome":"frozen_spire", "time_sec": 312.5 }`

### GET `/spire/leaderboard`
Top 50 runs, sorted by floor (desc) then score (desc).

## Matchmaking — Ghost Battles

### POST `/match/ghost` *(auth)*
Body: `{ "floor": 10, "score": 3200, "frames": [{ "t":0.0, "pos":{...}, "aim":{...}, "firing": true }, …] }`

### GET `/match/ghost/random/:floor`
Returns a random ghost record within ±2 floors of the requested floor.

### WebSocket `/ws/ghost?floor=N&token=ACCESS_TOKEN`
Bidirectional frame stream. Each peer connected at the same floor receives messages from other peers in real time.

## Admin

### GET `/admin/stats` *(admin)*
Returns `{ users, saves, spire_runs }`.

### GET `/admin/players` *(admin)*
List of player rows with summary stats.

### POST `/admin/players/:id/grant_gold` *(admin)*
Body: `{ "amount": 1000 }` — adds gold to player save.

### DELETE `/admin/players/:id` *(admin)*
Deletes the user and all their data.

## Error Format
```json
{ "error": "human readable message" }
```
HTTP status codes: 400/401/403/404/409/500.
