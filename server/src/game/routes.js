// ============================================================================
// Dionite Server — game routes (save/load, spire leaderboard, item templates)
// ============================================================================
const express = require('express');
const { pool } = require('../database/models');
const { authMiddleware } = require('../auth/jwt');

const router = express.Router();

router.get('/save', authMiddleware(true), async (req, res, next) => {
  try {
    const r = await pool.query('SELECT blob FROM saves WHERE user_id=$1', [req.user.sub]);
    if (!r.rowCount) return res.json({});
    res.json(r.rows[0].blob);
  } catch (e) { next(e); }
});

router.put('/save', authMiddleware(true), async (req, res, next) => {
  try {
    const blob = req.body || {};
    await pool.query(
      `INSERT INTO saves (user_id, blob, updated_at)
       VALUES ($1, $2, NOW())
       ON CONFLICT (user_id) DO UPDATE SET blob = EXCLUDED.blob, updated_at = NOW()`,
      [req.user.sub, blob]
    );
    res.json({ ok: true });
  } catch (e) { next(e); }
});

router.post('/save/reset', authMiddleware(true), async (req, res, next) => {
  try {
    await pool.query('UPDATE saves SET blob = $1, updated_at = NOW() WHERE user_id=$2', [defaultSave(), req.user.sub]);
    res.json({ ok: true });
  } catch (e) { next(e); }
});

router.post('/spire/run', authMiddleware(true), async (req, res, next) => {
  try {
    const { floor, score, biome, time_sec } = req.body;
    await pool.query(
      'INSERT INTO spire_runs (user_id, floor, score, biome, time_sec) VALUES ($1,$2,$3,$4,$5)',
      [req.user.sub, floor | 0, score | 0, String(biome), Number(time_sec) || 0]
    );
    res.json({ ok: true });
  } catch (e) { next(e); }
});

router.get('/spire/leaderboard', async (req, res, next) => {
  try {
    const r = await pool.query(`
      SELECT sr.floor, sr.score, sr.biome, sr.time_sec, u.name, u.id AS user_id
      FROM spire_runs sr
      JOIN users u ON u.id = sr.user_id
      ORDER BY sr.floor DESC, sr.score DESC
      LIMIT 50`);
    res.json(r.rows);
  } catch (e) { next(e); }
});

function defaultSave() {
  return {
    level: 1, xp: 0, gold: 100, soul_fragments: 0, skill_points: 0, spent_skills: {},
    health_max: 100, mana_max: 50, inventory: [], equipped: {}, bank: [],
    runes_owned: {}, gems_owned: {}, spire_best_floor: 0, campaign_progress: 0,
    unlocked_biomes: ['verdant_wilds'], play_seconds: 0, last_biome: 'verdant_wilds',
  };
}

module.exports = router;
