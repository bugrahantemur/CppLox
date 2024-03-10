#include <optional>

#include "../../Types/Tokens/Token.hpp"
#include "../Cursor/Cursor.hpp"

namespace LOX::Common::Scanner {

[[nodiscard]] auto handle_string_literal(Cursor& cursor)
    -> LOX::Common::Types::Token;

[[nodiscard]] auto handle_number_literal(Cursor& cursor)
    -> LOX::Common::Types::Token;

[[nodiscard]] auto handle_identifier(Cursor& cursor)
    -> LOX::Common::Types::Token;

[[nodiscard]] auto build_special_character_token(Cursor& cursor, char const c)
    -> std::optional<LOX::Common::Types::Token>;

[[nodiscard]] auto handle_slash(Cursor& cursor)
    -> std::optional<LOX::Common::Types::Token>;

auto handle_whitespace(Cursor& cursor) -> std::nullopt_t;

auto handle_newline(Cursor& cursor) -> std::nullopt_t;

}  // namespace LOX::Common::Scanner
