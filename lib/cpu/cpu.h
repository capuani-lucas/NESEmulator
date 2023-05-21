//
// Created by Lucas Capuani
//

#ifndef NESEMULATOR_CPU_H
#define NESEMULATOR_CPU_H

#include "../types/types.h"

// https://www.nesdev.org/obelisk-6502-guide/registers.html
struct CPU_Registers {

// Main Registers
    BYTE A;

//  Index Registers
    BYTE X;
    BYTE Y;
    BYTE SP;

//  Program counter
    WORD PC;

//  Processor Flags - Eight 1 bit flags
    union {
        struct {
            BYTE C : 1; // Carry bit
            BYTE Z : 1; // Zero bit
            BYTE I : 1; // Interrupt disable
            BYTE D : 1; // Decimal mode
            BYTE B : 1; // Break command
            BYTE _ : 1; // Unused
            BYTE O : 1; // Overflow
            BYTE N : 1; // Negative
        };
        BYTE PF;
    };

    CPU_Registers() {
        A  = 0x00;
        X  = 0x00;
        Y  = 0x00;
        SP = 0x00;
        PC = 0x0000;
        C  = 0x00;
        Z  = 0x00;
        I  = 0x00;
        D  = 0x00;
        B  = 0x00;
        _  = 0x00;
        O  = 0x00;
        N  = 0x00;
        PF = 0x00;
    }
};

// https://www.nesdev.org/obelisk-6502-guide/addressing.html
struct CPU_AddressModes {
    BYTE imm (); // Immediate
    BYTE imp (); // Impilcit
    BYTE acc (); // Accumulator
    BYTE zp  (); // Zero Page
    BYTE zpx (); // Zero Page X
    BYTE zpy (); // Zero Page Y
    BYTE rel (); // Relative
    BYTE abs (); // Absolute
    BYTE abx (); // Absolute X
    BYTE aby (); // Absolute Y
    BYTE ind (); // Indirect
    BYTE inx (); // Indexed Indirect
    BYTE iny (); // Indirect Indexed
};

// https://www.nesdev.org/obelisk-6502-guide/instructions.html
struct CPU_Instructions {
    // Load/store operations
    BYTE LDA (); // Load Accumulator
    BYTE LDX (); // Load X Register
    BYTE LDY (); // Load Y Register
    BYTE STA (); // Store Accumulator
    BYTE STX (); // Store X Register
    BYTE STY (); // Store Y Register

    // Register transfers
    BYTE TAX (); // Transfer Accumulator to X
    BYTE TAY (); // Transfer Accumulator to Y
    BYTE TXA (); // Transfer X to Accumulator
    BYTE TYA (); // Transfer Y to Accumulator

    // Stack operations
    BYTE TSX (); // Transfer Stack Pointer to X
    BYTE TXS (); // Transfer X to Stack Pointer
    BYTE PHA (); // Push Accumulator
    BYTE PHP (); // Push Processor Status
    BYTE PLA (); // Pull Accumulator
    BYTE PLP (); // Pull Processor Status

    // Logical operations
    BYTE AND (); // Logical AND
    BYTE EOR (); // Exclusive OR
    BYTE ORA (); // Logical Inclusive OR
    BYTE BIT (); // Bit Test

    // Arithmetic operations
    BYTE ADC (); // Add with Carry
    BYTE SBC (); // Subtract with Carry
    BYTE CMP (); // Compare accumulator
    BYTE CPX (); // Compare X register
    BYTE CPY (); // Compare Y register

    // Increment/decrement operations
    BYTE INC (); // Increment a memory location
    BYTE INX (); // Increment the X register
    BYTE INY (); // Increment the Y register
    BYTE DEC (); // Decrement a memory location
    BYTE DEX (); // Decrement the X register
    BYTE DEY (); // Decrement the Y register

    // Shift operations
    BYTE ASL (); // Arithmetic Shift Left
    BYTE LSR (); // Logical Shift Right
    BYTE ROL (); // Rotate Left
    BYTE ROR (); // Rotate Right

