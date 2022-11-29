from SIMPL.Constants import *
from SIMPL.Errors import *

#####################################
#Interpreter
#####################################

class Interpreter:
    def visit(self, node, context):
        #gets the appropriate visit function
        method_name = f'visit_{type(node).__name__}'
        method = getattr(self, method_name, self.no_visit_method)

        return method(node, context)

    def no_visit_method(self, node, context):
        raise Exception(f'No visit_{type(node).__name__}')

    def visit_NumberNode(self, node, context):
        return RTResult().success(Number(node.tok.value).set_context(context).set_pos(node.pos_start, node.pos_end))

    def visit_BinOpNode(self, node, context):
        res = RTResult()
        #visit the nodes on left/right
        left = res.register(self.visit(node.left_node, context))
        if res.error: return res
        right = res.register(self.visit(node.right_node, context))
        if res.error: return res

        if node.op_tok.type == TT_PLUS:
            result, error = left.added_to(right)
        elif node.op_tok.type == TT_MINUS:
            result, error = left.sub_by(right)
        elif node.op_tok.type == TT_MUL:
            result, error = left.multiply_by(right)
        elif node.op_tok.type == TT_DIV:
            result, error = left.divide_by(right)

        if error:
            return res.failure(error)
        else:
            return res.success(result.set_pos(node.pos_start, node.pos_end))
        
    def visit_UnaryOpNode(self, node, context):
        res = RTResult()
        number = res.register(self.visit(node.node, context))
        if res.error: return res

        error = None

        if node.op_tok.type == TT_MINUS:
            number = number.multiply_by(Number(-1))
        
        if error:
            return res.failure(error)
        else:
            return res.succes(number.set_pos(node.pos_start, node.pos_end))

#####################################
#VALUES
#####################################

class Number:
    def __init__(self, value) -> None:
        self.value = value
        self.set_pos()
        self.set_context()

    def __repr__(self):
        return f'{self.value}'

    def set_context(self, context = None):
        self.context = context
        return self

    def set_pos(self, pos_start=None, pos_end=None):
        self.pos_start = pos_start
        self.pos_end = pos_end

        return self

    def added_to(self,other):
        if isinstance(other, Number):
            return Number(self.value + other.value).set_context(self.context), None

    def sub_by(self,other):
        if isinstance(other, Number):
            return Number(self.value - other.value).set_context(self.context), None

    def multiply_by(self,other):
        if isinstance(other, Number):
            return Number(self.value * other.value).set_context(self.context), None

    def divide_by(self,other):
        if isinstance(other, Number):
            if other.value == 0:
                return None, RTError(
                    other.pos_start, other.pos_end, "Division By Zero", self.context
                )
            return Number(self.value / other.value).set_context(self.context), None

#####################################
#CONTEXT
#####################################
#provides context for Runtime errors
class Context:
    def __init__(self, display_name, parent=None, parent_entry_pos=None): # parent entry is the line in which the context changed -> new function calls another function
        self.display_name = display_name
        self.parent = parent
        self.parent_entry_pos = parent_entry_pos


#####################################
#RUNTIME RESULT
#####################################

class RTResult:
    def __init__(self):
        self.value = None
        self.error = None

    def register(self, res):
        if res.error: self.error = res.error
        return res.value

    def success(self, value):
        self.value = value
        return self

    def failure(self, error):
        self.error = error
        return self
