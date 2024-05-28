%{
    #include "../src/token_schema.hpp"
    #include "y.tab.h"
    void yyerror(inst_t **, char *);
    
    #include "ctype.h"
    char* to_upper(char* s) {
        for(char *p=s; *p; p++) *p=toupper(*p);
        return s;
    }

    uint32_t line_no = 1;
%}

%%

"#"[0][xX][0-9a-fA-F]+ {
    yylval.t_value.t_type = TOKEN_TYPE_REG;
    yylval.t_value.t_u.t_imm = (token_imm_t *) calloc(1, 
        sizeof(token_imm_t));
    yylval.t_value.t_u.t_imm->imm_value = (int16_t) strtol(yytext + 1, NULL, 0);
    return T_IMM;
}

"#"-?[0-9]+ {
    yylval.t_value.t_type = TOKEN_TYPE_REG;
    yylval.t_value.t_u.t_imm = (token_imm_t *) calloc(1, 
        sizeof(token_imm_t));
    yylval.t_value.t_u.t_imm->imm_value = (int16_t) strtol(yytext + 1, NULL, 0);
    return T_IMM;
}

[,:!\[\]] {
    return *yytext;
}

"\n" {
    line_no++;
}

[a-zA-Z0-9]* {
    yytext = to_upper(yytext);
    if (0 == strcmp(yytext, "B")) {
        return B;
    } else if (0 == strcmp(yytext, "BEQ")) {
        return BEQ;
    } else if (0 == strcmp(yytext, "BCS")) {
        return BCS;
    } else if (0 == strcmp(yytext, "MOV")) {
        return MOV;
    } else if (0 == strcmp(yytext, "ADD")) {
        return ADD;
    } else if (0 == strcmp(yytext, "SUB")) {
        return SUB;
    } else if (0 == strcmp(yytext, "MUL")) {
        return MUL;
    } else if (0 == strcmp(yytext, "MULU")) {
        return MULU;
    } else if (0 == strcmp(yytext, "DIV")) {
        return DIV;
    } else if (0 == strcmp(yytext, "MOD")) {
        return MOD;
    } else if (0 == strcmp(yytext, "AND")) {
        return AND;
    } else if (0 == strcmp(yytext, "OR")) {
        return OR;
    } else if (0 == strcmp(yytext, "EOR")) {
        return EOR;
    } else if (0 == strcmp(yytext, "NOT")) {
        return NOT;
    } else if (0 == strcmp(yytext, "LSL")) {
        return LSL;
    } else if (0 == strcmp(yytext, "LSR")) {
        return LSR;
    } else if (0 == strcmp(yytext, "ASR")) {
        return ASR;
    } else if (0 == strcmp(yytext, "ROL")) {
        return ROL;
    } else if (0 == strcmp(yytext, "CMP")) {
        return CMP;
    } else if (0 == strcmp(yytext, "NOP")) {
        return NOP;
    } else if (0 == strcmp(yytext, "RTI")) {
        return RTI;
    } else if (0 == strcmp(yytext, "HALT")) {
        return HALT;
    } else if (0 == strcmp(yytext, "FLAGS")) {
        return FLAGS;
    } else if (0 == strcmp(yytext, "LDR")) {
        return LDR;
    } else if (0 == strcmp(yytext, "STR")) {
        return STR;
    } else if (0 == strcmp(yytext, "R0")) {
        yylval.t_value.t_type = TOKEN_TYPE_REG;
        yylval.t_value.t_u.t_reg = (token_register_t *) calloc(1, 
            sizeof(token_register_t));
        yylval.t_value.t_u.t_reg->reg_index = 0;
        return T_REG;
    } else if (0 == strcmp(yytext, "R1")) {
        yylval.t_value.t_type = TOKEN_TYPE_REG;
        yylval.t_value.t_u.t_reg = (token_register_t *) calloc(1, 
            sizeof(token_register_t));
        yylval.t_value.t_u.t_reg->reg_index = 1;
        return T_REG;
    } else if (0 == strcmp(yytext, "R2")) {
        yylval.t_value.t_type = TOKEN_TYPE_REG;
        yylval.t_value.t_u.t_reg = (token_register_t *) calloc(1, 
            sizeof(token_register_t));
        yylval.t_value.t_u.t_reg->reg_index = 2; 
        return T_REG;
    } else if (0 == strcmp(yytext, "R3")) {
        yylval.t_value.t_type = TOKEN_TYPE_REG;
        yylval.t_value.t_u.t_reg = (token_register_t *) calloc(1, 
            sizeof(token_register_t));
        yylval.t_value.t_u.t_reg->reg_index = 3; 
        return T_REG;
    } else if (0 == strcmp(yytext, "R4")) {
        yylval.t_value.t_type = TOKEN_TYPE_REG;
        yylval.t_value.t_u.t_reg = (token_register_t *) calloc(1, 
            sizeof(token_register_t));
        yylval.t_value.t_u.t_reg->reg_index = 4; 
        return T_REG;
    } else if (0 == strcmp(yytext, "R5")) {
        yylval.t_value.t_type = TOKEN_TYPE_REG;
        yylval.t_value.t_u.t_reg = (token_register_t *) calloc(1, 
            sizeof(token_register_t));
        yylval.t_value.t_u.t_reg->reg_index = 5; 
        return T_REG;
    } else if (0 == strcmp(yytext, "R6")) {
        yylval.t_value.t_type = TOKEN_TYPE_REG;
        yylval.t_value.t_u.t_reg = (token_register_t *) calloc(1, 
            sizeof(token_register_t));
        yylval.t_value.t_u.t_reg->reg_index = 6;
        return T_REG;
    } else if (0 == strcmp(yytext, "R7")) {
        yylval.t_value.t_type = TOKEN_TYPE_REG;
        yylval.t_value.t_u.t_reg = (token_register_t *) calloc(1, 
            sizeof(token_register_t));
        yylval.t_value.t_u.t_reg->reg_index = 7;
        return T_REG;
    } else {
        yylval.t_value.t_type = TOKEN_TYPE_STR;
        yylval.t_value.t_u.t_str = (uint8_t *) calloc(1, 1 + strlen(yytext));
        strcpy((char *)yylval.t_value.t_u.t_str, yytext);
        return T_STR;
    }

}


%%

void yyerror(inst_t **inst_head, char *s) {
    fprintf(stderr, "Line %d: Syntax Error\n", line_no);
}

int yywrap(void) {
    return 1;
}