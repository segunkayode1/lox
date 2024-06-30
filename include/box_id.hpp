#ifndef LOX_BOX_ID_HPP
#define LOX_BOX_ID_HPP

#include <cstdlib>

#include "expr.hpp"

namespace lox {

[[nodiscard]] auto get_id(Expr const &expr) -> std::size_t;

struct get_id_visitor {

  [[nodiscard]] auto get_id(Expr const &expr) const -> std::size_t;

  template <typename T> auto operator()(T const &box) const -> std::size_t;

  auto operator()(Expr_Monostate /*unused*/) const -> std::size_t;
};

}; // namespace lox

#endif