import json
import os
import re
import tempfile
from datetime import datetime, timezone
from flask import Flask, request, jsonify

# Sirve /static automáticamente
app = Flask(__name__, static_folder="static")

APP_DIR = os.path.dirname(os.path.abspath(__file__))
DATA_DIR = os.path.join(APP_DIR, "data")
STATE_FILE = os.path.join(DATA_DIR, "state.json")

HEX_COLOR_RE = re.compile(r"^#[0-9a-fA-F]{6}$")
LED_MIN, LED_MAX = 0, 300  # ajusta si quieres


def now_iso():
    return datetime.now(timezone.utc).isoformat()


def atomic_write(path: str, content: str):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    fd, tmp_path = tempfile.mkstemp(prefix="tmp_", suffix=".tmp", dir=os.path.dirname(path))
    try:
        with os.fdopen(fd, "w", encoding="utf-8") as f:
            f.write(content)
        os.replace(tmp_path, path)
    finally:
        if os.path.exists(tmp_path):
            try:
                os.remove(tmp_path)
            except OSError:
                pass


def default_state():
    return {"color": "#ff0000", "count": 8, "rev": 1, "updated_at": now_iso()}


def load_state():
    os.makedirs(DATA_DIR, exist_ok=True)

    if not os.path.exists(STATE_FILE):
        st = default_state()
        atomic_write(STATE_FILE, json.dumps(st, ensure_ascii=False))
        return st

    try:
        with open(STATE_FILE, "r", encoding="utf-8") as f:
            st = json.load(f)
        st.setdefault("color", "#ff0000")
        st.setdefault("count", 8)
        st.setdefault("rev", 1)
        st.setdefault("updated_at", now_iso())
        return st
    except Exception:
        st = default_state()
        atomic_write(STATE_FILE, json.dumps(st, ensure_ascii=False))
        return st


def validate_state(color, count):
    if not isinstance(color, str) or not HEX_COLOR_RE.match(color):
        return False, "color inválido (usa #RRGGBB)"
    if not isinstance(count, int) or not (LED_MIN <= count <= LED_MAX):
        return False, f"count inválido (debe estar entre {LED_MIN} y {LED_MAX})"
    return True, ""


def save_state(color: str, count: int):
    st = load_state()
    st["color"] = color
    st["count"] = count
    st["rev"] = int(st.get("rev", 1)) + 1
    st["updated_at"] = now_iso()
    atomic_write(STATE_FILE, json.dumps(st, ensure_ascii=False))
    return st

#
@app.get("/")
def index():
    # Sirve static/index.html
    return app.send_static_file("index.html")


@app.get("/api/health")
def health():
    return jsonify({"ok": True, "time": now_iso()})


@app.get("/api/state")
def get_state():
    return jsonify(load_state())


@app.post("/api/state")
def set_state():
    payload = request.get_json(silent=True) or {}
    color = payload.get("color")
    count = payload.get("count")

    try:
        count = int(count)
    except Exception:
        pass

    ok, msg = validate_state(color, count)
    if not ok:
        return jsonify({"ok": False, "error": msg}), 400

    st = save_state(color, count)
    return jsonify({"ok": True, **st})


if __name__ == "__main__":
    # 0.0.0.0 para celular/ESP32 (misma red)
    app.run(host="0.0.0.0", port=5500, debug=True)
