// ============================================================================
// Dionite Server — Analytics (event capture for balance tuning)
// ============================================================================
const events = [];
module.exports = {
  capture(userId, eventName, payload = {}) {
    events.push({ userId, eventName, payload, t: Date.now() });
    if (events.length > 100000) events.shift();
  },
  drain() { const out = events.slice(); events.length = 0; return out; },
  snapshot() { return events.slice(-1000); },
};
