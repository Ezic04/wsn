from lib.backend_module import backend_module as backend

s = backend.Sensor(backend.Point(1,2))

s.SetRadious(10)
print(s.GetPosition().x)