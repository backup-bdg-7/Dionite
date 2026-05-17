// ============================================================================
// Dionite Server — matchmaking routes (ghost battle records)
// ============================================================================
const express = require('express');
const { pool } = require('../database/models');
const { authMiddleware } = require('../auth/jwt');

const router = express.Router();

router.post('/ghost', authMiddleware(true), async (req, res, next) => {
  try {
    const { floor, score, frames } = req.body;
    await pool.query(
      'INSERT INTO ghost_records (user_id, floor, score, frames) VALUES ($1,$2,$3,$4)',
      [req.user.sub, floor | 0, score | 0, frames || []]
    );
    res.json({ ok: true });
  } catch (e) { next(e); }
});

// Get a random ghost at-or-near the given floor (within +/- 2)
router.get('/ghost/random/:floor', async (req, res, next) => {
  try {
    const f = parseInt(req.params.floor, 10) || 1;
    const r = await pool.query(`
      SELECT gr.id, gr.floor, gr.score, gr.frames, u.name
      FROM ghost_records gr
      JOIN users u ON u.id = gr.user_id
      WHERE gr.floor BETWEEN $1 AND $2
      ORDER BY random() LIMIT 1`,
      [Math.max(1, f - 2), f + 2]
    );
    if (!r.rowCount) return res.status(404).json({ error: 'No ghosts available' });
    res.json(r.rows[0]);
  } catch (e) { next(e); }
});

module.exports = router;
