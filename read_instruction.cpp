#include "definitions.h"
#include <vector>

vector<Inst> address(2000);

int main() {
    char ch = getchar();
    inst_addr addr;
    while (ch != EOF) {
        if (ch == '@') {
            addr = 0;
            for (int i = 0; i < 8; i++) {
                addr <<= 4;
                addr += to_dec(getchar());
            }
            cout << "addr: 0x" << hex << addr << endl;
            addr >>= 2;
            ch = getchar();
        } else if (ch == '\n' || ch == ' ') {
            ch = getchar();
        } else {
            unsigned long inst = 0;
            for (int i = 0; i < 4; i++) {
                unsigned long num = (to_dec(ch) << 4) + to_dec(getchar());
                inst += num << (i << 3);
                getchar();
                ch = getchar();
            }
            cout << "inst: 0x" << hex << inst << "\taddr:0x" << (addr << 2) << endl;
            address[addr++] = inst;
            address[addr - 1].print();
        }
    }
}