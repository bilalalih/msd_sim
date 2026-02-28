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

static std::string tag(double c, double dt, const std::string& method) {
  auto fmt = [](double x){
    std::string s = std::to_string(x);
    // crude trimming
    while (!s.empty() && s.back() == '0') s.pop_back();
    if(!s.empty() && s.back() == '.') s.pop_back();
    for (auto& ch: s) if (ch == '.') ch = 'p';
      return s;
  };
  return "c" + fmt(c) + "_dt" + fmt(dt) + "_" + method;
}

static void run_case(
  const Params& base,
  double c, double dt, double t0, double t1,
  const State& s0,
  const Input& input,
  double x_final,
  double t_step
) {
  Params p = base;
  p.c = c;

  auto euler_rows = run_sim(p, s0, t0, t1, dt, input, false);
  auto rk4_rows = run_sim(p, s0, t0, t1, dt, input, true);

  const std::string euler_csv = "data/msd_" + tag(c, dt, "euler") + ".csv";
  const std::string rk4_csv = "data/msd_" + tag(c, dt, "rk4") + ".csv";

  write_csv(euler_csv, euler_rows);
  write_csv(rk4_csv, rk4_rows);

  const auto m_euler = compute_step_metrics(euler_rows, x_final, t_step);
  const auto m_rk4 = compute_step_metrics(rk4_rows, x_final, t_step);

  std::cout << "\n--- Case: c=" << c <<" dt=" << dt << "---\n";
  print_metrics("Semi-Implicit Euler", m_euler);
  std::cout << "  csv: " << euler_csv << "\n";
  print_metrics("RK4", m_rk4);
  std::cout << "  csv: " << rk4_csv << "\n";
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

  Params base;
  base.m = 1.0;
  base.k = 12.0;

  const double t0 = 0.0;
  const double t1 = 15.0;
  const double t_step = 0.2;

  State s0{0.0, 0.0};

  const double step_amp = 1.0;
  auto input = make_step(t_step, step_amp);

  const double x_final = step_amp / base.k;

  const double dt_small = 0.001;
  const double dt_big = 0.05;

  // Case A: clean settling (near critical)
  run_case(base, 7.0, dt_small, t0, t1, s0, *input, x_final, t_step);
  run_case(base, 7.0, dt_big, t0, t1, s0, *input, x_final, t_step);

  // Case B: underdamped (overshoot + ringing)
  run_case(base, 0.6, dt_small, t0, t1, s0, *input, x_final, t_step);
  run_case(base, 0.6, dt_big, t0, t1, s0, *input, x_final, t_step);

  std::cout << "\nDone. Now run:\n python scripts/plot.py\n";
  return 0;
}
