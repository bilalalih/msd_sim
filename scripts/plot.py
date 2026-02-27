import csv
import os
import math
import matplotlib.pyplot as plt

def read_csv(path):
  t=[]; x=[]; v=[]; a=[]; u=[]; E=[];
  with open(path, newline="") as f:
    r = csv.DictReader(f)
    for row in r:
      t.append(float(row["time"]))
      x.append(float(row["x"]))
      v.append(float(row["v"]))
      a.append(float(row["a"]))
      u.append(float(row["u"]))
      E.append(float(row["E"]))

  return t, x, v, a, u, E

def main():
  euler_path = "data/msd_euler.csv"
  rk4_path = "data/msd_rk4.csv"
  out_path = "plots/msd_plot.png"

  if not os.path.exists(euler_path) or not os.path.exists(rk4_path):
    raise SystemExit("Missing CSV(s). Run the simulator first.")

  te, xe, ve, ae, ue, Ee = read_csv(euler_path)
  tr, xr, vr, ar, ur, Er = read_csv(rk4_path)

  # Position plot
  plt.figure()
  plt.plot(te, xe, label='Euler (semi-implicit)')
  plt.plot(tr, xr, label='RK4')
  plt.xlabel("time (s)")
  plt.ylabel("x (m)")
  plt.title("Mass-Spring-Damper: position")
  plt.legend()
  plt.tight_layout()
  plt.savefig("plots/msd_x.png", dpi=150)
  plt.close()

  # Energy plot
  plt.figure()
  plt.plot(te, Ee, label='Euler (semi-implicit)')
  plt.plot(tr, Er, label='RK4')
  plt.xlabel("time (s)")
  plt.ylabel("Energy (J)")
  plt.title("Mass-Spring-Damper: energy sanity check")
  plt.legend()
  plt.tight_layout()
  plt.savefig("plots/msd_energy.png", dpi=150)
  plt.close()

  # Combined summary (optional single image)
  plt.figure()
  plt.plot(te, xe, label='x Euler')
  plt.plot(tr, xr, label='x RK4')
  plt.xlabel("time (s)")
  plt.ylabel("x (m)")
  plt.title("MSD: Euler vs RK4 (position overlay)")
  plt.legend()
  plt.tight_layout()
  plt.savefig(out_path, dpi=150)
  plt.close()

  print("Wrote: ")
  print("  plots/msd_x.png")
  print("  plots/msd_energy.png")
  print(f" {out_path}")

if __name__ == "__main__":
  os.makedirs("plots", exist_ok=True)
  main()
