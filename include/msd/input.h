#pragma once
#include <memory>

namespace msd {
  struct Input {
    virtual ~Input() = default;
    virtual double u(double t) const = 0;
  };

  struct StepInput final: Input {
    double t0{0.0};
    double amp{1.0};
    double u(double t) const override {
      return (t >= t0)? amp: 0.0;
    }
  };

  struct SineInput final: Input {
    double amp{1.0};
    double freq_hz{1.0};
    double phase{0.0};
    double u(double t) const override;
  };

  struct ImpulseInput final: Input {
    // Approx impulse: u(t) = amp/dt for t in [t0, t0+dt]
    double t0{0.0};
    double amp{1.0}; // impulse area
    double dt{1e-3};
    double u(double t) const override;
  };

  std::unique_ptr<Input> make_step(double t0, double amp);
  std::unique_ptr<Input> make_sine(double amp, double freq_hz, double phase);
  std::unique_ptr<Input> make_impulse(double t0, double area, double dt);

} // namespace msd
