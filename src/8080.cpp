#include "8080.h"

int main() {
	Emu8080 emu = Emu8080();

	while (1) {
		emu.emulateCycle();
		printf("%u, %u \n", emu.pc, emu.sp);
	}
	return 0;
}

int f() {
	return 3;
}

Emu8080::Emu8080() : pc(0), sp(0), intEnable(0), registers(Registers()), flags(Flags()), opcode(0) {
	loadROM();
	table[0x00] = &Emu8080::NOP;
	table[0x01] = &Emu8080::LXI_B_d16;
	table[0x02] = &Emu8080::STAX_B;
	table[0x03] = &Emu8080::INX_B;
	table[0x04] = &Emu8080::INR_B;
	table[0x05] = &Emu8080::DCR_B;
	table[0x06] = &Emu8080::MVI_B_d8;
	table[0x07] = &Emu8080::RLC;
	table[0x08] = &Emu8080::NOP;
	table[0x09] = &Emu8080::DAD_B;
	table[0x0A] = &Emu8080::LDAX_B;
	table[0x0B] = &Emu8080::DCX_B;
	table[0x0C] = &Emu8080::INR_C;
	table[0x0D] = &Emu8080::DCR_C;
	table[0x0E] = &Emu8080::MVI_C_d8;
	table[0x0F] = &Emu8080::RRC;
	table[0x10] = &Emu8080::NOP;
	table[0x11] = &Emu8080::LXI_D_d16;
	table[0x12] = &Emu8080::STAX_D;
	table[0x13] = &Emu8080::INX_D;
	table[0x14] = &Emu8080::INR_D;
	table[0x15] = &Emu8080::DCR_D;
	table[0x16] = &Emu8080::MVI_D_d8;
	table[0x17] = &Emu8080::RAL;
	table[0x18] = &Emu8080::NOP;
	table[0x19] = &Emu8080::DAD_D;
	table[0x1A] = &Emu8080::LDAX_D;
	table[0x1B] = &Emu8080::DCX_D;
	table[0x1C] = &Emu8080::INR_E;
	table[0x1D] = &Emu8080::DCR_E;
	table[0x1E] = &Emu8080::MVI_E_d8;
	table[0x1F] = &Emu8080::RAR;
	table[0x20] = &Emu8080::NOP;
	table[0x21] = &Emu8080::LXI_H_d16;
	table[0x22] = &Emu8080::SHLD_a16;
	table[0x23] = &Emu8080::INX_H;
	table[0x24] = &Emu8080::INR_H;
	table[0x25] = &Emu8080::DCR_H;
	table[0x26] = &Emu8080::MVI_H_d8;
	table[0x27] = &Emu8080::DAA;
	table[0x28] = &Emu8080::NOP;
	table[0x29] = &Emu8080::DAD_H;
	table[0x2A] = &Emu8080::LHLD_a16;
	table[0x2B] = &Emu8080::DCX_H;
	table[0x2C] = &Emu8080::INR_L;
	table[0x2D] = &Emu8080::DCR_L;
	table[0x2E] = &Emu8080::MVI_L_d8;
	table[0x2F] = &Emu8080::CMA;
	table[0x30] = &Emu8080::NOP;
	table[0x31] = &Emu8080::LXI_SP_d16;
	table[0x32] = &Emu8080::STA_a16;
	table[0x33] = &Emu8080::INX_SP;
	table[0x34] = &Emu8080::INR_M;
	table[0x35] = &Emu8080::DCR_M;
	table[0x36] = &Emu8080::MVI_M_d8;
	table[0x37] = &Emu8080::STC;
	table[0x38] = &Emu8080::NOP;
	table[0x39] = &Emu8080::DAD_SP;
	table[0x3A] = &Emu8080::LDA_a16;
	table[0x3B] = &Emu8080::DCX_SP;
	table[0x3C] = &Emu8080::INR_A;
	table[0x3D] = &Emu8080::DCR_A;
	table[0x3E] = &Emu8080::MVI_A_d8;
	table[0x3F] = &Emu8080::CMC;
	table[0x40] = &Emu8080::MOV_B_B;
	table[0x41] = &Emu8080::MOV_B_C;
	table[0x42] = &Emu8080::MOV_B_D;
	table[0x43] = &Emu8080::MOV_B_E;
	table[0x44] = &Emu8080::MOV_B_H;
	table[0x45] = &Emu8080::MOV_B_L;
	table[0x46] = &Emu8080::MOV_B_M;
	table[0x47] = &Emu8080::MOV_B_A;
	table[0x48] = &Emu8080::MOV_C_B;
	table[0x49] = &Emu8080::MOV_C_C;
	table[0x4A] = &Emu8080::MOV_C_D;
	table[0x4B] = &Emu8080::MOV_C_E;
	table[0x4C] = &Emu8080::MOV_C_H;
	table[0x4D] = &Emu8080::MOV_C_L;
	table[0x4E] = &Emu8080::MOV_C_M;
	table[0x4F] = &Emu8080::MOV_C_A;
	table[0x50] = &Emu8080::MOV_D_B;
	table[0x51] = &Emu8080::MOV_D_C;
	table[0x52] = &Emu8080::MOV_D_D;
	table[0x53] = &Emu8080::MOV_D_E;
	table[0x54] = &Emu8080::MOV_D_H;
	table[0x55] = &Emu8080::MOV_D_L;
	table[0x56] = &Emu8080::MOV_D_M;
	table[0x57] = &Emu8080::MOV_D_A;
	table[0x58] = &Emu8080::MOV_E_B;
	table[0x59] = &Emu8080::MOV_E_C;
	table[0x5A] = &Emu8080::MOV_E_D;
	table[0x5B] = &Emu8080::MOV_E_E;
	table[0x5C] = &Emu8080::MOV_E_H;
	table[0x5D] = &Emu8080::MOV_E_L;
	table[0x5E] = &Emu8080::MOV_E_M;
	table[0x5F] = &Emu8080::MOV_E_A;
	table[0x60] = &Emu8080::MOV_H_B;
	table[0x61] = &Emu8080::MOV_H_C;
	table[0x62] = &Emu8080::MOV_H_D;
	table[0x63] = &Emu8080::MOV_H_E;
	table[0x64] = &Emu8080::MOV_H_H;
	table[0x65] = &Emu8080::MOV_H_L;
	table[0x66] = &Emu8080::MOV_H_M;
	table[0x67] = &Emu8080::MOV_H_A;
	table[0x68] = &Emu8080::MOV_L_B;
	table[0x69] = &Emu8080::MOV_L_C;
	table[0x6A] = &Emu8080::MOV_L_D;
	table[0x6B] = &Emu8080::MOV_L_E;
	table[0x6C] = &Emu8080::MOV_L_H;
	table[0x6D] = &Emu8080::MOV_L_L;
	table[0x6E] = &Emu8080::MOV_L_M;
	table[0x6F] = &Emu8080::MOV_L_A;
	table[0x70] = &Emu8080::MOV_M_B;
	table[0x71] = &Emu8080::MOV_M_C;
	table[0x72] = &Emu8080::MOV_M_D;
	table[0x73] = &Emu8080::MOV_M_E;
	table[0x74] = &Emu8080::MOV_M_H;
	table[0x75] = &Emu8080::MOV_M_L;
	table[0x76] = &Emu8080::HLT;
	table[0x77] = &Emu8080::MOV_M_A;
	table[0x78] = &Emu8080::MOV_A_B;
	table[0x79] = &Emu8080::MOV_A_C;
	table[0x7A] = &Emu8080::MOV_A_D;
	table[0x7B] = &Emu8080::MOV_A_E;
	table[0x7C] = &Emu8080::MOV_A_H;
	table[0x7D] = &Emu8080::MOV_A_L;
	table[0x7E] = &Emu8080::MOV_A_M;
	table[0x7F] = &Emu8080::MOV_A_A;
	table[0x80] = &Emu8080::ADD_B;
	table[0x81] = &Emu8080::ADD_C;
	table[0x82] = &Emu8080::ADD_D;
	table[0x83] = &Emu8080::ADD_E;
	table[0x84] = &Emu8080::ADD_H;
	table[0x85] = &Emu8080::ADD_L;
	table[0x86] = &Emu8080::ADD_M;
	table[0x87] = &Emu8080::ADD_A;
	table[0x88] = &Emu8080::ADC_B;
	table[0x89] = &Emu8080::ADC_C;
	table[0x8A] = &Emu8080::ADC_D;
	table[0x8B] = &Emu8080::ADC_E;
	table[0x8C] = &Emu8080::ADC_H;
	table[0x8D] = &Emu8080::ADC_L;
	table[0x8E] = &Emu8080::ADC_M;
	table[0x8F] = &Emu8080::ADC_A;
	table[0x90] = &Emu8080::SUB_B;
	table[0x91] = &Emu8080::SUB_C;
	table[0x92] = &Emu8080::SUB_D;
	table[0x93] = &Emu8080::SUB_E;
	table[0x94] = &Emu8080::SUB_H;
	table[0x95] = &Emu8080::SUB_L;
	table[0x96] = &Emu8080::SUB_M;
	table[0x97] = &Emu8080::SUB_A;
	table[0x98] = &Emu8080::SBB_B;
	table[0x99] = &Emu8080::SBB_C;
	table[0x9A] = &Emu8080::SBB_D;
	table[0x9B] = &Emu8080::SBB_E;
	table[0x9C] = &Emu8080::SBB_H;
	table[0x9D] = &Emu8080::SBB_L;
	table[0x9E] = &Emu8080::SBB_M;
	table[0x9F] = &Emu8080::SBB_A;
	table[0xA0] = &Emu8080::ANA_B;
	table[0xA1] = &Emu8080::ANA_C;
	table[0xA2] = &Emu8080::ANA_D;
	table[0xA3] = &Emu8080::ANA_E;
	table[0xA4] = &Emu8080::ANA_H;
	table[0xA5] = &Emu8080::ANA_L;
	table[0xA6] = &Emu8080::ANA_M;
	table[0xA7] = &Emu8080::ANA_A;
	table[0xA8] = &Emu8080::XRA_B;
	table[0xA9] = &Emu8080::XRA_C;
	table[0xAA] = &Emu8080::XRA_D;
	table[0xAB] = &Emu8080::XRA_E;
	table[0xAC] = &Emu8080::XRA_H;
	table[0xAD] = &Emu8080::XRA_L;
	table[0xAE] = &Emu8080::XRA_M;
	table[0xAF] = &Emu8080::XRA_A;
	table[0xB0] = &Emu8080::ORA_B;
	table[0xB1] = &Emu8080::ORA_C;
	table[0xB2] = &Emu8080::ORA_D;
	table[0xB3] = &Emu8080::ORA_E;
	table[0xB4] = &Emu8080::ORA_H;
	table[0xB5] = &Emu8080::ORA_L;
	table[0xB6] = &Emu8080::ORA_M;
	table[0xB7] = &Emu8080::ORA_A;
	table[0xB8] = &Emu8080::CMP_B;
	table[0xB9] = &Emu8080::CMP_C;
	table[0xBA] = &Emu8080::CMP_D;
	table[0xBB] = &Emu8080::CMP_E;
	table[0xBC] = &Emu8080::CMP_H;
	table[0xBD] = &Emu8080::CMP_L;
	table[0xBE] = &Emu8080::CMP_M;
	table[0xBF] = &Emu8080::CMP_A;
	table[0xC0] = &Emu8080::RNZ;
	table[0xC1] = &Emu8080::POP_B;
	table[0xC2] = &Emu8080::JNZ_a16;
	table[0xC3] = &Emu8080::JMP_a16;
	table[0xC4] = &Emu8080::CNZ_a16;
	table[0xC5] = &Emu8080::PUSH_B;
	table[0xC6] = &Emu8080::ADI_d8;
	table[0xC7] = &Emu8080::RST_0;
	table[0xC8] = &Emu8080::RZ;
	table[0xC9] = &Emu8080::RET;
	table[0xCA] = &Emu8080::JZ_a16;
	table[0xCB] = &Emu8080::NOP;
	table[0xCC] = &Emu8080::CZ_a16;
	table[0xCD] = &Emu8080::CALL_a16;
	table[0xCE] = &Emu8080::ACI_d8;
	table[0xCF] = &Emu8080::RST_1;
	table[0xD0] = &Emu8080::RNC;
	table[0xD1] = &Emu8080::POP_D;
	table[0xD2] = &Emu8080::JNC_a16;
	table[0xD3] = &Emu8080::OUT_d8;
	table[0xD4] = &Emu8080::CNC_a16;
	table[0xD5] = &Emu8080::PUSH_D;
	table[0xD6] = &Emu8080::SUI_d8;
	table[0xD7] = &Emu8080::RST_2;
	table[0xD8] = &Emu8080::RC;
	table[0xD9] = &Emu8080::NOP;
	table[0xDA] = &Emu8080::JC_a16;
	table[0xDB] = &Emu8080::IN_d8;
	table[0xDC] = &Emu8080::CC_a16;
	table[0xDD] = &Emu8080::NOP;
	table[0xDE] = &Emu8080::SBI_d8;
	table[0xDF] = &Emu8080::RST_3;
	table[0xE0] = &Emu8080::RPO;
	table[0xE1] = &Emu8080::POP_H;
	table[0xE2] = &Emu8080::JPO_a16;
	table[0xE3] = &Emu8080::XTHL;
	table[0xE4] = &Emu8080::CPO_a16;
	table[0xE5] = &Emu8080::PUSH_H;
	table[0xE6] = &Emu8080::ANI_d8;
	table[0xE7] = &Emu8080::RST_4;
	table[0xE8] = &Emu8080::RPE;
	table[0xE9] = &Emu8080::PCHL;
	table[0xEA] = &Emu8080::JPE_a16;
	table[0xEB] = &Emu8080::XCHG;
	table[0xEC] = &Emu8080::CPE_a16;
	table[0xED] = &Emu8080::NOP;
	table[0xEE] = &Emu8080::XRI_d8;
	table[0xEF] = &Emu8080::RST_5;
	table[0xF0] = &Emu8080::RP;
	table[0xF1] = &Emu8080::POP_PSW;
	table[0xF2] = &Emu8080::JP_a16;
	table[0xF3] = &Emu8080::DI;
	table[0xF4] = &Emu8080::CP_a16;
	table[0xF5] = &Emu8080::PUSH_PSW;
	table[0xF6] = &Emu8080::ORI_a8;
	table[0xF7] = &Emu8080::RST_6;
	table[0xF8] = &Emu8080::RM;
	table[0xF9] = &Emu8080::SPHL;
	table[0xFA] = &Emu8080::JM_a16;
	table[0xFB] = &Emu8080::EI;
	table[0xFC] = &Emu8080::CM_a16;
	table[0xFD] = &Emu8080::NOP;
	table[0xFE] = &Emu8080::CPI_d8;
	table[0xFF] = &Emu8080::RST_7;

}

