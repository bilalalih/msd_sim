#pragma once
#include "types.h"
#include <functional>

namespace msd {
  using InputFn = std::function<double(double)>;

  State step_semi_implicit_euler(const Params& p, const State& s, double t, double dt, const InputFn& u);
  State step_rk4(const Params& p, const State& s, double t, double dt, const InputFn& u);

} // namespace msd
