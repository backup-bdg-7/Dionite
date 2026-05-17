// ============================================================================
// Dionite Server — multi-character roster + creation endpoints
// (Each user can have multiple characters, Diablo-style.)
// ============================================================================
const express = require('express');
const Joi = require('joi');
const { pool } = require('../database/models');
const { authMiddleware } = require('../auth/jwt');

const router = express.Router();

const CLASSES = ['crusader','necromancer','sorcerer','ranger','monk'];

const CreateSchema = Joi.object({
  name: Joi.string().min(2).max(20).required(),
  class_id: Joi.string().valid(...CLASSES).required(),
  hardcore: Joi.boolean().default(false),
  cosmetic: Joi.object({
    body: Joi.string().default('default_body'),
    head: Joi.string().default('default_head'),
    tint: Joi.string().default('#A5854C'),
    sigil: Joi.string().default('sigil_doom'),
  }).default(),
});

function defaultCharacterSave(classId, name, hardcore, cosmetic) {
  return {
    name, class_id: classId, hardcore: !!hardcore, cosmetic,
    level: 1, xp: 0, total_xp: 0, paragon_level: 0, paragon_spent: {}, paragon_glyphs: {},
    gold: 100, soul_fragments: 0, skill_points: 0,
    skill_ranks: {},     // skillId -> 0..4
    loadout: { active: [null, null, null, null, null, null], passives: [null, null, null, null] },
    spent_skills: {},
    health_max: 100, resource_max: 100, resource: 100,
    inventory: [], equipped: {}, bank: [], runes_owned: {}, gems_owned: {},
    spire_best_floor: 0, campaign_progress: 0,
    unlocked_biomes: ['verdant_wilds'], play_seconds: 0, last_biome: 'verdant_wilds',
    created_at: new Date().toISOString(),
  };
}

router.use(authMiddleware(true));

// List all characters owned by this user.
router.get('/characters', async (req, res, next) => {
  try {
    const r = await pool.query(
      `SELECT id, name, class_id, level, paragon_level, last_biome, spire_best_floor,
              play_seconds, hardcore, dead, updated_at
       FROM characters WHERE user_id=$1 ORDER BY updated_at DESC`,
      [req.user.sub]
    );
    res.json(r.rows);
  } catch (e) { next(e); }
});

// Create a new character (max 6 per user, like Diablo IV).
router.post('/characters', async (req, res, next) => {
  try {
    const { error, value } = CreateSchema.validate(req.body);
    if (error) return res.status(400).json({ error: error.details[0].message });
    const count = await pool.query('SELECT COUNT(*)::int AS c FROM characters WHERE user_id=$1', [req.user.sub]);
    if (count.rows[0].c >= 6) return res.status(409).json({ error: 'Character roster full (max 6).' });
    const dup = await pool.query('SELECT id FROM characters WHERE user_id=$1 AND name=$2', [req.user.sub, value.name]);
    if (dup.rowCount) return res.status(409).json({ error: 'Name already used on another character.' });
    const blob = defaultCharacterSave(value.class_id, value.name, value.hardcore, value.cosmetic);
    const ins = await pool.query(
      `INSERT INTO characters (user_id, name, class_id, hardcore, blob)
       VALUES ($1,$2,$3,$4,$5) RETURNING id`,
      [req.user.sub, value.name, value.class_id, value.hardcore, blob]
    );
    res.json({ id: ins.rows[0].id, ...blob });
  } catch (e) { next(e); }
});

// Load a single character's full blob.
router.get('/characters/:id', async (req, res, next) => {
  try {
    const r = await pool.query(
      `SELECT id, blob FROM characters WHERE user_id=$1 AND id=$2`,
      [req.user.sub, req.params.id]
    );
    if (!r.rowCount) return res.status(404).json({ error: 'Character not found.' });
    res.json({ id: r.rows[0].id, ...r.rows[0].blob });
  } catch (e) { next(e); }
});

// Save a character's full blob.
router.put('/characters/:id', async (req, res, next) => {
  try {
    const owns = await pool.query('SELECT id FROM characters WHERE user_id=$1 AND id=$2', [req.user.sub, req.params.id]);
    if (!owns.rowCount) return res.status(404).json({ error: 'Character not found.' });
    await pool.query(
      `UPDATE characters SET blob=$1,
                              level = COALESCE(($1->>'level')::int, level),
                              paragon_level = COALESCE(($1->>'paragon_level')::int, paragon_level),
                              spire_best_floor = COALESCE(($1->>'spire_best_floor')::int, spire_best_floor),
                              play_seconds = COALESCE(($1->>'play_seconds')::int, play_seconds),
                              last_biome = COALESCE($1->>'last_biome', last_biome),
                              dead = ($1->>'dead')::boolean,
                              updated_at = NOW()
        WHERE id=$2`,
      [req.body, req.params.id]
    );
    res.json({ ok: true });
  } catch (e) { next(e); }
});

router.delete('/characters/:id', async (req, res, next) => {
  try {
    const r = await pool.query('DELETE FROM characters WHERE user_id=$1 AND id=$2', [req.user.sub, req.params.id]);
    if (!r.rowCount) return res.status(404).json({ error: 'Character not found.' });
    res.json({ ok: true });
  } catch (e) { next(e); }
});

module.exports = router;
