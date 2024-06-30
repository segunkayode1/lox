#ifndef LOX_BOX_HPP
#define LOX_BOX_HPP

#include <cstdlib>
#include <memory>
#include <type_traits>
#include <utility>

namespace lox {
template <typename T>
// implementation from https://www.foonathan.net/2022/05/recursive-variant-box/
class Box {
public:
  Box() = default;
  Box(T &&obj) : data{new T(std::move(obj))}, box_id{BOX_ID++} {}
  Box(const T &obj) : data{new T(obj)}, box_id{BOX_ID++} {}

  Box(const Box &other) : data{new T(*other.data)}, box_id{other.box_id} {}

  auto operator=(const Box &other) -> Box & {
    *data = *other.data;
    box_id = other.box_id;
    return *this;
  }

  Box(Box &&other) = default;
  auto operator=(Box &&other) -> Box & = default;

  template <typename Arg, typename... Args> Box(Arg arg, Args &&...args) {
    using k = std::remove_const_t<std::remove_reference_t<Arg>>;
    if constexpr (std::is_same_v<k, Box> and sizeof...(args) == 0U) {
      data = std::make_unique<T>(*arg);
      box_id = arg.box_id;
    } else {
      data = std::make_unique<T>(arg, std::forward<Args>(args)...);
      box_id = BOX_ID++;
    }
  }

  auto operator*() -> T & {
    box_id = BOX_ID++;
    return *data;
  }
  auto operator*() const -> T const & { return *data; }

  auto operator->() -> T * {
    box_id = BOX_ID++;
    return data.get();
  }
  auto operator->() const -> T const * { return data.get(); }
  [[nodiscard]] auto get_data() const -> T * { return data.get(); }
  [[nodiscard]] auto get_id() const -> std::size_t { return box_id; }
  ~Box() = default;

private:
  std::size_t box_id{};
  inline static std::size_t BOX_ID{}; // NOLINT : i'm not sure how to implement
                                      // without non const global variable
  std::unique_ptr<T> data;
};
}; // namespace lox

#endif