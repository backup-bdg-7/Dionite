// ============================================================================
// Dionite — Paragon Board (Diablo IV-style endgame progression).
// Unlocks at level 100 (or earlier if user chooses). Infinite paragon levels.
// Each paragon level grants 1 paragon point. Players slot points into nodes
// arranged on a 21x21 board, with magic / rare / legendary / glyph sockets.
// Glyphs amplify nearby nodes within a radius.
// ============================================================================
#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace dionite::progression {

enum class ParagonNodeKind : uint8_t {
    Normal,    // small stat boost
    Magic,     // +5/+10/+15/+20 stat at 1/2/3/4 points
    Rare,      // bigger boost + condition
    Legendary, // unique passive
    GlyphSocket // accepts a glyph
};

struct ParagonNode {
    std::string id;
    int boardId;
    int x, y; // 0..20
    ParagonNodeKind kind;
    std::string statName;
    float perPoint;
    int maxPoints;       // 1 for Legendary, 4 typically for Magic
    std::string description;
    std::vector<std::string> neighbors; // adjacent node ids
};

struct ParagonGlyph {
    std::string id;
    std::string name;
    int level = 1;           // 1..21
    int radius = 3;          // tile radius of effect
    std::string stat;        // empowers nodes containing this stat
    float multiplier = 1.05f; // +5% per level
    std::string description;
};

struct ParagonBoard {
    std::string id;
    std::string name;
    std::vector<ParagonNode> nodes;
};

class ParagonSystem {
public:
    static ParagonSystem& instance() { static ParagonSystem p; p.init(); return p; }

    const std::vector<ParagonBoard>& boards() const { return boards_; }
    const std::vector<ParagonGlyph>& glyphs() const { return glyphCatalog_; }

    bool spend(const std::string& boardId, const std::string& nodeId,
               int& availablePoints,
               std::unordered_map<std::string, int>& spent) {
        if (availablePoints <= 0) return false;
        auto* n = findNode(boardId, nodeId);
        if (!n) return false;
        int already = spent[nodeId];
        if (already >= n->maxPoints) return false;
        // Require an allocated neighbor (except for board origin)
        if (already == 0 && !n->neighbors.empty()) {
            bool reached = false;
            for (auto& nb : n->neighbors) if (spent[nb] > 0) { reached = true; break; }
            if (!reached) return false;
        }
        spent[nodeId] = already + 1;
        availablePoints--;
        return true;
    }

    bool socketGlyph(const std::string& boardId, const std::string& nodeId,
                      const std::string& glyphId,
                      std::unordered_map<std::string, std::string>& socketed) {
        auto* n = findNode(boardId, nodeId);
        if (!n || n->kind != ParagonNodeKind::GlyphSocket) return false;
        socketed[nodeId] = glyphId;
        return true;
    }

    // Compute aggregated stats from spent allocations + glyph empowerment.
    std::unordered_map<std::string, float>
    aggregate(const std::unordered_map<std::string, int>& spent,
              const std::unordered_map<std::string, std::string>& glyphSockets) const {
        std::unordered_map<std::string, float> out;
        for (auto& b : boards_) {
            for (auto& n : b.nodes) {
                auto it = spent.find(n.id);
                if (it == spent.end() || it->second <= 0) continue;
                float amount = n.perPoint * it->second;
                // Glyph amplification: if any socketed glyph in this board is within
                // its radius of this node AND empowers this stat, multiply.
                for (auto& [socketNodeId, glyphId] : glyphSockets) {
                    auto* sn = findNode(b.id, socketNodeId);
                    if (!sn) continue;
                    const ParagonGlyph* g = findGlyph(glyphId);
                    if (!g) continue;
                    int dx = sn->x - n.x, dy = sn->y - n.y;
                    if (dx * dx + dy * dy > g->radius * g->radius) continue;
                    if (!g->stat.empty() && g->stat != n.statName) continue;
                    amount *= (1.f + (g->multiplier - 1.f) * g->level);
                }
                out[n.statName] += amount;
            }
        }
        return out;
    }

