//
// Created by Anderson Schroeder on 10/26/23.
//

#include "InstructionStream.h"

#include <utility>
#include <iostream>
#include <iomanip>

InstructionStream::InstructionStream(std::string name) {
    this->name = std::move(name);
}

void InstructionStream::addByte(uint8_t byte) {
    this->byteStream.push_back(byte);
}

void InstructionStream::disassemble() {
    // Print the name of the stream
    std::cout << "-~- " << this->name << " -~-\n";

    // Loop through each byte in the stream and disassemble each instruction
    // We can't simply increment by 1 each time because instructions have different lengths
    for (int offset = 0; offset < this->byteStream.size();)
    {
        offset = disassembleInstruction(offset);
    }
}

int InstructionStream::simpleInstruction(std::string opName, int offset) {
    std::cout << opName << '\n';
    return offset + 1;
}

int InstructionStream::disassembleInstruction(int offset) {
    // Print the current offset
    std::cout << std::setw(4) << std::setfill('0') << offset << " ";

    // Get the next byte from the stream and switch on its value
    uint8_t instruction = this->byteStream.at(offset);

    switch (instruction)
    {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);

        default:
            std::cout << "Unknown opcode " << instruction << '\n';
            return offset + 1;
    }
}
