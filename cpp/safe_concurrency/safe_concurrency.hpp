#pragma once

#include <map>
#include <mutex>

extern std::recursive_mutex mtx_blocked_pair;
extern std::map<std::thread::id, std::thread::id> blocked_pair;