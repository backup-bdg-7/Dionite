// ============================================================================
// Dionite Server — Economy service (server-authoritative pricing & rates)
// ============================================================================
module.exports = {
  sellPrice(item) {
    return Math.floor(20 + (item.damage || 10) * 2 * (1 + (item.rarity_idx || 0)));
  },
  buyPrice(item) {
    return Math.floor(80 + (item.damage || 10) * 5 * (1 + (item.rarity_idx || 0)));
  },
  spireEntryFee(floor) { return floor * 50; },
  goldDropMultiplier(level) { return 1 + level / 8; },
};
