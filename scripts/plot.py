import os, re, glob, csv
import matplotlib.pyplot as plt
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
os.chdir(ROOT)

def read_csv(path):
    t=[]; x=[]; v=[]; a=[]; u=[]; E=[]
    with open(path, newline="") as f:
        r = csv.DictReader(f)
        r.fieldnames = [name.strip() for name in r.fieldnames]
        for row in r:
          row = {k.strip(): v for k, v in row.items()}
          t.append(float(row["time"]))
          x.append(float(row["x"]))
          v.append(float(row["v"]))
          a.append(float(row["a"]))
          u.append(float(row["u"]))
          E.append(float(row["E"]))
    return t,x,v,a,u,E

# filenames like: data/msd_c0p6_dt0p05_euler.csv
PAT = re.compile(r"msd_c(?P<c>[0-9p]+)_dt(?P<dt>[0-9p]+)_(?P<meth>euler|rk4)\.csv$")

def main():
    os.makedirs("plots", exist_ok=True)

    files = glob.glob("data/msd_c*_dt*_*.*csv")
    if not files:
        raise SystemExit("No data files found. Run the simulator first.")

    groups = {}  # (c, dt) -> {"euler": path, "rk4": path}
    for p in files:
        m = PAT.search(p.replace("\\", "/"))
        if not m:
            continue
        key = (m.group("c"), m.group("dt"))
        groups.setdefault(key, {})[m.group("meth")] = p

    if not groups:
        raise SystemExit("No matching msd_c*_dt*_(euler|rk4).csv files found.")

    for (c, dt), d in sorted(groups.items()):
        if "euler" not in d or "rk4" not in d:
            print(f"Skipping incomplete pair for c={c} dt={dt}: {d}")
            continue

        te, xe, ve, ae, ue, Ee = read_csv(d["euler"])
        tr, xr, vr, ar, ur, Er = read_csv(d["rk4"])

        # x(t) overlay
        plt.figure()
        plt.plot(te, xe, label=f"Euler c={c} dt={dt}")
        plt.plot(tr, xr, label=f"RK4   c={c} dt={dt}")
        plt.xlabel("time (s)")
        plt.ylabel("x (m)")
        plt.title(f"MSD: x(t) overlay (c={c}, dt={dt})")
        plt.legend()
        plt.tight_layout()
        out1 = f"plots/msd_c{c}_dt{dt}.png"
        plt.savefig(out1, dpi=150)
        plt.close()

        # energy overlay
        plt.figure()
        plt.plot(te, Ee, label=f"Euler c={c} dt={dt}")
        plt.plot(tr, Er, label=f"RK4   c={c} dt={dt}")
        plt.xlabel("time (s)")
        plt.ylabel("Energy (J)")
        plt.title(f"MSD: energy sanity (c={c}, dt={dt})")
        plt.legend()
        plt.tight_layout()
        out2 = f"plots/msd_energy_c{c}_dt{dt}.png"
        plt.savefig(out2, dpi=150)
        plt.close()

        # difference plot (only if time grids match)
        if len(te) == len(tr) and all(abs(a-b) < 1e-12 for a,b in zip(te, tr)):
            diff = [a-b for a,b in zip(xe, xr)]
            plt.figure()
            plt.plot(te, diff, label="x_euler - x_rk4")
            plt.xlabel("time (s)")
            plt.ylabel("Δx (m)")
            plt.title(f"MSD: Euler - RK4 (c={c}, dt={dt})")
            plt.legend()
            plt.tight_layout()
            out3 = f"plots/msd_diff_c{c}_dt{dt}.png"
            plt.savefig(out3, dpi=150)
            plt.close()
        else:
            out3 = None

        print("Wrote:", out1)
        print("Wrote:", out2)
        if out3: print("Wrote:", out3)

if __name__ == "__main__":
    main()
