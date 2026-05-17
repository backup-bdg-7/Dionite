// ============================================================================
// Dionite Server — WebSocket gateway for live ghost-battle streaming
// ============================================================================
const { WebSocketServer } = require('ws');
const { verifyToken } = require('../auth/jwt');

const rooms = new Map(); // floor -> Set<WebSocket>

function attachWs(httpServer) {
  const wss = new WebSocketServer({ server: httpServer, path: '/ws/ghost' });
  wss.on('connection', (ws, req) => {
    const url = new URL(req.url, 'http://localhost');
    const tok = url.searchParams.get('token') || '';
    const payload = verifyToken(tok);
    if (!payload) { ws.close(4001, 'unauthorized'); return; }
    const floor = parseInt(url.searchParams.get('floor') || '1', 10);
    if (!rooms.has(floor)) rooms.set(floor, new Set());
    rooms.get(floor).add(ws);

    ws.on('message', (raw) => {
      // Forward frames to other ghosts on the same floor.
      for (const peer of rooms.get(floor) || []) {
        if (peer !== ws && peer.readyState === 1) peer.send(raw);
      }
    });
    ws.on('close', () => { rooms.get(floor)?.delete(ws); });
  });
}

module.exports = { attachWs };