    int paragonLevelFor(uint64_t totalXp) const {
        // 1.5 million xp per paragon level past 100
        return std::max(0, (int)(totalXp / 1500000ULL));
    }

private:
    ParagonNode* findNode(const std::string& boardId, const std::string& nodeId) {
        for (auto& b : boards_) if (b.id == boardId)
            for (auto& n : b.nodes) if (n.id == nodeId) return &n;
        return nullptr;
    }
    const ParagonNode* findNode(const std::string& boardId, const std::string& nodeId) const {
        for (auto& b : boards_) if (b.id == boardId)
            for (auto& n : b.nodes) if (n.id == nodeId) return &n;
        return nullptr;
    }
    const ParagonGlyph* findGlyph(const std::string& id) const {
        for (auto& g : glyphCatalog_) if (g.id == id) return &g;
        return nullptr;
    }

    void init() {
        if (!boards_.empty()) return;
        // Starter board "Inception" — 21x21 grid. Real boards in JSON later.
        ParagonBoard b;
        b.id = "board_inception";
        b.name = "Inception";
        auto add = [&](const std::string& id, int x, int y, ParagonNodeKind k,
                       const std::string& stat, float per, int maxPts,
                       const std::string& desc, std::vector<std::string> nb) {
            b.nodes.push_back({id, 1, x, y, k, stat, per, maxPts, desc, std::move(nb)});
        };
        add("origin", 10, 10, ParagonNodeKind::Normal, "all", 1.f, 1, "Board origin.", {});
        add("n_str_1", 10, 9, ParagonNodeKind::Normal, "strength", 5.f, 4, "+5 Strength per point.", {"origin"});
        add("n_dex_1", 11, 10, ParagonNodeKind::Normal, "dexterity", 5.f, 4, "+5 Dexterity per point.", {"origin"});
        add("n_int_1", 10, 11, ParagonNodeKind::Normal, "intelligence", 5.f, 4, "+5 Int per point.", {"origin"});
        add("n_will_1", 9, 10, ParagonNodeKind::Normal, "willpower", 5.f, 4, "+5 Willpower per point.", {"origin"});
        add("magic_dmg_1", 11, 9, ParagonNodeKind::Magic, "dmgPct", 0.03f, 4, "+3% damage per point.", {"n_str_1","n_dex_1"});
        add("magic_def_1", 9, 9, ParagonNodeKind::Magic, "dmgReduce", 0.025f, 4, "+2.5% DR per point.", {"n_str_1","n_will_1"});
        add("rare_crit", 12, 9, ParagonNodeKind::Rare, "critChance", 0.05f, 4, "+5% crit chance per point.", {"magic_dmg_1"});
        add("legendary_keystone", 13, 9, ParagonNodeKind::Legendary, "lifesteal", 0.10f, 1,
            "Legendary: Crits restore 10% HP.", {"rare_crit"});
        add("socket_a", 14, 10, ParagonNodeKind::GlyphSocket, "", 0.f, 1, "Glyph socket.", {"legendary_keystone"});
        boards_.push_back(std::move(b));

        // Glyph catalog
        glyphCatalog_ = {
            {"glyph_might",   "Might",   1, 3, "strength",     1.05f, "+5% per glyph level to Strength nodes."},
            {"glyph_grace",   "Grace",   1, 3, "dexterity",    1.05f, "+5% per glyph level to Dexterity nodes."},
            {"glyph_genius",  "Genius",  1, 3, "intelligence", 1.05f, "+5% per glyph level to Intelligence nodes."},
            {"glyph_will",    "Resolve", 1, 3, "willpower",    1.05f, "+5% per glyph level to Willpower nodes."},
            {"glyph_destruction","Destruction",1,3,"dmgPct",   1.06f, "+6% per glyph level to damage nodes."},
            {"glyph_warding", "Warding", 1, 3, "dmgReduce",    1.05f, "+5% per glyph level to defense nodes."},
            {"glyph_executioner","Executioner",1,3,"critChance",1.08f,"+8% per glyph level to crit nodes."},
        };
    }

    std::vector<ParagonBoard> boards_;
    std::vector<ParagonGlyph> glyphCatalog_;
};

} // namespace dionite::progression
