#include "msd/model.h"

namespace msd {
  double accel(const Params& p, const State& s, double u) {
    // m x_ddot + c x_dot + k x = u
    // => x_ddot = (u - c v - k x)/m
    return (u - p.c * s.v - p.k * s.x) / p.m;
  }

  double energy(const Params& p, const State& s) {
    return 0.5 * p.m * (s.v * s.v) + 0.5 * p.k * (s.x * s.x);
  }

} // namespace std
