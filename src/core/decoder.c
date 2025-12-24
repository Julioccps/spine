#include <stdio.h>
#include <stdint.h>

#define GET_OPCODE(inst) ((inst >> 26) &  0x3F)
#define GET_RS(inst)     ((inst >> 21) & 0x1F)
#define GET_RT(inst)     ((inst >> 16) & 0x1F)
#define GET_RD(inst)     ((inst >> 11) & 0x1F)
#define GET_FUNCT(inst)  (inst & 0x3F)
#define GET_IMM(inst)    (inst & 0xFFFF)
#define GET_TARGET(inst) (inst & 0x3FFFFFF)
#define GET_SHAMT(inst)  ((inst >> 6) & 0x1F)
#define GET_OFFSET(inst) (int16_t)(inst & 0xFFFF)

const char* reg_names[32] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
	"$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
	"$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
	"$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};

void disassemble(uint32_t inst, uint32_t pc, FILE *stream){
	uint8_t opcode = GET_OPCODE(inst);

	switch (opcode){
	case 0x00:{
		uint8_t funct = GET_FUNCT(inst);
		switch (funct){
		case 0x00:
			fprintf(stream, "sll %s, %s, 0x%X\n", 
				reg_names[GET_RD(inst)], reg_names[GET_RT(inst)], GET_SHAMT(inst));
			break;
		case 0x02:
			fprintf(stream, "srl %s, %s, 0x%X\n",
				reg_names[GET_RD(inst)], reg_names[GET_RT(inst)], GET_SHAMT(inst));
			break;
		case 0x03:
			fprintf(stream, "sra %s, %s, 0x%X\n", 
				reg_names[GET_RD(inst)], reg_names[GET_RT(inst)], GET_SHAMT(inst));
			break;
		case 0x08:
			fprintf(stream, "jr %s\n", reg_names[GET_RS(inst)]);
			break;
		case 0x09:
			fprintf(stream, "jalr %s, %s\n",
				reg_names[GET_RD(inst)], reg_names[GET_RS(inst)]);
			break;
		case 0x0C: 
			fprintf(stream, "syscall\n"); 
			break;
		case 0x0D: 
			fprintf(stream, "break\n"); 
			break;
		case 0x20:
			fprintf(stream, "add %s, %s, %s\n", 
				reg_names[GET_RD(inst)], reg_names[GET_RS(inst)], reg_names[GET_RT(inst)]);
			break;
		case 0x21:
			fprintf(stream, "addu %s, %s, %s\n", 
				reg_names[GET_RD(inst)], reg_names[GET_RS(inst)], reg_names[GET_RT(inst)]);
			break;
		case 0x22:
			fprintf(stream, "sub %s, %s, %s\n",
				reg_names[GET_RD(inst)], reg_names[GET_RS(inst)], reg_names[GET_RT(inst)]);
			break;
		case 0x23:
			fprintf(stream, "subu %s, %s, %s\n",
				reg_names[GET_RD(inst)], reg_names[GET_RS(inst)], reg_names[GET_RT(inst)]);
			break;
		case 0x24:
			fprintf(stream, "and %s, %s, %s\n",
				reg_names[GET_RD(inst)], reg_names[GET_RS(inst)], reg_names[GET_RT(inst)]);
			break;
		case 0x25:
			fprintf(stream, "or %s, %s, %s\n",
				reg_names[GET_RD(inst)], reg_names[GET_RS(inst)], reg_names[GET_RT(inst)]);
			break;
		case 0x26:
			fprintf(stream, "xor %s, %s, %s\n",
				reg_names[GET_RD(inst)], reg_names[GET_RS(inst)], reg_names[GET_RT(inst)]);
			break;
		case 0x27:
			fprintf(stream, "nor %s, %s, %s\n",
				reg_names[GET_RD(inst)], reg_names[GET_RS(inst)], reg_names[GET_RT(inst)]);
			break;
		case 0x2A:
			fprintf(stream, "slt %s, %s, %s\n", 
				reg_names[GET_RD(inst)], reg_names[GET_RS(inst)], reg_names[GET_RT(inst)]);
			break;
		case 0x2B:
			fprintf(stream, "sltu %s, %s, %s\n", 
				reg_names[GET_RD(inst)], reg_names[GET_RS(inst)], reg_names[GET_RT(inst)]);
			break;
		default:
			fprintf(stream, "UNKNOWN SPECIAL FUNCT: [0x%02X]", funct);
			break;
		}
	}
		break;
	case 0x01:{
		uint8_t rt = GET_RT(inst);
		switch (rt){
		case 0x00:
			fprintf(stream, "bltz %s, %d\n", reg_names[GET_RS(inst)], GET_OFFSET(inst));
			break;
		case 0x01:
			fprintf(stream, "bgez %s, %d\n", reg_names[GET_RS(inst)], GET_OFFSET(inst));
			break;
		case 0x10:
			fprintf(stream, "bltzal %s, %d\n", reg_names[GET_RS(inst)], GET_OFFSET(inst));
			break;
		case 0x11:
			fprintf(stream, "bgezal %s, %d\n", reg_names[GET_RS(inst)], GET_OFFSET(inst));
			break;
		default:
			fprintf(stream, "UNKNOWN REGIMM [rt: 0x%02X]\n", rt);
		}
	}
		break;
	case 0x02:
		fprintf(stream, "j 0x%08X\n", (pc & 0xF0000000) | GET_TARGET(inst) << 2);
		break;
	case 0x03:
		fprintf(stream, "jal 0x%08X\n", (pc & 0xF0000000) | (GET_TARGET(inst) << 2)); 
		break;
	case 0x04:
		fprintf(stream, "beq %s, %s, %d\n", 
			reg_names[GET_RS(inst)], reg_names[GET_RT(inst)], GET_OFFSET(inst));
		break;
	case 0x05:
		fprintf(stream, "bne %s, %s, %d\n",
			reg_names[GET_RS(inst)], reg_names[GET_RT(inst)], GET_OFFSET(inst));
		break;
	case 0x06:
		fprintf(stream, "blez %s, %d\n", reg_names[GET_RS(inst)], GET_OFFSET(inst));
		break;
	case 0x07:
		fprintf(stream, "bgtz %s, %d\n", reg_names[GET_RS(inst)], GET_OFFSET(inst));
		break;
	case 0x08:
		fprintf(stream, "addi %s, %s, %d\n", reg_names[GET_RT(inst)], reg_names[GET_RS(inst)], GET_OFFSET(inst));
		break;
	case 0x09:
		fprintf(stream, "addiu %s, %s, %d\n", reg_names[GET_RT(inst)], reg_names[GET_RS(inst)], GET_OFFSET(inst));
		break;
	case 0x0A:
		fprintf(stream, "slti %s, %s, %d\n", reg_names[GET_RT(inst)], reg_names[GET_RS(inst)], GET_IMM(inst));
		break;
	case 0x0B:
		fprintf(stream, "sltiu %s, %s, %d\n", reg_names[GET_RT(inst)], reg_names[GET_RS(inst)], GET_IMM(inst));
		break;
	case 0x0C:
		fprintf(stream, "andi %s, %s, 0x%X\n", reg_names[GET_RT(inst)], reg_names[GET_RS(inst)], GET_IMM(inst));
		break;
	case 0x0D:
		fprintf(stream, "ori %s, %s, 0x%X\n", reg_names[GET_RT(inst)], reg_names[GET_RS(inst)], GET_IMM(inst));
		break;
	case 0x0E:
		fprintf(stream, "xori %s, %s, 0x%X\n", reg_names[GET_RT(inst)], reg_names[GET_RS(inst)], GET_IMM(inst));
		break;
	case 0x0F:
		fprintf(stream, "lui %s, 0x%X\n", reg_names[GET_RT(inst)], GET_IMM(inst));
		break;
	case 0X10:{
		uint8_t rs = GET_RS(inst);
		switch (rs){
		case 0x00:
			fprintf(stream, "mfc0 %s, %d\n", reg_names[GET_RT(inst)], GET_RD(inst));
			break;
		case 0x04:
			fprintf(stream, "mtc0 %s, %d\n", reg_names[GET_RT(inst)], GET_RD(inst));
			break;
		default:
			fprintf(stream, "UNKNOWN COP0 (rs: %d)\n", rs);
			break;
		}
	}
		break;

	case 0x20:
		fprintf(stream, "lb %s, %d(%s)\n", reg_names[GET_RT(inst)], GET_OFFSET(inst), reg_names[GET_RS(inst)]);
		break;
	case 0x21:
		fprintf(stream, "lh %s, %d(%s)\n", reg_names[GET_RT(inst)], GET_OFFSET(inst), reg_names[GET_RS(inst)]);
		break;
	case 0x23:
		fprintf(stream, "lw %s, %d(%s)\n", reg_names[GET_RT(inst)], GET_OFFSET(inst), reg_names[GET_RS(inst)]);
		break;
	case 0x24:
		fprintf(stream, "lbu %s, %d(%s)\n", reg_names[GET_RT(inst)], GET_OFFSET(inst), reg_names[GET_RS(inst)]);
		break;
	case 0x25:
		fprintf(stream, "lhu %s, %d(%s)\n", reg_names[GET_RT(inst)], GET_OFFSET(inst), reg_names[GET_RS(inst)]);
		break;
	case 0x28:
		fprintf(stream, "sb %s, %d(%s)\n", reg_names[GET_RT(inst)], GET_OFFSET(inst), reg_names[GET_RS(inst)]);
		break;
	case 0x29:
		fprintf(stream, "sh %s, %d(%s)\n", reg_names[GET_RT(inst)], GET_OFFSET(inst), reg_names[GET_RS(inst)]);
		break;
	case 0x2B:
		fprintf(stream, "sw %s, %d(%s)\n", reg_names[GET_RT(inst)], GET_OFFSET(inst), reg_names[GET_RS(inst)]);
		break;
	default:
		fprintf(stream, "UNKNOWN OPCODE: [%08X]\n", opcode);
		break;

	}
}
