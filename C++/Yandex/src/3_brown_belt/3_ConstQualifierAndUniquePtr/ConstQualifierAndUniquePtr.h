#pragma once

#include <functional>
#include <optional>

namespace BrownBeltWeek3
{
#pragma region Task 2. LazyValue

  template<typename T>
  class LazyValue {
    public:
    explicit LazyValue(std::function<T()> init) : d_body(init) {}

    bool HasValue() const
    {
      return d_object.has_value();
    }

    const T& Get() const
    {
      if (!d_object.has_value())
        d_object = d_body();
      return *d_object;
    }

    private:
    std::function<T()> d_body;
    mutable std::optional<T> d_object{ std::nullopt };
  };

#pragma endregion

}  // namespace BrownBeltWeek3
