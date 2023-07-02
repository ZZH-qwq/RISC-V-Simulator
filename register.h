#pragma once
#include "definitions.h"
#include "ports.h"
#include <vector>

class Registers {
   public:
    struct Reg_st {
        Reg reg;
        reg_state state = available;
        inst_addr provider;
    };
    vector<Reg_st> reg_st;

    Registers() : reg_st(33) {}

    void set(reg_name pos, inst_addr prov, reg_state st) {
        reg_st[pos].state = st;
        reg_st[pos].provider = prov;
    }

    Reg_st& operator[](reg_name pos) { return reg_st[pos]; }

    /**
     * 检查并从可用位置(Reg/ALU/MEM)获取寄存器值
     */
    bool query_reg(reg_name pos, uint32_t& val) {
        switch (reg_st[pos].state) {
            case available:
                val = reg_st[pos].reg.uword();
                return true;
            case blocked:
                return false;
            case ALU_ready:
                val = ex_mem.result;
                return true;
            case MEM_ready:
                val = mem_wb.result;
                return true;
            case RWB_ready:
                val = wb_.result;
                return true;
        }
        return false;
    }

    void print() {
        for (int i = 0; i < 33; i++) {
            cout << "x" << dec << i << ":0x" << hex << reg_st[i].reg.uword() << "\t";
        }
        cout << endl;
    }
} regs;