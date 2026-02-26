#pragma once
#include "types.h"

namespace msd {

  double accel(const Params& p, const State& s, double u);
  double energy(const Params& p, const State& s); // 0.5*m*v^2 + 0.5*k*v^2

} // namespace msd
