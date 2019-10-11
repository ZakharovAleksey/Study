#pragma once

#include <algorithm>

template<typename It>
class IteratorRange {
  public:
  IteratorRange(It first, It last) : d_first(first), d_last(last) {}

  It begin() const
  {
    return d_first;
  }

  It end() const
  {
    return d_last;
  }

  size_t size() const
  {
    return d_last - d_first;
  }

  private:
  It d_first, d_last;
};

template<typename Container>
auto Head(Container& i_container, int i_top)
{
  return IteratorRange(
      std::begin(i_container),
      std::begin(i_container) +
          std::min<size_t>(std::max(i_top, 0), i_container.size()));
}
