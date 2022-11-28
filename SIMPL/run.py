from Lexer import Lexer
from Parser import Parser
from Interpreter import Interpreter, Context
def run(fn, text):
    # Gen Tokens
    lexer = Lexer(fn, text)
    tokens, error = lexer.make_tokens()
    if error: return None, error

    # Gen Abstract Syntax Tree
    parser = Parser(tokens)
    ast = parser.parse()
    if ast.error: return None, ast.error

    interpreter = Interpreter()
    context = Context('<program>')
    result = interpreter.visit(ast.node, context)

    return result.value, result.error