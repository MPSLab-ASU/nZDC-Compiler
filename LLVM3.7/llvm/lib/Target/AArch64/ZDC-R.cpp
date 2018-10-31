// //                     The LLVM Compiler Infrastructure //
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===--------------------------------------------------------------------===
//
//===--------------------------------------------------------------------===

#include "AArch64.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/LivePhysRegs.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineInstrBundle.h"
#include "llvm/CodeGen/MachineLoopInfo.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/RegAllocRegistry.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetInstrInfo.h"
#include "llvm/Target/TargetMachine.h"
#include <cassert>
#include <iostream>
#include <stdlib.h>

using namespace llvm;
static cl::opt<bool>
    nZDCisEnable("enable-nZDC", cl::init(false),
                 cl::desc("nZDC pass for soft errors detection."), cl::Hidden);

namespace llvm {

class masterRegisters {
public:
  int numMasterRegs = 14;
  int masterRegs[14] = {AArch64::X0,  AArch64::X1,  AArch64::X2,  AArch64::X3,
                        AArch64::X4,  AArch64::X5,  AArch64::X19, AArch64::X20,
                        AArch64::X23, AArch64::X24, AArch64::X28, AArch64::FP,
                        AArch64::LR,  AArch64::SP};
  int masterRegsW[14] = {AArch64::W0,  AArch64::W1,  AArch64::W2,  AArch64::W3,
                         AArch64::W4,  AArch64::W5,  AArch64::W19, AArch64::W20,
                         AArch64::W23, AArch64::W24, AArch64::W28, AArch64::W29,
                         AArch64::W30, AArch64::WSP};
  int masterRegsD[14] = {AArch64::D0,  AArch64::D1,  AArch64::D2,  AArch64::D3,
                         AArch64::D4,  AArch64::D5,  AArch64::D19, AArch64::D20,
                         AArch64::D23, AArch64::D24, AArch64::D28, AArch64::D29,
                         AArch64::D30, AArch64::D31};
  int masterRegsQ[14] = {AArch64::Q0,  AArch64::Q1,  AArch64::Q2,  AArch64::Q3,
                         AArch64::Q4,  AArch64::Q5,  AArch64::Q19, AArch64::Q20,
                         AArch64::Q23, AArch64::Q24, AArch64::Q28, AArch64::Q29,
                         AArch64::Q30, AArch64::Q31};
  int masterRegsH[14] = {AArch64::H0,  AArch64::H1,  AArch64::H2,  AArch64::H3,
                         AArch64::H4,  AArch64::H5,  AArch64::H19, AArch64::H20,
                         AArch64::H23, AArch64::H24, AArch64::H28, AArch64::H29,
                         AArch64::H30, AArch64::H31};
  int masterRegsS[14] = {AArch64::S0,  AArch64::S1,  AArch64::S2,  AArch64::S3,
                         AArch64::S4,  AArch64::S5,  AArch64::S19, AArch64::S20,
                         AArch64::S23, AArch64::S24, AArch64::S28, AArch64::S29,
                         AArch64::S30, AArch64::S31};
  int masterRegsB[14] = {AArch64::B0,  AArch64::B1,  AArch64::B2,  AArch64::B3,
                         AArch64::B4,  AArch64::B5,  AArch64::B19, AArch64::B20,
                         AArch64::B23, AArch64::B24, AArch64::B28, AArch64::B29,
                         AArch64::B30, AArch64::B31};
  bool ischecked[14];

  masterRegisters(int a) {
    numMasterRegs = a;
    for (int i = 0; i < numMasterRegs; i++)
      ischecked[i] = false;
  }
  void intRegischecked(int regNum) {
    // assert ( regNum < numMasterRegs);
    for (int idx = 0; idx < 14; idx++) {
      if (masterRegs[idx] == regNum)
        ischecked[idx] = true;
    }
  }
  void uncheckedAllIntRegs() {
    for (int i = 0; i < numMasterRegs; i++)
      ischecked[i] = false;
  }
};

masterRegisters ZDCmasterRegs = masterRegisters(14);
struct ZDCR : public MachineFunctionPass {
  // const	TargetMachine &TM;
  // const TargetInstrInfo *TII;
public:
  static char ID;
  // Pass identification, replacement for typeid
  // bool runOnMachineFunction(MachineFunction &F);
  //**********************************************************************
  // constructor
  //**********************************************************************

  ZDCR() : MachineFunctionPass(ID) {}

  //**********************************************************************
  // runOnMachineFunction
  //**********************************************************************

