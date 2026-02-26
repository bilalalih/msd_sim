#pragma once

namespace msd {

  struct Params {
    double m{1.0};
    double c{0.2};
    double k{10.0};
  };

  struct State {
    double x{0.0};
    double v{0.0};
  };

  struct Sample {
    double t{0.0};
    double x{0.0};
    double v{0.0};
    double a{0.0};
    double u{0.0};
    double E{0.0};
  };

} // namespace msd