Emu8080::~Emu8080() {
	return;
}

void Emu8080::emulateCycle() {
	opcode = &memory[pc];
	if (pc == 6714) {
		pc = 6714;
	}
	pc += (this->*table[*opcode])();
}

uint16_t Emu8080::getBC() {
	return (registers.b << 8) | registers.c;
}

uint16_t Emu8080::getDE() {
	return (registers.d << 8) | registers.e;
}

uint16_t Emu8080::getHL() {
	return (registers.h << 8) | registers.l;
}

void Emu8080::setBC(uint16_t num) {
	registers.b = num >> 8;
	registers.c = num & 0xFF;
}

void Emu8080::setDE(uint16_t num) {
	registers.d = num >> 8;
	registers.e = num & 0xFF;
}

void Emu8080::setHL(uint16_t num) {
	registers.h = num >> 8;
	registers.l = num & 0xFF;
}

void Emu8080::unimplemented() {
	pc -= 1;
	printf("Error: Unimplemented instruction \n");
	exit(1);
}

void Emu8080::loadROM() {
	const char* filePath = "D:/Personal Projects/Projects/8080Emulator/8080Emulator/roms/invaders";
	FILE* f = fopen(filePath, "rb");
	if (f == NULL) {
		printf("Could not open \n");
		exit(1);
	}
	fseek(f, 0L, SEEK_END);
	fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);
	memory = (unsigned char*)malloc(fsize);
	fread(memory, fsize, 1, f);
	fclose(f);
}

