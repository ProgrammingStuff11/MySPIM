#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    // 1. Implement the operations on input parameters A and B according to
    // ALUControl.
    // 2. Output the result (Z) to ALUresult.
    // 3. Assign Zero to 1 if the result is zero; otherwise, assign 0.
    // 4. The following table shows the operations of the ALU.

    // 000 Z = A + B
    if(ALUControl == 0) {
        *ALUresult = A + B;
    }
    // 001 (1) Z = A – B
    else if(ALUControl == 1) {
        *ALUresult = A - B;
    }
    // 010 (2) if A < B, Z = 1; otherwise, Z = 0
    else if(ALUControl == 2) {
        if(A < B) {
            *ALUresult = 1;
        } else {
            *ALUresult = 0;
        }
    }
    // 011 (3) if A < B, Z = 1; otherwise, Z = 0 (A and B are unsigned integers)
    else if(ALUControl == 3) {
        if(A < B) {
            *ALUresult = 1;
        } else {
            *ALUresult = 0;
        }
    }
    // 100 (4) Z = A AND B
    else if(ALUControl == 4) {
        *ALUresult = A & B;
    }
    // 101 (5) Z = A OR B
    else if(ALUControl == 5) {
        *ALUresult = A | B;
    }
    // 110 (6) Z = Shift B left by 16 bits
    else if(ALUControl == 6) {
        *ALUresult = B << 16;
    }
    // 111 (7) Z = NOT A
    else if(ALUControl == 7) {
        *ALUresult = ~A;
    }

    // 3. Assign Zero to 1 if the result is zero; otherwise, assign 0.
    if(*ALUresult == 0) {
        *Zero = 1;
    } else {
        *Zero = 0;
    }
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{   
    // 2. Return 1 if a halt condition occurs; otherwise, return 0.
    if(PC % 4 != 0) {
        return 1;
    } else {
        // 1. Fetch the instruction addressed by PC from Mem and write it to instruction.
        *instruction = Mem[PC >> 2];
        return 0;
    }
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    // 1. Partition instruction into several parts (op, r1, r2, r3, funct, offset, jsec).
    // instruction [31-26] (0xFC000000 = 1111 1100 0000 0000 0000 0000 0000 0000 in binary)
    *op = (instruction & 0xFC000000) >> 26;
    // instruction [25-21] (0x3E00000 = 0011 1110 0000 0000 0000 0000 0000 in binary)
    *r1 = (instruction & 0x3E00000) >> 21;
    // instruction [20-16] (0x1F0000 = 0001 1111 0000 0000 0000 0000 in binary)
    *r2 = (instruction & 0x1F0000) >> 16;
    // instruction [15-11] (0xF800 = 1111 1000 0000 0000 in binary)
    *r3 = (instruction & 0xF800) >> 11;
    // instruction [5-0] (0x3F = 0011 1111 in binary)
    *funct = instruction & 0x3F;
    // instruction [15-0] (0xFFFF = 1111 1111 1111 1111 in binary)
    *offset = instruction & 0xFFFF;
    // instruction [25-0] (0x3FFFFFF = 0011 1111 1111 1111 1111 1111 1111 in binary)
    *jsec = instruction & 0x3FFFFFF;
    // 2. Read line 41 to 47 of spimcore.c for more information.
}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

    // 1. Decode the instruction using the opcode (op).
    // 2. Assign the values of the control signals to the variables in the structure controls
    // (See spimcore.h file).
    // The meanings of the values of the control signals:
    // For MemRead, MemWrite or RegWrite, the value 1 means that enabled, 0
    // means that disabled, 2 means “don’t care”.
    // For RegDst, Jump, Branch, MemtoReg or ALUSrc, the value 0 or 1 indicates the
    // selected path of the multiplexer; 2 means “don’t care”.
    // The following table shows the meaning of the values of ALUOp.
    // 3. Return 1 if a halt condition occurs; otherwise, return 0.

    // r-type
    if(op == 0x00) {
        controls->ALUOp = 7;
        controls->ALUSrc = 0;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 1;
        controls->RegWrite = 1;
    }
    // addi
    else if(op == 0x08) {
        controls->ALUOp = 0;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
    }
    // sw
    else if(op == 0x2B) {
        controls->ALUOp = 0;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 2;
        controls->MemWrite = 1;
        controls->RegDst = 2;
        controls->RegWrite = 0;
    }
    // lw
    else if(op == 0x23) {
        controls->ALUOp = 0;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 1;
        controls->MemtoReg = 1;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
    }
    // lui
    else if(op == 0x0F) {
        controls->ALUOp = 6;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
    }
    // slti
    else if(op == 0x0A) {
        controls->ALUOp = 2;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
    }
    // sltiu
    else if(op == 0x0B) {
        controls->ALUOp = 3;
        controls->ALUSrc = 1;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
    }
    // beq
    else if(op == 0x04) {
        controls->ALUOp = 1;
        controls->ALUSrc = 0;
        controls->Branch = 1;
        controls->Jump = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 2;
        controls->MemWrite = 0;
        controls->RegDst = 2;
        controls->RegWrite = 0;
    }
    // j
    else if(op == 0x02) {
        controls->ALUOp = 0;
        controls->ALUSrc = 0;
        controls->Branch = 0;
        controls->Jump = 1;
        controls->MemRead = 0;
        controls->MemtoReg = 2;
        controls->MemWrite = 0;
        controls->RegDst = 2;
        controls->RegWrite = 0;
    } else {
        return 1;
    }

    return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    // 1. Read the registers addressed by r1 and r2 from Reg, and write the read values to data1 and data2 respectively.
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    // 1. Assign the sign-extended value of offset to extended_value.
    
    // check if 16th bit (0x8000 = 1000 0000 0000 0000 in binary) is negative
    if(offset & 0x8000) {
        // if negative fill upper 16 bits with 1s (0xFFFF0000 = 1111 1111 1111 1111 0000 0000 0000 0000 in binary)
        *extended_value = offset | 0xFFFF0000;
    } else {
        // if positive fill upper 16 bits with 0s (0x0000FFFF = 0000 0000 0000 0000 1111 1111 1111 1111 in binary)
        *extended_value = offset & 0x0000FFFF;
    }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{  
    // 5. Return 1 if a halt condition occurs; otherwise, return 0.
    if(ALUOp > 7) {
        return 1;
    }

    // 1. Apply ALU operations on data1, and data2 or extended_value (determined by ALUSrc).
    unsigned num;
    // use sign extended value if ALUSrc is 1
    if(ALUSrc == 1) {
        num = extended_value;
    } 
    // else use data2 if ALUSrc is 0
    else {
        num = data2;
    }
    // 2. The operation performed is based on ALUOp and funct.
    if(ALUOp == 7) {
        // add
        if(funct == 0x20) {
            ALUOp = 0;
        } 
        // sub
        else if(funct == 0x22) {
            ALUOp = 1;
        }
        // slt
        else if(funct == 0x2A) {
            ALUOp = 2;
        }
        // sltu
        else if(funct == 0x2B) {
            ALUOp = 3;
        }
        // and
        else if(funct == 0x24) {
            ALUOp = 4;
        }
        // or
        else if(funct == 0x25) {
            ALUOp = 5;
        }
        // sll
        else if(funct == 0x00) {
            ALUOp = 6;
        }
        // nor
        else if(funct == 0x27) {
            ALUOp = 7;
        }
    }

    // 3. Apply the function ALU(...).
    // 4. Output the result to ALUresult.
    ALU(data1, num, ALUOp, ALUresult, Zero);
    
    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned *memdata, unsigned *Mem)
{
    // 4. Return 1 if a halt condition occurs; otherwise, return 0.
    // Have to check halt conditions first to avoid segmentation fault
    /*
        Halt causes: 
        - An illegal instruction is encountered.
        - Jumping to an address that is not word-aligned (being multiple of 4).
        - The address of lw or sw is not word-aligned.
        - Accessing data or jumping to an address that is beyond the memory.
    */
    if (MemRead == 1 || MemWrite == 1) 
    {
        if (ALUresult % 4 != 0 || ALUresult > 0xFFFF) // check to see if out of 64kB memory range
        {
            return 1;
        }
    }
    
    // 1. Use the value of MemWrite or MemRead to determine if a memory write
    // operation or memory read operation or neither is occurring.

    // 2. If reading from memory, read the content of the memory location addressed by ALUresult to memdata.
    if (MemRead == 1) 
    {
        // MEM bit shift to convert byte address to word index
        *memdata = Mem[ALUresult >> 2];
    }

    // 3. If writing to memory, write the value of data2 to the memory location addressed by ALUresult.
    if (MemWrite == 1) 
    {
        Mem[ALUresult >> 2] = data2;
    }

    return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    // 1. Write the data (ALUresult or memdata) to a register (Reg) addressed by r2 or r3.

    // R type rd if 1 and I type rt if 0
    unsigned dest;
    if(RegDst == 1) {
       dest = r3;
    } else {
       dest = r2;
    }

    // memory
    if(RegWrite == 1 && MemtoReg == 1) {
        Reg[dest] = memdata;
    } 
    // ALUresult
    else if(RegWrite == 1 && MemtoReg == 0) {
        Reg[dest] = ALUresult;
    }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC)
{
    // 1. Update the program counter (PC).
    // if you cannot branch or jump, update regularly
    unsigned pc_Counter = *PC + 4;

    // 1 means enabled
    // 0 means disabled
    if (Branch == 1 && Zero == 1) // Is this a branch instruction AND did ALU give us the green light?
    {
        // Shift left by 2, convert word offset to byte offset 
        pc_Counter += (extended_value << 2);
    }
    if (Jump == 1) // Is this a jump instruction 
    {
        // jsec is 26-bit 
        // Shift left by 2, get 28 bits
        pc_Counter = (pc_Counter & 0xF0000000) | (jsec << 2);
    }

    *PC = pc_Counter;
}

