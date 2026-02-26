#pragma once
#include "types.h"
#include <string>
#include <vector>

namespace msd {

  void write_csv(const std::string& path, const std::vector<Sample>& rows);

} // namespace msd
