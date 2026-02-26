#include "msd/csv.h"
#include <fstream>
#include <iomanip>
#include <stdexcept>

namespace msd {
  void write_csv(const std::string& path, const std::vector<Sample>& rows) {
    std::ofstream f(path);
    if(!f) throw std::runtime_error("Failed to open file: " + path);

    f << "time, x, v, a, u, E\n";
    f << std::setprecision(17);

    for(const auto& r: rows) {
      f << r.t << "," << r.x << "," << r.v << "," << r.a << "," << r.u << "," << r.E << "\n";
    }
  }

}// namespace msd
