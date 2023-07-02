#include "stages.h"

int main() {
    mem_pool.initialize();
    size_t cycle = 0;
    size_t ret_count = 0;
    inst_addr PC_next = 0;
    bool if_paused = false;
    bool mem_avail = true;
    while (1) {
        if (mem.is_ready()) {
            wb_ = wb.submit();
            mem_wb = mem.submit();
            ex_mem = alu.submit();
            mem_avail = true;
            id_ex = decoder.submit();
            _if = insf.next();
            if_id = insf.submit();
        } else {
            mem_avail = false;
        }
        cycle++;

        if (mem_avail) {
            if (!wb_.bubble) {
                regs.set(wb_.RD, wb_.IA, available);
            }
            if (!ex_mem.bubble) {
                switch (ex_mem.opcode) {
                    case B_branch:
                        if (ex_mem.result == ex_mem.predict) {
                            // predict correct
                        } else {
                            // predict incorrect
                            flush();
                            _if.PC = ex_mem.result ? ex_mem.target_addr : (ex_mem.IA + 4);
                            _if.bubble = false;
                        }
                        break;
                }
            }
            if (!id_ex.bubble) {
                if (!id_ex.r1_ready) {
                    id_ex.r1_ready = regs.query_reg(id_ex.rs1, id_ex.rv1);
                    if (id_ex.r1_ready) {
                        _if = {id_ex.rv1 + id_ex.rv2, false};
                    }
                }
                if (!id_ex.r2_ready) {
                    id_ex.r2_ready = regs.query_reg(id_ex.rs2, id_ex.rv2);
                }
                if (!id_ex.rs_ready) {
                    if (id_ex.opcode == S_save) {
                        id_ex.rs_ready = regs.query_reg(id_ex.RD, id_ex.rvs);
                    }
                }
                if (id_ex.r1_ready && id_ex.r2_ready && id_ex.rs_ready) {
                    if_paused = false;
                } else {
                    if_paused = true;
                    id_ex.bubble = true;
                }
            }
            if (!if_paused) {
                if (mem_pool.get_inst(_if.PC).ret()) {
                    _if = {0, true};
                    ret_count = 1;
                } else if (ret_count) {
                    _if = {0, true};
                    ++ret_count;
                }
                if (ret_count == 5) break;
                insf.input(_if);
                // cout << "[IF]:" << hex << _if.PC << "\t";
                decoder.input(if_id);
                // cout << "[ID]:" << hex << if_id.IA << "\t";
            }
            alu.input(id_ex);
            // cout << "[EX]:" << hex << id_ex.IA << "\t";
            mem.input(ex_mem);
            // cout << "[MEM]:" << hex << ex_mem.IA << "\t";
            wb.input(mem_wb);
            // cout << "[WB]:" << hex << mem_wb.IA << "\t";
            // cout << endl;
            // if (cycle >= 50) {
            //     cycle -= 50;
            //     cout << endl;
            // }
        }
    }
    cout << dec << (regs[10].reg.uword() & 0xFF);
}