#include "safe_concurrency.hpp"

std::recursive_mutex mtx_blocked_pair;
std::map<std::thread::id, std::thread::id> blocked_pair;
