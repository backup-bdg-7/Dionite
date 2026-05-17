import React, { useEffect, useState, useMemo } from 'react';
import axios from 'axios';
import './index.css';

const API = `${process.env.REACT_APP_BACKEND_URL}/api`;

function Stat({ label, value, color = '#a5854c' }) {
  return (
    <div className="panel" style={{ padding: 16, minHeight: 96 }} data-testid={`stat-${label}`}>
      <div style={{ color: '#6e6862', fontSize: 11, textTransform: 'uppercase', letterSpacing: 1 }}>{label}</div>
      <div style={{ color, fontSize: 36, fontFamily: 'Cinzel Decorative, serif', marginTop: 8 }}>
        {value ?? '—'}
      </div>
    </div>
  );
}

function FileBrowser({ files, onPick, activePath }) {
  return (
    <div className="panel" style={{ padding: 12, maxHeight: 520, overflowY: 'auto' }}>
      {files.length === 0 && <div style={{ color: '#6e6862' }}>No files.</div>}
      {files.map((f) => (
        <button
          key={f.path}
          onClick={() => onPick(f.path)}
          data-testid={`file-${f.path}`}
          style={{
            display: 'block', width: '100%', textAlign: 'left',
            padding: '6px 10px', marginBottom: 2,
            background: activePath === f.path ? 'rgba(165,133,76,0.18)' : 'transparent',
            border: '1px solid ' + (activePath === f.path ? '#a5854c' : 'transparent'),
            color: '#e4dcd3', cursor: 'pointer', fontFamily: 'JetBrains Mono, monospace',
            fontSize: 12,
          }}
        >
          <span style={{ color: '#6e6862' }}>{f.lines.toString().padStart(4)} L · </span>
          {f.path}
        </button>
      ))}
    </div>
  );
}

