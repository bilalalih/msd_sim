import os
import csv
import matplotlib.pyplot as plt
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
os.chdir(ROOT)

def read_csv_tx(path):
    t=[]; x=[]
    with open(path, newline="") as f:
        r = csv.DictReader(f)
        r.fieldnames = [name.strip() for name in r.fieldnames]
        for row in r:
          row = {k.strip(): v for k, v in row.items()}
          t.append(float(row["time"]))
          x.append(float(row["x"]))
    return t, x

def plot_reference_case(c_value, dt_ref="0p001", dt_test="0p05"):
    ref = f"data/msd_c{c_value}_dt{dt_ref}_rk4.csv"
    eul = f"data/msd_c{c_value}_dt{dt_test}_euler.csv"
    rk4 = f"data/msd_c{c_value}_dt{dt_test}_rk4.csv"

    for p in (ref, eul, rk4):
        if not os.path.exists(p):
            raise FileNotFoundError(f"Missing required file: {p}")

    t_ref, x_ref = read_csv_tx(ref)
    t_e, x_e     = read_csv_tx(eul)
    t_r, x_r     = read_csv_tx(rk4)

    plt.figure()
    plt.plot(t_ref, x_ref, label=f"RK4 dt={dt_ref.replace('p','.') } (Reference)", linewidth=2)
    plt.plot(t_e,   x_e,   "--", label=f"Euler dt={dt_test.replace('p','.')}")
    plt.plot(t_r,   x_r,   "--", label=f"RK4 dt={dt_test.replace('p','.')}")
    plt.xlabel("time (s)")
    plt.ylabel("x (m)")
    plt.title(f"MSD Integrator Comparison (c={c_value})")
    plt.legend()
    plt.tight_layout()

    out = f"plots/msd_reference_c{c_value}.png"
    plt.savefig(out, dpi=150)
    plt.close()
    print("Wrote:", out)

def main():
    os.makedirs("plots", exist_ok=True)

    # match your c tags exactly (0p6, 7)
    plot_reference_case("7")
    plot_reference_case("0p6")

if __name__ == "__main__":
    main()
