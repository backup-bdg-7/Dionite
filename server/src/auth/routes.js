// ============================================================================
// Dionite Server — auth routes (register, login, refresh, /me, apple sign-in)
// ============================================================================
const express = require('express');
const Joi = require('joi');
const appleSignIn = require('apple-signin-auth');
const { pool } = require('../database/models');
const {
  hashPassword, verifyPassword, signAccess, signRefresh, verifyToken, authMiddleware
} = require('./jwt');

const router = express.Router();

const RegisterSchema = Joi.object({
  email: Joi.string().email().required(),
  password: Joi.string().min(6).required(),
  name: Joi.string().min(1).max(40).optional(),
});

router.post('/register', async (req, res, next) => {
  try {
    const { error, value } = RegisterSchema.validate(req.body);
    if (error) return res.status(400).json({ error: error.details[0].message });
    const email = value.email.toLowerCase();
    const existing = await pool.query('SELECT id FROM users WHERE email=$1', [email]);
    if (existing.rowCount) return res.status(409).json({ error: 'Email already registered' });
    const hash = hashPassword(value.password);
    const ins = await pool.query(
      "INSERT INTO users (email, password_hash, name, role) VALUES ($1,$2,$3,'player') RETURNING id, email, name, role",
      [email, hash, value.name || email.split('@')[0]]
    );
    const u = ins.rows[0];
    await pool.query('INSERT INTO saves (user_id, blob) VALUES ($1, $2)', [u.id, defaultSave()]);
    res.json({
      id: u.id, email: u.email, name: u.name, role: u.role,
      access_token: signAccess(u), refresh_token: signRefresh(u),
    });
  } catch (e) { next(e); }
});

router.post('/login', async (req, res, next) => {
  try {
    const email = String(req.body.email || '').toLowerCase();
    const password = String(req.body.password || '');
    const r = await pool.query('SELECT id, email, name, role, password_hash FROM users WHERE email=$1', [email]);
    if (!r.rowCount) return res.status(401).json({ error: 'Invalid email or password' });
    const u = r.rows[0];
    if (!u.password_hash || !verifyPassword(password, u.password_hash))
      return res.status(401).json({ error: 'Invalid email or password' });
    res.json({
      id: u.id, email: u.email, name: u.name, role: u.role,
      access_token: signAccess(u), refresh_token: signRefresh(u),
    });
  } catch (e) { next(e); }
});

router.post('/refresh', async (req, res) => {
  const payload = verifyToken(String(req.body.refresh_token || ''));
  if (!payload || payload.type !== 'refresh') return res.status(401).json({ error: 'Invalid refresh' });
  const r = await pool.query('SELECT id, email, role FROM users WHERE id=$1', [payload.sub]);
  if (!r.rowCount) return res.status(401).json({ error: 'User not found' });
  res.json({ access_token: signAccess(r.rows[0]) });
});

router.get('/me', authMiddleware(true), async (req, res) => {
  const r = await pool.query('SELECT id, email, name, role, created_at FROM users WHERE id=$1', [req.user.sub]);
  if (!r.rowCount) return res.status(404).json({ error: 'User not found' });
  res.json(r.rows[0]);
});

router.post('/apple', async (req, res, next) => {
  try {
    const { identity_token, auth_code } = req.body;
    const data = await appleSignIn.verifyIdToken(identity_token, { audience: process.env.APPLE_BUNDLE_ID || 'com.dionite.shatteredwilds' });
    const sub = data.sub;
    const email = data.email || `${sub}@privaterelay.appleid.com`;
    let user = (await pool.query('SELECT id, email, name, role FROM users WHERE apple_subject=$1', [sub])).rows[0];
    if (!user) {
      const ins = await pool.query(
        "INSERT INTO users (email, apple_subject, name, role) VALUES ($1,$2,$3,'player') RETURNING id, email, name, role",
        [email, sub, 'Apple Player']
      );
      user = ins.rows[0];
      await pool.query('INSERT INTO saves (user_id, blob) VALUES ($1, $2)', [user.id, defaultSave()]);
    }
    res.json({ id: user.id, email: user.email, name: user.name, role: user.role, access_token: signAccess(user), refresh_token: signRefresh(user) });
  } catch (e) { next(e); }
});

function defaultSave() {
  return {
    level: 1, xp: 0, gold: 100, soul_fragments: 0, skill_points: 0, spent_skills: {},
    health_max: 100, mana_max: 50,
    inventory: [], equipped: {}, bank: [], runes_owned: {}, gems_owned: {},
    spire_best_floor: 0, campaign_progress: 0,
    unlocked_biomes: ['verdant_wilds'], play_seconds: 0, last_biome: 'verdant_wilds',
  };
}

module.exports = router;
