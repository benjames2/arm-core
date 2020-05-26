#include <inc/decode_structure.h>
#include <sstream>

#define THROW_INVALID_METACODE(opcode) throw std::runtime_error("opcode(" #opcode ") : invalid meta opcode " + std::to_string(in.meta_opcode))
#define THROW_INVALID_METACODE_32B(opcode) throw std::runtime_error("opcode(" #opcode ") : invalid meta opcode for 32-bit instruction")
#define THROW_INVALID_ENCODING(instruction) throw std::runtime_error("Invalid encoding for : " #instruction " instruction")
#define THROW_UNDEFINED(instruction) throw std::runtime_error ("Operator overload undefined for " #instruction)

std::string instruction_16b_t::str(void) {
    std::stringstream ss;
    ss << *this << std::flush;
    return ss.str();
}

std::string instruction_32b_t::str(void){
    std::stringstream ss;
    ss << *this << std::flush;
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const instruction_16b_t& in) {

    // maintain a single instruction for BL <-- c'mon joe, what does this even mean?
    static instruction_t sInstruction;

    switch(in.opcode) {

        case i_ADC   : // add with carry
            //ADC   = 4
            os << "ADC r" << in.Rd << ", r" << in.Rs; // 
            break;

        case i_ADD   : // add
            //ADD   = 2*(RRR, RRC), 3(RC), 5*(RR), 12*(RC_pc, RC_sp), 13*(C_sp)
            os << "ADD ";
            {
                switch(in.meta_opcode) {
                    case meta_RRR:   os << 'r' << in.Rd << ", r" << in.Rs << ", r" << in.Rn;          break;
                    case meta_RRC:   os << 'r' << in.Rd << ", r" << in.Rs << ", #" << in.u_immediate; break;
                    case meta_RC:    os << 'r' << in.Rd << ", #" << in.u_immediate;                   break;
                    case meta_RR:    os << 'r' << in.Rd << ", r" << in.Rs;                            break;
                    case meta_RC_pc: os << 'r' << in.Rd << ", PC, #" << in.u_immediate;               break;
                    case meta_RC_sp: os << 'r' << in.Rd << ", SP, #" << in.u_immediate;               break;
                    case meta_C_sp:  os << "SP, #" << in.i_immediate;                                 break;
                    default:
                        THROW_INVALID_METACODE(ADD);
                        //throw std::runtime_error("opcode(ADD) : invalid meta opcode");
                }
            }
            break;

        case i_AND   : // bitwise AND
            // AND   = 4
            os << "AND r" << in.Rd << ", r" << in.Rs;
            break;

        case i_ASR   : // arithmetic shift right
            // ASR   = 1(RRC), 4(RR) 
            os << "ASR ";
            {
                switch(in.meta_opcode) {
                    case meta_RRC: os << 'r' << in.Rd << ", r" << in.Rs << ", #" << in.u_immediate; break;
                    case meta_RR:  os << 'r' << in.Rd << ", r" << in.Rs; break;
                    default:
                        THROW_INVALID_METACODE(ASR);
                }
            }
            break;

        case i_B     : // unconditional branch
            // B     = 18
            os << "B #" << in.i_immediate;
            break;

        case i_Bxx   : // conditional branch
            // Bxx   = 16* 
            switch(in.condition_code) {
                case 0:  os << "BEQ "; break;
                case 1:  os << "BNE "; break;
                case 2:  os << "BCS "; break;
                case 3:  os << "BCC "; break;
                case 4:  os << "BMI "; break;
                case 5:  os << "BPL "; break;
                case 6:  os << "BVS "; break;
                case 7:  os << "BVC "; break;
                case 8:  os << "BHI "; break;
                case 9:  os << "BLS "; break;
                case 10: os << "BGE "; break;
                case 11: os << "BLT "; break;
                case 12: os << "BGT "; break;
                case 13: os << "BLE "; break;
                default:
                    throw std::runtime_error("opcode(Bxx) : invalid condition code: " + std::to_string(in.condition_code));
            }
            os << "#" << in.i_immediate;
            break;

        case i_BIC   : // bit clear
            // BIC   = 4 
            os << "BIC r" << in.Rd << ", r" << in.Rs;
            break;

        case i_BL    : // branch and link
            //BL    = 19
            //os << "BL ";
            if(in.H == 0) {
                sInstruction = in;
                os << "BL (H=0)";

            } else if(in.H == 1) {

                int offset = sInstruction.i_immediate + in.i_immediate;
                os << "BL #" << offset;

                sInstruction.u_immediate = 0x00;            
            }
            break;

        case i_BX    : // branch and exchange
            //BX    = 5*
            os << "BX r" << in.Rs;
            break;

        case i_CBNZ:
            os << "CBNZ r" << in.Rn << " #" << in.u32;
            break;

        case i_CBZ:
            os << "CBZ r" << in.Rn << " #" << in.u32;
            break;
            
        case i_CMN   : // compare negative
            //CMN   = 4
            os << "CMN r" << in.Rd << ", r" << in.Rs; 
            break;
            
        case i_CMP   : // compare 
            //CMP   = 3(RC), 4(RR), 5*(RR) // 4 and 5 have the same meta opcode
            os << "CMP ";
            switch(in.meta_opcode){
                case meta_RC: os << "r" << in.Rd << ", #" << in.u_immediate; break;
                case meta_RR: os << "r" << in.Rd << ", r" << in.Rs;          break; //format 4 and 5 are both taking care of by meta__RR 
                default:
                    THROW_INVALID_METACODE(CMP);
            }
            break;

        case i_EOR   : // bitwise XOR
            //EOR   = 4
            os << "EOR r" << in.Rd << ", r" << in.Rs; 
            break;
        
        case i_LDMIA : // load multiple
            //LDMIA = 15
            os << "LDMIA r" << in.Rb << "!, { "; 
            
            for(int i = 0; i < 8; i++) {
                if((in.Rlist >> i) & 0x01)
                    os << "r" << i << ' ';
            }

            os << "}";  
            break;

        case i_LDR   : // load word
            //LDR   = 6(RC_pc), 7(RRR), 9(RRC), 11(RC_sp)
            os << "LDR ";
            switch(in.meta_opcode){
                case meta_RC_pc: os << "r" << in.Rd << ", [PC, #" << in.u_immediate << "]";             break;
                case meta_RRR: os << "r" << in.Rd << ", [r" << in.Rb << ", r" << in.Ro << "]";          break;
                case meta_RRC: os << "r" << in.Rd << ", [r" << in.Rb << ", #" << in.u_immediate << "]"; break;
                case meta_RC_sp: os << 'r' << in.Rd << ", [SP, #" << in.u_immediate << ']';             break;
                default:
                    THROW_INVALID_METACODE(LDR);
            }
            break;

        case i_LDRB  : // load byte
            //LDRB  = 7, 9 
            os << "LDRB ";
            switch(in.meta_opcode){
                case meta_RRR: os << "r" << in.Rd << ", [r" << in.Rb << ", r" << in.Ro << "]";          break;
                case meta_RRC: os << "r" << in.Rd << ", [r" << in.Rb << ", #" << in.u_immediate << "]"; break;
                default:
                    THROW_INVALID_METACODE(LDRB);
                    //throw std::runtime_error("opcode(LDRB) : invalid meta opcode");
            }
            break;

        case i_LDRH  : // load halfword
            //LDRH  = 8, 10
            os << "LDRH ";
            switch(in.meta_opcode){
                case meta_RRR: os << "r" << in.Rd << ", [r" << in.Rb << ", r" << in.Ro << "]"; break;
                case meta_RRC: os << "r" << in.Rd << ", [r" << in.Rb << ", #" << in.u_immediate << "]"; break;
                default:
                    THROW_INVALID_METACODE(LDRH);
            }
            break;

        case i_LSL   : // logical shift left
            //LSL   = 1, 4 
            os << "LSL ";
            switch(in.meta_opcode){
                case meta_RRC: os << "r" << in.Rd << ", r" << in.Rs << ", #" << in.u_immediate; break;
                case meta_RR: os << "r" << in.Rd << ", r" << in.Rs; break;
                default:
                    THROW_INVALID_METACODE(LSL);
            }
            break;

        case i_LDSB  : // load sign-extended byte
            //LDSB  = 8
            os << "LDSB r" << in.Rd << ", [r" << in.Rb << ", r" << in.Ro << "]";
            break;
        
        case i_LDSH  : // load sign-extended halfword
            //LDSH  = 8
            os << "LDSH r" << in.Rd << ", [r" << in.Rb << ", r" << in.Ro << "]";
            break;
        
        case i_LSR   : // logical shift right
            //LSR   = 1, 4
            os << "LSR ";
            switch(in.meta_opcode){
                case meta_RRC: os << "r" << in.Rd << ", r" << in.Rs << ", #" << in.u_immediate; break;
                case meta_RR: os << "r" << in.Rd << ", r" << in.Rs; break;
                default:
                    THROW_INVALID_METACODE(LSR);
            }
            break; 

        case i_MOV   : // move register
            //MOV   = 3, 5*
            os << "MOV ";
            switch(in.meta_opcode){
                case meta_RC: os << "r" << in.Rd << ", #" << in.u_immediate; break;
                case meta_RR: os << "r" << in.Rd << ", r" << in.Rs; break;
                default:
                    THROW_INVALID_METACODE(MOV);
            }
            break;

        case i_MUL   : // multiply
            //MUL   = 4*
            os << "MUL r" << in.Rd << ", r" << in.Rs;
            break;
        
        case i_MVN   : // move negative register
            //MVN   = 4*
            os << "MVN r" << in.Rd << ", r" << in.Rs;
            break;
        
        case i_NEG   : // negate
            //NEG   = 4*
            os << "NEG r" << in.Rd << ", r" << in.Rs;
            break;

        case i_NOP   : //nop
            os << "NOP";
            break;
        
        case i_ORR   : // bitwise OR
            //ORR   = 4
            os << "ORR r" << in.Rd << ", r" << in.Rs;
            break;
        
        case i_POP   : // pop registers
            //POP   = 14*
            os << "POP { ";

            for(int i = 0; i < 8; i++) {
                if((in.Rlist >> i) & 0x01)
                    os << "r" << i << " ";
            }

            switch(in.meta_opcode){
                case meta_C:                 break; // <-- do nothing for this case
                case meta_C_pc: os << "PC "; break;
                default:
                    THROW_INVALID_METACODE(POP);
            }

            os << "}";
            break;
        
        case i_PUSH  : // push registers
            //PUSH  = 14*
            os << "PUSH { ";

            for(int i = 0; i < 8; i++) {
                if((in.Rlist >> i) & 0x01)
                    os << "r" << i << " ";
            }

            switch(in.meta_opcode){
                case meta_C:                 break; // <-- do nothing for this case
                case meta_C_lr: os << "LR "; break;
                default:
                    THROW_INVALID_METACODE(PUSH);
            }

            os << "}";
            break;


        case i_ROR   : // rotate right
            //ROR   = 4
            os << "ROR ";
            os << "r" << in.Rd << ", r" << in.Rs;
            break;
        
        case i_SBC   : // subtract with carry
            //SBC   = 4
            os <<  "SBC ";
            os << "r" << in.Rd << ", r" << in.Rs;
            break;
        
        case i_STMIA : // store multiple
            //STMIA = 15
            os << "STMIA r" << in.Rb << "!, { "; 
            
            for(int i = 0; i < 8; i++) {
                if((in.Rlist >> i) & 0x01)
                    os << "r" << i << ' ';
            }

            os << "}";
            break;

        case i_STR   : // store word
            //STR   = 7, 9, 11 
            os << "STR ";
            switch(in.meta_opcode) {
                case meta_RRR:   os << 'r' << in.Rd << ", [r" << in.Rb << ", r" << in.Ro << ']';          break;
                case meta_RRC:   os << 'r' << in.Rd << ", [r" << in.Rb << ", #" << in.u_immediate << ']'; break;
                case meta_RC_sp: os << 'r' << in.Rd << ", [SP, #" << in.u_immediate << ']';               break;
                default: 
                    THROW_INVALID_METACODE(STR);
            }
            break;

        case i_STRB  : // store byte
            //STRB  = 7, 9
            os << "STRB ";
            switch(in.meta_opcode) {
                case meta_RRR:
                    os << 'r' << in.Rd << ", [r" << in.Rb << ", r" << in.Ro << ']';          
                    break;
                case meta_RRC: 
                    os << 'r' << in.Rd << ", [r" << in.Rb << ", #" << in.u_immediate << ']'; 
                    break;
                default:
                    THROW_INVALID_METACODE(STRB);               
            }
            break;

        case i_STRH  : // store halfword
            //STRH  = 8, 10
            os << "STRH ";
            switch (in.meta_opcode) {
                case meta_RRR: os << 'r' << in.Rd <<", [r" << in.Rb << ", r" << in.Ro << ']';           break;
                case meta_RRC: os << 'r' << in.Rd << ", [r" << in.Rb << ", #" << in.u_immediate << ']'; break;
                default:
                    THROW_INVALID_METACODE(STRH);
            }
            break;

        case i_SWI   : // software interrupt
            //SWI   = 17
            os << "SWI #" << in.u_immediate;
            break;

        case i_SUB   : // subtract
            //SUB   = 2*(RRR, RRC), 3(RC)
            os << "SUB " ;
            switch (in.meta_opcode) {
                case meta_RRR: os << 'r' << in.Rd << ", r" << in.Rs << ", r" << in.Rn;         break;
                case meta_RRC: os << 'r' << in.Rd << ", r" << in.Rs << ", #"<< in.u_immediate; break;   
                case meta_RC:  os << 'r' << in.Rd << ", #"<< in.u_immediate;                   break;     
                default:
                    THROW_INVALID_METACODE(SUB);
            }
            break;

        case i_TST   : // test bits
            // TST   = 4
            os << "TST r" << in.Rs << ", r" << in.Rd;
            break;

        default:
            throw std::runtime_error("instruction_t : invalid opcode");

    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const instruction_32b_t& in){

    switch(in.opcode){
        case t32_ADC:
            switch(in.meta_opcode){
                case meta_t32_imm:
                    os << "ADC";
                    if(in.S)
                        os << "s";
                    os << " r" << in.Rd << ", r" << in.Rn << ", #" << in.i32;
                    break;
                default:
                    THROW_INVALID_METACODE_32B(ADC);
            }
            break;

        case t32_ADR:
            THROW_UNDEFINED(ADR);

        case t32_ADD:
            switch(in.meta_opcode){
                case meta_t32_imm:
                    switch(in.encoding){
                        case instruction_32b_t::encoding_T3:
                            os << "ADD";
                            if(in.S)
                                os << "s";
                            os << " r" << in.Rd << ", r" << in.Rn << ", #" << in.i32;
                            break;
                        default: 
                            THROW_INVALID_ENCODING(ADD);
                    }
                    break;
                default:
                    THROW_INVALID_METACODE_32B(ADD);
            }
            break;

        case t32_AND:
            switch(in.meta_opcode){
                case meta_t32_imm:
                    os << "AND";
                    if(in.S)
                        os << "s";
                    os << " r" << in.Rd << ", r" << in.Rn << ", #" << in.i32;
                    break;
                default:
                    THROW_INVALID_METACODE_32B(AND);
            }
            break;

        case t32_ASR:
            THROW_UNDEFINED(ASR);

        case t32_B:
            THROW_UNDEFINED(B);

        case t32_BFC:
            THROW_UNDEFINED(t32_BFC);

        case t32_BFI:
            THROW_UNDEFINED(BFI);

        case t32_BIC:
            switch(in.meta_opcode){
                case meta_t32_imm:
                    os << "BIC";
                    if(in.S)
                        os << "s";
                    os << " r" << in.Rd << ", r" << in.Rn << ", #" << in.i32;
                    break;
                default:
                    THROW_INVALID_METACODE_32B(BIC);
            }
            break;

        case t32_BKPT:
            THROW_UNDEFINED(BKPT);

        case t32_BL:
            THROW_UNDEFINED(BL);

        case t32_BLX:
            THROW_UNDEFINED(BLX);

        case t32_BX:
            THROW_UNDEFINED(BX);

        case t32_CBNZ:
            THROW_UNDEFINED(CBNZ);

        case t32_CBZ:
            THROW_UNDEFINED(CBZ);

        case t32_CDP:
            THROW_UNDEFINED(CDP);

        case t32_CDP2:
            THROW_UNDEFINED(CDP2);

        case t32_CLREX:
            THROW_UNDEFINED(CLREX);

        case t32_CLZ:
            THROW_UNDEFINED(CLZ);

        case t32_CMN:
            switch(in.meta_opcode){
                case meta_t32_imm:
                    os << "CMN r" << in.Rn << ", #" << in.i32;
                    break;
                default:
                    THROW_INVALID_METACODE_32B(CMN);
            }
            break;

        case t32_CMP:
            switch(in.meta_opcode){
                case meta_t32_imm:
                    os << "CMP r" << in.Rn << ", #" << in.i32;
                    break;
                default:
                    THROW_INVALID_METACODE_32B(CMP);
            }
            break;

        case t32_CPS:
            THROW_UNDEFINED(CPS);

        case t32_CPY:
            THROW_UNDEFINED(CPY);

        case t32_DBG:
            THROW_UNDEFINED(DBG);

        case t32_DMB:
            THROW_UNDEFINED(DMB);

        case t32_DSB:
            THROW_UNDEFINED(DSB);

        case t32_EOR:
            switch(in.meta_opcode){
                case meta_t32_imm:
                    os << "EOR";
                    if(in.S)
                        os << "s";
                    os << " r" << in.Rd << ", r" << in.Rn << ", #" << in.i32;
                    break;
                default:
                    THROW_INVALID_METACODE_32B(EOR);
            }
            break;

        case t32_ISB:
            THROW_UNDEFINED(ISB);

        case t32_IT:
            THROW_UNDEFINED(IT);

        case t32_LDC:
            THROW_UNDEFINED(LDC);

        case t32_LDC2:
            THROW_UNDEFINED(LDC2);

        case t32_LDM:
            THROW_UNDEFINED(LDM);

        case t32_LDR:
            switch(in.meta_opcode){
                case meta_t32_imm:
                    switch(in.encoding){
                        case instruction_32b_t::encoding_T3:
                            os << "LDR r" << in.Rt << ", r" << in.Rn << ", #" << in.u32;
                            break;
                        case instruction_32b_t::encoding_T4:
                            os << "LDR";
                            if(in.P)
                                os << "p";
                            if(in.  U)
                                os << "u";
                            if(in.W)
                                os << "w";
                            os << " r" << in.Rt << ", r" << in.Rn << ", #" << in.i32;
                            break;
                        default: 
                            THROW_INVALID_ENCODING(LDR);
                    }
                    break;
                case meta_t32_literal:
                    std::runtime_error("LDR literal not done yet");
                    break;
                case meta_t32_reg:
                    os << "LDR r" << in.Rt << ", r" << in.Rn << ", r" << in.Rm << ", #" << in.u32;
                    break;
                default:
                    THROW_INVALID_METACODE_32B(LDR);
            }
            break;

        case t32_LDRB:
            switch(in.meta_opcode){
                case meta_t32_reg:
                    os << "LDRB_REG r" << in.Rt << ", [r" << in.Rn << ", r" << in.Rm << ", LSL #" << in.u32 << "]";
                    break;
                default:
                    THROW_INVALID_METACODE_32B(LDRB);
            }
            break;

        case t32_LDRD:
            switch(in.meta_opcode){
                case meta_t32_imm:
                    os << "LDRD_IMM r" << in.Rt << ", r" << in.Rt2 << ", r" << in.Rn << ", #" << in.u32;
                    break;
                case meta_t32_literal:
                    os << "LDRD_LIT r" << in.Rt << ", r" << in.Rt2 << ", #" << in.u32;
                    break;
                default:
                    THROW_INVALID_METACODE_32B(LDRD);
            }
            break;
        case t32_LDREX: 
            THROW_UNDEFINED(LDREX);
        case t32_LDREXB: 
            THROW_UNDEFINED(LDREXB);
        case t32_LDREXH: 
            THROW_UNDEFINED(LDREXH);
        case t32_LDRT:
            THROW_UNDEFINED(LDRT);
        case t32_MOV:
            switch(in.meta_opcode){
                case meta_t32_imm:
                    switch(in.encoding){
                        case 2:
                            os << "MOV";
                            if(in.S)
                                os << "s";
                            os << " r" << in.Rd << ", #" << in.i32;
                            break;
                        default: 
                            THROW_INVALID_ENCODING(MOV);
                    }
                    break;
                default:
                    THROW_INVALID_METACODE_32B(MOV);
            }
            break;
        case t32_MVN:
            THROW_UNDEFINED(MVN);
        case t32_ORN:
            THROW_UNDEFINED(ORN);
        case t32_ORR:
            switch(in.meta_opcode){
                case meta_t32_imm:
                    os << "ORR";
                    if(in.S)
                        os << "s";
                    os << " r" << in.Rd << ", r" << in.Rn << ", #" << in.i32;
                    break;
                default:
                    THROW_INVALID_METACODE_32B(ORR);
            }
            break;
        case t32_POP:
            THROW_UNDEFINED(POP);
        case t32_PUSH:
            THROW_UNDEFINED(PUSH);
        case t32_RSB:
            THROW_UNDEFINED(RSB);
        case t32_SBC:
            THROW_UNDEFINED(SBC);
        case t32_STM:
            THROW_UNDEFINED(STM);
        case t32_STMDB:
            THROW_UNDEFINED(STMDB);

        case t32_STR:
            switch(in.meta_opcode){
                case meta_t32_imm:
                    switch(in.encoding){
                        case instruction_32b_t::encoding_T3:
                            os << "STR r" << in.Rt << ", [r" << in.Rn << ", #" << in.u32 << "]";
                            break;
                        case instruction_32b_t::encoding_T4:
                            os << "STR";
                            if(in.P)
                                os << "p";
                            if(in.  U)
                                os << "u";
                            if(in.W)
                                os << "w";
                            os << " r" << in.Rt << ", [r" << in.Rn << ", #" << in.i32 << "]";
                            break;
                        default: 
                            THROW_INVALID_ENCODING(STR);
                    }
                    break;
                case meta_t32_reg:
                    os << "STR r" << in.Rt << ", [r" << in.Rn << ", r" << in.Rm << ", LSL #" << in.u32 << "]";
                    break;
                default:
                    THROW_INVALID_METACODE_32B(STR);
            }
            break;

        case t32_STRB:
            THROW_UNDEFINED(STRB);
        case t32_STRD:
            THROW_UNDEFINED(STRD);
        case t32_STREX:
            THROW_UNDEFINED(STREX);
        case t32_STREXB:
            THROW_UNDEFINED(STREXB);
        case t32_STREXH:
            THROW_UNDEFINED(STREXH);
        case t32_STRH:
            THROW_UNDEFINED(STRH);
        case t32_SUB:
            THROW_UNDEFINED(SUB);
        case t32_TBB:
            THROW_UNDEFINED(TBB);
        case t32_TBH:
            THROW_UNDEFINED(TBH);
        case t32_TEQ:
            THROW_UNDEFINED(TEQ);
        case t32_TST:
            THROW_UNDEFINED(TST);
        default:
            std::runtime_error("Undefined 32-bit instruction format");
    }
    return os;
}
