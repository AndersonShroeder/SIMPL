#include <iostream>
#include "InstructionStream/InstructionStream.h"

int main() {
    InstructionStream stream{"test"};
    stream.addByte(OP_RETURN);
    stream.disassemble();
    return 0;
}
