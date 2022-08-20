#ifndef UTILS_H
#define UTILS_H

#include <string.h>  // memset

// https://github.com/chenshuo/muduo/blob/cpp17/muduo/base/Types.h#L62
template<typename To, typename From>
inline To implicit_cast(From const &f)
{
  return f;
}

inline void mem_zero(void* p, size_t n)
{
  ::memset(p, 0, n);
}

#endif