#include "msd/metrics.h"
#include <cmath>
#include <algorithm>

namespace msd {

  Metrics compute_step_metrics(const std::vector<Sample>& rows, double x_final){
    Metrics m{};

    if (rows.empty()) return m;

    // max |x|
    for (const auto& r: rows) {
      m.max_abs_x = std::max(m.max_abs_x, std::abs(r.x));
    }

    // peak overshoot relative to final (only meaningful if x_final != 0)
    if (std::abs(x_final) > 1e-12) {
      double x_peak = rows.front().x;
      for (const auto& r: rows) x_peak = std::max(x_peak, r.x);
      m.peak_overshoot = (x_peak - x_final) / std::abs(x_final);
      if (m.peak_overshoot < 0.0) m.peak_overshoot = 0.0;
    } else {
      m.peak_overshoot = 0.0;
    }

    // steady state error: final sample to x_final
    m.steady_state_error = std::abs(rows.back().x - x_final);

    // settling time 2%: earliest time after which all remaining samples stay within 2% band
    const double band = 0.02 * std::max(1.0, std::abs(x_final)); // robust if x_final small
    int idx_settle = -1;

    for (size_t i = 0; i < rows.size(); ++i) {
      bool ok = true;
      for (size_t j = 0; j < rows.size(); ++j) {
        if (std::abs(rows[j].x - x_final) > band) {
          ok = false;
          break;
        }
      }
      if (ok) {
          idx_settle = static_cast<int>(i);
          break;
      }
    }
    m.settling_time_2pct = (idx_settle >= 0) ? rows[static_cast<size_t>(idx_settle)].t: -1.0;
    return m;
  }

} // namespace msd
