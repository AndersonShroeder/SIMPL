//
// Created by Anderson Schroeder on 10/26/23.
//

#ifndef SIMPL_INSTRUCTIONSTREAM_H
#define SIMPL_INSTRUCTIONSTREAM_H


#include <vector>
#include <string>

enum OpCode {
    OP_RETURN
};

class InstructionStream {
private:
    std::vector<uint8_t> byteStream;
    std::string name;

public:
    InstructionStream(std::string name);

    void addByte(uint8_t byte);

    void disassemble();

    static int simpleInstruction(std::string name, int offset);

    int disassembleInstruction(int offset);
};


#endif //SIMPL_INSTRUCTIONSTREAM_H
