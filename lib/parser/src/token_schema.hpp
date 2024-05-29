#ifndef __TOKEN_SCHEMA_HH__
#define __TOKEN_SCHEMA_HH__


#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


enum token_type_e {
    TOKEN_TYPE_INVALID = 0,
    TOKEN_TYPE_STR,
    TOKEN_TYPE_REG,
    TOKEN_TYPE_IMM,
    TOKEN_TYPE_MAX
};

enum no_oper_inst_e {
    I_NO_INVALID = 0,
    I_NO_NOP,
    I_NO_RTI,
    I_NO_HALT,
    I_NO_MAX
};

enum branch_inst_e {
    I_BR_INVALID = 0,
    I_BR_B,
    I_BR_BEQ,
    I_BR_BCS,
    I_BR_MAX
};

enum alu_inst_e {
    I_ALU_INVALID = 0,
    I_ALU_NOT,
    I_ALU_CMP,
    I_ALU_ADD,
    I_ALU_SUB,
    I_ALU_MUL,
    I_ALU_MULU,
    I_ALU_DIV,
    I_ALU_MOD,
    I_ALU_AND,
    I_ALU_OR,
    I_ALU_EOR,
    I_ALU_LSL,
    I_ALU_LSR,
    I_ALU_ASR,
    I_ALU_ROL,
    I_ALU_ROR,
    I_ALU_MAX
};

enum mem_inst_e {
    I_MEM_INVALID = 0,
    I_MEM_LDR,
    I_MEM_STR,
    I_MEM_MAX
};

enum mem_inst_mode_e {
    I_MEM_M_INVALID = 0,
    I_MEM_M_OFFSET,
    I_MEM_M_OFFSET_2,
    I_MEM_M_PRE,
    I_MEM_M_POST,
    I_MEM_M_MAX
};

enum mov_inst_e {
    I_MOV_INVALID = 0,
    I_MOV_IMM,
    I_MOV_REG,
    I_MOV_FLAGS,
    I_MOV_MAX
};

enum inst_e {
    I_TYPE_INVALID = 0,
    I_TYPE_NO_OPER,
    I_TYPE_BRANCH,
    I_TYPE_ALU,
    I_TYPE_MEM,
    I_TYPE_MOV,
    I_TYPE_MAX
};

/* Token Structs */
typedef struct _token_register_s {
    uint8_t reg_index;
} token_register_t;

typedef struct _token_imm_s {
    int16_t imm_value;
} token_imm_t;

typedef struct _token_s {
    enum token_type_e t_type;
    union {
        uint8_t *t_str;
        token_register_t *t_reg;
        token_imm_t *t_imm;
    } t_u;
} token_t;

/* Parsing Structs */
typedef struct _no_oper_inst_s {
    enum no_oper_inst_e inst_type;
} no_oper_inst_t;

typedef struct _branch_inst_s {
    enum branch_inst_e inst_type;
    uint8_t *label;
} branch_inst_t;

typedef struct _alu_inst_s {
    enum alu_inst_e inst_type;
    uint8_t rn;
    uint8_t rm;
    uint8_t rd;
} alu_inst_t;

typedef struct _mem_inst_s {
    enum mem_inst_e inst_type;
    enum mem_inst_mode_e mem_mode;
    uint8_t rn;
    uint8_t rm;
    uint8_t rd;
} mem_inst_t;

typedef struct _mov_inst_imm_s {
    uint8_t rd;
    int16_t imm;
} mov_inst_imm_t;

typedef struct _mov_inst_reg_s {
    uint8_t rd;
    uint8_t rn;
} mov_inst_reg_t;

typedef struct _mov_inst_flags_s {
    bool is_flag_dest;
    uint8_t reg;
} mov_inst_flags_t;

typedef struct _mov_inst_s {
    enum mov_inst_e inst_type;
    union {
        mov_inst_imm_t *imm;
        mov_inst_reg_t *reg;
        mov_inst_flags_t *flags;
    } inst_u;
} mov_inst_t;

typedef struct _inst_s {
    enum inst_e inst_type;
    uint8_t *label;
    union {
        no_oper_inst_t *no_oper;
        branch_inst_t *branch;
        alu_inst_t *alu;
        mem_inst_t *mem;
        mov_inst_t *mov;
    } inst_u;
    struct _inst_s *prev;
    struct _inst_s *next;
} inst_t;

inst_t *create_no_oper_inst(enum no_oper_inst_e inst_type);
inst_t *create_branch_inst(enum branch_inst_e inst_type, token_t *label);
inst_t *create_alu_inst(enum alu_inst_e inst_type, token_t *rd, token_t *rn, token_t *rm);
inst_t *create_mem_inst(enum mem_inst_e inst_type, enum mem_inst_mode_e mem_mode, token_t *rd, token_t *rn, token_t *rm);
inst_t *create_mov_inst(enum mov_inst_e inst_type, bool is_flag_dest, token_t *rd, token_t *rn_or_imm);
inst_t *attach_label(token_t *label, inst_t *inst);
inst_t *add_inst(inst_t *inst_tail, inst_t *new_inst);
inst_t *reverse_inst(inst_t *inst_tail);

#ifdef __cplusplus
}
#endif

#endif /* __TOKEN_SCHEMA_HH__ */