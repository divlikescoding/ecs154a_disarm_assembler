#include "token_schema.hpp"

#include <iostream>

inst_t *create_no_oper_inst(enum no_oper_inst_e inst_type) {
    inst_t *inst = new inst_t();
    if (inst == nullptr) {
        std::cout << "Memory Limit Reached" << std::endl;
        exit(0);
    }
    inst->inst_type = I_TYPE_NO_OPER;
    inst->label = nullptr;
    inst->prev = nullptr;
    inst->next = nullptr;
    inst->inst_u.no_oper = new no_oper_inst_t();
    if (inst->inst_u.no_oper == nullptr) {
        exit(0);
        std::cout << "Memory Limit Reached" << std::endl;
    }
    inst->inst_u.no_oper->inst_type = inst_type;
    return inst;
}

inst_t *create_branch_inst(enum branch_inst_e inst_type, token_t *label) {
    inst_t *inst = new inst_t();
    if (inst == nullptr) {
        std::cout << "Memory Limit Reached" << std::endl;
        exit(0);
    }
    inst->inst_type = I_TYPE_BRANCH;
    inst->label = nullptr;
    inst->prev = nullptr;
    inst->next = nullptr;
    inst->inst_u.branch = new branch_inst_t();
    if (inst->inst_u.branch == nullptr) {
        std::cout << "Memory Limit Reached" << std::endl;
        exit(0);
    }
    inst->inst_u.branch->inst_type = inst_type;
    inst->inst_u.branch->label = label->t_u.t_str;
    return inst;
}

inst_t *create_alu_inst(enum alu_inst_e inst_type, token_t *rd, token_t *rn, token_t *rm) {
    inst_t *inst = new inst_t();
    if (inst == nullptr) {
        std::cout << "Memory Limit Reached" << std::endl;
    }
    inst->inst_type = I_TYPE_ALU;
    inst->label = nullptr;
    inst->prev = nullptr;
    inst->next = nullptr;
    inst->inst_u.alu = new alu_inst_t();
    if (inst->inst_u.alu == nullptr) {
        std::cout << "Memory Limit Reached" << std::endl;
        exit(0);
    }
    inst->inst_u.alu->inst_type = inst_type;
    if (rd != nullptr) {
        inst->inst_u.alu->rd = rd->t_u.t_reg->reg_index;
    } else {
        inst->inst_u.alu->rd = 0;
    }
    inst->inst_u.alu->rn = rn->t_u.t_reg->reg_index;
    if (rm != nullptr) {
        inst->inst_u.alu->rm = rm->t_u.t_reg->reg_index;
    } else {
        inst->inst_u.alu->rm = 0;
    }
    return inst;
}

inst_t *create_mem_inst(enum mem_inst_e inst_type, enum mem_inst_mode_e mem_mode, token_t *rd, token_t *rn, token_t *rm) {
    inst_t *inst = new inst_t();
    if (inst == nullptr) {
        std::cout << "Memory Limit Reached" << std::endl;
        exit(0);
    }
    inst->inst_type = I_TYPE_MEM;
    inst->label = nullptr;
    inst->prev = nullptr;
    inst->next = nullptr;
    inst->inst_u.mem = new mem_inst_t();
    if (inst->inst_u.mem == nullptr) {
        std::cout << "Memory Limit Reached" << std::endl;
        exit(0);
    }
    inst->inst_u.mem->inst_type = inst_type;
    inst->inst_u.mem->mem_mode = mem_mode;
    inst->inst_u.mem->rd = rd->t_u.t_reg->reg_index;
    inst->inst_u.mem->rn = rn->t_u.t_reg->reg_index;
    if (rm != nullptr) {
        inst->inst_u.mem->rm = rm->t_u.t_reg->reg_index;
    } else {
        inst->inst_u.mem->rm = 0;
    }
    return inst;
}

inst_t *create_mov_inst(enum mov_inst_e inst_type, bool is_flag_dest, token_t *rd, token_t *rn_or_imm) {
    inst_t *inst = new inst_t();
    if (inst == nullptr) {
        std::cout << "Memory Limit Reached" << std::endl;
        exit(0);
    }
    inst->inst_type = I_TYPE_MOV;
    inst->label = nullptr;
    inst->prev = nullptr;
    inst->next = nullptr;
    inst->inst_u.mov = new mov_inst_t();
    if (inst->inst_u.mov == nullptr) {
        std::cout << "Memory Limit Reached" << std::endl;
        exit(0);
    }
    inst->inst_u.mov->inst_type = inst_type;
    if (inst_type == I_MOV_IMM) {
        inst->inst_u.mov->inst_u.imm = new mov_inst_imm_t();
        if (inst->inst_u.mov->inst_u.imm == nullptr) {
            std::cout << "Memory Limit Reached" << std::endl;
            exit(0);
        }
        inst->inst_u.mov->inst_u.imm->rd = rd->t_u.t_reg->reg_index;
        inst->inst_u.mov->inst_u.imm->imm = rn_or_imm->t_u.t_imm->imm_value;
    } else if (inst_type == I_MOV_REG) {
        inst->inst_u.mov->inst_u.reg = new mov_inst_reg_t();
        if (inst->inst_u.mov->inst_u.reg == nullptr) {
            std::cout << "Memory Limit Reached" << std::endl;
            exit(0);
        }
        inst->inst_u.mov->inst_u.reg->rd = rd->t_u.t_reg->reg_index;
        inst->inst_u.mov->inst_u.reg->rn = rn_or_imm->t_u.t_reg->reg_index;
    } else if (inst_type == I_MOV_FLAGS) {
        inst->inst_u.mov->inst_u.flags = new mov_inst_flags_t();
        if (inst->inst_u.mov->inst_u.flags == nullptr) {
            std::cout << "Memory Limit Reached" << std::endl;
            exit(0);
        }
        inst->inst_u.mov->inst_u.flags->is_flag_dest = is_flag_dest;
        inst->inst_u.mov->inst_u.flags->reg = rd->t_u.t_reg->reg_index;
    }
    return inst;
}

inst_t *attach_label(token_t *label, inst_t *inst) {
    inst->label = label->t_u.t_str;
    return inst;
}

inst_t *add_inst(inst_t *inst_tail, inst_t *new_inst) {
    inst_tail->next = new_inst;
    new_inst->prev = inst_tail;
    return new_inst;
}

inst_t *reverse_inst(inst_t *inst_tail) {
    inst_t *temp;
    for (temp = inst_tail; temp->prev != nullptr; temp = temp->prev);
    return temp;
}