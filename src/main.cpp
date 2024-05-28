#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"

#include "../lib/parser/bin/lex.yy.c"

#ifdef __cplusplus
}
#endif

#include <map>
#include <iostream>
#include <vector>

typedef struct cmd_line_s {
    std::string source_file;
    std::string object_file;
    bool is_list_option;
} cmd_line_t;

uint16_t parse_inst(inst_t *inst, uint16_t pc, std::map<std::string, uint16_t> symbol_table) {
    uint16_t assembled_inst;
    if (inst->inst_type == I_TYPE_NO_OPER) {
        if (inst->inst_u.no_oper->inst_type == I_NO_NOP) {
            assembled_inst = 0x0000;
        } else if (inst->inst_u.no_oper->inst_type == I_NO_RTI) {
            assembled_inst = 0x0200;
        } else if (inst->inst_u.no_oper->inst_type == I_NO_HALT) {
            assembled_inst = 0x0600;
        }
    } else if (inst->inst_type == I_TYPE_BRANCH) {
        if (symbol_table.find((char *) inst->inst_u.branch->label) == symbol_table.end()) {
            std::cout << "Label " << inst->inst_u.branch->label << " is not defined." << std::endl;
            exit(0);
        }
        uint16_t abs_address = symbol_table[(char *) inst->inst_u.branch->label];
        if (inst->inst_u.branch->inst_type == I_BR_B) {
            assembled_inst = 0x4000;
        } else if (inst->inst_u.branch->inst_type == I_BR_BEQ) {
            assembled_inst = 0x6000;
        } else if (inst->inst_u.branch->inst_type == I_BR_BCS) {
            assembled_inst = 0x7000;
        }
        assembled_inst += (abs_address - (pc + 1)) & 0x0FFF;
    } else if (inst->inst_type == I_TYPE_ALU) {
        if (inst->inst_u.alu->inst_type == I_ALU_ADD) {
            assembled_inst = 0x0;
        } else if (inst->inst_u.alu->inst_type == I_ALU_SUB) {
            assembled_inst = 0x1;
        } else if (inst->inst_u.alu->inst_type == I_ALU_MUL) {
            assembled_inst = 0x2;
        } else if (inst->inst_u.alu->inst_type == I_ALU_MULU) {
            assembled_inst = 0x3;
        } else if (inst->inst_u.alu->inst_type == I_ALU_DIV) {
            assembled_inst = 0x4;
        } else if (inst->inst_u.alu->inst_type == I_ALU_MOD) {
            assembled_inst = 0x5;
        } else if (inst->inst_u.alu->inst_type == I_ALU_AND) {
            assembled_inst = 0x6;
        } else if (inst->inst_u.alu->inst_type == I_ALU_OR) {
            assembled_inst = 0x7;
        } else if (inst->inst_u.alu->inst_type == I_ALU_EOR) {
            assembled_inst = 0x8;
        } else if (inst->inst_u.alu->inst_type == I_ALU_NOT) {
            assembled_inst = 0x9;
        } else if (inst->inst_u.alu->inst_type == I_ALU_LSL) {
            assembled_inst = 0xA;
        } else if (inst->inst_u.alu->inst_type == I_ALU_LSR) {
            assembled_inst = 0xB;
        } else if (inst->inst_u.alu->inst_type == I_ALU_ASR) {
            assembled_inst = 0xC;
        } else if (inst->inst_u.alu->inst_type == I_ALU_ROL) {
            assembled_inst = 0xD;
        } else if (inst->inst_u.alu->inst_type == I_ALU_CMP) {
            assembled_inst = 0xF;
        } 
        assembled_inst += 0x10;
        assembled_inst <<= 3;
        assembled_inst += inst->inst_u.alu->rn;
        assembled_inst <<= 3;
        assembled_inst += inst->inst_u.alu->rd;
        assembled_inst <<= 3;
        assembled_inst += inst->inst_u.alu->rm;
    } else if (inst->inst_type == I_TYPE_MEM) {
        if (inst->inst_u.mem->inst_type == I_MEM_LDR) {
            assembled_inst = 0x08;
        } else if (inst->inst_u.mem->inst_type == I_MEM_STR) {
            assembled_inst = 0x0C;
        }
        if (inst->inst_u.mem->mem_mode == I_MEM_M_OFFSET) {
            assembled_inst += 0x0;
        } else if (inst->inst_u.mem->mem_mode == I_MEM_M_OFFSET_2) {
            assembled_inst += 0x3;
        } else if (inst->inst_u.mem->mem_mode == I_MEM_M_PRE) {
            assembled_inst += 0x1;
        } else if (inst->inst_u.mem->mem_mode == I_MEM_M_POST) {
            assembled_inst += 0x2;
        }
        assembled_inst <<= 3;
        assembled_inst += inst->inst_u.mem->rn;
        assembled_inst <<= 3;
        assembled_inst += inst->inst_u.mem->rd;
        assembled_inst <<= 3;
        assembled_inst += inst->inst_u.mem->rm;
    } else if (inst->inst_type == I_TYPE_MOV) {
        if (inst->inst_u.mov->inst_type == I_MOV_IMM) {
            assembled_inst = 0x8;
            assembled_inst += inst->inst_u.mov->inst_u.imm->rd;
            assembled_inst <<= 12;
            assembled_inst += inst->inst_u.mov->inst_u.imm->imm & 0x0FFF;
        } else if (inst->inst_u.mov->inst_type == I_MOV_REG) {
            assembled_inst = 0x0;
            assembled_inst += inst->inst_u.mov->inst_u.reg->rn;
            assembled_inst <<= 3;
            assembled_inst += inst->inst_u.mov->inst_u.reg->rd;
            assembled_inst <<= 3;
            assembled_inst += 0x0800;
        } else if (inst->inst_u.mov->inst_type == I_MOV_FLAGS) {
            if (inst->inst_u.mov->inst_u.flags->is_flag_dest) {
                assembled_inst = 0x0;
                assembled_inst += inst->inst_u.mov->inst_u.flags->reg;
                assembled_inst <<= 6;
                assembled_inst += 0x0E00;
            } else {
                assembled_inst = 0x0;
                assembled_inst += inst->inst_u.mov->inst_u.flags->reg;
                assembled_inst <<= 3;
                assembled_inst += 0x0C00;
            }
        }
    }
    return assembled_inst;
}