  bool isMasterReg(int reg) {
    for (int i = 0; i < ZDCmasterRegs.numMasterRegs; i++)
      if (ZDCmasterRegs.masterRegs[i] == reg ||
          ZDCmasterRegs.masterRegsW[i] == reg)
        return true;
    return false;
  }
  //
  int getTheSlave(unsigned Reg) {
    switch (Reg) {

    // X0 shadow is X6
    case AArch64::X0:
      return AArch64::X6;
    case AArch64::W0:
      return AArch64::W6;
      // X1 shadow is X7
    case AArch64::X1:
      return AArch64::X7;
    case AArch64::W1:
      return AArch64::W7;
      // 2 shadow is X7
    case AArch64::X2:
      return AArch64::X8;
    case AArch64::W2:
      return AArch64::W8;
      // 3 shadow is X9
    case AArch64::X3:
      return AArch64::X9;
    case AArch64::W3:
      return AArch64::W9;
      // 4 shadow is X10
    case AArch64::X4:
      return AArch64::X10;
    case AArch64::W4:
      return AArch64::W10;

    case AArch64::X5:
      return AArch64::X11;
    case AArch64::W5:
      return AArch64::W11;

    case AArch64::X19:
      return AArch64::X12;
    case AArch64::W19:
      return AArch64::W12;
    case AArch64::X20:
      return AArch64::X13;
    case AArch64::W20:
      return AArch64::W13;
      // X23 shadow is 14
    case AArch64::X23:
      return AArch64::X14;
    case AArch64::W23:
      return AArch64::W14;
      // X24 shadow is 15
    case AArch64::X24:
      return AArch64::X15;
    case AArch64::W24:
      return AArch64::W15;
      // x28 shadow is 16
    case AArch64::X28:
      return AArch64::X16;
    case AArch64::W28:
      return AArch64::W16;

      // FP shadow is 17
    case AArch64::FP:
      return AArch64::X17;
    case AArch64::W29:
      return AArch64::W17;
      // LR shadow is 18
    case AArch64::LR:
      return AArch64::X18;
    case AArch64::W30:
      return AArch64::W18;
      // SP shadow1 is 21
    case AArch64::SP:
      return AArch64::X21;
    case AArch64::WSP:
      return AArch64::W21;
      // x25/x26/x27/x28
    case AArch64::X26:
      return AArch64::X25;
    case AArch64::W26:
      return AArch64::W25;
      ///////////////////////////////////////////////Floating point

      // X0 shadow is X6
    case AArch64::Q0:
      return AArch64::Q6;
    case AArch64::D0:
      return AArch64::D6;
      // X1 shadow is X7
    case AArch64::Q1:
      return AArch64::Q7;
    case AArch64::D1:
      return AArch64::D7;
      // 2 shadow is X7
    case AArch64::Q2:
      return AArch64::Q8;
    case AArch64::D2:
      return AArch64::D8;
      // 3 shadow is X9
    case AArch64::Q3:
      return AArch64::Q9;
    case AArch64::D3:
      return AArch64::D9;
      // 4 shadow is X10
    case AArch64::Q4:
      return AArch64::Q10;
    case AArch64::D4:
      return AArch64::D10;

    case AArch64::Q5:
      return AArch64::Q11;
    case AArch64::D5:
      return AArch64::D11;

    case AArch64::Q19:
      return AArch64::Q12;
    case AArch64::D19:
      return AArch64::D12;
    case AArch64::Q20:
      return AArch64::Q13;
    case AArch64::D20:
      return AArch64::D13;

      // Q23 shadow is 14
    case AArch64::Q23:
      return AArch64::Q14;
    case AArch64::D23:
      return AArch64::D14;
      // Q24 shadow is 15
    case AArch64::Q24:
      return AArch64::Q15;
    case AArch64::D24:
      return AArch64::D15;
      // Q28 shadow is 16
    case AArch64::Q28:
      return AArch64::Q16;
    case AArch64::D28:
      return AArch64::D16;

      // Q29 shadow is 17
    case AArch64::Q29:
      return AArch64::Q17;
    case AArch64::D29:
      return AArch64::D17;
      // LR shadow is 18
    case AArch64::Q30:
      return AArch64::Q18;
    case AArch64::D30:
      return AArch64::D18;
      // Q31 shadow1 is 21
    case AArch64::Q31:
      return AArch64::Q21;
    case AArch64::D31:
      return AArch64::D21;

      // H0 shadow is 6
    case AArch64::H0:
      return AArch64::H6;
    case AArch64::B0:
      return AArch64::B6;
      // X1 shadow is X7
    case AArch64::H1:
      return AArch64::H7;
    case AArch64::B1:
      return AArch64::B7;
      // 2 shadow is X7
    case AArch64::H2:
      return AArch64::H8;
    case AArch64::B2:
      return AArch64::B8;
      // 3 shadow is X9
    case AArch64::H3:
      return AArch64::H9;
    case AArch64::B3:
      return AArch64::B9;
      // 4 shadow is X10
    case AArch64::H4:
      return AArch64::H10;
    case AArch64::B4:
      return AArch64::B10;

    case AArch64::H5:
      return AArch64::H11;
    case AArch64::B5:
      return AArch64::B11;

    case AArch64::H19:
      return AArch64::H12;
    case AArch64::B19:
      return AArch64::B12;
    case AArch64::H20:
      return AArch64::H13;
    case AArch64::B20:
      return AArch64::B13;

      // H23 shadow is 14
    case AArch64::H23:
      return AArch64::H14;
    case AArch64::B23:
      return AArch64::B14;
      // H24 shadow is 15
    case AArch64::H24:
      return AArch64::H15;
    case AArch64::B24:
      return AArch64::B15;
      // H28 shadow is 16
    case AArch64::H28:
      return AArch64::H16;
    case AArch64::B28:
      return AArch64::B16;

      // H29 shadow is 17
    case AArch64::H29:
      return AArch64::H17;
    case AArch64::B29:
      return AArch64::B17;
      // LR shadow is 18
    case AArch64::H30:
      return AArch64::H18;
    case AArch64::B30:
      return AArch64::B18;
      // H31 shadow1 is 21
    case AArch64::H31:
      return AArch64::H21;
    case AArch64::B31:
      return AArch64::B21;

      // H0 shadow is 6
    case AArch64::S0:
      return AArch64::S6;
      // X1 shadow is X7
    case AArch64::S1:
      return AArch64::S7;
      // 2 shadow is X7
    case AArch64::S2:
      return AArch64::S8;
      // 3 shadow is X9
    case AArch64::S3:
      return AArch64::S9;
      // 4 shadow is X10
    case AArch64::S4:
      return AArch64::S10;
    case AArch64::S5:
      return AArch64::S11;
    case AArch64::S19:
      return AArch64::S12;
    case AArch64::S20:
      return AArch64::S13;
      // H23 shadow is 14
    case AArch64::S23:
      return AArch64::S14;
      // H24 shadow is 15
    case AArch64::S24:
      return AArch64::S15;
      // H28 shadow is 16
    case AArch64::S28:
      return AArch64::S16;
      // H29 shadow is 17
    case AArch64::S29:
      return AArch64::S17;
      // LR shadow is 18
    case AArch64::S30:
      return AArch64::S18;
      // H31 shadow1 is 21
    case AArch64::S31:
      return AArch64::S21;

    default:
      return Reg;
    }
  }

