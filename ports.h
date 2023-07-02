#pragma once
#include "definitions.h"

struct _IF {
    inst_addr PC;
    bool bubble;
} _if;

struct IF_ID {
    // 指令相关
    inst_addr IA;
    Inst ins;
    Opcode opcode;
    bool bubble;
    // 分支相关
    inst_addr target_addr;
    bool predict;
} if_id;

struct ID_EX {
    // 指令相关
    inst_addr IA;
    Inst ins;
    Opcode opcode;
    reg_name RD;
    bool bubble;
    // ALU相关
    uint32_t rv1, rv2;
    reg_name rs1, rs2;
    bool r1_ready, r2_ready;
    // MEM相关
    uint32_t rvs;
    bool rs_ready;
    // 分支相关
    inst_addr target_addr;
    bool predict;
} id_ex;

struct EX_MEM {
    // 指令相关
    inst_addr IA;
    Inst ins;
    Opcode opcode;
    reg_name RD;
    bool bubble;
    // ALU相关
    uint32_t result;
    // MEM相关
    uint32_t rs;
    // 分支相关
    inst_addr target_addr;
    bool predict;
} ex_mem;

struct MEM_WB {
    inst_addr IA;
    Opcode opcode;
    uint32_t result;
    reg_name RD;
    bool bubble;
} mem_wb;

struct WB_ {
    inst_addr IA;
    uint32_t result;
    reg_name RD;
    bool bubble;
} wb_;