void Emu8080::disassemble() {
	int temppc = pc;
	pc = 0;
	unsigned char* code;
	int opbytes;
	while (pc < fsize) {
		code = &memory[pc];
		opbytes = 1;
		printf("%04X %02X ", pc, *code);
		switch (*code)
		{
		case 0x00: printf("NOP"); break;
		case 0x01: printf("LXI    B,#$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0x02: printf("STAX   B"); break;
		case 0x03: printf("INX    B"); break;
		case 0x04: printf("INR    B"); break;
		case 0x05: printf("DCR    B"); break;
		case 0x06: printf("MVI    B,#$%02X", code[1]); opbytes = 2; break;
		case 0x07: printf("RLC"); break;
		case 0x08: printf("NOP"); break;
		case 0x09: printf("DAD    B"); break;
		case 0x0A: printf("LDAX   B"); break;
		case 0x0B: printf("DCX    B"); break;
		case 0x0C: printf("INR    C"); break;
		case 0x0D: printf("DCR    C"); break;
		case 0x0E: printf("MVI    C,#$%02X", code[1]); opbytes = 2; break;
		case 0x0F: printf("RRC"); break;

		case 0x10: printf("NOP"); break;
		case 0x11: printf("LXI    D,#$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0x12: printf("STAX   D"); break;
		case 0x13: printf("INX    D"); break;
		case 0x14: printf("INR    D"); break;
		case 0x15: printf("DCR    D"); break;
		case 0x16: printf("MVI    D,#$%02X", code[1]); opbytes = 2; break;
		case 0x17: printf("RAL"); break;
		case 0x18: printf("NOP"); break;
		case 0x19: printf("DAD    D"); break;
		case 0x1A: printf("LDAX   D"); break;
		case 0x1B: printf("DCX    D"); break;
		case 0x1C: printf("INR    E"); break;
		case 0x1D: printf("DCR    E"); break;
		case 0x1E: printf("MVI    E,#$%02X", code[1]); opbytes = 2; break;
		case 0x1F: printf("RAR"); break;

		case 0x20: printf("NOP"); break;
		case 0x21: printf("LXI    H,#$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0x22: printf("SHLD   #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0x23: printf("INX    H"); break;
		case 0x24: printf("INR    H"); break;
		case 0x25: printf("DCR    H"); break;
		case 0x26: printf("MVI    H,#$%02X", code[1]); opbytes = 2; break;
		case 0x27: printf("DAA"); break;
		case 0x28: printf("NOP"); break;
		case 0x29: printf("DAD    H"); break;
		case 0x2A: printf("LHLD   #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0x2B: printf("DCX    H"); break;
		case 0x2C: printf("INR    L"); break;
		case 0x2D: printf("DCR    L"); break;
		case 0x2E: printf("MVI    L,#$%02X", code[1]); opbytes = 2; break;
		case 0x2F: printf("CMA"); break;

		case 0x30: printf("NOP"); break;
		case 0x31: printf("LXI    SP,#$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0x32: printf("STA    #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0x33: printf("INX    SP"); break;
		case 0x34: printf("INR    M"); break;
		case 0x35: printf("DCR    M"); break;
		case 0x36: printf("MVI    M,#$%02X", code[1]); opbytes = 2; break;
		case 0x37: printf("STC"); break;
		case 0x38: printf("NOP"); break;
		case 0x39: printf("DAD    SP"); break;
		case 0x3A: printf("LDA    #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0x3B: printf("DCX    SP"); break;
		case 0x3C: printf("INR    A"); break;
		case 0x3D: printf("DCR    A"); break;
		case 0x3E: printf("MVI    A,#$%02X", code[1]); opbytes = 2; break;
		case 0x3F: printf("CMC"); break;

		case 0x40: printf("MOV    B,B"); break;
		case 0x41: printf("MOV    B,C"); break;
		case 0x42: printf("MOV    B,D"); break;
		case 0x43: printf("MOV    B,E"); break;
		case 0x44: printf("MOV    B,H"); break;
		case 0x45: printf("MOV    B,L"); break;
		case 0x46: printf("MOV    B,M"); break;
		case 0x47: printf("MOV    B,A"); break;
		case 0x48: printf("MOV    C,B"); break;
		case 0x49: printf("MOV    C,C"); break;
		case 0x4A: printf("MOV    C,D"); break;
		case 0x4B: printf("MOV    C,E"); break;
		case 0x4C: printf("MOV    C,H"); break;
		case 0x4D: printf("MOV    C,L"); break;
		case 0x4E: printf("MOV    C,M"); break;
		case 0x4F: printf("MOV    C,A"); break;

		case 0x50: printf("MOV    D,B"); break;
		case 0x51: printf("MOV    D,C"); break;
		case 0x52: printf("MOV    D,D"); break;
		case 0x53: printf("MOV    D,E"); break;
		case 0x54: printf("MOV    D,H"); break;
		case 0x55: printf("MOV    D,L"); break;
		case 0x56: printf("MOV    D,M"); break;
		case 0x57: printf("MOV    D,A"); break;
		case 0x58: printf("MOV    E,B"); break;
		case 0x59: printf("MOV    E,C"); break;
		case 0x5A: printf("MOV    E,D"); break;
		case 0x5B: printf("MOV    E,E"); break;
		case 0x5C: printf("MOV    E,H"); break;
		case 0x5D: printf("MOV    E,L"); break;
		case 0x5E: printf("MOV    E,M"); break;
		case 0x5F: printf("MOV    E,A"); break;

		case 0x60: printf("MOV    H,B"); break;
		case 0x61: printf("MOV    H,C"); break;
		case 0x62: printf("MOV    H,D"); break;
		case 0x63: printf("MOV    H,E"); break;
		case 0x64: printf("MOV    H,H"); break;
		case 0x65: printf("MOV    H,L"); break;
		case 0x66: printf("MOV    H,M"); break;
		case 0x67: printf("MOV    H,A"); break;
		case 0x68: printf("MOV    L,B"); break;
		case 0x69: printf("MOV    L,C"); break;
		case 0x6A: printf("MOV    L,D"); break;
		case 0x6B: printf("MOV    L,E"); break;
		case 0x6C: printf("MOV    L,H"); break;
		case 0x6D: printf("MOV    L,L"); break;
		case 0x6E: printf("MOV    L,M"); break;
		case 0x6F: printf("MOV    L,A"); break;

		case 0x70: printf("MOV    M,B"); break;
		case 0x71: printf("MOV    M,C"); break;
		case 0x72: printf("MOV    M,D"); break;
		case 0x73: printf("MOV    M,E"); break;
		case 0x74: printf("MOV    M,H"); break;
		case 0x75: printf("MOV    M,L"); break;
		case 0x76: printf("HLT"); break;
		case 0x77: printf("MOV    M,A"); break;
		case 0x78: printf("MOV    A,B"); break;
		case 0x79: printf("MOV    A,C"); break;
		case 0x7A: printf("MOV    A,D"); break;
		case 0x7B: printf("MOV    A,E"); break;
		case 0x7C: printf("MOV    A,H"); break;
		case 0x7D: printf("MOV    A,L"); break;
		case 0x7E: printf("MOV    A,M"); break;
		case 0x7F: printf("MOV    A,A"); break;

		case 0x80: printf("ADD    B"); break;
		case 0x81: printf("ADD    C"); break;
		case 0x82: printf("ADD    D"); break;
		case 0x83: printf("ADD    E"); break;
		case 0x84: printf("ADD    H"); break;
		case 0x85: printf("ADD    L"); break;
		case 0x86: printf("ADD    M"); break;
		case 0x87: printf("ADD    A"); break;
		case 0x88: printf("ADC    B"); break;
		case 0x89: printf("ADC    C"); break;
		case 0x8A: printf("ADC    D"); break;
		case 0x8B: printf("ADC    E"); break;
		case 0x8C: printf("ADC    H"); break;
		case 0x8D: printf("ADC    L"); break;
		case 0x8E: printf("ADC    M"); break;
		case 0x8F: printf("ADC    A"); break;

		case 0x90: printf("SUB    B"); break;
		case 0x91: printf("SUB    C"); break;
		case 0x92: printf("SUB    D"); break;
		case 0x93: printf("SUB    E"); break;
		case 0x94: printf("SUB    H"); break;
		case 0x95: printf("SUB    L"); break;
		case 0x96: printf("SUB    M"); break;
		case 0x97: printf("SUB    A"); break;
		case 0x98: printf("SBB    B"); break;
		case 0x99: printf("SBB    C"); break;
		case 0x9A: printf("SBB    D"); break;
		case 0x9B: printf("SBB    E"); break;
		case 0x9C: printf("SBB    H"); break;
		case 0x9D: printf("SBB    L"); break;
		case 0x9E: printf("SBB    M"); break;
		case 0x9F: printf("SBB    A"); break;

		case 0xA0: printf("ANA    B"); break;
		case 0xA1: printf("ANA    C"); break;
		case 0xA2: printf("ANA    D"); break;
		case 0xA3: printf("ANA    E"); break;
		case 0xA4: printf("ANA    H"); break;
		case 0xA5: printf("ANA    L"); break;
		case 0xA6: printf("ANA    M"); break;
		case 0xA7: printf("ANA    A"); break;
		case 0xA8: printf("XRA    B"); break;
		case 0xA9: printf("XRA    C"); break;
		case 0xAA: printf("XRA    D"); break;
		case 0xAB: printf("XRA    E"); break;
		case 0xAC: printf("XRA    H"); break;
		case 0xAD: printf("XRA    L"); break;
		case 0xAE: printf("XRA    M"); break;
		case 0xAF: printf("XRA    A"); break;


		case 0xB0: printf("ORA    B"); break;
		case 0xB1: printf("ORA    C"); break;
		case 0xB2: printf("ORA    D"); break;
		case 0xB3: printf("ORA    E"); break;
		case 0xB4: printf("ORA    H"); break;
		case 0xB5: printf("ORA    L"); break;
		case 0xB6: printf("ORA    M"); break;
		case 0xB7: printf("ORA    A"); break;
		case 0xB8: printf("CMP    B"); break;
		case 0xB9: printf("CMP    C"); break;
		case 0xBA: printf("CMP    D"); break;
		case 0xBB: printf("CMP    E"); break;
		case 0xBC: printf("CMP    H"); break;
		case 0xBD: printf("CMP    L"); break;
		case 0xBE: printf("CMP    M"); break;
		case 0xBF: printf("CMP    A"); break;


		case 0xC0: printf("RNZ"); break;
		case 0xC1: printf("POP    B"); break;
		case 0xC2: printf("JNZ    #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xC3: printf("JMP    #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xC4: printf("CNZ    #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xC5: printf("PUSH   B"); break;
		case 0xC6: printf("ADI    #$%02X", code[1]); opbytes = 2; break;
		case 0xC7: printf("RST    0"); break;
		case 0xC8: printf("RZ"); break;
		case 0xC9: printf("RET"); break;
		case 0xCA: printf("JZ     #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xCB: printf("*JMP   #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xCC: printf("CZ     #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xCD: printf("CALL   #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xCE: printf("ACI    #$%02X", code[1]); opbytes = 2; break;
		case 0xCF: printf("RST    1"); break;

		case 0xD0: printf("RNC"); break;
		case 0xD1: printf("POP    D"); break;
		case 0xD2: printf("JNC    #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xD3: printf("OUT    #$%02X", code[1]); opbytes = 2; break;
		case 0xD4: printf("CNC    #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xD5: printf("PUSH   D"); break;
		case 0xD6: printf("SUI    #$%02X", code[1]); opbytes = 2; break;
		case 0xD7: printf("RST    2"); break;
		case 0xD8: printf("RC"); break;
		case 0xD9: printf("*RET"); break;
		case 0xDA: printf("JC     #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xDB: printf("IN     #$%02X", code[1]); opbytes = 2; break;
		case 0xDC: printf("CC     #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xDD: printf("*CALL  #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xDE: printf("SBI    #$%02X", code[1]); opbytes = 2; break;
		case 0xDF: printf("RST    3"); break;

		case 0xE0: printf("RPO"); break;
		case 0xE1: printf("POP    H"); break;
		case 0xE2: printf("JPO    #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xE3: printf("XTHL"); break;
		case 0xE4: printf("CPO    #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xE5: printf("PUSH   H"); break;
		case 0xE6: printf("ANI    #$%02X", code[1]); opbytes = 2; break;
		case 0xE7: printf("RST    4"); break;
		case 0xE8: printf("RPE"); break;
		case 0xE9: printf("PCHL"); break;
		case 0xEA: printf("JPE    #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xEB: printf("XCHG"); break;
		case 0xEC: printf("CPE    #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xED: printf("*CALL   #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xEE: printf("XRI    #$%02X", code[1]); opbytes = 2; break;
		case 0xEF: printf("RST    5"); break;

		case 0xF0: printf("RP"); break;
		case 0xF1: printf("POP    PSW"); break;
		case 0xF2: printf("JP     #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xF3: printf("DI"); break;
		case 0xF4: printf("CP     #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xF5: printf("PUSH   PSW"); break;
		case 0xF6: printf("ORI    #$%02X", code[1]); opbytes = 2; break;
		case 0xF7: printf("RST    6"); break;
		case 0xF8: printf("RM"); break;
		case 0xF9: printf("SPHL"); break;
		case 0xFA: printf("JM     #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xFB: printf("EI"); break;
		case 0xFC: printf("CM     #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xFD: printf("*CALL   #$%02X%02X", code[2], code[1]); opbytes = 3; break;
		case 0xFE: printf("CPI    #$%02X", code[1]); opbytes = 2; break;
		case 0xFF: printf("RST    7"); break;
		}
		printf("\n");
		pc += opbytes;
	}
	pc = temppc;
}

void Emu8080::setZeroFlag(uint16_t num)
{
	if ((num & 0xFF) == 0)
		flags.z = 1;
	else
		flags.z = 0;
}

void Emu8080::setSignFlag(uint16_t num)
{
	if (num & 0x80) // Check if the most significant bit is set
		flags.s = 1;
	else
		flags.s = 0;
}

void Emu8080::setParityFlag(uint16_t num)
{
	bool parity = 0;
	while (num) {
		parity = !parity;
		num = num & (num - 1);
	}
	flags.p = parity;
}

void Emu8080::setCarryFlag(uint16_t num)
{
	if (num < 0)
		flags.cy = 1;
	else
		flags.cy = 0;
}

int Emu8080::NOP() {
	return 1;
}

/*
* Arithmetic Group
*/
int Emu8080::ADD_B() {
	uint16_t ans = registers.a + registers.b;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::ADD_C() {
	uint16_t ans = registers.a + registers.c;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::ADD_D() {
	uint16_t ans = registers.a + registers.d;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::ADD_E() {
	uint16_t ans = registers.a + registers.e;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::ADD_H() {
	uint16_t ans = registers.a + registers.h;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::ADD_L() {
	uint16_t ans = registers.a + registers.l;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::ADD_M() {
	uint16_t offset = (registers.h << 8) | (registers.l);
	uint16_t ans = registers.a + memory[offset];
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::ADD_A() {
	uint16_t ans = registers.a + registers.a;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::ADI_d8() {
	uint16_t ans = registers.a + opcode[1];
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 2;
}

int Emu8080::ADC_B() {
	uint16_t ans = registers.a + registers.b + flags.cy;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::ADC_C() {
	uint16_t ans = registers.a + registers.c + flags.cy;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::ADC_D() {
	uint16_t ans = registers.a + registers.d + flags.cy;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::ADC_E() {
	uint16_t ans = registers.a + registers.e + flags.cy;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::ADC_H() {
	uint16_t ans = registers.a + registers.h + flags.cy;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::ADC_L() {
	uint16_t ans = registers.a + registers.l + flags.cy;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::ADC_M() {
	uint16_t offset = (registers.h << 8) | (registers.l);
	uint16_t ans = registers.a + memory[offset] + flags.cy;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::ADC_A() {
	uint16_t ans = registers.a + registers.a + flags.cy;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::ACI_d8() {
	uint16_t ans = registers.a + opcode[1] + flags.cy;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 2;
}

int Emu8080::SUB_B() {
	uint16_t ans = registers.a - registers.b;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::SUB_C() {
	uint16_t ans = registers.a - registers.c;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::SUB_D() {
	uint16_t ans = registers.a - registers.d;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::SUB_E() {
	uint16_t ans = registers.a - registers.e;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::SUB_H() {
	uint16_t ans = registers.a - registers.h;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::SUB_L() {
	uint16_t ans = registers.a - registers.l;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::SUB_M() {
	uint16_t offset = (registers.h << 8) | (registers.l);
	uint16_t ans = registers.a - memory[offset];
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::SUB_A() {
	uint16_t ans = registers.a - registers.a;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::SUI_d8() {
	uint16_t ans = registers.a - opcode[1];
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 2;
}

int Emu8080::SBB_B() {
	uint16_t ans = registers.a - registers.b - flags.cy;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::SBB_C() {
	uint16_t ans = registers.a - registers.c - flags.cy;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::SBB_D() {
	uint16_t ans = registers.a - registers.d - flags.cy;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::SBB_E() {
	uint16_t ans = registers.a - registers.e - flags.cy;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::SBB_H() {
	uint16_t ans = registers.a - registers.h - flags.cy;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::SBB_L() {
	uint16_t ans = registers.a - registers.l - flags.cy;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::SBB_M() {
	uint16_t offset = (registers.h << 8) | (registers.l);
	uint16_t ans = registers.a - memory[offset] - flags.cy;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::SBB_A() {
	uint16_t ans = registers.a - registers.a - flags.cy;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 1;
}

int Emu8080::SBI_d8() {
	uint16_t ans = registers.a - opcode[1] - flags.cy;
	registers.a = ans & 0xFF;
	setCarryFlag(ans);
	setParityFlag(ans);
	setSignFlag(ans);
	setZeroFlag(ans);
	return 2;
}

int Emu8080::INR_B() {
	uint16_t ans = registers.b + 1;
	registers.b = ans & 0xFF;
	setZeroFlag(ans);
	setSignFlag(ans);
	setParityFlag(ans);
	return 1;
}

int Emu8080::INR_C() {
	uint16_t ans = registers.c + 1;
	registers.c = ans & 0xFF;
	setZeroFlag(ans);
	setSignFlag(ans);
	setParityFlag(ans);
	return 1;
}

int Emu8080::INR_D() {
	uint16_t ans = registers.d + 1;
	registers.d = ans & 0xFF;
	setZeroFlag(ans);
	setSignFlag(ans);
	setParityFlag(ans);
	return 1;
}

int Emu8080::INR_E() {
	uint16_t ans = registers.e + 1;
	registers.e = ans & 0xFF;
	setZeroFlag(ans);
	setSignFlag(ans);
	setParityFlag(ans);
	return 1;
}

int Emu8080::INR_H() {
	uint16_t ans = registers.h + 1;
	registers.h = ans & 0xFF;
	setZeroFlag(ans);
	setSignFlag(ans);
	setParityFlag(ans);
	return 1;
}

int Emu8080::INR_L() {
	uint16_t ans = registers.l + 1;
	registers.l = ans & 0xFF;
	setZeroFlag(ans);
	setSignFlag(ans);
	setParityFlag(ans);
	return 1;
}

int Emu8080::INR_M() {
	uint16_t offset = (registers.h << 8) | (registers.l);
	uint16_t ans = memory[offset] + 1;
	memory[offset] = ans & 0xFF;
	setZeroFlag(ans);
	setSignFlag(ans);
	setParityFlag(ans);
	return 1;
}

int Emu8080::INR_A() {
	uint16_t ans = registers.a + 1;
	registers.a = ans & 0xFF;
	setZeroFlag(ans);
	setSignFlag(ans);
	setParityFlag(ans);
	return 1;
}

int Emu8080::DCR_B() {
	uint16_t ans = registers.b - 1;
	registers.b = ans & 0xFF;
	setZeroFlag(ans);
	setSignFlag(ans);
	setParityFlag(ans);
	return 1;
}

int Emu8080::DCR_C() {
	uint16_t ans = registers.c - 1;
	registers.c = ans & 0xFF;
	setZeroFlag(ans);
	setSignFlag(ans);
	setParityFlag(ans);
	return 1;
}

int Emu8080::DCR_D() {
	uint16_t ans = registers.d - 1;
	registers.d = ans & 0xFF;
	setZeroFlag(ans);
	setSignFlag(ans);
	setParityFlag(ans);
	return 1;
}

int Emu8080::DCR_E() {
	uint16_t ans = registers.e - 1;
	registers.e = ans & 0xFF;
	setZeroFlag(ans);
	setSignFlag(ans);
	setParityFlag(ans);
	return 1;
}

int Emu8080::DCR_H() {
	uint16_t ans = registers.h - 1;
	registers.h = ans & 0xFF;
	setZeroFlag(ans);
	setSignFlag(ans);
	setParityFlag(ans);
	return 1;
}

int Emu8080::DCR_L() {
	uint16_t ans = registers.l - 1;
	registers.l = ans & 0xFF;
	setZeroFlag(ans);
	setSignFlag(ans);
	setParityFlag(ans);
	return 1;
}

int Emu8080::DCR_M() {
	uint16_t offset = (registers.h << 8) | (registers.l);
	uint16_t ans = memory[offset] - 1;
	memory[offset] = ans & 0xFF;
	setZeroFlag(ans);
	setSignFlag(ans);
	setParityFlag(ans);
	return 1;
}

int Emu8080::DCR_A() {
	uint16_t ans = registers.a + 1;
	registers.a = ans & 0xFF;
	setZeroFlag(ans);
	setSignFlag(ans);
	setParityFlag(ans);
	return 1;
}

int Emu8080::INX_B() {
	setBC(getBC() + 1);
	return 1;
}

int Emu8080::INX_D() {
	setDE(getDE() + 1);
	return 1;
}

int Emu8080::INX_H() {
	setHL(getHL() + 1);
	return 1;
}

int Emu8080::INX_SP() {
	uint16_t ans = sp + 1;
	sp = ans & 0xFFFF;
	return 1;
}

int Emu8080::DCX_B() {
	setBC(getBC() - 1);
	return 1;
}

int Emu8080::DCX_D() {
	setDE(getDE() - 1);
	return 1;
}

int Emu8080::DCX_H() {
	setHL(getHL() - 1);
	return 1;
}

int Emu8080::DCX_SP() {
	uint16_t ans = sp - 1;
	sp = ans & 0xFFFF;
	return 1;
}

int Emu8080::DAD_B() {
	uint16_t ans = getBC() + getHL();
	setHL(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::DAD_D() {
	uint16_t ans = getDE() + getHL();
	setHL(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::DAD_H() {
	uint16_t ans = getHL() + getHL();
	setHL(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::DAD_SP() {
	uint16_t ans = getHL() + sp;
	setHL(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::DAA() {
	if ((registers.a & 0xF) > 9) { // Didn't use AC for this implementation
		registers.a = (registers.a + 6) & 0xFF;
	}
	if ((registers.a >> 4) > 9 || flags.cy == 1) {
		uint8_t temp = (((registers.a >> 4) + 6) & 0xF) << 4;
		temp = temp | (registers.a & 0xF);
		registers.a = temp;
	}
	return 1;
}

/*
* Branch Group
*/
int Emu8080::getNewAddr() {
	return (opcode[2] << 8) | opcode[1];
}

int Emu8080::JMP_a16() {
	pc = getNewAddr();
	return 0;
}

int Emu8080::JNZ_a16() {
	if (flags.z == 0) {
		return JMP_a16();
	}
	else {
		return 3;
	}
}

int Emu8080::JZ_a16() {
	if (flags.z == 1) {
		return JMP_a16();
	}
	else {
		return 3;
	}
}

int Emu8080::JNC_a16() {
	if (flags.cy == 0) {
		return JMP_a16();
	}
	else {
		return 3;
	}
}

int Emu8080::JC_a16() {
	if (flags.cy == 1) {
		return JMP_a16();
	}
	else {
		return 3;
	}
}

int Emu8080::JPO_a16() {
	if (flags.p == 0) {
		return JMP_a16();
	}
	else {
		return 3;
	}
}

int Emu8080::JPE_a16() {
	if (flags.p == 1) {
		return JMP_a16();
	}
	else {
		return 3;
	}
}

int Emu8080::JP_a16() {
	if (flags.s == 0) {
		return JMP_a16();
	}
	else {
		return 3;
	}
}

int Emu8080::JM_a16() {
	if (flags.s == 1) {
		return JMP_a16();
	}
	else {
		return 3;
	}
}

int Emu8080::CALL_a16() {
	uint16_t temp = pc + 2;
	temp = temp & 0xFF;
	memory[sp - 1] = temp >> 8;
	memory[sp - 2] = temp & 0xFF;
	sp -= 2;
	return JMP_a16();
}

int Emu8080::RET() {
	pc = (memory[sp + 1] << 8) | memory[sp];
	sp += 2;
	return 1;
}

int Emu8080::CNZ_a16() {
	if (flags.z == 0) {
		return CALL_a16();
	}
	else {
		return 3;
	}
}

int Emu8080::CZ_a16() {
	if (flags.z == 1) {
		return CALL_a16();
	}
	else {
		return 3;
	}
}

int Emu8080::CNC_a16() {
	if (flags.cy == 0) {
		return CALL_a16();
	}
	else {
		return 3;
	}
}

int Emu8080::CC_a16() {
	if (flags.cy == 1) {
		return CALL_a16();
	}
	else {
		return 3;
	}
}

int Emu8080::CPO_a16() {
	if (flags.p == 0) {
		return CALL_a16();
	}
	else {
		return 3;
	}
}

int Emu8080::CPE_a16() {
	if (flags.p == 1) {
		return CALL_a16();
	}
	else {
		return 3;
	}
}

int Emu8080::CP_a16() {
	if (flags.s == 0) {
		return CALL_a16();
	}
	else {
		return 3;
	}
}

int Emu8080::CM_a16() {
	if (flags.s == 1) {
		return CALL_a16();
	}
	else {
		return 3;
	}
}

int Emu8080::RNZ() {
	if (flags.z == 0) {
		return RET();
	}
	else {
		return 3;
	}
}

int Emu8080::RZ() {
	if (flags.z == 1) {
		return RET();
	}
	else {
		return 3;
	}
}

int Emu8080::RNC() {
	if (flags.cy == 0) {
		return RET();
	}
	else {
		return 3;
	}
}

int Emu8080::RC() {
	if (flags.cy == 1) {
		return RET();
	}
	else {
		return 3;
	}
}

int Emu8080::RPO() {
	if (flags.p == 0) {
		return RET();
	}
	else {
		return 3;
	}
}

int Emu8080::RPE() {
	if (flags.p == 1) {
		return RET();
	}
	else {
		return 3;
	}
}

int Emu8080::RP() {
	if (flags.s == 0) {
		return RET();
	}
	else {
		return 3;
	}
}

int Emu8080::RM() {
	if (flags.s == 1) {
		return RET();
	}
	else {
		return 3;
	}
}

int Emu8080::RST_0() {
	uint16_t temp = pc + 2;
	temp = temp & 0xFF;
	memory[sp - 1] = temp >> 8;
	memory[sp - 2] = temp & 0xFF;
	sp -= 2;
	pc = 8 * 0b000;
	return 1;
}

int Emu8080::RST_1() {
	uint16_t temp = pc + 2;
	temp = temp & 0xFF;
	memory[sp - 1] = temp >> 8;
	memory[sp - 2] = temp & 0xFF;
	sp -= 2;
	pc = 8 * 0b001;
	return 1;
}

int Emu8080::RST_2() {
	uint16_t temp = pc + 2;
	temp = temp & 0xFF;
	memory[sp - 1] = temp >> 8;
	memory[sp - 2] = temp & 0xFF;
	sp -= 2;
	pc = 8 * 0b010;
	return 1;
}

int Emu8080::RST_3() {
	uint16_t temp = pc + 2;
	temp = temp & 0xFF;
	memory[sp - 1] = temp >> 8;
	memory[sp - 2] = temp & 0xFF;
	sp -= 2;
	pc = 8 * 0b011;
	return 1;
}

int Emu8080::RST_4() {
	uint16_t temp = pc + 2;
	temp = temp & 0xFF;
	memory[sp - 1] = temp >> 8;
	memory[sp - 2] = temp & 0xFF;
	sp -= 2;
	pc = 8 * 0b100;
	return 1;
}

int Emu8080::RST_5() {
	uint16_t temp = pc + 2;
	temp = temp & 0xFF;
	memory[sp - 1] = temp >> 8;
	memory[sp - 2] = temp & 0xFF;
	sp -= 2;
	pc = 8 * 0b101;
	return 1;
}

int Emu8080::RST_6() {
	uint16_t temp = pc + 2;
	temp = temp & 0xFF;
	memory[sp - 1] = temp >> 8;
	memory[sp - 2] = temp & 0xFF;
	sp -= 2;
	pc = 8 * 0b110;
	return 1;
}

int Emu8080::RST_7() {
	uint16_t temp = pc + 2;
	temp = temp & 0xFF;
	memory[sp - 1] = temp >> 8;
	memory[sp - 2] = temp & 0xFF;
	sp -= 2;
	pc = 8 * 0b111;
	return 1;
}

int Emu8080::PCHL() {
	pc = getHL();
	return 1;
}

/*
* Logical Group
*/
int Emu8080::ANA_B() {
	uint8_t ans = registers.a & registers.b;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::ANA_C() {
	uint8_t ans = registers.a & registers.c;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::ANA_D() {
	uint8_t ans = registers.a & registers.d;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::ANA_E() {
	uint8_t ans = registers.a & registers.e;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::ANA_H() {
	uint8_t ans = registers.a & registers.h;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::ANA_L() {
	uint8_t ans = registers.a & registers.l;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::ANA_M() {
	uint8_t ans = registers.a & memory[getHL()];
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::ANA_A() {
	uint8_t ans = registers.a & registers.a;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::ANI_d8() {
	uint8_t ans = registers.a & opcode[1];
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 2;
}

int Emu8080::XRA_B() {
	uint8_t ans = registers.a ^ registers.b;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::XRA_C() {
	uint8_t ans = registers.a ^ registers.c;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::XRA_D() {
	uint8_t ans = registers.a ^ registers.d;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::XRA_E() {
	uint8_t ans = registers.a ^ registers.e;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::XRA_H() {
	uint8_t ans = registers.a ^ registers.h;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::XRA_L() {
	uint8_t ans = registers.a ^ registers.l;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::XRA_M() {
	uint8_t ans = registers.a ^ memory[getHL()];
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::XRA_A() {
	uint8_t ans = registers.a ^ registers.a;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::XRI_d8() {
	uint8_t ans = registers.a ^ opcode[1];
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 2;
}

int Emu8080::ORA_B() {
	uint8_t ans = registers.a | registers.b;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::ORA_C() {
	uint8_t ans = registers.a | registers.c;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::ORA_D() {
	uint8_t ans = registers.a | registers.d;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::ORA_E() {
	uint8_t ans = registers.a | registers.e;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::ORA_H() {
	uint8_t ans = registers.a | registers.h;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::ORA_L() {
	uint8_t ans = registers.a | registers.l;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::ORA_M() {
	uint8_t ans = registers.a | memory[getHL()];
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::ORA_A() {
	uint8_t ans = registers.a | registers.a;
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::ORI_a8() {
	uint8_t ans = registers.a | opcode[1];
	registers.a = ans;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 2;
}

int Emu8080::CMP_B() {
	uint8_t ans = registers.a - registers.b;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::CMP_C() {
	uint8_t ans = registers.a - registers.c;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::CMP_D() {
	uint8_t ans = registers.a - registers.d;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::CMP_E() {
	uint8_t ans = registers.a - registers.e;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::CMP_H() {
	uint8_t ans = registers.a - registers.h;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::CMP_L() {
	uint8_t ans = registers.a - registers.l;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::CMP_M() {
	uint8_t ans = registers.a - memory[getHL()];
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::CMP_A() {
	uint8_t ans = registers.a - registers.a;
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 1;
}

int Emu8080::CPI_d8() {
	uint8_t ans = registers.a - opcode[1];
	setParityFlag(ans);
	setZeroFlag(ans);
	setSignFlag(ans);
	setCarryFlag(ans);
	return 2;
}

int Emu8080::RLC() {
	bool shiftedOut = (registers.a & 0x80) >> 7;
	registers.a = registers.a << 1;
	flags.cy = shiftedOut;
	registers.a = registers.a | shiftedOut;
	return 1;
}

int Emu8080::RRC() {
	bool shiftedOut = registers.a & 0x1;
	registers.a = registers.a >> 1;
	flags.cy = shiftedOut;
	registers.a = registers.a | (shiftedOut << 7);
	return 1;
}

int Emu8080::RAL() {
	uint8_t temp = registers.a;
	registers.a = (temp << 1) | flags.cy;
	flags.cy = (1 == (temp & 0x80));
	return 1;
}

int Emu8080::RAR() {
	uint8_t temp = registers.a;
	registers.a = (flags.cy << 7) | (temp >> 1);
	flags.cy = (1 == (temp & 0x1));
	return 1;
}

int Emu8080::CMA() {
	registers.a = ~registers.a;
	return 1;
}

int Emu8080::CMC() {
	flags.cy = !flags.cy;
	return 1;
}

int Emu8080::STC() {
	flags.cy = 1;
	return 1;
}

/*
* IO and Special Group
*/
int Emu8080::EI() {
	interrupt = true;
	return 1;
}

int Emu8080::DI() {
	interrupt = false; 
	return 1;
}

int Emu8080::HLT() {
	exit(0);
	return 1;
}

int Emu8080::IN_d8() {
	return 2;
}

int Emu8080::OUT_d8() {
	return 2;
}

/*
* Stack Group
*/
int Emu8080::PUSH_B() {
	memory[sp - 1] = registers.b;
	memory[sp - 2] = registers.c;
	sp -= 2;
	return 1;
}

int Emu8080::PUSH_D() {
	memory[sp - 1] = registers.d;
	memory[sp - 2] = registers.e;
	sp -= 2;
	return 1;
}

int Emu8080::PUSH_H() {
	memory[sp - 1] = registers.h;
	memory[sp - 2] = registers.l;
	sp -= 2;
	return 1;
}

int Emu8080::PUSH_PSW() {
	memory[sp - 1] = registers.a;
	memory[sp - 2] = flags.z | (flags.s << 1) | (flags.p << 2) | (flags.cy << 3) | (flags.ac << 4);
	sp -= 2;
	return 1;
}

int Emu8080::POP_B() {
	registers.c = memory[sp];
	registers.b = memory[sp + 1];
	sp += 2;
	return 1;
}

int Emu8080::POP_D() {
	registers.d = memory[sp];
	registers.e = memory[sp + 1];
	sp += 2;
	return 1;
}

int Emu8080::POP_H() {
	registers.h = memory[sp];
	registers.l = memory[sp + 1];
	sp += 2;
	return 1;
}

int Emu8080::POP_PSW() {
	uint8_t psw = memory[sp];
	flags.z  = (0x01 == (psw & 0x01));
	flags.s  = (0x02 == (psw & 0x02));
	flags.p  = (0x04 == (psw & 0x04));
	flags.cy = (0x05 == (psw & 0x08));
	flags.ac = (0x10 == (psw & 0x10));
	registers.a = memory[sp + 1];
	sp += 2;
	return 1;
}

int Emu8080::SPHL() {
	sp = getHL();
	return 1;
}

int Emu8080::XTHL() {
	uint8_t temp = memory[sp];
	memory[sp] = registers.l;
	registers.l = temp;
	temp = memory[sp + 1];
	memory[sp + 1] = registers.h;
	registers.h = temp;
	return 1;
}

/*
* Data Transfer Group
*/
int Emu8080::MOV_B_B() {
	registers.b = registers.b;
	return 1;
}

int Emu8080::MOV_B_C() {
	registers.b = registers.c;
	return 1;
}

int Emu8080::MOV_B_D() {
	registers.b = registers.d;
	return 1;
}

int Emu8080::MOV_B_E() {
	registers.b = registers.e;
	return 1;
}

int Emu8080::MOV_B_H() {
	registers.b = registers.h;
	return 1;
}

int Emu8080::MOV_B_L() {
	registers.b = registers.l;
	return 1;
}

int Emu8080::MOV_B_M() {
	registers.b = memory[getHL()];
	return 1;
}

int Emu8080::MOV_B_A() {
	registers.b = registers.a;
	return 1;
}

int Emu8080::MOV_C_B() {
	registers.c = registers.b;
	return 1;
}

int Emu8080::MOV_C_C() {
	registers.c = registers.c;
	return 1;
}

int Emu8080::MOV_C_D() {
	registers.c = registers.d;
	return 1;
}

int Emu8080::MOV_C_E() {
	registers.c = registers.e;
	return 1;
}

int Emu8080::MOV_C_H() {
	registers.c = registers.h;
	return 1;
}

int Emu8080::MOV_C_L() {
	registers.c = registers.l;
	return 1;
}

int Emu8080::MOV_C_M() {
	registers.c = memory[getHL()];
	return 1;
}

int Emu8080::MOV_C_A() {
	registers.c = registers.a;
	return 1;
}

int Emu8080::MOV_D_B() {
	registers.d = registers.b;
	return 1;
}

int Emu8080::MOV_D_C() {
	registers.d = registers.c;
	return 1;
}

int Emu8080::MOV_D_D() {
	registers.d = registers.d;
	return 1;
}

int Emu8080::MOV_D_E() {
	registers.d = registers.e;
	return 1;
}

int Emu8080::MOV_D_H() {
	registers.d = registers.h;
	return 1;
}

int Emu8080::MOV_D_L() {
	registers.d = registers.l;
	return 1;
}

int Emu8080::MOV_D_M() {
	registers.d = memory[getHL()];
	return 1;
}

int Emu8080::MOV_D_A() {
	registers.d = registers.a;
	return 1;
}

int Emu8080::MOV_E_B() {
	registers.e = registers.b;
	return 1;
}

int Emu8080::MOV_E_C() {
	registers.e = registers.c;
	return 1;
}

int Emu8080::MOV_E_D() {
	registers.e = registers.d;
	return 1;
}

int Emu8080::MOV_E_E() {
	registers.e = registers.e;
	return 1;
}

int Emu8080::MOV_E_H() {
	registers.e = registers.h;
	return 1;
}

int Emu8080::MOV_E_L() {
	registers.e = registers.l;
	return 1;
}

int Emu8080::MOV_E_M() {
	registers.e = memory[getHL()];
	return 1;
}

int Emu8080::MOV_E_A() {
	registers.e = registers.a;
	return 1;
}

int Emu8080::MOV_H_B() {
	registers.h = registers.b;
	return 1;
}

int Emu8080::MOV_H_C() {
	registers.h = registers.c;
	return 1;
}

int Emu8080::MOV_H_D() {
	registers.h = registers.d;
	return 1;
}

int Emu8080::MOV_H_E() {
	registers.h = registers.e;
	return 1;
}

int Emu8080::MOV_H_H() {
	registers.h = registers.h;
	return 1;
}

int Emu8080::MOV_H_L() {
	registers.h = registers.l;
	return 1;
}

int Emu8080::MOV_H_M() {
	registers.h = memory[getHL()];
	return 1;
}

int Emu8080::MOV_H_A() {
	registers.h = registers.a;
	return 1;
}

int Emu8080::MOV_L_B() {
	registers.l = registers.b;
	return 1;
}

int Emu8080::MOV_L_C() {
	registers.l = registers.c;
	return 1;
}

int Emu8080::MOV_L_D() {
	registers.l = registers.d;
	return 1;
}

int Emu8080::MOV_L_E() {
	registers.l = registers.e;
	return 1;
}

int Emu8080::MOV_L_H() {
	registers.l = registers.h;
	return 1;
}

int Emu8080::MOV_L_L() {
	registers.l = registers.l;
	return 1;
}

int Emu8080::MOV_L_M() {
	registers.l = memory[getHL()];
	return 1;
}

int Emu8080::MOV_L_A() {
	registers.l = registers.a;
	return 1;
}

int Emu8080::MOV_A_B() {
	registers.a = registers.b;
	return 1;
}

int Emu8080::MOV_A_C() {
	registers.a = registers.c;
	return 1;
}

int Emu8080::MOV_A_D() {
	registers.a = registers.d;
	return 1;
}

int Emu8080::MOV_A_E() {
	registers.a = registers.e;
	return 1;
}

int Emu8080::MOV_A_H() {
	registers.a = registers.h;
	return 1;
}

int Emu8080::MOV_A_L() {
	registers.a = registers.l;
	return 1;
}

int Emu8080::MOV_A_M() {
	registers.a = memory[getHL()];
	return 1;
}

int Emu8080::MOV_A_A() {
	registers.a = registers.a;
	return 1;
}

int Emu8080::MOV_M_B() {
	memory[getHL()] = registers.b;
	return 1;
}

int Emu8080::MOV_M_C() {
	memory[getHL()] = registers.b;
	return 1;
}

int Emu8080::MOV_M_D() {
	memory[getHL()] = registers.b;
	return 1;
}

int Emu8080::MOV_M_E() {
	memory[getHL()] = registers.b;
	return 1;
}

int Emu8080::MOV_M_H() {
	memory[getHL()] = registers.b;
	return 1;
}

int Emu8080::MOV_M_L() {
	memory[getHL()] = registers.b;
	return 1;
}

int Emu8080::MOV_M_A() {
	memory[getHL()] = registers.b;
	return 1;
}

int Emu8080::MVI_B_d8() {
	registers.b = opcode[1];
	return 2;
}

int Emu8080::MVI_C_d8() {
	registers.c = opcode[1];
	return 2;
}

int Emu8080::MVI_D_d8() {
	registers.d = opcode[1];
	return 2;
}

int Emu8080::MVI_E_d8() {
	registers.e = opcode[1];
	return 2;
}

int Emu8080::MVI_H_d8() {
	registers.h = opcode[1];
	return 2;
}

int Emu8080::MVI_L_d8() {
	registers.l = opcode[1];
	return 2;
}

int Emu8080::MVI_M_d8() {
	memory[getHL()] = opcode[1];
	return 2;
}

int Emu8080::MVI_A_d8() {
	registers.a = opcode[1];
	return 2;
}

int Emu8080::LXI_B_d16() {
	registers.b = opcode[2];
	registers.c = opcode[1];
	return 3;
}

int Emu8080::LXI_D_d16() {
	registers.d = opcode[2];
	registers.e = opcode[1];
	return 3;
}

int Emu8080::LXI_H_d16() {
	registers.h = opcode[2];
	registers.l = opcode[1];
	return 3;
}

int Emu8080::LXI_SP_d16() {
	sp = (opcode[2] << 8) | opcode[1];
	return 3;
}

int Emu8080::LDA_a16() {
	registers.a = memory[(opcode[2] << 8) | opcode[1]];
	return 3;
}

int Emu8080::STA_a16() {
	memory[(opcode[2] << 8) | opcode[1]] = registers.a;
	return 3;
}

int Emu8080::LHLD_a16() {
	registers.l = memory[(opcode[2] << 8) | opcode[1]];
	registers.h = memory[((opcode[2] << 8) | opcode[1]) + 1];
	return 3;
}

int Emu8080::SHLD_a16() {
	memory[(opcode[2] << 8) | opcode[1]] = registers.l;
	memory[((opcode[2] << 8) | opcode[1]) + 1] = registers.h;
	return 3;
}

int Emu8080::LDAX_B() {
	registers.a = memory[getBC()];
	return 1;
}

int Emu8080::LDAX_D() {
	registers.a = memory[getDE()];
	return 1;
}

int Emu8080::STAX_B() {
	memory[getBC()] = registers.a;
	return 1;
}

int Emu8080::STAX_D() {
	memory[getDE()] = registers.a;
	return 1;
}

int Emu8080::XCHG() {
	uint8_t temp = registers.h;
	registers.h = registers.d;
	registers.d = temp;
	temp = registers.l;
	registers.l = registers.e;
	registers.e = temp;
	return 1;
}