  void duplicateInstructionsZDC(MachineFunction &MF) {
    const TargetMachine &TM = MF.getTarget();
    const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
    MachineRegisterInfo &MRI = MF.getRegInfo();
    const TargetRegisterInfo *RegInfo = MF.getSubtarget().getRegisterInfo();
    for (MachineFunction::iterator MBB = MF.begin(), MBE = MF.end(); MBB != MBE;
         ++MBB) {
      for (MachineBasicBlock::instr_iterator I = MBB->instr_begin(),
                                             E = MBB->instr_end();
           I != E; ++I) {
        if (!(I->isBranch()) && !(I->mayStore()) && !(I->isCall()) &&
            !(I->getOpcode() == AArch64::RET) && !(I->isCompare())) {
          bool duplicatable = true;
          MachineInstr *slaveinst = MF.CloneMachineInstr(I);
          for (int opcount = 0; opcount < I->getNumOperands(); opcount++) {
            if (I->getOperand(opcount).isReg()) {
              slaveinst->getOperand(opcount).setReg(
                  getTheSlave(I->getOperand(opcount).getReg())); // 20
            }
          }
          slaveinst->copyImplicitOps(MF, I);
          MachineBasicBlock::instr_iterator t1 = MBB->insert(I, slaveinst);
        }
      }
    }
  }

  void MasterRegsToSlaveRegsCall(MachineFunction &MF) {
    const TargetMachine &TM = MF.getTarget();
    const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
    MachineRegisterInfo &MRI = MF.getRegInfo();
    const TargetRegisterInfo *RegInfo = MF.getSubtarget().getRegisterInfo();

    int masterReg, slaveReg;
    MachineBasicBlock *MB = MF.begin();
    MachineBasicBlock::instr_iterator I = (MF.begin())->instr_begin();
    DebugLoc DL3 = I->getDebugLoc();
    MachineInstr *copyInst = NULL;
    MachineBasicBlock::iterator t;
    if (MF.getName() == "main")
      for (int i = 0; i < ZDCmasterRegs.numMasterRegs; i++) {
        masterReg = ZDCmasterRegs.masterRegs[i];
        slaveReg = getTheSlave(masterReg);
        copyInst = BuildMI(MF, DL3, TII->get(AArch64::SUBXrs), slaveReg)
                       .addReg(masterReg)
                       .addReg(AArch64::XZR)
                       .addImm(0);
        t = MB->insert(I, copyInst);
      }
    else {
      masterReg = ZDCmasterRegs.masterRegs[12];
      slaveReg = getTheSlave(masterReg);
      copyInst = BuildMI(MF, DL3, TII->get(AArch64::SUBXrs), slaveReg)
                     .addReg(masterReg)
                     .addReg(AArch64::XZR)
                     .addImm(0);
      t = MB->insert(I, copyInst);
    }
  }

