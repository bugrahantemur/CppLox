#include "./Compiler.hpp"

#include <iostream>

#include "../../Common/Scanner/Scanner.hpp"
#include "../../Common/Types/Tokens/Token.hpp"
#include "../../Common/Types/Tokens/TokenTypes.hpp"

namespace LOX::ByteCode::Compiler {

using LOX::Common::Types::Token;

auto compile(std::string const& source) -> void {
  std::vector<Token> tokens = Common::Scanner::scan(source);
  int line = -1;
  for (Token const& token : tokens) {
    if (token.line != line) {
      std::cout << "   " << token.line << ' ';
      line = token.line;
    } else {
      printf("   | ");
    }
    std::cout << static_cast<int>(token.type) << " '" << token.lexeme << "'\n";

    if (token.type == LOX::Common::Types::Tokens::TokenType::EOFF) break;
  }
}

}  // namespace LOX::ByteCode::Compiler
