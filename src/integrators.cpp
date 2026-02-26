#include "msd/integrators.h"
#include "msd/model.h"

namespace msd {
  State step_semi_implicit_euler(const Params& p, const State& s, double t, double dt, const InputFn& ufn) {
    const double u = ufn(t);
    const double a = accel(p, s, u);

    // semi implicit: v_{n+1} = v_n + a_n dt; x_{n+1} = x_n + v_{n+1} dt
    State ns = s;
    ns.v = s.v + a * dt;
    ns.x = s.x + ns.v * dt;
    return ns;
  }

  static inline void deriv(const Params& p, double t, const State& s, const InputFn& ufn, State& dsdt) {
    const double u = ufn(t);
    dsdt.x = s.v;
    dsdt.v = accel(p, s, u);
  }

  State step_rk4(const Params& p, const State& s, double t, double dt, const InputFn& ufn) {
    State k1{}, k2{}, k3{}, k4{};
    deriv(p, t, s, ufn, k1);

    State s2{
      s.x + 0.5*dt*k1.x,
      s.v + 0.5*dt*k1.v
    };
    deriv(p, t + 0.5*dt, s2, ufn, k2);

    State s3{
      s.x + 0.5*dt*k2.x,
      s.v + 0.5*dt*k2.v
    };
    deriv(p, t + 0.5*dt, s3, ufn, k3);

    State s4{
      s.x + 0.5*dt*k3.x,
      s.v + 0.5*dt*k3.v
    };
    deriv(p, t + dt, s4, ufn, k4);

    State ns{};
    ns.x = s.x + (dt/6.0)*(k1.x + 2.0*k2.x + 2.0*k3.x + k4.x);
    ns.v = s.v + (dt/6.0)*(k1.v + 2.0*k2.v + 2.0*k3.v + k4.v);
    return ns;
  }

} // namespace msd