  bool is32Reg(int Reg) {
    if (Reg == AArch64::W0 || Reg == AArch64::W1 || Reg == AArch64::W2 ||
        Reg == AArch64::W3 || Reg == AArch64::W4 || Reg == AArch64::W5 ||
        Reg == AArch64::W6 || Reg == AArch64::W7 || Reg == AArch64::W8 ||
        Reg == AArch64::W9 || Reg == AArch64::W10 || Reg == AArch64::W11 ||
        Reg == AArch64::W12 || Reg == AArch64::W13 || Reg == AArch64::W14 ||
        Reg == AArch64::W15 || Reg == AArch64::W16 || Reg == AArch64::W17 ||
        Reg == AArch64::W18 || Reg == AArch64::W19 || Reg == AArch64::W20 ||
        Reg == AArch64::W26 || Reg == AArch64::W25 || Reg == AArch64::W23 ||
        Reg == AArch64::W24 || Reg == AArch64::W25 || Reg == AArch64::W26 ||
        Reg == AArch64::W27 || Reg == AArch64::W28 || Reg == AArch64::W29 ||
        Reg == AArch64::W30 || Reg == AArch64::WSP)

      return true;
    return false;
  }
  bool is64Reg(int Reg) {
    if (Reg == AArch64::X0 || Reg == AArch64::X1 || Reg == AArch64::X2 ||
        Reg == AArch64::X3 || Reg == AArch64::X4 || Reg == AArch64::X5 ||
        Reg == AArch64::X6 || Reg == AArch64::X7 || Reg == AArch64::X8 ||
        Reg == AArch64::X9 || Reg == AArch64::X10 || Reg == AArch64::X11 ||
        Reg == AArch64::X12 || Reg == AArch64::X13 || Reg == AArch64::X14 ||
        Reg == AArch64::X15 || Reg == AArch64::X16 || Reg == AArch64::X17 ||
        Reg == AArch64::X18 || Reg == AArch64::X19 || Reg == AArch64::X20 ||
        Reg == AArch64::X21 || Reg == AArch64::X22 || Reg == AArch64::X23 ||
        Reg == AArch64::X24 || Reg == AArch64::X25 || Reg == AArch64::X26 ||
        Reg == AArch64::X27 || Reg == AArch64::X28 || Reg == AArch64::FP ||
        Reg == AArch64::LR || Reg == AArch64::SP)

      return true;
    return false;
  }
  bool isDReg(int Reg) {
    if (Reg == AArch64::D0 || Reg == AArch64::D1 || Reg == AArch64::D2 ||
        Reg == AArch64::D3 || Reg == AArch64::D4 || Reg == AArch64::D5 ||
        Reg == AArch64::D6 || Reg == AArch64::D7 || Reg == AArch64::D8 ||
        Reg == AArch64::D9 || Reg == AArch64::D10 || Reg == AArch64::D11 ||
        Reg == AArch64::D12 || Reg == AArch64::D13 || Reg == AArch64::D14 ||
        Reg == AArch64::D15 || Reg == AArch64::D16 || Reg == AArch64::D17 ||
        Reg == AArch64::D18 || Reg == AArch64::D19 || Reg == AArch64::D20 ||
        Reg == AArch64::D21 || Reg == AArch64::D22 || Reg == AArch64::D23 ||
        Reg == AArch64::D24 || Reg == AArch64::D25 || Reg == AArch64::D26 ||
        Reg == AArch64::D27 || Reg == AArch64::D28 || Reg == AArch64::D29 ||
        Reg == AArch64::D30 || Reg == AArch64::SP)

      return true;
    return false;
  }
  void copyMasterRegToSlaveReg(MachineFunction &MF,
                               MachineBasicBlock::instr_iterator I,
                               unsigned int opNum) {
    const TargetMachine &TM = MF.getTarget();
    const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
    MachineRegisterInfo &MRI = MF.getRegInfo();
    DebugLoc DL3 = (I->getParent())->findDebugLoc(I);
    int masterReg = I->getOperand(opNum).getReg();
    int slaveReg = getTheSlave(masterReg);
    if (slaveReg != masterReg && masterReg != AArch64::WSP &&
        masterReg != AArch64::SP) {
      if (is32Reg(masterReg)) {

        MachineInstrBuilder copyMRtoSR =
            llvm::BuildMI(MF, DL3, TII->get(AArch64::ORRWrs), slaveReg)
                .addReg(AArch64::WZR)
                .addReg(masterReg)
                .addImm(0);
        MachineBasicBlock::iterator t =
            (I->getParent())->insertAfter(I, copyMRtoSR);
      } else {
        MachineInstrBuilder copyMRtoSR =
            llvm::BuildMI(MF, DL3, TII->get(AArch64::ORRXrs), slaveReg)
                .addReg(AArch64::XZR)
                .addReg(masterReg)
                .addImm(0);

        MachineBasicBlock::iterator t =
            (I->getParent())->insertAfter(I, copyMRtoSR);
      }
    } else if (masterReg == AArch64::WSP || masterReg == AArch64::SP) {
      if (is32Reg(masterReg)) {
        MachineInstr *copySP =
            BuildMI(MF, DL3, TII->get(AArch64::SUBXrs), AArch64::W28)
                .addReg(AArch64::WSP)
                .addReg(AArch64::WZR)
                .addImm(0);
        MachineBasicBlock::iterator t =
            (I->getParent())->insertAfter(I, copySP);
      } else {
        MachineInstr *copySP =
            BuildMI(MF, DL3, TII->get(AArch64::SUBXrs), AArch64::X28)
                .addReg(AArch64::SP)
                .addReg(AArch64::XZR)
                .addImm(0);
        MachineBasicBlock::iterator t =
            (I->getParent())->insertAfter(I, copySP);
      }
    }
  }
  void copyMasterRegToSlaveRegs(MachineFunction &MF,
                                MachineBasicBlock::instr_iterator I,
                                unsigned int opNum) {
    const TargetMachine &TM = MF.getTarget();
    const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
    MachineRegisterInfo &MRI = MF.getRegInfo();
    DebugLoc DL3 = (I->getParent())->findDebugLoc(I);
    const TargetRegisterInfo *TRI = MF.getSubtarget().getRegisterInfo();
    const TargetRegisterClass *RC = TII->getRegClass(I->getDesc(), 0, TRI, MF);
    unsigned RegSize = TII->getRegClass(I->getDesc(), 0, TRI, MF)->getSize();
    int masterReg = I->getOperand(opNum).getReg();
    int slaveReg = getTheSlave(masterReg);

    if (slaveReg != masterReg && masterReg != AArch64::WSP &&
        masterReg != AArch64::SP) {
      if (is32Reg(masterReg)) {

        MachineInstrBuilder copyMRtoSR1 =
            llvm::BuildMI(MF, DL3, TII->get(AArch64::ORRWrs), slaveReg)
                .addReg(AArch64::WZR)
                .addReg(masterReg)
                .addImm(0);
        MachineBasicBlock::iterator t =
            (I->getParent())->insertAfter(I, copyMRtoSR1);

      } else if (is64Reg(masterReg)) {
        MachineInstrBuilder copyMRtoSR1 =
            llvm::BuildMI(MF, DL3, TII->get(AArch64::ORRXrs), slaveReg)
                .addReg(AArch64::XZR)
                .addReg(masterReg)
                .addImm(0);
        MachineBasicBlock::iterator t =
            (I->getParent())->insertAfter(I, copyMRtoSR1);

      }

      else if (RC->getID() == AArch64::FPR8RegClassID ||
               RC->getID() == AArch64::FPR16RegClassID ||
               RC->getID() == AArch64::FPR32RegClassID ||
               RC->getID() == AArch64::FPR32RegClassID ||
               RC->getID() == AArch64::FPR64RegClassID ||
               RC->getID() ==
                   AArch64::FPR128RegClassID) // assume it is d register
      {
        if (RC->getID() == AArch64::FPR128RegClassID) {
          masterReg -= 64; // convert Q to D  Q0 = 104, D0 = 40
          slaveReg = getTheSlave(masterReg);
        }

        MachineInstrBuilder copyMRtoSR1 =
            llvm::BuildMI(MF, DL3, TII->get(AArch64::FMOVDr))
                .addReg(slaveReg)
                .addReg(masterReg);
        MachineBasicBlock::iterator t =
            (I->getParent())->insertAfter(I, copyMRtoSR1);
      }

    } else if (masterReg == AArch64::WSP || masterReg == AArch64::SP) {
      if (is32Reg(masterReg)) {
        MachineInstr *copySP1 =
            BuildMI(MF, DL3, TII->get(AArch64::SUBXrs), slaveReg)
                .addReg(AArch64::WSP)
                .addReg(AArch64::WZR)
                .addImm(0);
        MachineBasicBlock::iterator t =
            (I->getParent())->insertAfter(I, copySP1);

      } else {
        MachineInstr *copySP1 =
            BuildMI(MF, DL3, TII->get(AArch64::SUBXrs), slaveReg)
                .addReg(AArch64::SP)
                .addReg(AArch64::XZR)
                .addImm(0);
        MachineBasicBlock::iterator t =
            (I->getParent())->insertAfter(I, copySP1);
      }
    }
  }

