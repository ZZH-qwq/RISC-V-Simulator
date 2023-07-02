#pragma once
#include "utils.h"
#include <iostream>
#include <bitset>
using namespace std;

using inst_addr = unsigned long;
using reg_name = unsigned long;

enum Num_type { byte = 8,
                half_word = 16,
                word = 32
};

enum Num_sign { Signed,
                Unsigned };

enum Opcode { I_load = 0x03,
              I_func = 0x13,
              U_auipc = 0x17,
              S_save = 0x23,
              R_func = 0x33,
              U_lui = 0x37,
              B_branch = 0x63,
              I_jalr = 0x67,
              J_jal = 0x6F
};

string opcode_name(Opcode code) {
    switch (code) {
        case I_load:
            return "I_load";
        case I_func:
            return "I_func";
        case U_auipc:
            return "U_auipc";
        case S_save:
            return "S_save";
        case R_func:
            return "R_func";
        case U_lui:
            return "U_lui";
        case B_branch:
            return "B_branch";
        case I_jalr:
            return "I_jalr";
        case J_jal:
            return "J_jal";
        default:
            return "Invalid Opcode";
    }
}

enum ALU_optype { A_NONE,
                  SUM,
                  SUB,
                  AND,
                  OR,
                  XOR,
                  SLL,
                  SRL,
                  SRA,
                  LT,
                  GE,
                  LTU,
                  GEU,
                  EQ,
                  NE
};

string AUL_optype_name(ALU_optype optype) {
    const string name[] = {"A_NONE", "SUM", "SUB", "AND", "OR", "XOR", "SLL", "SRL", "SRA", "LT", "GE", "LTU", "GEU", "EQ", "NE"};
    return name[(size_t)optype];
}

enum MEM_optype { M_NONE,
                  LOAD,
                  SAVE };

enum reg_state { available,
                 blocked,
                 ALU_ready,
                 MEM_ready,
                 RWB_ready };

class Reg {
   protected:
    bitset<32> data;

   public:
    Reg() {}
    template <typename T>
    Reg(T val) : data(val) {}
    Reg(const Reg& other) : data(other.data) {}

    constexpr bool operator[](size_t pos) const { return data[pos]; }
    auto operator[](size_t pos) { return data[pos]; }

    template <typename T>
    auto operator=(T val) { return data = val; }
    auto operator=(const Reg& other) {
        data = other.data;
        return *this;
    }

    int32_t word() const {
        return data.to_ulong();
    }
    uint32_t uword() const {
        return data.to_ulong();
    }
};

class Inst : public Reg {
   public:
    Inst() {}
    template <typename T>
    Inst(T val) : Reg(val) {}
    Inst(const Inst& other) : Reg(other.data) {}

    Opcode opcode() { return (Opcode)(data.to_ulong() & 0x7F); }
    reg_name rd() { return (data.to_ulong() & 0xF80) >> 7; }
    reg_name rs1() { return (data.to_ulong() & 0xF8000) >> 15; }
    reg_name rs2() { return (data.to_ulong() & 0x1F00000) >> 20; }
    uint32_t func3() { return (data.to_ulong() & 0x7000) >> 12; }

    uint32_t imm_u() { return data.to_ulong() & 0xFFFFF000; }
    uint32_t imm_j() { return extend_sign_bit((data.to_ulong() & 0xFF000) | (data[20] << 11) | ((data.to_ulong() & 0x7FE00000) >> 20) | (data[31] << 20), 21); }
    uint32_t imm_i() { return extend_sign_bit((data.to_ulong() & 0xFFF00000) >> 20, 12); }
    uint32_t imm_s() { return extend_sign_bit(((data.to_ulong() & 0xFE000000) >> 20) | ((data.to_ulong() & 0xF80) >> 7), 12); }
    uint32_t imm_b() { return extend_sign_bit(((data.to_ulong() & 0x7E000000) >> 20) | (data[31] << 12) | ((data.to_ulong() & 0xF00) >> 7) | (data[7] << 11), 13); }
    uint32_t imm() {
        switch (opcode()) {
            case U_auipc:
            case U_lui:
                return imm_u();
            case J_jal:
                return imm_j();
            case I_jalr:
            case I_load:
            case I_func:
                return imm_i();
            case S_save:
                return imm_s();
            case B_branch:
                return imm_b();
            default:
                return 0;
        }
    }

    bool ret() { return data == 0x0FF00513; }

    void print() {
        cout << "opcode:" << opcode_name(opcode()) << "\trd:" << dec << rd() << "\trs1:" << rs1() << "\trs2:" << rs2() << "\tfunc:" << func3() << hex << "\timm:0x" << imm() << endl;
    }
};

// typedef bitset<32> Inst;