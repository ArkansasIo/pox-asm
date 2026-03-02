#include "pox64_log.h"
#include <iostream>

namespace pox64 {
  void log_info(const std::string& s) { std::cout << "[INFO] " << s << "\n"; }
  void log_warn(const std::string& s) { std::cout << "[WARN] " << s << "\n"; }
  void log_err (const std::string& s) { std::cerr << "[ERR ] " << s << "\n"; }
}
