import os, glob, csv
import matplotlib.pyplot as plt
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
os.chdir(ROOT)

def read_csv(path):
  t=[]; x=[]; v=[]; a=[]; u=[]; E=[];
  with open(path, newline="") as f:
    r = csv.DictReader(f)
    r.fieldnames = [name.strip() for name in r.fieldnames]

    for row in r:
      row = {k.strip(): v for k, v in row.items()}
      t.append(float(row["time"]))
      x.append(float(row['x']))
      v.append(float(row['v']))
      a.append(float(row['a']))
      u.append(float(row['u']))
      E.append(float(row['E']))

  return t, x, v, a, u, E

def overlay(c, dt, out_png):
  euler = f"data/msd_c{c}_dt{dt}_euler.csv"
  rk4 = f"data/msd_c{c}_dt{dt}_rk4.csv"
  if not (os.path.exists(euler)) and os.path.exists(rk4):
    print("Missing:", euler, "or", rk4)
    return

  te, xe, _, _, _, _ = read_csv(euler)
  tr, xr, _, _, _, _ = read_csv(rk4)
  plt.figure()
  plt.plot(te, xe, label=f"Euler c={c} dt={dt}")
  plt.plot(tr, xr, label=f"RK4   c={c} dt={dt}")
  plt.xlabel("time (s)")
  plt.ylabel("x (m)")
  plt.legend()
  plt.tight_layout()
  plt.savefig(out_png, dpi=150)
  plt.close()
  print("Wrote:", out_png)

def main():
  os.makedirs("plots", exist_ok=True)

  overlay("7",  "0p001", "plots/msd_c7_dt0p001.png")
  overlay("7", "0p05", "plots/msd_c7_dt0p05.png")
  overlay("0p6",  "0p001", "plots/msd_c0p6_dt0p001.png")
  overlay("0p6",  "0p05", "plots/msd_c0p6_dt0p05.png")

if __name__ == "__main__":
  main()