    // Jump operations
    BYTE JMP (); // Jump
    BYTE JSR (); // Jump to Subroutine
    BYTE RTS (); // Return from Subroutine

    // Branch operations
    BYTE BCC (); // Branch if carry flag Clear
    BYTE BCS (); // Branch if carry flag Set
    BYTE BEQ (); // Branch if zero flag set
    BYTE BMI (); // Branch if negative flag set
    BYTE BNE (); // Branch if zero flag clear
    BYTE BPL (); // Branch if negative flag clear
    BYTE BVC (); // Branch if overflow flag clear
    BYTE BVS (); // Branch if overflow flag set

    // Status flag operations
    BYTE CLC (); // Clear carry flag
    BYTE CLD (); // Clear decimal mode flag
    BYTE CLI (); // Clear interrupt disable flag
    BYTE CLV (); // Clear overflow flag
    BYTE SEC (); // Set carry flag
    BYTE SED (); // Set decimal mode flag
    BYTE SEI (); // Set interrupt disable flag

    // System functions
    BYTE BRK (); // Force an interrupt
    BYTE NOP (); // No Operation
    BYTE RTI (); // Return from interrupt

    // Unofficial opcodes
    // Will implement if necessary
};

struct CPU_Instruction {
    BYTE opcode;
    BYTE (CPU_Instructions::*instruction) ();
    BYTE (CPU_AddressModes::*addressMode) ();
    BYTE bytes;
    BYTE cycles;
};

class CPU {

private:
    CPU_Registers registers;
    CPU_AddressModes addressModes;
    CPU_Instructions instructions;

    // Note - Will refactor table to take up less vertical space after opcodes are implemented
    // It's a lot easier to see each row in this format

