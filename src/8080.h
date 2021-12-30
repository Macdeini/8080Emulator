#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct Flags {
	// ac flag isn't used for space invaders
	uint8_t    z;
	uint8_t    s;
	uint8_t    p;
	uint8_t    cy; 
	uint8_t    ac;
	uint8_t    pad;
	Flags() : z(1), s(1), p(1), cy(1), ac(1), pad(3) { }
};

struct Registers {
	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t h;
	uint8_t l;
};

class Emu8080 {
public:
	Emu8080();
	~Emu8080();
	void emulateCycle();
	void disassemble();

//private:
	uint16_t pc;
	uint16_t sp;
	uint8_t intEnable;
	Registers registers;
	Flags flags;
	uint8_t* opcode;
	int fsize;
	unsigned char* memory;
	bool interrupt;
	typedef int (Emu8080::* opcodeFunc)();
	opcodeFunc table[256];

	void loadROM();
	void unimplemented();
	void setZeroFlag(uint16_t num);
	void setSignFlag(uint16_t num);
	void setParityFlag(uint16_t num);
	void setCarryFlag(uint16_t num);
	int getNewAddr();

	uint16_t getBC();
	uint16_t getDE();
	uint16_t getHL();

	void setBC(uint16_t num);
	void setDE(uint16_t num);
	void setHL(uint16_t num);

	// Opcodes, return the size of their bytes
	int NOP(); int LXI_B_d16(); int STAX_B(); int INX_B(); int INR_B(); int DCR_B(); int MVI_B_d8(); int RLC(); 
	int DAD_B();     int LDAX_B(); int DCX_B(); int INR_C(); int DCR_C(); int MVI_C_d8(); int RRC();

	int LXI_D_d16(); int STAX_D(); int INX_D(); int INR_D(); int DCR_D(); int MVI_D_d8(); int RAL();
	int DAD_D();     int LDAX_D(); int DCX_D(); int INR_E(); int DCR_E(); int MVI_E_d8(); int RAR();

	int LXI_H_d16(); int SHLD_a16(); int INX_H(); int INR_H(); int DCR_H(); int MVI_H_d8(); int DAA();
	int DAD_H();     int LHLD_a16(); int DCX_H(); int INR_L(); int DCR_L(); int MVI_L_d8(); int CMA();

	int LXI_SP_d16(); int STA_a16(); int INX_SP(); int INR_M(); int DCR_M(); int MVI_M_d8(); int STC();
	int DAD_SP();     int LDA_a16(); int DCX_SP(); int INR_A(); int DCR_A(); int MVI_A_d8(); int CMC();

	int MOV_B_B(); int MOV_B_C(); int MOV_B_D(); int MOV_B_E(); int MOV_B_H(); int MOV_B_L(); int MOV_B_M(); int MOV_B_A();
	int MOV_C_B(); int MOV_C_C(); int MOV_C_D(); int MOV_C_E(); int MOV_C_H(); int MOV_C_L(); int MOV_C_M(); int MOV_C_A();

	int MOV_D_B(); int MOV_D_C(); int MOV_D_D(); int MOV_D_E(); int MOV_D_H(); int MOV_D_L(); int MOV_D_M(); int MOV_D_A();
	int MOV_E_B(); int MOV_E_C(); int MOV_E_D(); int MOV_E_E(); int MOV_E_H(); int MOV_E_L(); int MOV_E_M(); int MOV_E_A();

	int MOV_H_B(); int MOV_H_C(); int MOV_H_D(); int MOV_H_E(); int MOV_H_H(); int MOV_H_L(); int MOV_H_M(); int MOV_H_A();
	int MOV_L_B(); int MOV_L_C(); int MOV_L_D(); int MOV_L_E(); int MOV_L_H(); int MOV_L_L(); int MOV_L_M(); int MOV_L_A();

	int MOV_M_B(); int MOV_M_C(); int MOV_M_D(); int MOV_M_E(); int MOV_M_H(); int MOV_M_L(); int HLT();     int MOV_M_A();
	int MOV_A_B(); int MOV_A_C(); int MOV_A_D(); int MOV_A_E(); int MOV_A_H(); int MOV_A_L(); int MOV_A_M(); int MOV_A_A();

	int ADD_B(); int ADD_C(); int ADD_D(); int ADD_E(); int ADD_H(); int ADD_L(); int ADD_M(); int ADD_A();
	int ADC_B(); int ADC_C(); int ADC_D(); int ADC_E(); int ADC_H(); int ADC_L(); int ADC_M(); int ADC_A();

	int SUB_B(); int SUB_C(); int SUB_D(); int SUB_E(); int SUB_H(); int SUB_L(); int SUB_M(); int SUB_A();
	int SBB_B(); int SBB_C(); int SBB_D(); int SBB_E(); int SBB_H(); int SBB_L(); int SBB_M(); int SBB_A();

	int ANA_B(); int ANA_C(); int ANA_D(); int ANA_E(); int ANA_H(); int ANA_L(); int ANA_M(); int ANA_A();
	int XRA_B(); int XRA_C(); int XRA_D(); int XRA_E(); int XRA_H(); int XRA_L(); int XRA_M(); int XRA_A();

	int ORA_B(); int ORA_C(); int ORA_D(); int ORA_E(); int ORA_H(); int ORA_L(); int ORA_M(); int ORA_A();
	int CMP_B(); int CMP_C(); int CMP_D(); int CMP_E(); int CMP_H(); int CMP_L(); int CMP_M(); int CMP_A();

	int RNZ(); int POP_B(); int JNZ_a16(); int JMP_a16();  int CNZ_a16(); int PUSH_B();   int ADI_d8(); int RST_0();
	int RZ();  int RET();   int JZ_a16();  int CZ_a16();  int CALL_a16(); int ACI_d8(); int RST_1();

	int RNC(); int POP_D();  int JNC_a16(); int OUT_d8(); int CNC_a16(); int PUSH_D();    int SUI_d8(); int RST_2();
	int RC();    int JC_a16();  int IN_d8();  int CC_a16(); int SBI_d8(); int RST_3();

	int RPO(); int POP_H();  int JPO_a16(); int XTHL(); int CPO_a16(); int PUSH_H();    int ANI_d8(); int RST_4();
	int RPE(); int PCHL();   int JPE_a16(); int XCHG(); int CPE_a16(); int XRI_d8(); int RST_5();

	int RP(); int POP_PSW(); int JP_a16(); int DI(); int CP_a16(); int PUSH_PSW();    int ORI_a8(); int RST_6();
	int RM(); int SPHL();    int JM_a16(); int EI(); int CM_a16();    int CPI_d8(); int RST_7();
};