#include <optional>

#include "../../../Common/Types/Tokens/Token.hpp"
#include "../Cursor/Cursor.hpp"

namespace LOX::Scanner {

[[nodiscard]] auto handle_string_literal(Cursor& cursor) -> LOX::Types::Token;

[[nodiscard]] auto handle_number_literal(Cursor& cursor) -> LOX::Types::Token;

[[nodiscard]] auto handle_identifier(Cursor& cursor) -> LOX::Types::Token;

[[nodiscard]] auto build_special_character_token(Cursor& cursor, char const c)
    -> std::optional<LOX::Types::Token>;

[[nodiscard]] auto handle_slash(Cursor& cursor)
    -> std::optional<LOX::Types::Token>;

auto handle_whitespace(Cursor& cursor) -> std::nullopt_t;

auto handle_newline(Cursor& cursor) -> std::nullopt_t;

}  // namespace LOX::Scanner
