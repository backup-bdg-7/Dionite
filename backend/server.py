"""Dionite — Studio Manifest stub backend.

This is NOT the game's production backend. The real backend lives under
/app/server/ (Node.js + PostgreSQL) and must be deployed separately.

This minimal FastAPI app exists ONLY so the Emergent preview container has a
healthy `/api/health` and a `/api/manifest` endpoint that powers the landing
page showing the contents of this Studio Starter Kit.
"""
from dotenv import load_dotenv
load_dotenv()

import os
import subprocess
from pathlib import Path
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

ROOT = Path("/app")
app = FastAPI(title="Dionite Studio Manifest")
app.add_middleware(CORSMiddleware, allow_origins=["*"], allow_methods=["*"], allow_headers=["*"])


def _count_lines(path: Path) -> int:
    try:
        return sum(1 for _ in path.open("r", encoding="utf-8", errors="ignore"))
    except Exception:
        return 0


def _collect(root: Path, exts: tuple[str, ...]) -> list[dict]:
    out: list[dict] = []
    skip = {"node_modules", "build", ".git", ".emergent", "build_ios", "dist"}
    for p in root.rglob("*"):
        if p.is_dir():
            continue
        if any(part in skip for part in p.parts):
            continue
        if p.suffix.lower() in exts:
            try:
                rel = p.relative_to(ROOT).as_posix()
            except ValueError:
                continue
            out.append({"path": rel, "size": p.stat().st_size, "lines": _count_lines(p)})
    return out


@app.get("/api/health")
def health():
    return {"ok": True, "service": "studio-manifest"}


@app.get("/api/manifest")
def manifest():
    cpp = _collect(ROOT / "src", (".h", ".cpp"))
    swift = _collect(ROOT / "platforms" / "ios", (".swift", ".metal", ".plist", ".cpp"))
    android = _collect(ROOT / "platforms" / "android", (".java", ".gradle"))
    server = _collect(ROOT / "server", (".js", ".jsx", ".sql", ".yml"))
    assets = _collect(ROOT / "assets", (".json",))
    docs = _collect(ROOT / "docs", (".md",))
    totals = {
        "cpp_files": len(cpp), "cpp_lines": sum(x["lines"] for x in cpp),
        "swift_files": len(swift), "swift_lines": sum(x["lines"] for x in swift),
        "android_files": len(android), "android_lines": sum(x["lines"] for x in android),
        "server_files": len(server), "server_lines": sum(x["lines"] for x in server),
        "data_files": len(assets), "data_lines": sum(x["lines"] for x in assets),
        "docs_files": len(docs), "docs_lines": sum(x["lines"] for x in docs),
    }
    totals["total_files"] = sum(v for k, v in totals.items() if k.endswith("_files"))
    totals["total_lines"] = sum(v for k, v in totals.items() if k.endswith("_lines"))
    return {
        "project": "Dionite — Shattered Wilds",
        "kind": "Studio Starter Kit (Option B)",
        "totals": totals,
        "modules": {
            "cpp": sorted(cpp, key=lambda x: x["path"]),
            "swift_ios": sorted(swift, key=lambda x: x["path"]),
            "android": sorted(android, key=lambda x: x["path"]),
            "server": sorted(server, key=lambda x: x["path"]),
            "assets_data": sorted(assets, key=lambda x: x["path"]),
            "docs": sorted(docs, key=lambda x: x["path"]),
        },
    }


@app.get("/api/file")
def get_file(path: str):
    target = (ROOT / path).resolve()
    try:
        target.relative_to(ROOT)
    except ValueError:
        return {"error": "outside workspace"}
    if not target.exists() or not target.is_file():
        return {"error": "not found"}
    try:
        text = target.read_text(encoding="utf-8", errors="ignore")
    except Exception as e:
        return {"error": str(e)}
    return {"path": path, "size": target.stat().st_size, "content": text[:200000]}


@app.get("/api/")
def root():
    return {"service": "Dionite Studio Manifest", "see": "/api/manifest"}
