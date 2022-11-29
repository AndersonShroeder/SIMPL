from SIMPL.Errors import *
from SIMPL.Constants import *

#####################################
#POSTION
#####################################
class Position:
    def __init__(self, idx, ln, col, filename, filetext) -> None:
        self.idx = idx
        self.ln = ln
        self.col = col
        self.ftxt = filetext
        self.fn = filename

    def advance(self, current_char=None):
        self.idx += 1
        self.col += 1

        if current_char == '\n':
            self.ln += 1
            self.col = 0

        return self

    def copy(self):
        return Position(self.idx, self.ln, self.col, self.fn, self.ftxt)


#####################################
#TOKENS
#####################################

#Token class

class Token:
    def __init__(self, type_, value=None, pos_start=None, pos_end=None):
        self.type = type_
        self.value = value

        if pos_start:
            self.pos_start = pos_start.copy()
            self.pos_end = pos_start.copy().advance()

        if pos_end:
            self.pos_end = pos_end


    def __repr__(self) -> str:
        if self.value: return f'{self.type}:{self.value}'
        return f'{self.type}'

#####################################
#LEXER
#####################################

class Lexer:
    def __init__(self, fn, text) -> None:
        self.fn = fn
        self.text = text
        self.pos = Position(-1, 0, -1, fn, text)   #start at -1 because advance will incrament this to zero on init
        self.current_char = None
        self.advance()

    #adjusts current_char to the next char in the text
    def advance(self):
        self.pos.advance(self.current_char)
        self.current_char = self.text[self.pos.idx] if self.pos.idx < len(self.text) else None

    def make_tokens(self):
        tokens = []

        #make a token as long as there are more chars to check
        while self.current_char != None:

            #ignore white space
            if self.current_char in ' \t':
                self.advance()

            elif self.current_char in DIGITS:
                tokens.append(self.make_number())
            
            elif self.current_char == '+':
                tokens.append(Token(TT_PLUS, pos_start=self.pos))
                self.advance()
                
            elif self.current_char == '-':
                tokens.append(Token(TT_MINUS, pos_start=self.pos))
                self.advance()

            elif self.current_char == '*':
                tokens.append(Token(TT_MUL, pos_start=self.pos))
                self.advance()

            elif self.current_char == '/':
                tokens.append(Token(TT_DIV, pos_start=self.pos))
                self.advance()

            elif self.current_char == '(':
                tokens.append(Token(TT_LPAREN, pos_start=self.pos))
                self.advance()

            elif self.current_char == ')':
                tokens.append(Token(TT_RPAREN, pos_start=self.pos))
                self.advance()

            else:
                pos_start = self.pos.copy()
                char = self.current_char
                self.advance()
                return [], IllegalCharError(pos_start, self.pos, char)
        
        tokens.append(Token(TT_EOF, pos_start=self.pos))
        return tokens, None
    
    def make_number(self):
        num_str = ''
        decimal = 0
        pos_start = self.pos.copy()

        while self.current_char != None and self.current_char in DIGITS + '.':
            if self.current_char == '.':
                decimal += 1
            num_str += self.current_char
            self.advance()

        if decimal == 0:
            return Token(TT_INT, int(num_str), pos_start, self.pos)
        
        else:
            return Token(TT_FLOAT, float(num_str), pos_start, self.pos)
