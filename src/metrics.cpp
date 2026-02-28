#include "msd/metrics.h"
#include <cmath>
#include <iostream>
#include <algorithm>

namespace msd {

  Metrics compute_step_metrics(const std::vector<Sample>& rows, double x_final, double t_step){
    Metrics m{};

    if (rows.empty()) return m;
    // Find first index after step
    size_t i0 = 0;
    while(i0 < rows.size() && rows[i0].t < t_step) ++i0;
    if (i0 >= rows.size()) return m;

    // max |x|
    for(size_t i = i0; i < rows.size(); ++i) m.max_abs_x = std::max(m.max_abs_x, std::abs(rows[i].x));
    // settling time 2%: earliest time after which all remaining samples stay within 2% band
    const double band = 0.02 * std::max(1e-9, std::abs(x_final)); // robust if x_final small

    // Overshoot: peak above final after step
    double x_peak = rows[i0].x;
    for(size_t i = i0; i < rows.size(); ++i) x_peak = std::max(x_peak, rows[i].x);
    if(std::abs(x_final) > 1e-12) {
      m.peak_overshoot = std::max(0.0, (x_peak - x_final)/ std::abs(x_final));
    }

    // steady state error: final sample to x_final
    m.steady_state_error = std::abs(rows.back().x - x_final);

    int idx_settle = -1;
    for (size_t i = i0; i < rows.size(); ++i) {
      bool ok = true;
      for (size_t j = i; j < rows.size(); ++j) {
        if (std::abs(rows[j].x - x_final) > band) {ok = false; break;}
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
