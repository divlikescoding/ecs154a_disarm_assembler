%code requires {
    #include "../src/token_schema.hpp"
    int yylex(void);
    void yyerror(inst_t **, char *);
}

%union {
    token_t t_value;
    inst_t *inst_head;
}

%token B BEQ BCS MOV ADD SUB MUL MULU DIV MOD AND OR EOR NOT LSL LSR ASR ROL CMP NOP RTI HALT FLAGS LDR STR

%token <t_value> T_STR T_REG T_IMM

%type <inst_head> instr instr_advance instr_advances

%parse-param { inst_t **inst_head }

%%

program:
        instr_advances {*inst_head = reverse_inst($1); }
        |
        ;

instr_advances:
        instr_advances instr_advance { $$ = add_inst($1, $2); }
        | instr_advance { $$ = $1; }

instr_advance:
        T_STR ':' instr { $$ = attach_label(&$1, $3); }
        | instr { $$ = $1; }

instr:
        NOP { $$ = create_no_oper_inst(I_NO_NOP); }
        | RTI { $$ = create_no_oper_inst(I_NO_RTI); }
        | HALT { $$ = create_no_oper_inst(I_NO_HALT); }

        | B T_STR { $$ = create_branch_inst(I_BR_B, &$2); }
        | BEQ T_STR { $$ = create_branch_inst(I_BR_BEQ, &$2); }
        | BCS T_STR { $$ = create_branch_inst(I_BR_BCS, &$2); }

        | MOV T_REG ',' T_IMM { $$ = create_mov_inst(I_MOV_IMM, false, &$2, &$4); }
        | MOV T_REG ',' T_REG { $$ = create_mov_inst(I_MOV_REG, false, &$2, &$4); }
        | MOV T_REG ',' FLAGS { $$ = create_mov_inst(I_MOV_FLAGS, false, &$2, NULL); }
        | MOV FLAGS ',' T_REG { $$ = create_mov_inst(I_MOV_FLAGS, true, &$4, NULL); }

        | NOT T_REG ',' T_REG { $$ = create_alu_inst(I_ALU_NOT, &$2, &$4, NULL); }
        | CMP T_REG ',' T_REG { $$ = create_alu_inst(I_ALU_CMP, NULL, &$2, &$4); }

        | ADD T_REG ',' T_REG ',' T_REG { $$ = create_alu_inst(I_ALU_ADD, &$2, &$4, &$6); }
        | SUB T_REG ',' T_REG ',' T_REG { $$ = create_alu_inst(I_ALU_SUB, &$2, &$4, &$6); }
        | MUL T_REG ',' T_REG ',' T_REG { $$ = create_alu_inst(I_ALU_MUL, &$2, &$4, &$6); }
        | MULU T_REG ',' T_REG ',' T_REG { $$ = create_alu_inst(I_ALU_MULU, &$2, &$4, &$6); }
        | DIV T_REG ',' T_REG ',' T_REG { $$ = create_alu_inst(I_ALU_DIV, &$2, &$4, &$6); }
        | MOD T_REG ',' T_REG ',' T_REG { $$ = create_alu_inst(I_ALU_MOD, &$2, &$4, &$6); }
        | AND T_REG ',' T_REG ',' T_REG { $$ = create_alu_inst(I_ALU_AND, &$2, &$4, &$6); }
        | OR T_REG ',' T_REG ',' T_REG { $$ = create_alu_inst(I_ALU_OR, &$2, &$4, &$6); }
        | EOR T_REG ',' T_REG ',' T_REG { $$ = create_alu_inst(I_ALU_EOR, &$2, &$4, &$6); }
        | LSL T_REG ',' T_REG ',' T_REG { $$ = create_alu_inst(I_ALU_LSL, &$2, &$4, &$6); }
        | LSR T_REG ',' T_REG ',' T_REG { $$ = create_alu_inst(I_ALU_LSR, &$2, &$4, &$6); }
        | ASR T_REG ',' T_REG ',' T_REG { $$ = create_alu_inst(I_ALU_ASR, &$2, &$4, &$6); }
        | ROL T_REG ',' T_REG ',' T_REG { $$ = create_alu_inst(I_ALU_ROL, &$2, &$4, &$6); }

        | LDR T_REG ',' '[' T_REG ']' { $$ = create_mem_inst(I_MEM_LDR, I_MEM_M_OFFSET_2, &$2, &$5, NULL); }
        | LDR T_REG ',' '[' T_REG ',' T_REG ']' { $$ = create_mem_inst(I_MEM_LDR, I_MEM_M_OFFSET, &$2, &$5, &$7); }
        | LDR T_REG ',' '[' T_REG ',' T_REG ']' '!' { $$ = create_mem_inst(I_MEM_LDR, I_MEM_M_PRE, &$2, &$5, &$7); }
        | LDR T_REG ',' '[' T_REG ']' ',' T_REG { $$ = create_mem_inst(I_MEM_LDR, I_MEM_M_POST, &$2, &$5, &$8); }
        
        | STR T_REG ',' '[' T_REG ']' { $$ = create_mem_inst(I_MEM_STR, I_MEM_M_OFFSET_2, &$2, &$5, NULL); }
        | STR T_REG ',' '[' T_REG ',' T_REG ']' { $$ = create_mem_inst(I_MEM_STR, I_MEM_M_OFFSET, &$2, &$5, &$7); }
        | STR T_REG ',' '[' T_REG ',' T_REG ']' '!' { $$ = create_mem_inst(I_MEM_STR, I_MEM_M_PRE, &$2, &$5, &$7); }
        | STR T_REG ',' '[' T_REG ']' ',' T_REG { $$ = create_mem_inst(I_MEM_STR, I_MEM_M_POST, &$2, &$5, &$8); }

%%

void yyerror(inst_t **inst_head, char *s) {
    fprintf(stderr, "Syntax Error\n");
}