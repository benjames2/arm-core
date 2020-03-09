#include <inc/decode_structure.h>
#include <sstream>

#define THROW_INVALID_METACODE(opcode) throw std::runtime_error("opcode(" #opcode ") : invalid meta opcode")

instruction_t::instruction_t(void) {
    this->opcode = -1;

    this->meta_opcode = -1; // some opcodes get shared

    this->PC = 0x00;
    this->i_immediate = 0x00;
    this->u_immediate = 0x00;

    this->Rs = -1;
    this->Rd = -1;
    this->Rn = -1;
    this->Rb = -1;
    this->Ro = -1;

    //this->Hs = -1;
    //this->Hd = -1;
    this->Rlist = -1;
}

std::string instruction_t::str(void) {
    std::stringstream ss;
    ss << *this << std::flush;
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, instruction_t& in) {

    // maintain a single instruction for BL
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
                        //throw std::runtime_error("opcode(ASR) : invalid meta opcode");
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
                case 0:  os << "Beq "; break;
                case 1:  os << "Bne "; break;
                case 2:  os << "Bcs "; break;
                case 3:  os << "Bcc "; break;
                case 4:  os << "Bmi "; break;
                case 5:  os << "Bpl "; break;
                case 6:  os << "Bvs "; break;
                case 7:  os << "Bvc "; break;
                case 8:  os << "Bhi "; break;
                case 9:  os << "Bls "; break;
                case 10: os << "Bge "; break;
                case 11: os << "Blt "; break;
                case 12: os << "Bgt "; break;
                case 13: os << "Ble "; break;
                default:
                    throw std::runtime_error("opcode(Bxx) : invalid condition code");
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

            switch(in.i_immediate){
                case meta_C:                        break; // do nothing for this case
                case meta_C_pc: os << "PC "; break;
                default:
                    THROW_INVALID_METACODE(POP);
                    //throw std::runtime_error("opcode(POP) : invalid meta opcode");
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

            switch(in.i_immediate){
                case meta_C:                 break; // do nothing for this case
                case meta_C_pc: os << "LR "; break;
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
                case meta_RRR: os << 'r' << in.Rd << ", r" << in.Rs << ", r" << in.Rn;          break;
                case meta_RRC: os << 'r' << in.Rd << ", r" << in.Rs << ", #"<< in.u_immediate; break;   
                case meta_RC:  os << 'r' << in.Rd << ", #"<< in.u_immediate;                    break;     
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