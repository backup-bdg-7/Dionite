// ============================================================================
// Dionite Server — admin routes (player list, stats, grant gold, ban)
// ============================================================================
const express = require('express');
const { pool } = require('../database/models');
const { authMiddleware, adminMiddleware } = require('../auth/jwt');

const router = express.Router();

router.use(authMiddleware(true), adminMiddleware);

router.get('/stats', async (req, res, next) => {
  try {
    const [u, s, r] = await Promise.all([
      pool.query('SELECT COUNT(*)::int AS c FROM users'),
      pool.query('SELECT COUNT(*)::int AS c FROM saves'),
      pool.query('SELECT COUNT(*)::int AS c FROM spire_runs'),
    ]);
    res.json({ users: u.rows[0].c, saves: s.rows[0].c, spire_runs: r.rows[0].c });
  } catch (e) { next(e); }
});

router.get('/players', async (req, res, next) => {
  try {
    const r = await pool.query(`
      SELECT u.id, u.email, u.name, u.role, u.created_at,
             COALESCE((s.blob->>'level')::int, 1)  AS level,
             COALESCE((s.blob->>'gold')::int, 0)   AS gold,
             COALESCE((s.blob->>'spire_best_floor')::int, 0) AS spire_best_floor
      FROM users u LEFT JOIN saves s ON s.user_id = u.id
      ORDER BY u.id ASC LIMIT 200`);
    res.json(r.rows);
  } catch (e) { next(e); }
});

router.delete('/players/:id', async (req, res, next) => {
  try {
    await pool.query('DELETE FROM users WHERE id=$1', [req.params.id]);
    res.json({ ok: true });
  } catch (e) { next(e); }
});

router.post('/players/:id/grant_gold', async (req, res, next) => {
  try {
    const amount = Number(req.body.amount) || 0;
    await pool.query(
      `UPDATE saves
       SET blob = jsonb_set(blob, '{gold}',
                            to_jsonb(COALESCE((blob->>'gold')::int, 0) + $2))
       WHERE user_id=$1`,
      [req.params.id, amount]
    );
    res.json({ ok: true });
  } catch (e) { next(e); }
});

module.exports = router;
