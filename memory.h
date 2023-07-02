#pragma once
#include "definitions.h"
#include <map>

class Mem_pool {
    map<size_t, uint8_t> memory;

   public:
    uint8_t& operator[](size_t pos) { return memory[pos]; }

    void initialize() {
        inst_addr addr = 0;
        char ch = getchar();
        while (ch != EOF) {
            if (ch == '@') {
                addr = 0;
                for (int i = 0; i < 8; i++) {
                    addr <<= 4;
                    addr += to_dec(getchar());
                }
                // cout << "addr: 0x" << hex << addr << endl;
            } else if (ch != '\n' && ch != ' ') {
                unsigned long num = (to_dec(ch) << 4) + to_dec(getchar());
                getchar();
                memory[addr++] = num;
                // cout << "inst: 0x" << hex << num << "\taddr:0x" << (addr - 1) << endl;
            }
            ch = getchar();
        }
    }

    Inst get_inst(inst_addr IA) {
        return (memory[IA + 3] << 24) + (memory[IA + 2] << 16) + (memory[IA + 1] << 8) + memory[IA];
    }

    uint32_t read(size_t pos, Num_type num_t = word, Num_sign num_s = Signed) {
        uint32_t ret = 0;
        for (size_t i = 0, p = pos; i < size_t(num_t); i += 8, ++p) {
            ret |= (memory[p] << i);
        }
        if (num_s == Signed) {
            extend_sign_bit(ret, size_t(num_t));
        }
        return ret;
    }
    void write(size_t pos, uint32_t reg, Num_type num_t = word) {
        for (size_t i = 0, p = pos, r = reg; i < size_t(num_t); i += 8, ++p, r >>= 8) {
            memory[p] = r;
        }
    }

    void print() {
        for (auto it : memory) {
            cout << hex << "[0x" << it.first << "]: 0x" << it.second << "\t";
        }
        cout << endl;
    }
} mem_pool;