    // Opcode table
    // https://www.masswerk.at/6502/6502_instruction_set.html
    // Big thanks to ChatGPT for helping populate this table
    // I really hope it's all correct because I reallyyyyy don't want to check 256 opcodes
    using IS = CPU_Instructions;
    using AM = CPU_AddressModes;
    CPU_Instruction opcodeTable[0x100] = {

            {0x00, &IS::BRK, &AM::imp, 1, 7},
            {0x01, &IS::ORA, &AM::inx, 2, 6},
            {0x02, &IS::NOP, &AM::imp, 1, 2},
            {0x03, &IS::NOP, &AM::imp, 1, 8},
            {0x04, &IS::NOP, &AM::imp, 1, 3},
            {0x05, &IS::ORA, &AM::zp, 2, 3},
            {0x06, &IS::ASL, &AM::zp, 2, 5},
            {0x07, &IS::NOP, &AM::imp, 1, 5},
            {0x08, &IS::PHP, &AM::imp, 1, 3},
            {0x09, &IS::ORA, &AM::imm, 2, 2},
            {0x0A, &IS::ASL, &AM::acc, 1, 2},
            {0x0B, &IS::NOP, &AM::imp, 1, 2},
            {0x0C, &IS::NOP, &AM::imp, 1, 4},
            {0x0D, &IS::ORA, &AM::abs, 3, 4},
            {0x0E, &IS::ASL, &AM::abs, 3, 6},
            {0x0F, &IS::NOP, &AM::imp, 1, 6},

            {0x10, &IS::BPL, &AM::rel, 2, 2},
            {0x11, &IS::ORA, &AM::iny, 2, 5},
            {0x12, &IS::NOP, &AM::imp, 1, 2},
            {0x13, &IS::NOP, &AM::imp, 1, 8},
            {0x14, &IS::NOP, &AM::imp, 1, 4},
            {0x15, &IS::ORA, &AM::zpx, 2, 4},
            {0x16, &IS::ASL, &AM::zpx, 2, 6},
            {0x17, &IS::NOP, &AM::imp, 1, 6},
            {0x18, &IS::CLC, &AM::imp, 1, 2},
            {0x19, &IS::ORA, &AM::aby, 3, 4},
            {0x1A, &IS::NOP, &AM::imp, 1, 2},
            {0x1B, &IS::NOP, &AM::imp, 1, 7},
            {0x1C, &IS::NOP, &AM::imp, 1, 4},
            {0x1D, &IS::ORA, &AM::abx, 3, 4},
            {0x1E, &IS::ASL, &AM::abx, 3, 7},
            {0x1F, &IS::NOP, &AM::imp, 1, 7},

            {0x20, &IS::JSR, &AM::abs, 3, 6},
            {0x21, &IS::AND, &AM::inx, 2, 6},
            {0x22, &IS::NOP, &AM::imp, 1, 2},
            {0x23, &IS::NOP, &AM::imp, 1, 8},
            {0x24, &IS::BIT, &AM::zp, 2, 3},
            {0x25, &IS::AND, &AM::zp, 2, 3},
            {0x26, &IS::ROL, &AM::zp, 2, 5},
            {0x27, &IS::NOP, &AM::imp, 1, 5},
            {0x28, &IS::PLP, &AM::imp, 1, 4},
            {0x29, &IS::AND, &AM::imm, 2, 2},
            {0x2A, &IS::ROL, &AM::acc, 1, 2},
            {0x2B, &IS::NOP, &AM::imp, 1, 2},
            {0x2C, &IS::BIT, &AM::abs, 3, 4},
            {0x2D, &IS::AND, &AM::abs, 3, 4},
            {0x2E, &IS::ROL, &AM::abs, 3, 6},
            {0x2F, &IS::NOP, &AM::imp, 1, 6},

            {0x30, &IS::BMI, &AM::rel, 2, 2},
            {0x31, &IS::AND, &AM::iny, 2, 5},
            {0x32, &IS::NOP, &AM::imp, 1, 2},
            {0x33, &IS::NOP, &AM::imp, 1, 8},
            {0x34, &IS::NOP, &AM::imp, 1, 4},
            {0x35, &IS::AND, &AM::zpx, 2, 4},
            {0x36, &IS::ROL, &AM::zpx, 2, 6},
            {0x37, &IS::NOP, &AM::imp, 1, 6},
            {0x38, &IS::SEC, &AM::imp, 1, 2},
            {0x39, &IS::AND, &AM::aby, 3, 4},
            {0x3A, &IS::NOP, &AM::imp, 1, 2},
            {0x3B, &IS::NOP, &AM::imp, 1, 7},
            {0x3C, &IS::NOP, &AM::imp, 1, 4},
            {0x3D, &IS::AND, &AM::abx, 3, 4},
            {0x3E, &IS::ROL, &AM::abx, 3, 7},
            {0x3F, &IS::NOP, &AM::imp, 1, 7},

            {0x40, &IS::RTI, &AM::imp, 1, 6},
            {0x41, &IS::EOR, &AM::inx, 2, 6},
            {0x42, &IS::NOP, &AM::imp, 1, 2},
            {0x43, &IS::NOP, &AM::imp, 1, 8},
            {0x44, &IS::NOP, &AM::imp, 1, 3},
            {0x45, &IS::EOR, &AM::zp, 2, 3},
            {0x46, &IS::LSR, &AM::zp, 2, 5},
            {0x47, &IS::NOP, &AM::imp, 1, 5},
            {0x48, &IS::PHA, &AM::imp, 1, 3},
            {0x49, &IS::EOR, &AM::imm, 2, 2},
            {0x4A, &IS::LSR, &AM::acc, 1, 2},
            {0x4B, &IS::NOP, &AM::imp, 1, 2},
            {0x4C, &IS::JMP, &AM::abs, 3, 3},
            {0x4D, &IS::EOR, &AM::abs, 3, 4},
            {0x4E, &IS::LSR, &AM::abs, 3, 6},
            {0x4F, &IS::NOP, &AM::imp, 1, 6},

            {0x50, &IS::BVC, &AM::rel, 2, 2},
            {0x51, &IS::EOR, &AM::iny, 2, 5},
            {0x52, &IS::NOP, &AM::imp, 1, 2},
            {0x53, &IS::NOP, &AM::imp, 1, 8},
            {0x54, &IS::NOP, &AM::imp, 1, 4},
            {0x55, &IS::EOR, &AM::zpx, 2, 4},
            {0x56, &IS::LSR, &AM::zpx, 2, 6},
            {0x57, &IS::NOP, &AM::imp, 1, 6},
            {0x58, &IS::CLI, &AM::imp, 1, 2},
            {0x59, &IS::EOR, &AM::aby, 3, 4},
            {0x5A, &IS::NOP, &AM::imp, 1, 2},
            {0x5B, &IS::NOP, &AM::imp, 1, 7},
            {0x5C, &IS::NOP, &AM::imp, 1, 4},
            {0x5D, &IS::EOR, &AM::abx, 3, 4},
            {0x5E, &IS::LSR, &AM::abx, 3, 7},
            {0x5F, &IS::NOP, &AM::imp, 1, 7},

            {0x60, &IS::RTS, &AM::imp, 1, 6},
            {0x61, &IS::ADC, &AM::inx, 2, 6},
            {0x62, &IS::NOP, &AM::imp, 1, 2},
            {0x63, &IS::NOP, &AM::imp, 1, 8},
            {0x64, &IS::NOP, &AM::imp, 1, 3},
            {0x65, &IS::ADC, &AM::zp, 2, 3},
            {0x66, &IS::ROR, &AM::zp, 2, 5},
            {0x67, &IS::NOP, &AM::imp, 1, 5},
            {0x68, &IS::PLA, &AM::imp, 1, 4},
            {0x69, &IS::ADC, &AM::imm, 2, 2},
            {0x6A, &IS::ROR, &AM::acc, 1, 2},
            {0x6B, &IS::NOP, &AM::imp, 1, 2},
            {0x6C, &IS::JMP, &AM::ind, 3, 5},
            {0x6D, &IS::ADC, &AM::abs, 3, 4},
            {0x6E, &IS::ROR, &AM::abs, 3, 6},
            {0x6F, &IS::NOP, &AM::imp, 1, 6},

            {0x70, &IS::BVS, &AM::rel, 2, 2},
            {0x71, &IS::ADC, &AM::iny, 2, 5},
            {0x72, &IS::NOP, &AM::imp, 1, 2},
            {0x73, &IS::NOP, &AM::imp, 1, 8},
            {0x74, &IS::NOP, &AM::imp, 1, 4},
            {0x75, &IS::ADC, &AM::zpx, 2, 4},
            {0x76, &IS::ROR, &AM::zpx, 2, 6},
            {0x77, &IS::NOP, &AM::imp, 1, 6},
            {0x78, &IS::SEI, &AM::imp, 1, 2},
            {0x79, &IS::ADC, &AM::aby, 3, 4},
            {0x7A, &IS::NOP, &AM::imp, 1, 2},
            {0x7B, &IS::NOP, &AM::imp, 1, 7},
            {0x7C, &IS::NOP, &AM::imp, 1, 4},
            {0x7D, &IS::ADC, &AM::abx, 3, 4},
            {0x7E, &IS::ROR, &AM::abx, 3, 7},
            {0x7F, &IS::NOP, &AM::imp, 1, 7},

            {0x80, &IS::NOP, &AM::imp, 1, 2},
            {0x81, &IS::STA, &AM::inx, 2, 6},
            {0x82, &IS::NOP, &AM::imp, 1, 2},
            {0x83, &IS::NOP, &AM::imp, 1, 6},
            {0x84, &IS::STY, &AM::zp, 2, 3},
            {0x85, &IS::STA, &AM::zp, 2, 3},
            {0x86, &IS::STX, &AM::zp, 2, 3},
            {0x87, &IS::NOP, &AM::imp, 1, 3},
            {0x88, &IS::DEY, &AM::imp, 1, 2},
            {0x89, &IS::NOP, &AM::imp, 1, 2},
            {0x8A, &IS::TXA, &AM::imp, 1, 2},
            {0x8B, &IS::NOP, &AM::imp, 1, 2},
            {0x8C, &IS::STY, &AM::abs, 3, 4},
            {0x8D, &IS::STA, &AM::abs, 3, 4},
            {0x8E, &IS::STX, &AM::abs, 3, 4},
            {0x8F, &IS::NOP, &AM::imp, 1, 4},

            {0x90, &IS::BCC, &AM::rel, 2, 2},
            {0x91, &IS::STA, &AM::iny, 2, 6},
            {0x92, &IS::NOP, &AM::imp, 1, 2},
            {0x93, &IS::NOP, &AM::imp, 1, 6},
            {0x94, &IS::STY, &AM::zpx, 2, 4},
            {0x95, &IS::STA, &AM::zpx, 2, 4},
            {0x96, &IS::STX, &AM::zpy, 2, 4},
            {0x97, &IS::NOP, &AM::imp, 1, 4},
            {0x98, &IS::TYA, &AM::imp, 1, 2},
            {0x99, &IS::STA, &AM::aby, 3, 5},
            {0x9A, &IS::TXS, &AM::imp, 1, 2},
            {0x9B, &IS::NOP, &AM::imp, 1, 5},
            {0x9C, &IS::NOP, &AM::imp, 1, 5},
            {0x9D, &IS::STA, &AM::abx, 3, 5},
            {0x9E, &IS::NOP, &AM::imp, 1, 5},
            {0x9F, &IS::NOP, &AM::imp, 1, 5},

            {0xA0, &IS::LDY, &AM::imm, 2, 2},
            {0xA1, &IS::LDA, &AM::inx, 2, 6},
            {0xA2, &IS::LDX, &AM::imm, 2, 2},
            {0xA3, &IS::NOP, &AM::imp, 1, 6},
            {0xA4, &IS::LDY, &AM::zp, 2, 3},
            {0xA5, &IS::LDA, &AM::zp, 2, 3},
            {0xA6, &IS::LDX, &AM::zp, 2, 3},
            {0xA7, &IS::NOP, &AM::imp, 1, 3},
            {0xA8, &IS::TAY, &AM::imp, 1, 2},
            {0xA9, &IS::LDA, &AM::imm, 2, 2},
            {0xAA, &IS::TAX, &AM::imp, 1, 2},
            {0xAB, &IS::NOP, &AM::imp, 1, 2},
            {0xAC, &IS::LDY, &AM::abs, 3, 4},
            {0xAD, &IS::LDA, &AM::abs, 3, 4},
            {0xAE, &IS::LDX, &AM::abs, 3, 4},
            {0xAF, &IS::NOP, &AM::imp, 1, 4},

            {0xB0, &IS::BCS, &AM::rel, 2, 2},
            {0xB1, &IS::LDA, &AM::iny, 2, 5},
            {0xB2, &IS::NOP, &AM::imp, 1, 2},
            {0xB3, &IS::NOP, &AM::imp, 1, 5},
            {0xB4, &IS::LDY, &AM::zpx, 2, 4},
            {0xB5, &IS::LDA, &AM::zpx, 2, 4},
            {0xB6, &IS::LDX, &AM::zpy, 2, 4},
            {0xB7, &IS::NOP, &AM::imp, 1, 4},
            {0xB8, &IS::CLV, &AM::imp, 1, 2},
            {0xB9, &IS::LDA, &AM::aby, 3, 4},
            {0xBA, &IS::TSX, &AM::imp, 1, 2},
            {0xBB, &IS::NOP, &AM::imp, 1, 4},
            {0xBC, &IS::LDY, &AM::abx, 3, 4},
            {0xBD, &IS::LDA, &AM::abx, 3, 4},
            {0xBE, &IS::LDX, &AM::aby, 3, 4},
            {0xBF, &IS::NOP, &AM::imp, 1, 4},

            {0xC0, &IS::CPY, &AM::imm, 2, 2},
            {0xC1, &IS::CMP, &AM::inx, 2, 6},
            {0xC2, &IS::NOP, &AM::imp, 1, 2},
            {0xC3, &IS::NOP, &AM::imp, 1, 8},
            {0xC4, &IS::CPY, &AM::zp, 2, 3},
            {0xC5, &IS::CMP, &AM::zp, 2, 3},
            {0xC6, &IS::DEC, &AM::zp, 2, 5},
            {0xC7, &IS::NOP, &AM::imp, 1, 5},
            {0xC8, &IS::INY, &AM::imp, 1, 2},
            {0xC9, &IS::CMP, &AM::imm, 2, 2},
            {0xCA, &IS::DEX, &AM::imp, 1, 2},
            {0xCB, &IS::NOP, &AM::imp, 1, 2},
            {0xCC, &IS::CPY, &AM::abs, 3, 4},
            {0xCD, &IS::CMP, &AM::abs, 3, 4},
            {0xCE, &IS::DEC, &AM::abs, 3, 6},
            {0xCF, &IS::NOP, &AM::imp, 1, 6},

            {0xD0, &IS::BNE, &AM::rel, 2, 2},
            {0xD1, &IS::CMP, &AM::iny, 2, 5},
            {0xD2, &IS::NOP, &AM::imp, 1, 2},
            {0xD3, &IS::NOP, &AM::imp, 1, 8},
            {0xD4, &IS::NOP, &AM::imp, 1, 4},
            {0xD5, &IS::CMP, &AM::zpx, 2, 4},
            {0xD6, &IS::DEC, &AM::zpx, 2, 6},
            {0xD7, &IS::NOP, &AM::imp, 1, 6},
            {0xD8, &IS::CLD, &AM::imp, 1, 2},
            {0xD9, &IS::CMP, &AM::aby, 3, 4},
            {0xDA, &IS::NOP, &AM::imp, 1, 2},
            {0xDB, &IS::NOP, &AM::imp, 1, 7},
            {0xDC, &IS::NOP, &AM::imp, 1, 4},
            {0xDD, &IS::CMP, &AM::abx, 3, 4},
            {0xDE, &IS::DEC, &AM::abx, 3, 7},
            {0xDF, &IS::NOP, &AM::imp, 1, 7},

            {0xE0, &IS::CPX, &AM::imm, 2, 2},
            {0xE1, &IS::SBC, &AM::inx, 2, 6},
            {0xE2, &IS::NOP, &AM::imp, 1, 2},
            {0xE3, &IS::NOP, &AM::imp, 1, 8},
            {0xE4, &IS::CPX, &AM::zp, 2, 3},
            {0xE5, &IS::SBC, &AM::zp, 2, 3},
            {0xE6, &IS::INC, &AM::zp, 2, 5},
            {0xE7, &IS::NOP, &AM::imp, 1, 5},
            {0xE8, &IS::INX, &AM::imp, 1, 2},
            {0xE9, &IS::SBC, &AM::imm, 2, 2},
            {0xEA, &IS::NOP, &AM::imp, 1, 2},
            {0xEB, &IS::NOP, &AM::imp, 1, 2},
            {0xEC, &IS::CPX, &AM::abs, 3, 4},
            {0xED, &IS::SBC, &AM::abs, 3, 4},
            {0xEE, &IS::INC, &AM::abs, 3, 6},
            {0xEF, &IS::NOP, &AM::imp, 1, 6},

            {0xF0, &IS::BEQ, &AM::rel, 2, 2},
            {0xF1, &IS::SBC, &AM::iny, 2, 5},
            {0xF2, &IS::NOP, &AM::imp, 1, 2},
            {0xF3, &IS::NOP, &AM::imp, 1, 8},
            {0xF4, &IS::NOP, &AM::imp, 1, 4},
            {0xF5, &IS::SBC, &AM::zpx, 2, 4},
            {0xF6, &IS::INC, &AM::zpx, 2, 6},
            {0xF7, &IS::NOP, &AM::imp, 1, 6},
            {0xF8, &IS::SED, &AM::imp, 1, 2},
            {0xF9, &IS::SBC, &AM::aby, 3, 4},
            {0xFA, &IS::NOP, &AM::imp, 1, 2},
            {0xFB, &IS::NOP, &AM::imp, 1, 7},
            {0xFC, &IS::NOP, &AM::imp, 1, 4},
            {0xFD, &IS::SBC, &AM::abx, 3, 4},
            {0xFE, &IS::INC, &AM::abx, 3, 7},
            {0xFF, &IS::NOP, &AM::imp, 1, 7}
    };




};


#endif //NESEMULATOR_CPU_H