  void checkStoresnZDC(MachineFunction &MF, MachineBasicBlock *errorBB) {
    const TargetMachine &TM = MF.getTarget();
    const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
    const TargetRegisterInfo *TRI = MF.getSubtarget().getRegisterInfo();
    MachineRegisterInfo &MRI = MF.getRegInfo();
    for (MachineFunction::iterator MBB = MF.begin(), MBE = MF.end(); MBB != MBE;
         ++MBB)
      for (MachineBasicBlock::instr_iterator I = MBB->instr_begin(),
                                             E = MBB->instr_end();
           I != E; ++I) {
        if (I->mayStore()) {
          /*insert checking load instruction*/
          MachineInstr *loadInst = MF.CloneMachineInstr(I);
          for (int opcount = I->NumOperands - 2; opcount < I->getNumOperands();
               opcount++) {
            if (I->getOperand(opcount).isReg()) {
              loadInst->getOperand(opcount).setReg(
                  getTheSlave(I->getOperand(opcount).getReg()));
            }
          }
          loadInst->copyImplicitOps(MF, I);
          loadInst->setMemRefs(I->memoperands_begin(), I->memoperands_end());

          const MCInstrDesc &MCID =
              TII->get(covertSTopcodeToLoad(I->getOpcode()));
          loadInst->setDesc(MCID);
          if (loadInst->getOpcode() != I->getOpcode()) {
            MachineBasicBlock::iterator loadPos = MBB->insertAfter(I, loadInst);

            for (int opnum = 0; opnum < loadInst->getNumOperands(); opnum++) {
              if (loadInst->getOperand(opnum).isReg()) {
                int masterReg = loadInst->getOperand(opnum).getReg();
                int slaveReg = getTheSlave(masterReg);
                if (isMasterReg(masterReg) || masterReg == AArch64::XZR ||
                    masterReg == AArch64::WZR) {
                  DebugLoc DL3 = MBB->findDebugLoc(I);

                  int destReg = AArch64::X25;
                  if (is32Reg(masterReg))
                    destReg = AArch64::W25;
                  MachineInstrBuilder copyInst =
                      BuildMI(MF, DL3, TII->get(AArch64::SUBXrs), destReg)
                          .addReg(masterReg)
                          .addReg(slaveReg)
                          .addImm(0);
                  if (masterReg != AArch64::WZR && masterReg != AArch64::XZR) {
                    MachineBasicBlock::iterator cmpPOS =
                        MBB->insertAfter(loadPos, copyInst);
                    copyInst =
                        BuildMI(MF, DL3, TII->get(AArch64::CBNZX), destReg)
                            .addMBB(errorBB);
                    MachineBasicBlock::iterator brPOS =
                        MBB->insertAfter(cmpPOS, copyInst);
                  }
                  if (masterReg == AArch64::WZR || masterReg == AArch64::XZR) {
                    copyInst =
                        BuildMI(MF, DL3, TII->get(AArch64::CBNZX), AArch64::XZR)
                            .addMBB(errorBB);
                    MachineBasicBlock::iterator brPOS =
                        MBB->insertAfter(loadPos, copyInst);
                  }
                }
              }
            }
          }
        }
      }
  }

