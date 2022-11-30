import Lexer
import Parser
import Interpreter
def run(fn, text):
    # Gen Tokens
    lexer = Lexer.Lexer(fn, text)
    tokens, error = lexer.make_tokens()
    if error: return None, error

    # Gen Abstract Syntax Tree
    parser = Parser.Parser(tokens)
    ast = parser.parse()
    if ast.error: return None, ast.error

    interpreter = Interpreter.Interpreter()
    context = Interpreter.Context('<program>')
    result = interpreter.visit(ast.node, context)

    return result.value, result.error