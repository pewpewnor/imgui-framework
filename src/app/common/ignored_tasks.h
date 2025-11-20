#include <future>
#include <mutex>
#include <vector>

namespace ignored_tasks {

inline std::vector<std::shared_future<void>> ignoredFutures;
inline std::mutex ignoredFuturesMutex;

void waitAllIgnoredFutures(std::chrono::seconds waitTimeSeconds);

void clearAllIgnoredFutures();

}