  char const *regs[228] = {
      "NoReg", "FP",  "LR",  "NZCV", "SP",  "WSP", "WZR", "XZR", "B0",  "B1",
      "B2",    "B3",  "B4",  "B5",   "B6",  "B7",  "B8",  "B9",  "B10", "B11",
      "B12",   "B13", "B14", "B15",  "B16", "B17", "B18", "B19", "B20", "B21",
      "B22",   "B23", "B24", "B25",  "B26", "B27", "B28", "B29", "B30", "B31",
      "D0",    "D1",  "D2",  "D3",   "D4",  "D5",  "D6",  "D7",  "D8",  "D9",
      "D10",   "D11", "D12", "D13",  "D14", "D15", "D16", "D17", "D18", "D19",
      "D20",   "D21", "D22", "D23",  "D24", "D25", "D26", "D27", "D28", "D29",
      "D30",   "D31", "H0",  "H1",   "H2",  "H3",  "H4",  "H5",  "H6",  "H7",
      "H8",    "H9",  "H10", "H11",  "H12", "H13", "H14", "H15", "H16", "H17",
      "H18",   "H19", "H20", "H21",  "H22", "H23", "H24", "H25", "H26", "H27",
      "H28",   "H29", "H30", "H31",  "Q0",  "Q1",  "Q2",  "Q3",  "Q4",  "Q5",
      "Q6",    "Q7",  "Q8",  "Q9",   "Q10", "Q11", "Q12", "Q13", "Q14", "Q15",
      "Q16",   "Q17", "Q18", "Q19",  "Q20", "Q21", "Q22", "Q23", "Q24", "Q25",
      "Q26",   "Q27", "Q28", "Q29",  "Q30", "Q31", "S0",  "S1",  "S2",  "S3",
      "S4",    "S5",  "S6",  "S7",   "S8",  "S9",  "S10", "S11", "S12", "S13",
      "S14",   "S15", "S16", "S17",  "S18", "S19", "S20", "S21", "S22", "S23",
      "S24",   "S25", "S26", "S27",  "S28", "S29", "S30", "S31", "W0",  "W1",
      "W2",    "W3",  "W4",  "W5",   "W6",  "W7",  "W8",  "W9",  "W10", "W11",
      "W12",   "W13", "W14", "W15",  "W16", "W17", "W18", "W19", "W20", "W21",
      "W22",   "W23", "W24", "W25",  "W26", "W27", "W28", "W29", "W30", "X0",
      "X1",    "X2",  "X3",  "X4",   "X5",  "X6",  "X7",  "X8",  "X9",  "X10",
      "X11",   "X12", "X13", "X14",  "X15", "X16", "X17", "X18", "X19", "X20",
      "X21",   "X22", "X23", "X24",  "X25", "X26", "X27", "X28"};

  enum regType {
    FPR8RegClassID,
    FPR16RegClassID,
    FPR32RegClassID,
    FPR64RegClassID,
    FPR128RegClassID,
    INT32RegClassID,
    INT64RegClassID
  };

  regType getRegClass(unsigned int reg) {
    regType regClass;
    if (reg >= AArch64::B0 && reg <= AArch64::B31)
      regClass = FPR8RegClassID;
    if (reg >= AArch64::H0 && reg <= AArch64::H31)
      regClass = FPR16RegClassID;
    if (reg >= AArch64::S0 && reg <= AArch64::S31)
      regClass = FPR32RegClassID;
    if (reg >= AArch64::D0 && reg <= AArch64::D31)
      regClass = FPR64RegClassID;
    if (reg >= AArch64::Q0 && reg <= AArch64::Q31)
      regClass = FPR128RegClassID;
    if ((reg >= AArch64::X0 && reg <= AArch64::X28) || reg == AArch64::LR ||
        reg == AArch64::FP || reg == AArch64::SP)
      return regClass = INT64RegClassID;
    if ((reg >= AArch64::W0 && reg <= AArch64::W30) || reg == AArch64::WSP ||
        reg == AArch64::WZR)
      return regClass = INT32RegClassID;
    errs() << "I can not determine the register class!!!!!!!!!!!!!!!!!!!!!\n";
    return regClass;
  }

