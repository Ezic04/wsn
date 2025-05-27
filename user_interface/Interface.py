import tkinter as tk
from tkinter import filedialog, messagebox
from PIL import Image, ImageTk, ImageDraw
from lib.backend_module import backend_module as backend

class Interface(tk.Tk):
  def __init__(self):
    super().__init__()
    self.title("WSN Simulation")

    screen_width = self.winfo_screenwidth()
    screen_height = self.winfo_screenheight()
    win_width = min(1000, screen_width - 100)
    win_height = min(900, screen_height - 100)
    self.geometry(f"{win_width}x{win_height}")

    self.ctrl = tk.Frame(self)
    self.ctrl.pack(fill="x", padx=10, pady=5)

    # Left group: Load, Set, Run, Save, Reset
    left_group = tk.Frame(self.ctrl)
    left_group.pack(side="left")

    # Load button with menu
    load_btn = tk.Menubutton(left_group, text="Load", relief="raised")
    load_btn.pack(side="left", padx=5)
    load_menu = tk.Menu(load_btn, tearoff=0)
    load_btn.config(menu=load_menu)
    load_menu.add_command(label="Load parameters", command=self._load_parameters_only)
    load_menu.add_command(label="Load scenario", command=self._load_scenario_only)
    load_menu.add_command(label="Load both", command=self._load_both)
  
    # Set button with menu
    set_btn = tk.Menubutton(left_group, text="Set", relief="raised")
    set_btn.pack(side="left", padx=5)
    set_menu = tk.Menu(set_btn, tearoff=0)
    set_btn.config(menu=set_menu)
    set_menu.add_command(label="Set parameters", command=self._set_parameters)
    set_menu.add_command(label="Set scenario", command=self._set_scenario)

    # Run, Save and Reset buttons
    tk.Button(left_group, text="Run Simulation", command=self._run_simulation).pack(side="left", padx=5)
    tk.Button(left_group, text="Save Report…", command=self._save_report).pack(side="left", padx=5)
    tk.Button(left_group, text="Reset", command=self._reset).pack(side="left", padx=5)

    # Right group: Manual, Auto, Replay, Delay
    right_group = tk.Frame(self.ctrl)
    right_group.pack(side="right")

    tk.Button(right_group, text="Manual Step ▶", command=self.step_once).pack(side="left", padx=5)
    self.auto_btn = tk.Button(right_group, text="Auto ▶▶", command=self._toggle_auto)
    self.auto_btn.pack(side="left", padx=5)
    tk.Button(right_group, text="Replay ⟲", command=self._replay).pack(side="left", padx=5)
    
    tk.Label(right_group, text="Delay(ms):").pack(side="left", padx=(20, 0))
    self.delay_var = tk.IntVar(value=50)
    tk.Spinbox(right_group, from_=10, to=1000, increment=10,
           textvariable=self.delay_var, width=5).pack(side="left")

    # Canvas and status
    self.canvas_size = min(win_width - 100, win_height - 100)
    self.canvas = tk.Canvas(self, bg="white",
                            width=self.canvas_size, height=self.canvas_size)
    self.canvas.pack(pady=10)

    self.status = tk.Label(self, text="No simulation loaded", anchor="w")
    self.status.pack(fill="x", padx=10, pady=(0, 10))


    self.manager = backend.SimulationManager()
    self.parameters = None
    self.scenario = None
    self.states = []
    self.final_state = None
    self.current_idx = 0
    self._job = None
    self.running = False

  def _toggle_auto(self):
    if self.running:
      self._stop_auto()
    else:
      self._start_auto()

  def _start_auto(self):
    if not self.states: return
    self.running = True
    self.auto_btn.config(text="Pause ❚❚")
    self._auto_step()

  def _stop_auto(self):
    self.running = False
    self.auto_btn.config(text="Auto ▶▶")
    if self._job:
      self.after_cancel(self._job)
      self._job = None

  def _auto_step(self):
    if self.current_idx < len(self.states) - 1 and self.running:
      self.current_idx += 1
      self._draw_state(self.states[self.current_idx])
      self._job = self.after(self.delay_var.get(), self._auto_step)
    else:
      self._stop_auto()

  def step_once(self):
    if not self.states: return
    if self.current_idx < len(self.states) - 1:
      self.current_idx += 1
      self._draw_state(self.states[self.current_idx])
  
  def _replay(self):
    """Reset playback to the first state."""
    if not self.states:
        return
    self._stop_auto()
    self.current_idx = 0
    self._draw_state(self.states[0])
    tick_info = f"Tick: 0 / {len(self.states)-1}"
    if hasattr(self.states[0], 'coverage_percentage') and hasattr(self.states[0], 'covered_target_count'):
        coverage_info = f"  Coverage: {self.states[0].coverage_percentage:.1f}% ({self.states[0].covered_target_count}/{len(self.states[0].is_target_covered)})"
        self.status.config(text=tick_info + coverage_info)
    else:
        self.status.config(text=tick_info)

  def _run_simulation(self):
    if not self.parameters or not self.scenario:
      messagebox.showwarning("Missing data", "Set both parameters and scenario first.")
      return
    if self.manager.IsInitialized():
      messagebox.showwarning("Warning", "Reset the simulation first")
      return
    try:
      self._stop_auto()
      # Initializing the simulation
      self.status.config(text="Initializing simulation...")
      self.config(cursor="watch") 
      self.update_idletasks()
      self.manager.Initialize()
      # Running the simulation
      self.status.config(text="Running simulation...")
      self.update_idletasks()
      self.manager.Run()
      self.config(cursor="")
      self.states = list(self.manager.GetSimulationStates())
      if not self.states:
        messagebox.showwarning("Warning", "No states generated")
        return
      self.final_state = self.states[-1]
      self.current_idx = 0
      self._draw_state(self.states[0])
      self.status.config(text=f"Tick: 0 / {len(self.states)-1}")
    except Exception as e:
      messagebox.showerror("Error", str(e))

  def _handle_load(self, choice):
    funcs = {
      "Load both": self._load_both,
      "Load parameters": self._load_parameters_only,
      "Load scenario": self._load_scenario_only
    }
    func = funcs.get(choice)
    if func:
      func()

  def _ask_and_load(self, load_params=False, load_scenario=False):
    if self.manager.IsInitialized():
      messagebox.showwarning("Warning", "Reset the simulation first")
      return
    path = filedialog.askopenfilename(filetypes=[("JSON files", "*.json")])
    if not path:
      return
    try:
      if load_params:
        self.manager.LoadParametersFromJSON(path)
        self.parameters = self.manager.GetParameters()
        self.status.config(text="Parameters loaded")
      if load_scenario:
        self.manager.LoadScenarioFromJSON(path)
        self.scenario = self.manager.GetScenario()
        self.status.config(text="Scenario loaded")
      if load_params and load_scenario:
        self.status.config(text="Parameters and scenario loaded")
    except Exception as e:
      messagebox.showerror("Error", str(e))

  def _load_both(self):
    self._ask_and_load(load_params=True, load_scenario=True)

  def _load_parameters_only(self):
    self._ask_and_load(load_params=True)

  def _load_scenario_only(self):
    self._ask_and_load(load_scenario=True)


  def _handle_set(self, choice):
    match choice:
      case "Set parameters":
        self._set_parameters()
      case "Set scenario":
        self._set_scenario()

  def _create_popup(self, title):
    win = tk.Toplevel(self)
    win.geometry(f"+{self.winfo_rootx()}+{self.winfo_rooty()}")
    win.title(title)
    win.transient(self)
    win.grab_set()
    win.focus_set()
    return win


  def _set_parameters(self):
    if self.manager.IsInitialized():
      messagebox.showwarning("Warning", "Reset the simulation first")
      return
    win = self._create_popup("Set Parameters")

    entries = {}
    fields = [
      ("Sensor radius", "sensor_radious"),
      ("Battery level", "initial_battery_lvl"),
      ("Reshuffle interval", "reshuffle_interval"),
      ("Stop threshold", "stop_threshold"),
      ("Max ticks", "max_ticks")
    ]
    for i, (label, key) in enumerate(fields):
      tk.Label(win, text=label).grid(row=i, column=0, sticky="w")
      entry = tk.Entry(win)
      entry.grid(row=i, column=1)
      entries[key] = entry

    stop_conditions = {
      "Manual": "kManual",
      "Zero coverage": "kZeroCoverage",
      "Coverage below threshold": "kCoverageBelowThreshold",
      "Any coverage lost": "kAnyCoverageLost"
    }
    tk.Label(win, text="Stop condition").grid(row=len(fields), column=0, sticky="w")
    stop_var = tk.StringVar(value="Manual")
    option_menu = tk.OptionMenu(win, stop_var, *stop_conditions.keys())
    option_menu.config(width=25)
    option_menu.grid(row=len(fields), column=1)

    def apply():
      try:
        enum_name = stop_conditions[stop_var.get()]
        stop_enum = getattr(backend.SimulationStopCondition, enum_name)
        params = backend.SimulationParameters(
          float(entries["sensor_radious"].get()),
          int(entries["initial_battery_lvl"].get()),
          int(entries["reshuffle_interval"].get()),
          stop_enum,
          float(entries["stop_threshold"].get()),
          int(entries["max_ticks"].get())
        )
        self.manager.SetParameters(params)
        self.parameters = self.manager.GetParameters()
        win.destroy()
        self.status.config(text="Parameters loaded")
      except Exception as e:
        messagebox.showerror("Error", str(e))

    tk.Button(win, text="Apply", command=apply).grid(row=len(fields)+1, columnspan=2, pady=5)



  def _set_scenario(self):
    if self.manager.IsInitialized():
      messagebox.showwarning("Warning", "Reset the simulation first")
      return
    win = self._create_popup("Set Scenario")

    tk.Label(win, text="Target positions (x,y;...)").grid(row=0, column=0, sticky="w")
    target_entry = tk.Entry(win, width=60)
    target_entry.grid(row=0, column=1)

    tk.Label(win, text="Sensor positions (x,y;...)").grid(row=1, column=0, sticky="w")
    sensor_entry = tk.Entry(win, width=60)
    sensor_entry.grid(row=1, column=1)

    def parse_points(text):
      pts = []
      for pair in text.split(";"):
        x, y = map(float, pair.strip().split(","))
        pts.append(backend.Point(x, y))
      return pts
    def apply():
      try:
        targets = parse_points(target_entry.get())
        sensors = parse_points(sensor_entry.get())
        scen = backend.SimulationScenario(targets, sensors)
        self.manager.SetScenario(scen)
        self.scenario = self.manager.GetScenario()
        win.destroy()
        self.status.config(text="Scenario loaded")
      except Exception as e:
        messagebox.showerror("Error", str(e))

    tk.Button(win, text="Apply", command=apply).grid(row=2, columnspan=2, pady=5)

 
  def _save_report(self):
    if not self.manager.IsInitialized():
      messagebox.showwarning("Warning", "No simulation to save")
      return
    if not self.final_state:
      messagebox.showwarning("Warning", "No simulation data to save")
      return
    path = filedialog.asksaveasfilename(defaultextension=".txt",
                                      filetypes=[("Text file", "*.txt")])
    if not path: return
    try:
      s = self.final_state
      with open(path, "w") as f:
        f.write("=== WSN Simulation Report ===\n\n")

        f.write(f"Total ticks: {s.tick}\n")
        f.write(f"Sensor count: {len(s.sensor_states)}\n")
        f.write(f"Target count: {len(s.is_target_covered)}\n")
        f.write(f"Final coverage: { 100 * s.coverage_percentage:.2f}% "
                f"({s.covered_target_count}/{len(s.is_target_covered)})\n\n")
              
        f.write("\nSimulation Parameters:\n")
        f.write(f"  Radius: {self.parameters.sensor_radious}\n") # type: ignore
        f.write(f"  Battery: {self.parameters.initial_battery_lvl}\n") # type: ignore
        f.write(f"  Reshuffle Interval: {self.parameters.reshuffle_interval}\n") # type: ignore
        
        f.write("Sensor states:\n")
        for i, st in enumerate(s.sensor_states):
          f.write(f"  Sensor {i}: {st.name}, Battery: {s.sensor_battery_lvls[i]}\n")

      messagebox.showinfo("Saved", f"Report saved to {path}")
    except Exception as e:
      messagebox.showerror("Error", str(e))

  def _reset(self):
    if not self.manager.IsInitialized():
      messagebox.showwarning("Warning", "No simulation to reset")
      return
    self._stop_auto()
    self.manager.Reset()
    self.parameters = None
    self.scenario = None
    self.states = []
    self.current_idx = 0
    self.final_state = None

    blank_img = Image.new("RGB", (self.canvas_size, self.canvas_size), (255, 255, 255))
    self.tk_img = ImageTk.PhotoImage(blank_img)
    self.canvas.create_image(0, 0, anchor="nw", image=self.tk_img)

    self.status.config(text="No simulation loaded")

  def _draw_state(self, state):
    size = self.canvas_size
    base = Image.new("RGBA", (size, size), (255,255,255,0))
    range_layer = base.copy()
    outline_layer = base.copy()
    sensor_layer = base.copy()
    target_layer = base.copy()
    draw_range = ImageDraw.Draw(range_layer)
    draw_outline = ImageDraw.Draw(outline_layer)
    draw_sensor = ImageDraw.Draw(sensor_layer)
    draw_target = ImageDraw.Draw(target_layer)

    for i, s_state in enumerate(state.sensor_states):
      if s_state == backend.SensorState.kOn:
        pt = self.scenario.sensor_positions[i] # type: ignore
        x = int(pt.x * size)
        y = int(pt.y * size)
        R = int(self.parameters.sensor_radious * size) # type: ignore
        draw_range.ellipse((x-R, y-R, x+R, y+R), fill=(243, 255, 243, 32))
        draw_outline.ellipse((x-R, y-R, x+R, y+R), outline=(96, 224, 128, 128), width=2)

    for i, s_state in enumerate(state.sensor_states):
      pt = self.scenario.sensor_positions[i] # type: ignore
      x = int(pt.x * size)
      y = int(pt.y * size)
      if s_state == backend.SensorState.kOn:
        color = (0,150,0)
      elif s_state == backend.SensorState.kOff:
        color = (150,150,150)
      elif s_state == backend.SensorState.kDead:
        color = (200,0,0)
      else:
        color = (180,180,0)
      draw_sensor.ellipse((x-3, y-3, x+3, y+3), fill=color)

    for j, covered in enumerate(state.is_target_covered):
      pt = self.scenario.target_positions[j] # type: ignore
      x = int(pt.x * size)
      y = int(pt.y * size)
      color = (70,70,200) if covered else (220,170,50)
      draw_target.rectangle((x-3, y-3, x+3, y+3), fill=color)

    img = Image.alpha_composite(base, range_layer)
    img = Image.alpha_composite(img, outline_layer)
    img = Image.alpha_composite(img, sensor_layer)
    img = Image.alpha_composite(img, target_layer)

    self.tk_img = ImageTk.PhotoImage(img.convert("RGB"))
    self.canvas.create_image(0, 0, anchor="nw", image=self.tk_img)

    tick_info = f"Tick: {state.tick} / {len(self.states)-1}"
    coverage_info = f"  Coverage: { 100 * state.coverage_percentage:.1f}% ({state.covered_target_count}/{len(state.is_target_covered)})"
    self.status.config(text=tick_info + coverage_info)

  def run(self):
    self.mainloop()

if __name__ == "__main__":
  Interface().run()