# Mass–Spring–Damper (MSD) Simulation — Euler vs RK4

## Overview

This project simulates the 1D mass–spring–damper system:

[
m\ddot{x} + c\dot{x} + kx = u(t)
]

using two numerical integrators:

* **Semi-implicit Euler** (energy-dissipative baseline)
* **Runge–Kutta 4 (RK4)** (higher-order integrator)

The goal is to compare **stability, accuracy, and numerical behavior** across damping regimes and timestep sizes.

The simulation generates:

* CSV time-series logs
* Automated plots
* Console metrics (overshoot, settling time, steady-state error)

---

## Problem

Continuous-time second-order systems must be discretized for simulation and control implementation.

The key questions explored:

* How does timestep size (`dt`) affect solution fidelity?
* How do integrators behave in:

  * near-critical damping (clean settling)
  * underdamped oscillatory systems?
* Does the numerical method introduce artificial damping or phase distortion?

---

## Approach

### Dynamics

State formulation:

[
\dot{x} = v
]
[
\dot{v} = \frac{u - c v - k x}{m}
]

Energy sanity check:

[
E(t) = \frac{1}{2} m v^2 + \frac{1}{2} k x^2
]

---

### Integrators

* **Semi-implicit Euler**

  * First-order
  * Adds numerical damping in oscillatory systems
* **RK4**

  * Fourth-order
  * Better trajectory fidelity at moderate timesteps

---

### Experimental Matrix

Two damping regimes:

* **Case A (near critical damping):** `c = 7`
* **Case B (underdamped oscillatory):** `c = 0.6`

Two timestep sizes:

* `dt = 0.001` (high fidelity reference)
* `dt = 0.05`  (coarse timestep stress test)

Total: **8 simulations**

---

## Results

### Clean settling case (c = 7)

Euler and RK4 are nearly identical at small timestep.
At coarse timestep, Euler shows slight numerical damping but remains stable.

| Case     | Integrator | Settling Time (2%) | Overshoot |
| -------- | ---------- | ------------------ | --------- |
| dt=0.001 | Euler      | 1.921 s            | 0         |
| dt=0.001 | RK4        | 1.918 s            | 0         |
| dt=0.05  | Euler      | 2.05 s             | 0         |
| dt=0.05  | RK4        | 1.90 s             | 0         |

---

### Underdamped case (c = 0.6)

At small timestep, Euler ≈ RK4 (baseline accuracy).

At coarse timestep:

* Euler exhibits **artificial numerical damping**
* RK4 preserves oscillatory behavior but shows **amplitude/phase error**

| Case     | Integrator | Peak Overshoot | Settling Time |
| -------- | ---------- | -------------- | ------------- |
| dt=0.001 | Euler      | 0.7609         | 13.06 s       |
| dt=0.001 | RK4        | 0.7617         | 13.07 s       |
| dt=0.05  | Euler      | 0.7602         | 12.95 s       |
| dt=0.05  | RK4        | 0.7966         | 15.00 s       |

This demonstrates that:

* Semi-implicit Euler can appear “stable” due to added numerical damping.
* RK4 better preserves physical oscillation but requires adequate timestep.

---

## Generated Outputs

After running:

**Data**

* `data/msd_c*_dt*_euler.csv`
* `data/msd_c*_dt*_rk4.csv`

**Plots**

* `plots/msd_c*_dt*.png`
* `plots/msd_overlay.png` (integrator comparison)
* `plots/msd_energy.png`
* `plots/msd_diff.png`

---

## Build & Run

```bash
cmake -S . -B build
cmake --build build
./build/msd_sim
python scripts/plot.py
```

Windows (PowerShell):

```powershell
cmake -S . -B build
cmake --build build
.\build\Debug\msd_sim.exe
python .\scripts\plot.py
```

---

## Key Takeaways

* Integrator choice directly affects oscillatory fidelity.
* Coarse timesteps distort dynamics differently across methods.
* Semi-implicit Euler introduces numerical damping.
* RK4 provides higher accuracy but still depends on timestep size.
* Energy tracking is a useful sanity diagnostic.


