import sys
import serial
import serial.tools.list_ports

from PyQt6.QtCore import QTimer, QSize
from PyQt6.QtGui import QPainter, QColor, QPen, QTextCursor
from PyQt6.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QHBoxLayout, QLabel,
    QComboBox, QPushButton, QTextEdit, QGroupBox
)

class LedIndicator(QWidget):
    def __init__(self, diameter=44):
        super().__init__()
        self._on = False
        self.setFixedSize(QSize(diameter, diameter))
        self._d = diameter

    def set_on(self, on: bool):
        self._on = on
        self.update()

    def paintEvent(self, e):
        p = QPainter(self)
        p.setRenderHint(QPainter.RenderHint.Antialiasing, True)

        if self._on:
            outer = QColor(0, 190, 90)
            inner = QColor(190, 255, 220)
        else:
            outer = QColor(120, 120, 120)
            inner = QColor(210, 210, 210)

        pen = QPen(QColor(25, 25, 25))
        pen.setWidth(2)
        p.setPen(pen)
        p.setBrush(outer)
        p.drawEllipse(6, 6, self._d - 12, self._d - 12)

        p.setPen(QPen(QColor(0, 0, 0, 0)))
        p.setBrush(inner)
        p.drawEllipse(12, 10, (self._d - 12)//2, (self._d - 12)//2)

class App(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("ESP32 LED - PyQt6 (Bluetooth SPP)")
        self.resize(760, 460)

        self.ser = None
        self.led_state = False
        self.pending = None  # "ON" o "OFF" esperando confirmación

        # UI
        self.port_combo = QComboBox()
        self.refresh_btn = QPushButton("Refrescar")
        self.connect_btn = QPushButton("Conectar")
        self.badge = QLabel("● Desconectado")
        self.badge.setObjectName("badge")

        self.led = LedIndicator()
        self.toggle_btn = QPushButton("Encender")
        self.toggle_btn.setCheckable(True)

        self.log = QTextEdit()
        self.log.setReadOnly(True)

        # Layout
        main = QVBoxLayout()
        title = QLabel("Control de LED por Bluetooth (SPP)")
        title.setObjectName("title")
        main.addWidget(title)

        box1 = QGroupBox("Conexión Bluetooth (COM SPP)")
        row = QHBoxLayout()
        row.addWidget(QLabel("Puerto:"))
        row.addWidget(self.port_combo, 1)
        row.addWidget(self.refresh_btn)
        row.addWidget(self.connect_btn)
        row.addWidget(self.badge)
        box1.setLayout(row)
        main.addWidget(box1)

        box2 = QGroupBox("Control")
        row2 = QHBoxLayout()
        row2.addWidget(QLabel("LED virtual:"))
        row2.addWidget(self.led)
        row2.addSpacing(10)
        row2.addWidget(self.toggle_btn)
        row2.addStretch(1)
        box2.setLayout(row2)
        main.addWidget(box2)

        box3 = QGroupBox("Log")
        l3 = QVBoxLayout()
        l3.addWidget(self.log)
        box3.setLayout(l3)
        main.addWidget(box3, 1)

        self.setLayout(main)

        # Timer lectura
        self.timer = QTimer(self)
        self.timer.setInterval(80)
        self.timer.timeout.connect(self.read_serial)

        # Signals
        self.refresh_btn.clicked.connect(self.refresh_ports)
        self.connect_btn.clicked.connect(self.toggle_connection)
        self.toggle_btn.clicked.connect(self.on_toggle)

        # Inicial
        self.set_connected(False)
        self.refresh_ports()
        self.apply_style()

    def apply_style(self):
        self.setStyleSheet("""
            QWidget { background:#0f1116; color:#e7e7e7; }
            QLabel#title { font-size:18px; font-weight:700; padding:6px 2px; }
            QGroupBox { border:1px solid #2a2f3a; border-radius:14px; margin-top:10px; padding:12px; background:#151824; }
            QGroupBox::title { subcontrol-origin: margin; left:12px; padding:0 6px; color:#bfc7d5; font-weight:600; }
            QComboBox { background:#0f1116; border:1px solid #2a2f3a; border-radius:10px; padding:8px 10px; }
            QPushButton { border-radius:10px; padding:10px 14px; border:1px solid #2a2f3a; background:#0f1116; }
            QPushButton:hover { background:#171a22; }
            QPushButton:disabled { color:#8a8a8a; background:#0b0d12; }
            QLabel#badge { padding:8px 10px; border-radius:999px; background:#231a1a; border:1px solid #3a2a2a; color:#ffb3b3; font-weight:600; }
            QTextEdit { background:#0f1116; border:1px solid #2a2f3a; border-radius:12px; padding:10px; font-family:Consolas, monospace; font-size:10px; }
        """)

    def log_append(self, s: str):
        self.log.moveCursor(QTextCursor.MoveOperation.End)
        self.log.insertPlainText(s)
        self.log.ensureCursorVisible()

    def refresh_ports(self):
        # Filtra para mostrar solo COMs Bluetooth típicamente (no perfecto, pero ayuda)
        self.port_combo.clear()
        ports = list(serial.tools.list_ports.comports())
        # Muchos BT salen como "Standard Serial over Bluetooth link"
        bt_ports = [p.device for p in ports if "Bluetooth" in (p.description or "") or "BT" in (p.description or "")]
        show = bt_ports if bt_ports else [p.device for p in ports]
        self.port_combo.addItems(show)
        if not show:
            self.log_append("[PC] No se detectaron puertos.\n")

    def set_connected(self, ok: bool):
        if ok:
            self.badge.setText("● Conectado")
            self.badge.setStyleSheet("QLabel#badge{background:#10231a;border:1px solid #1f6b42;color:#a6ffcf;font-weight:600;padding:8px 10px;border-radius:999px;}")
            self.connect_btn.setText("Desconectar")
            self.toggle_btn.setEnabled(True)
        else:
            self.badge.setText("● Desconectado")
            self.badge.setStyleSheet("QLabel#badge{background:#231a1a;border:1px solid #3a2a2a;color:#ffb3b3;font-weight:600;padding:8px 10px;border-radius:999px;}")
            self.connect_btn.setText("Conectar")
            self.toggle_btn.setEnabled(False)
            self.set_led(False)

    def set_led(self, on: bool):
        self.led_state = on
        self.led.set_on(on)
        self.toggle_btn.blockSignals(True)
        self.toggle_btn.setChecked(on)
        self.toggle_btn.blockSignals(False)
        self.toggle_btn.setText("Apagar" if on else "Encender")

    def toggle_connection(self):
        if self.ser and self.ser.is_open:
            self.disconnect()
        else:
            self.connect()

    def connect(self):
        if self.port_combo.count() == 0:
            self.log_append("[PC] No hay puerto COM.\n")
            return
        port = self.port_combo.currentText()
        try:
            # En SPP normalmente funciona sin importar baudrate, pero se pone 115200 por estándar
            self.ser = serial.Serial(port, 115200, timeout=0.1)
            self.set_connected(True)
            self.log_append(f"[PC] Conectado a {port} (Bluetooth SPP)\n")
            self.timer.start()
        except Exception as e:
            self.log_append(f"[ERROR] No se pudo conectar: {e}\n")

    def disconnect(self):
        self.timer.stop()
        try:
            if self.ser:
                self.ser.close()
        except:
            pass
        self.ser = None
        self.set_connected(False)
        self.log_append("[PC] Desconectado\n")
        self.pending = None

    def send_cmd(self, cmd: str):
        if not (self.ser and self.ser.is_open):
            self.log_append("[PC] No conectado.\n")
            return False
        try:
            self.ser.write((cmd + "\n").encode("utf-8"))
            self.log_append(f"[PC → ESP32(BT)] {cmd}\n")
            return True
        except Exception as e:
            self.log_append(f"[ERROR envío] {e}\n")
            return False

    def on_toggle(self):
        # Aquí lo dejamos simple: manda ON/OFF y espera confirmación para cambiar el LED virtual
        if self.toggle_btn.isChecked():
            self.pending = "ON"
            self.send_cmd("LED_ON")
        else:
            self.pending = "OFF"
            self.send_cmd("LED_OFF")

    def read_serial(self):
        if not (self.ser and self.ser.is_open):
            return
        try:
            data = self.ser.read(512)
            if not data:
                return
            text = data.decode("utf-8", errors="replace")
            self.log_append(f"[ESP32 → PC] {text}")

            # Confirmación simple
            if "LED_ON_OK" in text:
                self.set_led(True)
                self.pending = None
            elif "LED_OFF_OK" in text:
                self.set_led(False)
                self.pending = None

        except Exception as e:
            self.log_append(f"[ERROR lectura] {e}\n")

    def closeEvent(self, event):
        self.disconnect()
        event.accept()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    w = App()
    w.show()
    sys.exit(app.exec())
