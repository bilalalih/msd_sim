#include "msd/types.h"
#include "msd/model.h"
#include "msd/input.h"
#include "msd/integrators.h"
#include "msd/csv.h"
#include "msd/metrics.h"

#include <iostream>
#include <vector>
#include <filesystem>
#include <string>
#include <cmath>

using namespace msd;

static std::vector<Sample> run_sim(
  const Params& p,
  const State& s0,
  double t0, double t1, double dt,
  const Input& in,
  bool use_rk4
  ) {
  std::vector<Sample> rows;
  const int N = static_cast<int>(std::ceil((t1 - t0) / dt));
  rows.reserve(static_cast<size_t>(N + 1));

  State s = s0;
  double t = t0;

  auto ufn = [&](double tt) { return in.u(tt); };

  for (int i = 0; i <= N; ++i) {
    const double u = in.u(t);
    const double a = accel(p, s, u);
    const double E = energy(p, s);

    rows.push_back(Sample{t, s.x, s.v, a, u, E});

    if (i == N) break;
    s = use_rk4 ? step_rk4(p, s, t, dt, ufn): step_semi_implicit_euler(p, s, t, dt, ufn);
    t += dt;
  }
  return rows;
}

static void print_metrics(const std::string& name, const Metrics& m) {
  std::cout << "==" << name << "==\n";
  std::cout << "max_abs_x           = " << m.max_abs_x << "\n";
  std::cout << "peak_overshoot      = " << m.peak_overshoot << "(fraction)\n";
  std::cout << "settling_time_2pct  = " << m.settling_time_2pct << "(s) \n";
  std::cout << "steady_state_err    = " << m.steady_state_error << "\n";
}

static std::filesystem::path project_root() {
  #ifdef MSD_PROJECT_ROOT
    return std::filesystem::path(MSD_PROJECT_ROOT);
  #else
    return std::filesystem::current_path(); // fallback
  #endif
}

int main() {
  const auto root = project_root();
  const auto data_dir = root / "data";
  const auto plots_dir = root / "plots";

  std::filesystem::create_directories(data_dir);
  std::filesystem::create_directories(plots_dir);

  // Scenario: step input (best for overshoot/settling metrics)
  Params p;
  p.m = 1.0;
  p.c = 4.0;
  p.k = 60.0;

  const double dt = 0.01;
  const double t0 = 0.0;
  const double t1 = 15.0;

  State s0;
  s0.x = 0.0;
  s0.v = 0.0;

  const double step_amp = 1.0;
  auto input = make_step(0.2, step_amp);

  // For step input, steady-state x_final = u/k (for stable systems)
  const double x_final = step_amp / p.k;

  auto euler_rows = run_sim(p, s0, t0, t1, dt, *input, false);
  auto rk4_rows = run_sim(p, s0, t0, t1, dt, *input, true);

  write_csv((data_dir / "msd_euler.csv").string(), euler_rows);
  write_csv((data_dir / "msd_rk4.csv").string(), rk4_rows);

  const double t_step = 0.2;
  const auto m_euler = compute_step_metrics(euler_rows, x_final, t_step);
  const auto m_rk4 = compute_step_metrics(rk4_rows, x_final, t_step);

  std::cout << "MSD parameters: m=" << p.m << " c=" << p.c << " k=" << p.k << "\n";
  std::cout << "dt=" << dt << " T=" << (t1- t0) << " step_amp=" << step_amp << " x_final=" << x_final << "\n\n";

  print_metrics("Semi-Implicit Euler", m_euler);
  std::cout << "\n";
  print_metrics("RK4", m_rk4);

  std::cout << "\nWrote: \n" << (data_dir / "msd_euler.csv") << "\n" << (data_dir/"msd_rk4.csv") << "\n";
  std::cout << "Next:\n python scripts/plot.py\n";
  return 0;
}
