#include "msd/input.h"
#include <cmath>

namespace msd {
  double SineInput::u(double t) const {
    const double w = 2.0 * 3.14159265358979323846 * freq_hz;
    return amp * std::sin(w * t + phase);
  }

  double ImpulseInput::u(double t) const {
    return (t >= t0 && t < t0 + dt)? (amp / dt): 0.0;
  }

  std::unique_ptr<Input> make_step(double t0, double amp) {
    auto in = std::make_unique<StepInput>();
    in->t0 = t0; in->amp = amp;
    return in;
  }

  std::unique_ptr<Input> make_sine(double amp, double freq_hz, double phase) {
    auto in = std::make_unique<SineInput>();
    in->amp = amp; in->freq_hz = freq_hz; in->phase = phase;
    return in;
  }

  std::unique_ptr<Input> make_impulse(double t0, double area, double dt) {
    auto in = std::make_unique<ImpulseInput>();
    in->t0 = t0; in->amp = area; in->dt = dt;
    return in;
  }
}
