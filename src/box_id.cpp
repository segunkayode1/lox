#include <cstdlib>
#include <limits>
#include <variant>

#include "box_id.hpp"
#include "expr.hpp"

namespace lox {

auto get_id(Expr const &expr) -> std::size_t {
  static const auto visitor = get_id_visitor{};
  return visitor.get_id(expr);
}

auto get_id_visitor::get_id(Expr const &expr) const -> std::size_t {
  return std::visit(*this, expr);
}

template <typename T>
auto get_id_visitor::operator()(T const &box) const -> std::size_t {
  return box.get_id();
}

auto get_id_visitor::operator()(Expr_Monostate /*unused*/) const
    -> std::size_t {
  return std::numeric_limits<std::size_t>::max();
}

}; // namespace lox