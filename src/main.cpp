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
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>

std::pair<uint16_t, std::string> parse_inst(inst_t *inst, uint16_t pc, std::map<std::string, uint16_t> symbol_table) {
    uint16_t assembled_inst;
    std::string instruction;
    if (inst->inst_type == I_TYPE_NO_OPER) {
        if (inst->inst_u.no_oper->inst_type == I_NO_NOP) {
            assembled_inst = 0x0000;
            instruction = "NOP";
        } else if (inst->inst_u.no_oper->inst_type == I_NO_RTI) {
            assembled_inst = 0x0200;
            instruction = "RTI";
        } else if (inst->inst_u.no_oper->inst_type == I_NO_HALT) {
            assembled_inst = 0x0600;
            instruction = "HALT";
        }
    } else if (inst->inst_type == I_TYPE_BRANCH) {
        instruction = "B";
        if (symbol_table.find((char *) inst->inst_u.branch->label) == symbol_table.end()) {
            std::cerr << "Label " << inst->inst_u.branch->label << " is not defined." << std::endl;
            exit(0);
        }
        uint16_t abs_address = symbol_table[(char *) inst->inst_u.branch->label];
        if (inst->inst_u.branch->inst_type == I_BR_B) {
            assembled_inst = 0x4000;
        } else if (inst->inst_u.branch->inst_type == I_BR_BEQ) {
            assembled_inst = 0x6000;
            instruction += "EQ";
        } else if (inst->inst_u.branch->inst_type == I_BR_BCS) {
            assembled_inst = 0x7000;
            instruction += "CS";
        }
        std::string branch_label((char *) inst->inst_u.branch->label);
        assembled_inst += (abs_address - (pc + 1)) & 0x0FFF;
        std::stringstream sstream;
        sstream << std::setfill('0') << std::setw(3) << std::uppercase << std::hex << ((abs_address - (pc + 1)) & 0x0FFF);
        instruction += " " + sstream.str();
    } else if (inst->inst_type == I_TYPE_ALU) {
        if (inst->inst_u.alu->inst_type == I_ALU_ADD) {
            assembled_inst = 0x0;
            instruction = "ADD";
        } else if (inst->inst_u.alu->inst_type == I_ALU_SUB) {
            assembled_inst = 0x1;
            instruction = "SUB";
        } else if (inst->inst_u.alu->inst_type == I_ALU_MUL) {
            assembled_inst = 0x2;
            instruction = "MUL";
        } else if (inst->inst_u.alu->inst_type == I_ALU_MULU) {
            assembled_inst = 0x3;
            instruction = "MULU";
        } else if (inst->inst_u.alu->inst_type == I_ALU_DIV) {
            assembled_inst = 0x4;
            instruction = "DIV";
        } else if (inst->inst_u.alu->inst_type == I_ALU_MOD) {
            assembled_inst = 0x5;
            instruction = "MOD";
        } else if (inst->inst_u.alu->inst_type == I_ALU_AND) {
            assembled_inst = 0x6;
            instruction = "AND";
        } else if (inst->inst_u.alu->inst_type == I_ALU_OR) {
            assembled_inst = 0x7;
            instruction = "OR";
        } else if (inst->inst_u.alu->inst_type == I_ALU_EOR) {
            assembled_inst = 0x8;
            instruction = "EOR";
        } else if (inst->inst_u.alu->inst_type == I_ALU_NOT) {
            assembled_inst = 0x9;
            instruction = "NOT";
        } else if (inst->inst_u.alu->inst_type == I_ALU_LSL) {
            assembled_inst = 0xA;
            instruction = "LSL";
        } else if (inst->inst_u.alu->inst_type == I_ALU_LSR) {
            assembled_inst = 0xB;
            instruction = "LSR";
        } else if (inst->inst_u.alu->inst_type == I_ALU_ASR) {
            assembled_inst = 0xC;
            instruction = "ASR";
        } else if (inst->inst_u.alu->inst_type == I_ALU_ROL) {
            assembled_inst = 0xD;
            instruction = "ROL";
        } else if (inst->inst_u.alu->inst_type == I_ALU_ROR) {
            assembled_inst = 0xE;
            instruction = "ROR";
        } else if (inst->inst_u.alu->inst_type == I_ALU_CMP) {
            assembled_inst = 0xF;
            instruction = "CMP";
        } 
        assembled_inst += 0x10;
        assembled_inst <<= 3;
        assembled_inst += inst->inst_u.alu->rn;
        assembled_inst <<= 3;
        assembled_inst += inst->inst_u.alu->rd;
        assembled_inst <<= 3;
        assembled_inst += inst->inst_u.alu->rm;
        if (inst->inst_u.alu->inst_type == I_ALU_NOT) {
            instruction += " R" + std::to_string(unsigned(inst->inst_u.alu->rd)) + ", R" + 
                std::to_string(unsigned(inst->inst_u.alu->rn));
        } else if (inst->inst_u.alu->inst_type == I_ALU_CMP) {
            instruction += " R" + std::to_string(unsigned(inst->inst_u.alu->rn)) + ", R" + 
                std::to_string(unsigned(inst->inst_u.alu->rm));
        } else {
            instruction += " R" + std::to_string(unsigned(inst->inst_u.alu->rd)) + ", R" + 
                std::to_string(unsigned(inst->inst_u.alu->rn)) + ", R" + 
                std::to_string(unsigned(inst->inst_u.alu->rm));
        }
    } else if (inst->inst_type == I_TYPE_MEM) {
        if (inst->inst_u.mem->inst_type == I_MEM_LDR) {
            assembled_inst = 0x08;
            instruction = "LDR";
        } else if (inst->inst_u.mem->inst_type == I_MEM_STR) {
            assembled_inst = 0x0C;
            instruction = "STR";
        }
        if (inst->inst_u.mem->mem_mode == I_MEM_M_OFFSET) {
            assembled_inst += 0x0;
            instruction += " R" + std::to_string(unsigned(inst->inst_u.mem->rd)) + ", [R" +
                std::to_string(unsigned(inst->inst_u.mem->rn)) + ", R" +
                std::to_string(unsigned(inst->inst_u.mem->rm)) + "]";
        } else if (inst->inst_u.mem->mem_mode == I_MEM_M_OFFSET_2) {
            assembled_inst += 0x3;
            instruction += " R" + std::to_string(unsigned(inst->inst_u.mem->rd)) + ", [R" +
                std::to_string(unsigned(inst->inst_u.mem->rn)) + "]";
        } else if (inst->inst_u.mem->mem_mode == I_MEM_M_PRE) {
            assembled_inst += 0x1;
            instruction += " R" + std::to_string(unsigned(inst->inst_u.mem->rd)) + ", [R" +
                std::to_string(unsigned(inst->inst_u.mem->rn)) + ", R" +
                std::to_string(unsigned(inst->inst_u.mem->rm)) + "]!";
        } else if (inst->inst_u.mem->mem_mode == I_MEM_M_POST) {
            assembled_inst += 0x2;
            instruction += " R" + std::to_string(unsigned(inst->inst_u.mem->rd)) + ", [R" +
                std::to_string(unsigned(inst->inst_u.mem->rn)) + "], R" +
                std::to_string(unsigned(inst->inst_u.mem->rm));
        }
        assembled_inst <<= 3;
        assembled_inst += inst->inst_u.mem->rn;
        assembled_inst <<= 3;
        assembled_inst += inst->inst_u.mem->rd;
        assembled_inst <<= 3;
        assembled_inst += inst->inst_u.mem->rm;
    } else if (inst->inst_type == I_TYPE_MOV) {
        instruction = "MOV";
        if (inst->inst_u.mov->inst_type == I_MOV_IMM) {
            assembled_inst = 0x8;
            assembled_inst += inst->inst_u.mov->inst_u.imm->rd;
            assembled_inst <<= 12;
            assembled_inst += inst->inst_u.mov->inst_u.imm->imm & 0x0FFF;
            std::stringstream sstream;
            sstream << std::setfill('0') << std::setw(3) << std::uppercase << std::hex << unsigned(inst->inst_u.mov->inst_u.imm->imm & 0x0FFF);
            instruction += " R" + std::to_string(unsigned(inst->inst_u.mov->inst_u.imm->rd)) + ", #0x"
                + sstream.str();
        } else if (inst->inst_u.mov->inst_type == I_MOV_REG) {
            assembled_inst = 0x0;
            assembled_inst += inst->inst_u.mov->inst_u.reg->rn;
            assembled_inst <<= 3;
            assembled_inst += inst->inst_u.mov->inst_u.reg->rd;
            assembled_inst <<= 3;
            assembled_inst += 0x0800;
            instruction += " R" + std::to_string(unsigned(inst->inst_u.mov->inst_u.imm->rd)) + ", R"
                + std::to_string(unsigned(inst->inst_u.mov->inst_u.reg->rn));
        } else if (inst->inst_u.mov->inst_type == I_MOV_FLAGS) {
            if (inst->inst_u.mov->inst_u.flags->is_flag_dest) {
                assembled_inst = 0x0;
                assembled_inst += inst->inst_u.mov->inst_u.flags->reg;
                assembled_inst <<= 6;
                assembled_inst += 0x0E00;
                instruction += " Flags, R" + std::to_string(unsigned(inst->inst_u.mov->inst_u.flags->reg));
            } else {
                assembled_inst = 0x0;
                assembled_inst += inst->inst_u.mov->inst_u.flags->reg;
                assembled_inst <<= 3;
                assembled_inst += 0x0C00;
                instruction += " R" + std::to_string(unsigned(inst->inst_u.mov->inst_u.flags->reg)) + ", Flags";
            }
        }
    }
    return std::make_pair(assembled_inst, instruction);
}

