#include "./Compiler.hpp"

#include <iostream>

#include "../../Common/Parser/Parser.hpp"
#include "../../Common/Scanner/Scanner.hpp"
#include "../../Common/Types/Syntax/Statement.hpp"
#include "../../Common/Types/Tokens/Token.hpp"
#include "../../Common/Types/Tokens/TokenTypes.hpp"

namespace LOX::ByteCode::Compiler {

using LOX::Common::Types::Token;
using LOX::Common::Types::Syntax::Statements::Statement;

auto compile(std::string const& source) -> void {
  std::vector<Token> tokens = Common::Scanner::scan(source);
  std::vector<Statement> statements = Common::Parser::parse(tokens);
}

}  // namespace LOX::ByteCode::Compiler