export default function App() {
  const [manifest, setManifest] = useState(null);
  const [err, setErr] = useState('');
  const [tab, setTab] = useState('cpp');
  const [activePath, setActivePath] = useState('');
  const [fileContent, setFileContent] = useState('');
  const [filter, setFilter] = useState('');

  useEffect(() => {
    axios.get(`${API}/manifest`).then((r) => setManifest(r.data)).catch((e) => setErr(e.message));
  }, []);

  useEffect(() => {
    if (!activePath) { setFileContent(''); return; }
    axios.get(`${API}/file`, { params: { path: activePath } })
      .then((r) => setFileContent(r.data.content || `// ${r.data.error || 'no content'}`))
      .catch((e) => setFileContent(`// ${e.message}`));
  }, [activePath]);

  const moduleKeyMap = { cpp: 'cpp', ios: 'swift_ios', android: 'android', server: 'server', assets: 'assets_data', docs: 'docs' };
  const currentFiles = useMemo(() => {
    if (!manifest) return [];
    const list = manifest.modules[moduleKeyMap[tab]] || [];
    if (!filter) return list;
    const f = filter.toLowerCase();
    return list.filter((x) => x.path.toLowerCase().includes(f));
  }, [manifest, tab, filter]);

  return (
    <div className="app-shell vignette" style={{ overflow: 'auto' }}>
      <div className="ember" style={{ left: '15%', bottom: 0, animationDelay: '0s' }} />
      <div className="ember" style={{ left: '32%', bottom: 0, animationDelay: '1.5s' }} />
      <div className="ember" style={{ left: '58%', bottom: 0, animationDelay: '3s' }} />
      <div className="ember" style={{ left: '78%', bottom: 0, animationDelay: '4.5s' }} />

      <header style={{ padding: '40px 48px 12px', borderBottom: '1px solid #3a3632', position: 'relative' }}>
        <div style={{ fontFamily: 'JetBrains Mono, monospace', color: '#6e6862', fontSize: 11, letterSpacing: 3 }}>
          STUDIO STARTER KIT · v1.0.0
        </div>
        <h1 className="h-heading" style={{ fontSize: 56, margin: '8px 0 6px', color: '#e4dcd3',
            textShadow: '0 0 18px rgba(165,133,76,0.4), 0 2px 6px #000' }}>
          DIONITE
        </h1>
        <div className="h-heading" style={{ color: '#a5854c', letterSpacing: 4, fontSize: 14 }}>
          SHATTERED  ·  WILDS
        </div>
        <p style={{ maxWidth: 720, marginTop: 18, color: '#a39e96', fontSize: 15, lineHeight: 1.6 }}>
          An open-world ARPG blending <em>Elden Ring</em> exploration, <em>Diablo</em> loot depth, and <em>Fortnite</em> gun
          modularity. This repository ships the complete source scaffolding — C++ shared core, Swift + Metal iOS layer,
          Android JNI scaffold, Node.js + PostgreSQL backend, and React admin dashboard.
        </p>
        <div className="panel-gold" style={{ marginTop: 18, padding: 14, maxWidth: 720, fontSize: 13 }}>
          ⚠ This is a <strong>studio starter kit</strong>. It will NOT compile inside this preview container — take it
          to a Mac + Xcode for iOS, or a desktop with CMake + Clang for native builds. See{' '}
          <code style={{ color: '#fbbf24' }}>docs/build-instructions.md</code>.
        </div>
      </header>

      {err && <div style={{ color: '#e11d48', padding: 16 }}>{err}</div>}

      {manifest && (
        <main style={{ padding: '24px 48px 64px' }}>
          <section style={{ display: 'grid', gridTemplateColumns: 'repeat(4, 1fr)', gap: 16, marginBottom: 32 }}>
            <Stat label="Total Files" value={manifest.totals.total_files} />
            <Stat label="Total Lines" value={manifest.totals.total_lines.toLocaleString()} />
            <Stat label="C++ / Game Core" value={`${manifest.totals.cpp_files} files · ${manifest.totals.cpp_lines.toLocaleString()} lines`} color="#3b82f6" />
            <Stat label="Swift / Metal iOS" value={`${manifest.totals.swift_files} files · ${manifest.totals.swift_lines.toLocaleString()} lines`} color="#fbbf24" />
            <Stat label="Android (JNI)" value={`${manifest.totals.android_files} files`} color="#16a34a" />
            <Stat label="Server (Node+PG)" value={`${manifest.totals.server_files} files · ${manifest.totals.server_lines.toLocaleString()} lines`} color="#a855f7" />
            <Stat label="Data / Assets" value={`${manifest.totals.data_files} files`} color="#f97316" />
            <Stat label="Docs" value={`${manifest.totals.docs_files} files`} color="#e11d48" />
          </section>

          <section>
            <div style={{ display: 'flex', gap: 8, marginBottom: 12, flexWrap: 'wrap' }}>
              {[
                { id: 'cpp', label: 'C++ Core', color: '#3b82f6' },
                { id: 'ios', label: 'iOS (Swift + Metal)', color: '#fbbf24' },
                { id: 'android', label: 'Android', color: '#16a34a' },
                { id: 'server', label: 'Server', color: '#a855f7' },
                { id: 'assets', label: 'Data / JSON', color: '#f97316' },
                { id: 'docs', label: 'Docs', color: '#e11d48' },
              ].map((t) => (
                <button
                  key={t.id}
                  className="btn-arcane"
                  data-testid={`tab-${t.id}`}
                  onClick={() => { setTab(t.id); setActivePath(''); setFilter(''); }}
                  style={{ borderColor: tab === t.id ? t.color : '#3a3632', color: tab === t.id ? t.color : '#a39e96' }}>
                  {t.label}
                </button>
              ))}
            </div>

            <input
              data-testid="file-filter"
              value={filter}
              onChange={(e) => setFilter(e.target.value)}
              placeholder="Filter files..."
              className="input-arcane"
              style={{ marginBottom: 12 }}
            />

            <div style={{ display: 'grid', gridTemplateColumns: '380px 1fr', gap: 16 }}>
              <FileBrowser files={currentFiles} onPick={setActivePath} activePath={activePath} />
              <div className="panel" style={{ padding: 0, overflow: 'hidden', minHeight: 520, maxHeight: 720 }}>
                {!activePath && (
                  <div style={{ padding: 24, color: '#6e6862' }}>
                    Pick a file on the left to preview it. {currentFiles.length} files in the {tab} module.
                  </div>
                )}
                {activePath && (
                  <>
                    <div style={{ padding: '10px 14px', borderBottom: '1px solid #3a3632',
                                  fontFamily: 'JetBrains Mono, monospace', fontSize: 12, color: '#a5854c',
                                  display: 'flex', justifyContent: 'space-between' }}>
                      <span data-testid="active-file-path">{activePath}</span>
                      <button className="btn-arcane" style={{ padding: '4px 12px', fontSize: 11 }}
                        onClick={() => navigator.clipboard?.writeText(fileContent)}
                        data-testid="copy-content-btn">
                        Copy
                      </button>
                    </div>
                    <pre style={{ margin: 0, padding: 16, overflow: 'auto', maxHeight: 660,
                                  fontFamily: 'JetBrains Mono, monospace', fontSize: 12, lineHeight: 1.55,
                                  color: '#e4dcd3', background: '#050505' }}>
{fileContent}
                    </pre>
                  </>
                )}
              </div>
            </div>
          </section>

          <section style={{ marginTop: 48 }}>
            <h2 className="h-heading" style={{ color: '#a5854c', letterSpacing: 3 }}>NEXT STEPS</h2>
            <ol style={{ color: '#a39e96', lineHeight: 1.8, maxWidth: 720 }}>
              <li><strong>iOS:</strong> Clone repo to a Mac, open <code>platforms/ios/Dionite/</code> in Xcode 15+, link the static library built by CMake (<code>cmake -G Xcode -S . -B build_ios -DCMAKE_SYSTEM_NAME=iOS</code>), sign with your Apple Developer team, archive, ship to TestFlight.</li>
              <li><strong>Backend:</strong> <code>cd server &amp;&amp; docker compose up --build</code>. The admin dashboard will be at <code>http://localhost:5173</code>; API at <code>http://localhost:4000</code>.</li>
              <li><strong>Desktop validation:</strong> <code>cmake -S . -B build -DDIONITE_USE_BUNDLED=ON &amp;&amp; cmake --build build -j &amp;&amp; ./build/DioniteDesktop</code> runs a headless harness exercising every subsystem.</li>
              <li><strong>Debug iteration:</strong> Report compile errors and runtime issues — you mentioned you'll handle debugging. Send the errors and I'll patch the source.</li>
            </ol>
          </section>
        </main>
      )}
    </div>
  );
}