  MachineBasicBlock *createErrorDetectionBB(MachineFunction &MF) {
    MachineBasicBlock *recoveryBB = MF.CreateMachineBasicBlock();
    MachineBasicBlock *OrigBB = MF.begin();
    OrigBB->addSuccessor(recoveryBB);
    MF.push_back(recoveryBB);
    const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();

    MachineInstr *copyInst = BuildMI(MF, (OrigBB->instr_begin())->getDebugLoc(),
                                     TII->get(AArch64::SUBXrs), AArch64::X25)
                                 .addReg(AArch64::X25)
                                 .addReg(AArch64::X25)
                                 .addImm(0);

    const char *FnName = "exit\0";
    recoveryBB->push_back(copyInst);

    // MachineInstr *restart = BuildMI(MF,
    // //(OrigBB->instr_begin())->getDebugLoc(),TII->get(AArch64::BL).addGlobalAddress(FnName);
    // MachineInstr *restart = BuildMI(MF,
    // (OrigBB->instr_begin())->getDebugLoc(),
    //                                 TII->get(AArch64::BL))
    //                             .addSym(FnName);

    return recoveryBB;
  }
  bool runOnMachineFunction(MachineFunction &MF) {

    if (nZDCisEnable) {
      duplicateInstructionsZDC(MF);
      MasterRegsToSlaveRegsCall(MF);
      MachineBasicBlock *errorBB = createErrorDetectionBB(MF);
      checkErrosBeforeFuncCall(MF, errorBB);
      checkStoresnZDC(MF, errorBB);
    }
    return true;
  }
  int numMasterRegs = 10;
  int masterRegs[10] = {AArch64::X0,  AArch64::X1,  AArch64::X2,  AArch64::X19,
                        AArch64::X20, AArch64::X23, AArch64::X24, AArch64::FP,
                        AArch64::LR,  AArch64::SP};

  void checkErrosBeforeFuncCall(MachineFunction &MF,
                                MachineBasicBlock *errorBB) {
    const TargetMachine &TM = MF.getTarget();
    const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
    MachineRegisterInfo &MRI = MF.getRegInfo();
    const TargetRegisterInfo *RegInfo = MF.getSubtarget().getRegisterInfo();

    int masterReg, slaveReg;
    for (MachineFunction::iterator MBB = MF.begin(), MBE = MF.end(); MBB != MBE;
         ++MBB) {
      for (MachineBasicBlock::instr_iterator I = MBB->instr_begin(),
                                             E = MBB->instr_end();
           I != E; ++I) {
        if (I->isCall()) {
          DebugLoc DL3 = I->getDebugLoc();
          for (int i = 0; i < ZDCmasterRegs.numMasterRegs; i++) {
            masterReg = ZDCmasterRegs.masterRegs[i];
            slaveReg = getTheSlave(masterReg);
            MachineInstrBuilder copyInst =
                BuildMI(MF, DL3, TII->get(AArch64::SUBXrs), AArch64::X25)
                    .addReg(masterReg)
                    .addReg(slaveReg)
                    .addImm(0);
            MachineBasicBlock::instr_iterator cmpPOS = MBB->insert(I, copyInst);
            copyInst = BuildMI(MF, DL3, TII->get(AArch64::CBNZX), AArch64::X25)
                           .addMBB(errorBB);
            MBB->insert(I, copyInst);
          }
          for (int i = 0; i < ZDCmasterRegs.numMasterRegs; i++) {
            masterReg = ZDCmasterRegs.masterRegs[i];
            slaveReg = getTheSlave(masterReg);
            MachineInstrBuilder copyInst =
                BuildMI(MF, DL3, TII->get(AArch64::SUBXrs), slaveReg)
                    .addReg(masterReg)
                    .addReg(AArch64::XZR)
                    .addImm(0);
            MachineBasicBlock::iterator pos = MBB->insertAfter(I, copyInst);
          }
        }
      }
    }
  }
  int covertSTopcodeToLoad(int stOpcode) {

    switch (stOpcode) {
    case AArch64::STRWui:
      return AArch64::LDRWui;
    case AArch64::STRXui:
      return AArch64::LDRXui;
    case AArch64::STRDui:
      return AArch64::LDRDui;
    case AArch64::STURXi:
      return AArch64::LDURXi;
    case AArch64::STURDi:
      return AArch64::LDURDi;
    case AArch64::STPXi:
      return AArch64::LDPXi;
    case AArch64::STPXpre:
      return AArch64::LDPXpre;
    case AArch64::STPXpost:
      return AArch64::LDPXpost;
    case AArch64::STRBBroX:
      return AArch64::LDRBBroX;
    case AArch64::STRBBui:
      return AArch64::LDRBBui;
    case AArch64::STRBBpost:
      return AArch64::LDRBBpost;
    case AArch64::STRWpost:
      return AArch64::LDRWpost;
    case AArch64::STRQui:
      return AArch64::LDRQui;
    case AArch64::STPWi:
      return AArch64::LDPWi;
    case AArch64::STURWi:
      return AArch64::LDURWi;
    case AArch64::STRBBpre:
      return AArch64::LDRBBpre;
    case AArch64::STURBBi:
      return AArch64::LDURBBi;
    default:
      return stOpcode;
    }
  }

