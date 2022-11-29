from SIMPL.Errors import *
from SIMPL.Constants import *

#####################################
#Nodes
#####################################

#Nodes to be used in parser tree
class NumberNode:
    def __init__(self, tok) -> None:
        self.tok = tok

        self.pos_start = self.tok.pos_start
        self.pos_end = self.tok.pos_end

    def __repr__(self):
        return f'{self.tok}'

#represents an operation between two nodes
class BinOpNode:
    def __init__(self, left_node, op_tok, right_node) -> None:
        self.left_node = left_node
        self.op_tok = op_tok
        self.right_node = right_node

        self.pos_start = self.left_node.pos_start
        self.pos_end = self.right_node.pos_end

    def __repr__(self) -> str:
        return f'({self.left_node}, {self.op_tok}, {self.right_node})'

#used for Unary Operations
class UnaryOpNode:
    def __init__(self, op_tok, node) -> None:
        self.op_tok = op_tok
        self.node = node

        self.pos_start = self.op_tok.pos_start
        self.pos_end = self.node.pos_end

    def __repr__(self):
        return f'({self.op_tok}, {self.node})'
    
#####################################
#PARSER
#####################################

class ParseResult:
    def __init__(self) -> None:
        self.error = None
        self.node = None

    def register(self, res):
        if isinstance(res, ParseResult):
            if res.error: self.error = res.error
            return res.node
        return res

    def success(self, node):
        self.node = node
        return self
    
    def failure(self, error):
        self.error = error
        return self

class Parser:
    def __init__(self, tokens) -> None:
        self.tokens = tokens
        self.tok_idx = -1
        self.advance()
    
    def advance(self):
        self.tok_idx += 1
        if self.tok_idx < len(self.tokens):
            self.current_tok = self.tokens[self.tok_idx]
        return self.current_tok

    def parse(self):
        res = self.expression()
        if not res.error and self.current_tok.type != TT_EOF:
            return res.failure(InvalidSyntaxError(
                self.current_tok.pos_start, self.current_tok.pos_end, 'Expected +, -, * or /'
            ))
        return res


    #turn grammer rules into nodes for tree
    def factor(self):
        res = ParseResult()
        tok = self.current_tok

        if tok.type in (TT_PLUS, TT_MINUS):
            res.register(self.advance())
            factor = res.register(self.factor())
            if res.error: return res
            return res.success(UnaryOpNode(tok, factor))

        elif tok.type in (TT_INT, TT_FLOAT):
            res.register(self.advance())
            return res.success(NumberNode(tok))

        elif tok.type == TT_LPAREN:
            res.register(self.advance())
            expr = res.register(self.expression())
            if res.error: return res

            # check for right parenthesis
            if self.current_tok.type == TT_RPAREN:
                res.register(self.advance())
                return res.success(expr)

            else:
                return res.failure(InvalidSyntaxError(
                    self.current_tok.pos_start, self.current_tok.pos_end, "Expected )"
                ))

        return res.failure(InvalidSyntaxError(
            tok.pos_start, tok.pos_end, "Expected Int Or Float"
        ))

    def term(self):
        return self.binary_operation(self.factor, (TT_MUL, TT_DIV))

    def expression(self):
        return self.binary_operation(self.term, (TT_PLUS, TT_MINUS))
    
    def binary_operation(self, func, operation_toks):
        res = ParseResult()
        left = res.register(func()) #takes node from parse result
        if res.error: return res

        while self.current_tok.type in operation_toks:
            op_tok = self.current_tok
            res.register(self.advance())
            right = res.register(func())
            if res.error: return res
            left = BinOpNode(left, op_tok, right)
        
        return res.success(left)
