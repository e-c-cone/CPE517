#include <stdio.h>
#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

uint8_t opcode; //the opcode isolated from the instruction
uint32_t instruct; // instruction to be fetched and executed
uint32_t rs, rt, rd;      //register values
uint8_t funcCode;
uint16_t i;         //immediate values
uint32_t addrRS, addrRT, addrRD, addrJ;


void fetch()
{
    instruct = mem_read_32(CURRENT_STATE.PC); //the instruction
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void decode()
{
    opcode = instruct >> 26; //find the 31-26 bits, which contain the opcode which tells what instruction to execute

    if (opcode == 0) { //if opcode == 0, it will be an r type execution
        addrRS = (instruct >> 21) & 0x1f;
        addrRT = (instruct >> 16) & 0x1f;
        addrRD = (instruct >> 11) & 0x1f;
        rs = CURRENT_STATE.REGS[addrRS];   //2^5=32
        rt = CURRENT_STATE.REGS[addrRT];
        funcCode = instruct & 0x3f; //extracts function code
    }

    if (opcode == 2) { //opcode == 2 means jump instruction
        printf("jump\n");
	        addrJ = instruct & 0x3ffffff;
	        NEXT_STATE.PC = ((CURRENT_STATE.PC) + 4 & 0xf) | addrJ << 2; //jump address = most significant 4 bits of current PC address + 26 bits of given address
    }

    if (opcode == 7) { //opcode == 7 means bgtz
        printf("bgtz\n");
        i = instruct & 0x0000ffff;
        addrRS = (instruct >> 21) & 0x1f;
        rs = CURRENT_STATE.REGS[addrRS];
        if (rs > 0 && rs < 2147483647) { //accounts for unsigned values?
            printf("bgtz TRUE\n");         
            NEXT_STATE.PC = CURRENT_STATE.PC + 4 + (i << 2);
             }//if no branch, go to next instruction (PC+4)
    }
    

    if (opcode == 4) { //opcode == 4 means beq
        printf("beq\n");
		i = instruct & 0x0000ffff; //extracts immediate
		addrRS = (instruct >> 21) & 0x1f;
		addrRT = (instruct >> 16) & 0x1f;
		rs = CURRENT_STATE.REGS[addrRS];
		rt = CURRENT_STATE.REGS[addrRT];
		if (rs == rt)
			NEXT_STATE.PC = CURRENT_STATE.PC + 4 + (i << 2); // jump to new address
    }

    if (opcode == 5) { //opcode == 5 means bne
        printf("bne\n");
		i = instruct & 0x0000ffff; //extracts immediate
		addrRS = (instruct >> 21) & 0x1f;
		addrRT = (instruct >> 16) & 0x1f;
		rs = CURRENT_STATE.REGS[addrRS];
		rt = CURRENT_STATE.REGS[addrRT];
		if (rs != rt)
			NEXT_STATE.PC = CURRENT_STATE.PC + 4 + (i << 2); // jump to new address
    }

    if (opcode == 8) {   //if opcode == 8, it will be an addi execution -- WORKS
        printf("addi\n");
        addrRS = (instruct >> 21) & 0x1f;
        addrRT = (instruct >> 16) & 0x1f;
        addrRD = (instruct >> 11) & 0x1f;
        rs = CURRENT_STATE.REGS[addrRS];   //2^5=32
        rt = CURRENT_STATE.REGS[addrRT];
        funcCode = instruct & 0x3f; //extracts function code
        i = instruct & 0x0000ffff; //extracts immediate
        // account for signed addition
        if (i > 32767) {    //2^15-1=32767 is the largest number
            i = ~i + 1; //NOT i and add one to account for signed
            rt = rs - i;
        }
        else {
            rt = rs + i;
        }
        NEXT_STATE.REGS[addrRT] = rt;
    }

    if (opcode == 9) {        //if opcode == 9, it will be an addiu execution -- WORKS
        printf("addiu\n");
        addrRS = (instruct >> 21) & 0x1f;
        addrRT = (instruct >> 16) & 0x1f;
        rs = CURRENT_STATE.REGS[addrRS];   //2^5=32
        i = instruct & 0x0000ffff; //extracts immediate
        rt = rs + i; //add instruction
        NEXT_STATE.REGS[addrRT] = rt; // It is fine if you just execute here instead of executing in execute() function.
    }

    if (opcode == 10) {// opcode == 13 means slti instruction i type -- WORKS
        printf("slti\n");
		addrRS = (instruct >> 21) & 0x1f;
		addrRT = (instruct >> 16) & 0x1f;
		rs = CURRENT_STATE.REGS[addrRS];
		i = instruct & 0x0000ffff; //extracts immediate
		if (rs<i)
			rt = 1; //compares immediate to rt
		else
			rt = 0;
		NEXT_STATE.REGS[addrRT] = rt;
    }

    if (opcode == 13) {// opcode == 13 means ori instruction i ttype
        printf("ori\n");
	        addrRS = (instruct >> 21) & 0x1f;
	        addrRT = (instruct >> 16) & 0x1f;
	        rs = CURRENT_STATE.REGS[addrRS];
	        i = instruct & 0x0000ffff; //extracts immediate
	        rt = rs | i; //or the target register and the immediate
	        NEXT_STATE.REGS[addrRT] = rt;
    }

    if (opcode == 15) {// opcode == 15 means lui instruction i type
        printf("lui\n");
		addrRS = (instruct >> 21) & 0x1f;
		addrRT = (instruct >> 16) & 0x1f;
		rs = CURRENT_STATE.REGS[addrRS];
		i = instruct & 0x0000ffff; //extracts immediate
		rt = i << 16; //load rt with immediate shifted left 16 bits
		NEXT_STATE.REGS[addrRT] = rt;
    }

    if (opcode == 35) { // opcode == 23 means it is a lw instruction
        printf("lw\n");
        addrRS = (instruct >> 21) & 0x1f;
        addrRT = (instruct >> 16) & 0x1f;
        i = instruct & 0x0000ffff; //extracts immediate
        rs = CURRENT_STATE.REGS[addrRS];
        //signed addition again
        if (i > 32767) {
            i = ~i + 1; //NOT i and add one to account for signed
            rt = mem_read_32(rs - i);
        }
        else
            rt = mem_read_32(rs + i);
        NEXT_STATE.REGS[addrRT] = rt;
    }

    if (opcode == 43) { //opcode == 23 means sw
        printf("sw\n");
        addrRS = (instruct >> 21) & 0x1f;
        addrRT = (instruct >> 16) & 0x1f;
        i = instruct & 0x0000ffff; //extracts immediate
        rs = CURRENT_STATE.REGS[addrRS];
        rt = CURRENT_STATE.REGS[addrRT];
        //signed
        if (i > 32767) {
            i = ~i + 1;
            mem_write_32(rs - i, rt);
        }
        else
            mem_write_32(rs + i, rt);
    }

}

void execute()
{
    //printf("execute \n");
    if (opcode == 0) {
        switch (funcCode) {

		case 12:   //system call:001100 - WORKS
			if (CURRENT_STATE.REGS[2] == 10) {  //v0==10 then exit
				printf("systemcall: exit\n");
				RUN_BIT = FALSE;
			}
			if (CURRENT_STATE.REGS[2] == 1) {   //v0==1: print int
				rt = CURRENT_STATE.REGS[4];   //a0 is 4th register
				printf("print the number in a0:   %d \n", rt);

			}
            break;

        case 34:      //sub -- WORKS
            printf("sub\n");
            rd = 0;
            if (rt > 2147483647)
            {
                rt = ~rt + 1;
                rd = rs + rt;
            }
            else
                rd = rt - rs;
            NEXT_STATE.REGS[addrRD] = rd;
            break;

        case 32:      //add:100000 -- WORKS
            printf("add\n");
            if (rt > 2147483647)
            {
                rt = ~rt + 1;
                rd = rs + rt;
            }
            else
                rd = rt + rs;
            NEXT_STATE.REGS[addrRD] = rd;
            break;

        case 33: //add unsigned for func == 100001 -- WORKS
            printf("addu\n");
            rd = rt + rs;
            NEXT_STATE.REGS[addrRD] = rd;
            break;

        case 35: //sub unsigned for func == 100101
            printf("subu\n");
            rd = rs - rt;
            NEXT_STATE.REGS[addrRD] = rd;
            break;

        case 42:
        	printf("set less than instruction \n");
        	rd = 0;
        	if (rt > 2147483647)
			{
				rt = ~rt + 1;
			}
			if (rt>rs)
				rd = 1;
			NEXT_STATE.REGS[addrRD] = rd;
            break;

        case 43:
		    //sltu
		    printf("set less than unsigned instruction \n");
			rd = 0;
			if (rs<rt)
        		rd = 1;
		    NEXT_STATE.REGS[addrRD] = rd;
        	break;

        default:
            printf("error \n");
            break;
        }
    }
}
void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */
    fetch();
    decode();
    execute();

}
