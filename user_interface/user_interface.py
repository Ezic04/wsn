from lib.core_engine import core_engine
import tkinter as tk

root = tk.Tk()
root.title("WSN simulation")
root.geometry("400x300")  


label = tk.Label(root, text=f"wartosc 2+2 ={core_engine.add(2,2)}")
label.pack(pady=10)

root.mainloop()