  int getTheSlave(unsigned Reg, bool test) {
    switch (Reg) {
    // SP shadow is 28
    case AArch64::SP:
      return AArch64::X28;
    case AArch64::WSP:
      return AArch64::W28;
      // 0 shadow is 8
    case AArch64::X0:
      return AArch64::X8;
    case AArch64::W0:
      return AArch64::W8;
      // 1 shadow is 9
    case AArch64::X1:
      return AArch64::X9;
    case AArch64::W1:
      return AArch64::W9;
      // 2 shadow is 10
    case AArch64::X2:
      return AArch64::X10;
    case AArch64::W2:
      return AArch64::W10;
      // 3 shadow is 11
    case AArch64::X3:
      return AArch64::X11;
    case AArch64::W3:
      return AArch64::W11;
      // 4 shadow is 12
    case AArch64::X4:
      return AArch64::X12;
    case AArch64::W4:
      return AArch64::W12;
      // 5 shadow is 13
    case AArch64::X5:
      return AArch64::X13;
    case AArch64::W5:
      return AArch64::W13;
      // 6 shadow is 14
    case AArch64::X6:
      return AArch64::X14;
    case AArch64::W6:
      return AArch64::W14;
    case AArch64::X19:
      return AArch64::X16;
    case AArch64::W19:
      return AArch64::W16;
      // 20 shadow is 7
    case AArch64::X20:
      return AArch64::X7;
    case AArch64::W20:
      return AArch64::W7;
      ////////////////////////////////////////////////////////////
    case AArch64::X21:
      return AArch64::X22;
    case AArch64::W26:
      return AArch64::W25;
    case AArch64::X22:
      return AArch64::X21;
    case AArch64::W25:
      return AArch64::W26;
      //////////////////////////////////////////////////////////
      // 23 shahow is 25
    case AArch64::X23:
      return AArch64::X25;
    case AArch64::W23:
      return AArch64::W25;
      // 24 shahow is 26
    case AArch64::X24:
      return AArch64::X26;
    case AArch64::W24:
      return AArch64::W26;
      // FP shadow is 27
    case AArch64::FP:
      return AArch64::X27;
    case AArch64::W29:
      return AArch64::W27;
      // LR shadow is 28
    case AArch64::LR:
      return AArch64::X17;
    case AArch64::W30:
      return AArch64::W17;

      //////////////////////////////////floating point registers
    case AArch64::S0:
      return AArch64::S8;
    case AArch64::H0:
      return AArch64::H8;
      // 1 shadow is 9
    case AArch64::S1:
      return AArch64::S9;
    case AArch64::H1:
      return AArch64::H9;
      // 2 shadow is 10
    case AArch64::S2:
      return AArch64::S10;
    case AArch64::H2:
      return AArch64::H10;
      // 3 shadow is 11
    case AArch64::S3:
      return AArch64::S11;
    case AArch64::H3:
      return AArch64::H11;
      // 4 shadow is 12
    case AArch64::S4:
      return AArch64::S12;
    case AArch64::H4:
      return AArch64::H12;
      // 5 shadow is 13
    case AArch64::S5:
      return AArch64::S13;
    case AArch64::H5:
      return AArch64::H13;
      // 6 shadow is 14
    case AArch64::S6:
      return AArch64::S14;
    case AArch64::H6:
      return AArch64::H14;
    case AArch64::S7:
      return AArch64::S15;
    case AArch64::H7:
      return AArch64::H15;

    case AArch64::D0:
      return AArch64::D8;
    case AArch64::Q0:
      return AArch64::Q8;
      // 1 shadow is 9
    case AArch64::D1:
      return AArch64::D9;
    case AArch64::Q1:
      return AArch64::Q9;
      // 2 shadow is 10
    case AArch64::D2:
      return AArch64::D10;
    case AArch64::Q2:
      return AArch64::Q10;
      // 3 shadow is 11
    case AArch64::D3:
      return AArch64::D11;
    case AArch64::Q3:
      return AArch64::Q11;
      // 4 shadow is 12
    case AArch64::D4:
      return AArch64::D12;
    case AArch64::Q4:
      return AArch64::Q12;
      // 5 shadow is 13
    case AArch64::D5:
      return AArch64::D13;
    case AArch64::Q5:
      return AArch64::Q13;
      // 6 shadow is 14
    case AArch64::D6:
      return AArch64::D14;
    case AArch64::Q6:
      return AArch64::Q14;
    case AArch64::D7:
      return AArch64::D15;
    case AArch64::Q7:
      return AArch64::Q15;
      ///////////////////////////////////////

    default:
      return Reg;
    }
  }
}; // namespace llvm
char ZDCR::ID = 0;
FunctionPass *createZDCR() { return new ZDCR(); }
} // namespace llvm
