from Interface import Interface

from lib.backend_module import backend_module as backend
from PyQt6.QtCore import Qt, QRectF
from PyQt6.QtGui import QPainter, QImage, QColor
from PyQt6.QtWidgets import QApplication, QWidget, QGraphicsScene, QGraphicsView
import sys


class Interface(QWidget):
  def __init__(self):
    super().__init__()
    self.setWindowTitle("WSN simulation")
    self.setGeometry(100, 100, 800, 600)
    self.canvas_size = 500

    # Inicjalizacja symulacji
    sim = backend.Simulation()
    sim.Initialization(10, 20, 0.1)

    # Przygotowanie sceny
    self.scene = QGraphicsScene(self)
    self.view = QGraphicsView(self.scene, self)
    self.view.setGeometry(20, 20, self.canvas_size, self.canvas_size)
    
    # Rysowanie sensorów
    for sensor in sim.Sensors:
      pos = sensor.position
      pos.x *= self.canvas_size
      pos.y *= self.canvas_size
      self.DrawPoint(pos, backend.Sensor.radius * self.canvas_size, QColor(50, 50, 50, 128))  # Szare, półprzezroczyste koło

    for sensor in sim.Sensors:
      pos = sensor.position
      pos.x *= self.canvas_size
      pos.y *= self.canvas_size
      self.DrawPoint(pos, 3, QColor(0xFF, 0xFF, 0xFF, 0xFF)) 
    
    # Rysowanie celów (targets)
    for target in sim.Targets:
      pos = target.position
      pos.x *= self.canvas_size
      pos.y *= self.canvas_size
      self.DrawPoint(pos, 3, QColor(0xFF, 0, 0, 0xFF))

  def DrawPoint(self, p, r, color):
    """Rysuje koło na scenie QGraphics"""
    ellipse = self.scene.addEllipse(QRectF(p.x - r, p.y - r, r * 2, r * 2), QColor(color))
    ellipse.setBrush(color)
    
  def run(self):
    self.show()

if __name__ == "__main__":
  app = QApplication(sys.argv)
  window = Interface()
  window.run()
  sys.exit(app.exec())
