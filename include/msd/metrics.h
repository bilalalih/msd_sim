#pragma once
#include "types.h"
#include <vector>

namespace msd {
  struct Metrics {
    double max_abs_x{0.0};
    double peak_overshoot{0.0}; // relative to final value (step case)
    double steady_state_error{0.0}; // |x(T) - x_final|
    double settling_time_2pct{-1.0};
  };

  Metrics compute_step_metrics(const std::vector<Sample>& rows, double x_final, double t_step);

} // namespace msd