std::vector<std::pair<uint16_t, std::string> > parse_insts(inst_t *inst_head) {
    std::map<std::string, uint16_t> symbol_table;
    std::vector<std::pair<uint16_t, std::string> > encoded_inst;
    uint16_t index = 0;

    for (inst_t *curr_inst = inst_head; curr_inst != nullptr; curr_inst = curr_inst->next) {
        if (curr_inst->label != nullptr) {
            if (symbol_table.find((char *) curr_inst->label) != symbol_table.end()) {
                std::cerr << "Symbol " << curr_inst->label << " already defined." 
                    << std::endl;
                exit(0);
            }
            symbol_table[(char *) curr_inst->label] = index;
        }
        index++;
        if (index == 0) {
            std::cerr << "Instruction Memory Exceeded Limit" << std::endl;
            exit(0);
        }
    }

    index = 0; 
    for (inst_t *curr_inst = inst_head; curr_inst != nullptr; curr_inst = curr_inst->next) {
        std::pair<uint16_t, std::string> parsed_inst = parse_inst(curr_inst, index, symbol_table);
        encoded_inst.push_back(parsed_inst);
        index++;
    }

    return encoded_inst;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./disarm_as <input_assembly_file> <output_assembly_file>" << std::endl;
        return 0;
    }
    FILE* fin = fopen(argv[1], "r");
    yyin = fin;
    yyout = NULL;
    yy_flex_debug = 0;

    inst_t *inst_head;
    int status = yyparse(&inst_head);
    if (status > 0) {
        return 0;
    }

    std::vector<std::pair<uint16_t, std::string> > encoded_insts = parse_insts(inst_head);

    FILE* fout = fopen(argv[2], "w");
    fprintf(fout, "v2.0 raw");
    for (std::vector<std::pair<uint16_t, std::string> >::iterator curr_elem = encoded_insts.begin(); curr_elem != encoded_insts.end(); curr_elem++) {
        fprintf(fout, "\n%04jX", (uintmax_t) curr_elem->first);
        printf("\n%04jX: %s", (uintmax_t) curr_elem->first, curr_elem->second.c_str());
    }
    printf("\n");

    return 0;
}