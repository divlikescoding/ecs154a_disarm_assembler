start:
    not r3, r7
    ADD r0, r1, r2
    CMP r0, r1
    B start
    BEQ start
    BCS label2
    MOV R0, #10
    MOV R1, #-10
    MOV R2, #11
    MOV R3, #-10
    MOV R4, #0xFF
    MOV R5, #100
    MOV R6, #101
    MOV R7, #102
    MOV R2, R1
    MOV R2, R7
label2: 
    MOV R2, Flags
    MOV Flags, R2
    STR R7, [R1]
    STR R0, [R1, R2]
    STR R0, [R1, R2]!
    STR R0, [R1], R2
    LDR R0, [R1], R2
    NOP
    RTI
    HALT