std::vector<uint16_t> parse_insts(inst_t *inst_head) {
    std::map<std::string, uint16_t> symbol_table;
    std::vector<uint16_t> encoded_inst;
    uint16_t index = 0;

    for (inst_t *curr_inst = inst_head; curr_inst != nullptr; curr_inst = curr_inst->next) {
        if (curr_inst->label != nullptr) {
            if (symbol_table.find((char *) curr_inst->label) != symbol_table.end()) {
                std::cout << "Symbol " << curr_inst->label << " already defined." 
                    << std::endl;
                exit(0);
            }
            symbol_table[(char *) curr_inst->label] = index;
        }
        index++;
    }

    index = 0; 
    for (inst_t *curr_inst = inst_head; curr_inst != nullptr; curr_inst = curr_inst->next) {
        uint16_t parsed_inst = parse_inst(curr_inst, index, symbol_table);
        encoded_inst.push_back(parsed_inst);
        index++;
    }

    return encoded_inst;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Usage: ./disarm_as <input_assembly_file> <output_assembly_file>" << std::endl;
        return -1;
    }
    FILE* fin = fopen(argv[1], "r");
    yyin = fin;
    yyout = NULL;
    yy_flex_debug = 0;

    inst_t *inst_head;
    yyparse(&inst_head);

    std::vector<uint16_t> encoded_insts = parse_insts(inst_head);

    FILE* fout = fopen(argv[2], "w");
    fprintf(fout, "v2.0 raw");
    for (std::vector<uint16_t>::iterator curr_elem = encoded_insts.begin(); curr_elem != encoded_insts.end(); curr_elem++) {
        fprintf(fout, "\n%04jX", (uintmax_t) *curr_elem);
    }

    return 0;
}