from Lexer import *
from Parser import *
from Interpreter import *
global_symbol_table = SymbolTable()
global_symbol_table.set("null", Number(0))
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
    context.symbol_table = global_symbol_table
    result = interpreter.visit(ast.node, context)

    return result.value, result.error