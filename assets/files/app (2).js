// ✅ CAMBIA ESTO: IP de la PC B donde corre Flask
const BACKEND_BASE = "http://172.22.26.212:5000";

const $ = (id) => document.getElementById(id);

const els = {
  status: $("status"),
  colorPicker: $("colorPicker"),
  colorHex: $("colorHex"),
  colorPreview: $("colorPreview"),
  countRange: $("countRange"),
  countValue: $("countValue"),
  countMaxLabel: $("countMaxLabel"),
  btnSend: $("btnSend"),
  btnRefresh: $("btnRefresh"),
  msg: $("msg"),
  dot: $("dot"),
  stateText: $("stateText"),
  log: $("log"),
};

const swatches = document.querySelectorAll(".swatch");

function logLine(text){
  const ts = new Date().toLocaleTimeString();
  els.log.innerHTML = `<div>[${ts}] ${text}</div>` + els.log.innerHTML;
}

function setBadge(text, ok){
  els.status.textContent = text;
  els.status.style.color = ok ? "var(--ok)" : "var(--bad)";
}

function setMsg(text, ok=true){
  els.msg.textContent = text || "";
  els.msg.style.color = ok ? "var(--ok)" : "var(--bad)";
}

function updatePreview(color){
  els.colorPreview.style.background = color;
  els.colorPreview.style.boxShadow = `0 0 25px ${color}88`;
}

async function getState(){
  const r = await fetch(`${BACKEND_BASE}/api/state`);
  return await r.json();
}

function renderState(st){
  els.dot.style.background = st.color;
  els.stateText.textContent = `color=${st.color} count=${st.count} rev=${st.rev}`;

  els.colorPicker.value = st.color;
  els.colorHex.value = st.color;
  updatePreview(st.color);

  els.countRange.value = st.count;
  els.countValue.textContent = st.count;
}

async function sendState(){
  const color = els.colorHex.value.trim();
  const count = parseInt(els.countRange.value, 10);

  const r = await fetch(`${BACKEND_BASE}/api/state`, {
    method: "POST",
    headers: {"Content-Type":"application/json"},
    body: JSON.stringify({color, count})
  });

  const data = await r.json();
  if(!r.ok){
    setMsg(data.error || "Error", false);
    logLine(`ERROR: ${data.error || "Error"}`);
    return;
  }

  setMsg("✅ Enviado al Backend", true);
  renderState(data);
  logLine(`POST ok: color=${data.color} count=${data.count} rev=${data.rev}`);
}

async function refresh(){
  try{
    // health check (por si hay firewall)
    const hr = await fetch(`${BACKEND_BASE}/api/health`);
    if(!hr.ok) throw new Error("health failed");

    const st = await getState();
    renderState(st);

    setBadge("Conectado", true);
    setMsg("Refrescado", true);
    logLine(`GET state: color=${st.color} count=${st.count} rev=${st.rev}`);
  }catch(e){
    setBadge("Desconectado", false);
    setMsg("No se pudo conectar al Backend (revisa IP/Firewall)", false);
  }
}

function setup(){
  els.colorPicker.addEventListener("input", () => {
    els.colorHex.value = els.colorPicker.value;
    updatePreview(els.colorPicker.value);
  });

  els.colorHex.addEventListener("input", () => {
    const v = els.colorHex.value;
    if(/^#[0-9a-fA-F]{6}$/.test(v)){
      els.colorPicker.value = v;
      updatePreview(v);
    }
  });

  swatches.forEach(s => {
    s.addEventListener("click", () => {
      const color = s.dataset.color;
      els.colorPicker.value = color;
      els.colorHex.value = color;
      updatePreview(color);
    });
  });

  els.countRange.addEventListener("input", () => {
    els.countValue.textContent = els.countRange.value;
  });

  els.btnSend.onclick = sendState;
  els.btnRefresh.onclick = refresh;

  updatePreview(els.colorPicker.value);
}

(async function init(){
  setup();
  els.countMaxLabel.textContent = els.countRange.max;
  await refresh();
  logLine("FrontEnd listo");
})();