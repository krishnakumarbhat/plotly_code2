#include "sg_detection_cache.h"

namespace sg
{
   void DetectionCache::remove_detection(const Detection_T *const det_ptr)
   {
      assert(m_size > 0);
      for (std::size_t i = 0U; i < m_size; ++i)
      {
         if (m_cache[i] == det_ptr)
         {
            m_cache[i]           = m_cache[m_size - 1U];
            m_cache[m_size - 1U] = nullptr;
            --m_size;
            break;
         }
      }
   }
}
