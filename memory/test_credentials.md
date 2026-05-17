# Dionite — Test Credentials

These are the **default seeded** credentials for the Node.js backend admin (only after you run the seed migration).
For the local Postgres backend, you would manually run:

```sql
-- After registering admin@dionite.game via /api/auth/register, promote them:
UPDATE users SET role = 'admin' WHERE email = 'admin@dionite.game';
```

## Backend Admin (admin dashboard)
- **URL:** `http://localhost:5173`  (after `cd server && docker compose up`)
- **Email:** `admin@dionite.game`
- **Password:** `admin123`  (set yourself during registration)
- **Role:** admin

## Test Player
- **Email:** `player@dionite.game`
- **Password:** `password123` (set yourself)

## Auth Endpoints
- POST `/api/auth/register`
- POST `/api/auth/login`
- GET `/api/auth/me`
- POST `/api/auth/refresh`
- POST `/api/auth/apple`

## Notes
- The preview `/app/backend` FastAPI stub does NOT seed any admin — it only serves the Studio Manifest endpoints (`/api/manifest`, `/api/file`).
- Real auth/admin lives in `/app/server` (Node.js + PostgreSQL).
