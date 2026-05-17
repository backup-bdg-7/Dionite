// ============================================================================
// Dionite Server — JWT auth utilities
// ============================================================================
const jwt = require('jsonwebtoken');
const bcrypt = require('bcryptjs');

const SECRET = process.env.JWT_SECRET || 'dev-secret-CHANGE-ME';
const ACCESS_EXPIRES = '24h';
const REFRESH_EXPIRES = '30d';

function hashPassword(pw) { return bcrypt.hashSync(pw, 10); }
function verifyPassword(pw, hash) { return bcrypt.compareSync(pw, hash); }

function signAccess(user) {
  return jwt.sign({ sub: user.id, email: user.email, role: user.role, type: 'access' }, SECRET, { expiresIn: ACCESS_EXPIRES });
}

function signRefresh(user) {
  return jwt.sign({ sub: user.id, type: 'refresh' }, SECRET, { expiresIn: REFRESH_EXPIRES });
}

function verifyToken(tok) {
  try { return jwt.verify(tok, SECRET); } catch (e) { return null; }
}

function authMiddleware(required = true) {
  return (req, res, next) => {
    const h = req.headers.authorization || '';
    const tok = h.startsWith('Bearer ') ? h.slice(7) : null;
    const payload = tok ? verifyToken(tok) : null;
    if (required && (!payload || payload.type !== 'access')) {
      return res.status(401).json({ error: 'Not authenticated' });
    }
    req.user = payload;
    next();
  };
}

function adminMiddleware(req, res, next) {
  if (!req.user || req.user.role !== 'admin') return res.status(403).json({ error: 'Admin only' });
  next();
}

module.exports = { hashPassword, verifyPassword, signAccess, signRefresh, verifyToken, authMiddleware, adminMiddleware };
