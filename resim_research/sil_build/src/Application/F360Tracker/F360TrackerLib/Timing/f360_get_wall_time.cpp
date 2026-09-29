#include "f360_get_wall_time.h"

#if defined(_WIN32) || defined(_WIN64) || defined(__linux__) || defined(__QNX__)
#include <chrono>
namespace f360_variant_A
{
   float get_wall_time()
   {
      static bool f_initialized = false;
      static std::chrono::time_point<std::chrono::steady_clock> start_time;
      if (!f_initialized)
      {
         start_time = std::chrono::steady_clock::now();
         f_initialized = true;
      }
      const std::chrono::time_point<std::chrono::steady_clock> current_time = std::chrono::steady_clock::now();
      const int64_t time_since_start_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - start_time).count();
      
      return  static_cast<float>(time_since_start_ns) / 1e9F;
   }
}
#else
//when compiling unrecoginized platform
namespace f360_variant_A
{
   float get_wall_time()
   {
      return 0.0F;
   }
}
#endif
