//===- AArch64RegisterInfo.cpp - AArch64 Register Information -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the AArch64 implementation of the TargetRegisterInfo
// class.
//
//===----------------------------------------------------------------------===//

#include "AArch64RegisterInfo.h"
#include "AArch64FrameLowering.h"
#include "AArch64InstrInfo.h"
#include "AArch64Subtarget.h"
#include "MCTargetDesc/AArch64AddressingModes.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/Triple.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetFrameLowering.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "AArch64GenRegisterInfo.inc"

static cl::opt<bool>
ReserveX18("aarch64-reserve-x18", cl::Hidden,
          cl::desc("Reserve X18, making it unavailable as GPR"));

static cl::opt<bool>
ReserveX3("aarch64-reserve-x3", cl::Hidden,
          cl::desc("Reserve X3, making it unavailable as GPR"));

static cl::opt<bool>
ReserveRegisters(
                "reserveRegs",
                cl::init(false),
                cl::desc("reserve 19 registers for ZDC+Recovery."),
                cl::Hidden);
static cl::opt<bool>
ReserveRegforSWIFTR(
                "reserveRegs-SWIFTR",
                cl::init(false),
                cl::desc("reserve 19 registers for ZDC forward recovery."),
                cl::Hidden);

AArch64RegisterInfo::AArch64RegisterInfo(const Triple &TT)
    : AArch64GenRegisterInfo(AArch64::LR), TT(TT) {}

const MCPhysReg *
AArch64RegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  assert(MF && "Invalid MachineFunction pointer.");
  if (MF->getFunction()->getCallingConv() == CallingConv::GHC)
    // GHC set of callee saved regs is empty as all those regs are
    // used for passing STG regs around
    return CSR_AArch64_NoRegs_SaveList;
  if (MF->getFunction()->getCallingConv() == CallingConv::AnyReg)
    return CSR_AArch64_AllRegs_SaveList;
  else
    return CSR_AArch64_AAPCS_SaveList;
}

const uint32_t *
AArch64RegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                          CallingConv::ID CC) const {
  if (CC == CallingConv::GHC)
    // This is academic becase all GHC calls are (supposed to be) tail calls
    return CSR_AArch64_NoRegs_RegMask;
  if (CC == CallingConv::AnyReg)
    return CSR_AArch64_AllRegs_RegMask;
  else
    return CSR_AArch64_AAPCS_RegMask;
}

const uint32_t *AArch64RegisterInfo::getTLSCallPreservedMask() const {
  if (TT.isOSDarwin())
    return CSR_AArch64_TLS_Darwin_RegMask;

  assert(TT.isOSBinFormatELF() && "only expect Darwin or ELF TLS");
  return CSR_AArch64_TLS_ELF_RegMask;
}

const uint32_t *
AArch64RegisterInfo::getThisReturnPreservedMask(const MachineFunction &MF,
                                                CallingConv::ID CC) const {
  // This should return a register mask that is the same as that returned by
  // getCallPreservedMask but that additionally preserves the register used for
  // the first i64 argument (which must also be the register used to return a
  // single i64 return value)
  //
  // In case that the calling convention does not use the same register for
  // both, the function should return NULL (does not currently apply)
  assert(CC != CallingConv::GHC && "should not be GHC calling convention.");
  return CSR_AArch64_AAPCS_ThisReturn_RegMask;
}

BitVector
AArch64RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  const TargetFrameLowering *TFI = MF.getSubtarget().getFrameLowering();

  // FIXME: avoid re-calculating this every time.
  BitVector Reserved(getNumRegs());
  Reserved.set(AArch64::SP);
  Reserved.set(AArch64::XZR);
  Reserved.set(AArch64::WSP);
  Reserved.set(AArch64::WZR);

  if (TFI->hasFP(MF) || TT.isOSDarwin()) {
    Reserved.set(AArch64::FP);
    Reserved.set(AArch64::W29);
  }

  if (TT.isOSDarwin() || ReserveX18) {
    Reserved.set(AArch64::X18); // Platform register
    Reserved.set(AArch64::W18);
  }

  if (ReserveX3) {
    Reserved.set(AArch64::X3); // Platform register
    Reserved.set(AArch64::W3);
  }
  if (hasBasePointer(MF)) {
    Reserved.set(AArch64::X19);
    Reserved.set(AArch64::W19);
  }
if (ReserveRegforSWIFTR)
{

Reserved.set(AArch64::X6);
Reserved.set(AArch64::W6);
Reserved.set(AArch64::B6);
Reserved.set(AArch64::S6);
Reserved.set(AArch64::H6);
Reserved.set(AArch64::D6);
Reserved.set(AArch64::Q6);

Reserved.set(AArch64::X5);
Reserved.set(AArch64::W5);
Reserved.set(AArch64::B5);
Reserved.set(AArch64::S5);
Reserved.set(AArch64::H5);
Reserved.set(AArch64::D5);
Reserved.set(AArch64::Q5);

Reserved.set(AArch64::X4);
Reserved.set(AArch64::W4);
Reserved.set(AArch64::B4);
Reserved.set(AArch64::S4);
Reserved.set(AArch64::H4);
Reserved.set(AArch64::D4);
Reserved.set(AArch64::Q4);

Reserved.set(AArch64::X3);
Reserved.set(AArch64::W3);
Reserved.set(AArch64::B3);
Reserved.set(AArch64::S3);
Reserved.set(AArch64::H3);
Reserved.set(AArch64::D3);
Reserved.set(AArch64::Q3);

    Reserved.set(AArch64::X7);  //X7
    Reserved.set(AArch64::X8); // X0
    Reserved.set(AArch64::X9); //X1
    Reserved.set(AArch64::X10); //X2
    Reserved.set(AArch64::X11);  //X3
    Reserved.set(AArch64::X12);  //X4
    Reserved.set(AArch64::X13); //X5
    Reserved.set(AArch64::X14);  //X6
    Reserved.set(AArch64::X15);  //X7
    Reserved.set(AArch64::X16);  //X19
    Reserved.set(AArch64::X17);   //LR
    Reserved.set(AArch64::X18);   //CF
    Reserved.set(AArch64::X21); //CF
    Reserved.set(AArch64::X22);  //CF
    Reserved.set(AArch64::X25);  //23
    Reserved.set(AArch64::X26);  //24
    Reserved.set(AArch64::X27); //FP
    Reserved.set(AArch64::X28);  //SP
//////////////////////////////////////////////////////////////
    Reserved.set(AArch64::W7);  
    Reserved.set(AArch64::W8);
    Reserved.set(AArch64::W9);
    Reserved.set(AArch64::W10);
    Reserved.set(AArch64::W11);
    Reserved.set(AArch64::W12);
    Reserved.set(AArch64::W13);
    Reserved.set(AArch64::W14);
    Reserved.set(AArch64::W15);
    Reserved.set(AArch64::W16);
    Reserved.set(AArch64::W17);
    Reserved.set(AArch64::W18);
    Reserved.set(AArch64::W21);
    Reserved.set(AArch64::W22);
    Reserved.set(AArch64::W25);
    Reserved.set(AArch64::W26);
    Reserved.set(AArch64::W27);
    Reserved.set(AArch64::W28);
/////////////////////////////////////////////////////////////fLOAT
    Reserved.set(AArch64::B7);
    Reserved.set(AArch64::B8);
    Reserved.set(AArch64::B9);
    Reserved.set(AArch64::B10);
    Reserved.set(AArch64::B11);
    Reserved.set(AArch64::B12);
    Reserved.set(AArch64::B13);
    Reserved.set(AArch64::B14);
    Reserved.set(AArch64::B15);
    Reserved.set(AArch64::B16);
    Reserved.set(AArch64::B17); 
    Reserved.set(AArch64::B18);
    Reserved.set(AArch64::B19);
    Reserved.set(AArch64::B20);
    Reserved.set(AArch64::B21);
    Reserved.set(AArch64::B22);
    Reserved.set(AArch64::B25);
    Reserved.set(AArch64::B26);
    Reserved.set(AArch64::B27);
    Reserved.set(AArch64::B28);



    Reserved.set(AArch64::H7);
    Reserved.set(AArch64::H8);
    Reserved.set(AArch64::H9);
    Reserved.set(AArch64::H10);
    Reserved.set(AArch64::H11);
    Reserved.set(AArch64::H12);
    Reserved.set(AArch64::H13);
    Reserved.set(AArch64::H14);
    Reserved.set(AArch64::H15);
    Reserved.set(AArch64::H16);
    Reserved.set(AArch64::H17); 
    Reserved.set(AArch64::H18);
    Reserved.set(AArch64::H19);
    Reserved.set(AArch64::H20);
    Reserved.set(AArch64::H21);
    Reserved.set(AArch64::H22);
    Reserved.set(AArch64::H25);
    Reserved.set(AArch64::H26);
    Reserved.set(AArch64::H27);
    Reserved.set(AArch64::H28);

    Reserved.set(AArch64::S7);
    Reserved.set(AArch64::S8);
    Reserved.set(AArch64::S9);
    Reserved.set(AArch64::S10);
    Reserved.set(AArch64::S11);
    Reserved.set(AArch64::S12);
    Reserved.set(AArch64::S13);
    Reserved.set(AArch64::S14);
    Reserved.set(AArch64::S15);
    Reserved.set(AArch64::S16);
    Reserved.set(AArch64::S17); 
    Reserved.set(AArch64::S18);
    Reserved.set(AArch64::S19);
    Reserved.set(AArch64::S20);
    Reserved.set(AArch64::S21);
    Reserved.set(AArch64::S22);
    Reserved.set(AArch64::S25);
    Reserved.set(AArch64::S26);
    Reserved.set(AArch64::S27);
    Reserved.set(AArch64::S28);
  


    Reserved.set(AArch64::D7);
    Reserved.set(AArch64::D8);
    Reserved.set(AArch64::D9);
    Reserved.set(AArch64::D10);
    Reserved.set(AArch64::D11);
    Reserved.set(AArch64::D12);
    Reserved.set(AArch64::D13);
    Reserved.set(AArch64::D14);
    Reserved.set(AArch64::D15);
    Reserved.set(AArch64::D16);
    Reserved.set(AArch64::D17); 
    Reserved.set(AArch64::D18);
    Reserved.set(AArch64::D19);
    Reserved.set(AArch64::D20);
    Reserved.set(AArch64::D21);
    Reserved.set(AArch64::D22);
    Reserved.set(AArch64::D25);
    Reserved.set(AArch64::D26);
    Reserved.set(AArch64::D27);
    Reserved.set(AArch64::D28);

    Reserved.set(AArch64::Q7);
    Reserved.set(AArch64::Q8);
    Reserved.set(AArch64::Q9);
    Reserved.set(AArch64::Q10);
    Reserved.set(AArch64::Q11);
    Reserved.set(AArch64::Q12);
    Reserved.set(AArch64::Q13);
    Reserved.set(AArch64::Q14);
    Reserved.set(AArch64::Q15);
    Reserved.set(AArch64::Q16);
    Reserved.set(AArch64::Q17); 
    Reserved.set(AArch64::Q18);
    Reserved.set(AArch64::Q19);
    Reserved.set(AArch64::Q20);
    Reserved.set(AArch64::Q21);
    Reserved.set(AArch64::Q22);
    Reserved.set(AArch64::Q25);
    Reserved.set(AArch64::Q26);
    Reserved.set(AArch64::Q27);
    Reserved.set(AArch64::Q28);
}
//moslem for reserving registers
if (ReserveRegisters){

Reserved.set(AArch64::X6);
Reserved.set(AArch64::W6);
Reserved.set(AArch64::B6);
Reserved.set(AArch64::S6);
Reserved.set(AArch64::H6);
Reserved.set(AArch64::D6);
Reserved.set(AArch64::Q6);

    Reserved.set(AArch64::X7);  //X7
    Reserved.set(AArch64::X8); // X0
    Reserved.set(AArch64::X9); //X1
    Reserved.set(AArch64::X10); //X2
    Reserved.set(AArch64::X11);  //X3
    Reserved.set(AArch64::X12);  //X4
    Reserved.set(AArch64::X13); //X5
    Reserved.set(AArch64::X14);  //X6
    Reserved.set(AArch64::X15);  //X7
    Reserved.set(AArch64::X16);  //X19
    Reserved.set(AArch64::X17);   //LR
    Reserved.set(AArch64::X18);   //CF
    Reserved.set(AArch64::X21); //CF
    Reserved.set(AArch64::X22);  //CF
    Reserved.set(AArch64::X25);  //23
    Reserved.set(AArch64::X26);  //24
    Reserved.set(AArch64::X27); //FP
    Reserved.set(AArch64::X28);  //SP
//////////////////////////////////////////////////////////////
    Reserved.set(AArch64::W7);  
    Reserved.set(AArch64::W8);
    Reserved.set(AArch64::W9);
    Reserved.set(AArch64::W10);
    Reserved.set(AArch64::W11);
    Reserved.set(AArch64::W12);
    Reserved.set(AArch64::W13);
    Reserved.set(AArch64::W14);
    Reserved.set(AArch64::W15);
    Reserved.set(AArch64::W16);
    Reserved.set(AArch64::W17);
    Reserved.set(AArch64::W18);
    Reserved.set(AArch64::W21);
    Reserved.set(AArch64::W22);
    Reserved.set(AArch64::W25);
    Reserved.set(AArch64::W26);
    Reserved.set(AArch64::W27);
    Reserved.set(AArch64::W28);
/////////////////////////////////////////////////////////////fLOAT
    Reserved.set(AArch64::B7);
    Reserved.set(AArch64::B8);
    Reserved.set(AArch64::B9);
    Reserved.set(AArch64::B10);
    Reserved.set(AArch64::B11);
    Reserved.set(AArch64::B12);
    Reserved.set(AArch64::B13);
    Reserved.set(AArch64::B14);
    Reserved.set(AArch64::B15);
    Reserved.set(AArch64::B16);
    Reserved.set(AArch64::B17); 
    Reserved.set(AArch64::B18);
    Reserved.set(AArch64::B19);
    Reserved.set(AArch64::B20);
    Reserved.set(AArch64::B21);
    Reserved.set(AArch64::B22);
    Reserved.set(AArch64::B25);
    Reserved.set(AArch64::B26);
    Reserved.set(AArch64::B27);
    Reserved.set(AArch64::B28);



    Reserved.set(AArch64::H7);
    Reserved.set(AArch64::H8);
    Reserved.set(AArch64::H9);
    Reserved.set(AArch64::H10);
    Reserved.set(AArch64::H11);
    Reserved.set(AArch64::H12);
    Reserved.set(AArch64::H13);
    Reserved.set(AArch64::H14);
    Reserved.set(AArch64::H15);
    Reserved.set(AArch64::H16);
    Reserved.set(AArch64::H17); 
    Reserved.set(AArch64::H18);
    Reserved.set(AArch64::H19);
    Reserved.set(AArch64::H20);
    Reserved.set(AArch64::H21);
    Reserved.set(AArch64::H22);
    Reserved.set(AArch64::H25);
    Reserved.set(AArch64::H26);
    Reserved.set(AArch64::H27);
    Reserved.set(AArch64::H28);

    Reserved.set(AArch64::S7);
    Reserved.set(AArch64::S8);
    Reserved.set(AArch64::S9);
    Reserved.set(AArch64::S10);
    Reserved.set(AArch64::S11);
    Reserved.set(AArch64::S12);
    Reserved.set(AArch64::S13);
    Reserved.set(AArch64::S14);
    Reserved.set(AArch64::S15);
    Reserved.set(AArch64::S16);
    Reserved.set(AArch64::S17); 
    Reserved.set(AArch64::S18);
    Reserved.set(AArch64::S19);
    Reserved.set(AArch64::S20);
    Reserved.set(AArch64::S21);
    Reserved.set(AArch64::S22);
    Reserved.set(AArch64::S25);
    Reserved.set(AArch64::S26);
    Reserved.set(AArch64::S27);
    Reserved.set(AArch64::S28);
  


    Reserved.set(AArch64::D7);
    Reserved.set(AArch64::D8);
    Reserved.set(AArch64::D9);
    Reserved.set(AArch64::D10);
    Reserved.set(AArch64::D11);
    Reserved.set(AArch64::D12);
    Reserved.set(AArch64::D13);
    Reserved.set(AArch64::D14);
    Reserved.set(AArch64::D15);
    Reserved.set(AArch64::D16);
    Reserved.set(AArch64::D17); 
    Reserved.set(AArch64::D18);
    Reserved.set(AArch64::D19);
    Reserved.set(AArch64::D20);
    Reserved.set(AArch64::D21);
    Reserved.set(AArch64::D22);
    Reserved.set(AArch64::D25);
    Reserved.set(AArch64::D26);
    Reserved.set(AArch64::D27);
    Reserved.set(AArch64::D28);

    Reserved.set(AArch64::Q7);
    Reserved.set(AArch64::Q8);
    Reserved.set(AArch64::Q9);
    Reserved.set(AArch64::Q10);
    Reserved.set(AArch64::Q11);
    Reserved.set(AArch64::Q12);
    Reserved.set(AArch64::Q13);
    Reserved.set(AArch64::Q14);
    Reserved.set(AArch64::Q15);
    Reserved.set(AArch64::Q16);
    Reserved.set(AArch64::Q17); 
    Reserved.set(AArch64::Q18);
    Reserved.set(AArch64::Q19);
    Reserved.set(AArch64::Q20);
    Reserved.set(AArch64::Q21);
    Reserved.set(AArch64::Q22);
    Reserved.set(AArch64::Q25);
    Reserved.set(AArch64::Q26);
    Reserved.set(AArch64::Q27);
    Reserved.set(AArch64::Q28);

}

  return Reserved;
}

bool AArch64RegisterInfo::isReservedReg(const MachineFunction &MF,
                                      unsigned Reg) const {
  const TargetFrameLowering *TFI = MF.getSubtarget().getFrameLowering();

  switch (Reg) {
  default:
    break;
  case AArch64::SP:
  case AArch64::XZR:
  case AArch64::WSP:
  case AArch64::WZR:
    return true;
//moslem
  case AArch64::X3:
  case AArch64::W3:
    return ReserveX3;

//
  case AArch64::X18:
  case AArch64::W18:
    return TT.isOSDarwin() || ReserveX18;
  case AArch64::FP:
  case AArch64::W29:
    return TFI->hasFP(MF) || TT.isOSDarwin();
  case AArch64::W19:
  case AArch64::X19:
    return hasBasePointer(MF);
  case AArch64::X6:
  case AArch64::W6:
case AArch64::X7: //X7
case AArch64::X8: // X0
case AArch64::X9: //X1
case AArch64::X10: //X2
case AArch64::X11: //X3
case AArch64::X12: //X4
case AArch64::X13: //X5
case AArch64::X14: //X6
case AArch64::X15: //X7
case AArch64::X16: //X19
case AArch64::X17:   //LR

case AArch64::X21: //CF
case AArch64::X22: //CF
case AArch64::X25: //23
case AArch64::X26: //24
case AArch64::X27: //FP
case AArch64::X28: //FP
case AArch64::W7: //X7
case AArch64::W8: // X0
case AArch64::W9: //X1
case AArch64::W10: //X2
case AArch64::W11: //X3
case AArch64::W12: //X4
case AArch64::W13: //X5
case AArch64::W14: //X6
case AArch64::W15: //X7
case AArch64::W16: //X19
case AArch64::W17:   //LR
case AArch64::W21: //CF
case AArch64::W22: //CF
case AArch64::W25: //23
case AArch64::W26: //24
case AArch64::W27: //FP
case AArch64::W28: //FP

return ReserveRegisters;
  
//moslem



//////////////
  }

  return false;
}

const TargetRegisterClass *
AArch64RegisterInfo::getPointerRegClass(const MachineFunction &MF,
                                      unsigned Kind) const {
  return &AArch64::GPR64RegClass;
}

const TargetRegisterClass *
AArch64RegisterInfo::getCrossCopyRegClass(const TargetRegisterClass *RC) const {
  if (RC == &AArch64::CCRRegClass)
    return &AArch64::GPR64RegClass; // Only MSR & MRS copy NZCV.
  return RC;
}

unsigned AArch64RegisterInfo::getBaseRegister() const { return AArch64::X19; }

bool AArch64RegisterInfo::hasBasePointer(const MachineFunction &MF) const {
  const MachineFrameInfo *MFI = MF.getFrameInfo();

  // In the presence of variable sized objects, if the fixed stack size is
  // large enough that referencing from the FP won't result in things being
  // in range relatively often, we can use a base pointer to allow access
  // from the other direction like the SP normally works.
  // Furthermore, if both variable sized objects are present, and the
  // stack needs to be dynamically re-aligned, the base pointer is the only
  // reliable way to reference the locals.
  if (MFI->hasVarSizedObjects()) {
    if (needsStackRealignment(MF))
      return true;
    // Conservatively estimate whether the negative offset from the frame
    // pointer will be sufficient to reach. If a function has a smallish
    // frame, it's less likely to have lots of spills and callee saved
    // space, so it's all more likely to be within range of the frame pointer.
    // If it's wrong, we'll materialize the constant and still get to the
    // object; it's just suboptimal. Negative offsets use the unscaled
    // load/store instructions, which have a 9-bit signed immediate.
    if (MFI->getLocalFrameSize() < 256)
      return false;
    return true;
  }

  return false;
}

bool AArch64RegisterInfo::canRealignStack(const MachineFunction &MF) const {

  if (MF.getFunction()->hasFnAttribute("no-realign-stack"))
    return false;

  return true;
}

// FIXME: share this with other backends with identical implementation?
bool
AArch64RegisterInfo::needsStackRealignment(const MachineFunction &MF) const {
  const MachineFrameInfo *MFI = MF.getFrameInfo();
  const Function *F = MF.getFunction();
  unsigned StackAlign = MF.getTarget()
                            .getSubtargetImpl(*MF.getFunction())
                            ->getFrameLowering()
                            ->getStackAlignment();
  bool requiresRealignment =
      ((MFI->getMaxAlignment() > StackAlign) ||
       F->getAttributes().hasAttribute(AttributeSet::FunctionIndex,
                                       Attribute::StackAlignment));

  return requiresRealignment && canRealignStack(MF);
}

unsigned
AArch64RegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  const TargetFrameLowering *TFI = MF.getSubtarget().getFrameLowering();

  return TFI->hasFP(MF) ? AArch64::FP : AArch64::SP;
}

bool AArch64RegisterInfo::requiresRegisterScavenging(
    const MachineFunction &MF) const {
  return true;
}

bool AArch64RegisterInfo::requiresVirtualBaseRegisters(
    const MachineFunction &MF) const {
  return true;
}

bool
AArch64RegisterInfo::useFPForScavengingIndex(const MachineFunction &MF) const {
  const MachineFrameInfo *MFI = MF.getFrameInfo();
  // AArch64FrameLowering::resolveFrameIndexReference() can always fall back
  // to the stack pointer, so only put the emergency spill slot next to the
  // FP when there's no better way to access it (SP or base pointer).
  return MFI->hasVarSizedObjects() && !hasBasePointer(MF);
}

bool AArch64RegisterInfo::requiresFrameIndexScavenging(
    const MachineFunction &MF) const {
  return true;
}

bool
AArch64RegisterInfo::cannotEliminateFrame(const MachineFunction &MF) const {
  const MachineFrameInfo *MFI = MF.getFrameInfo();
  // Only consider eliminating leaf frames.
  if (MFI->hasCalls() || (MF.getTarget().Options.DisableFramePointerElim(MF) &&
                          MFI->adjustsStack()))
    return true;
  return MFI->hasVarSizedObjects() || MFI->isFrameAddressTaken();
}

/// needsFrameBaseReg - Returns true if the instruction's frame index
/// reference would be better served by a base register other than FP
/// or SP. Used by LocalStackFrameAllocation to determine which frame index
/// references it should create new base registers for.
bool AArch64RegisterInfo::needsFrameBaseReg(MachineInstr *MI,
                                            int64_t Offset) const {
  for (unsigned i = 0; !MI->getOperand(i).isFI(); ++i)
    assert(i < MI->getNumOperands() &&
           "Instr doesn't have FrameIndex operand!");

  // It's the load/store FI references that cause issues, as it can be difficult
  // to materialize the offset if it won't fit in the literal field. Estimate
  // based on the size of the local frame and some conservative assumptions
  // about the rest of the stack frame (note, this is pre-regalloc, so
  // we don't know everything for certain yet) whether this offset is likely
  // to be out of range of the immediate. Return true if so.

  // We only generate virtual base registers for loads and stores, so
  // return false for everything else.
  if (!MI->mayLoad() && !MI->mayStore())
    return false;

  // Without a virtual base register, if the function has variable sized
  // objects, all fixed-size local references will be via the frame pointer,
  // Approximate the offset and see if it's legal for the instruction.
  // Note that the incoming offset is based on the SP value at function entry,
  // so it'll be negative.
  MachineFunction &MF = *MI->getParent()->getParent();
  const TargetFrameLowering *TFI = MF.getSubtarget().getFrameLowering();
  MachineFrameInfo *MFI = MF.getFrameInfo();

  // Estimate an offset from the frame pointer.
  // Conservatively assume all GPR callee-saved registers get pushed.
  // FP, LR, X19-X28, D8-D15. 64-bits each.
  int64_t FPOffset = Offset - 16 * 20;
  // Estimate an offset from the stack pointer.
  // The incoming offset is relating to the SP at the start of the function,
  // but when we access the local it'll be relative to the SP after local
  // allocation, so adjust our SP-relative offset by that allocation size.
  Offset += MFI->getLocalFrameSize();
  // Assume that we'll have at least some spill slots allocated.
  // FIXME: This is a total SWAG number. We should run some statistics
  //        and pick a real one.
  Offset += 128; // 128 bytes of spill slots

  // If there is a frame pointer, try using it.
  // The FP is only available if there is no dynamic realignment. We
  // don't know for sure yet whether we'll need that, so we guess based
  // on whether there are any local variables that would trigger it.
  if (TFI->hasFP(MF) && isFrameOffsetLegal(MI, AArch64::FP, FPOffset))
    return false;

  // If we can reference via the stack pointer or base pointer, try that.
  // FIXME: This (and the code that resolves the references) can be improved
  //        to only disallow SP relative references in the live range of
  //        the VLA(s). In practice, it's unclear how much difference that
  //        would make, but it may be worth doing.
  if (isFrameOffsetLegal(MI, AArch64::SP, Offset))
    return false;

  // The offset likely isn't legal; we want to allocate a virtual base register.
  return true;
}

bool AArch64RegisterInfo::isFrameOffsetLegal(const MachineInstr *MI,
                                             unsigned BaseReg,
                                             int64_t Offset) const {
  assert(Offset <= INT_MAX && "Offset too big to fit in int.");
  assert(MI && "Unable to get the legal offset for nil instruction.");
  int SaveOffset = Offset;
  return isAArch64FrameOffsetLegal(*MI, SaveOffset) & AArch64FrameOffsetIsLegal;
}

/// Insert defining instruction(s) for BaseReg to be a pointer to FrameIdx
/// at the beginning of the basic block.
void AArch64RegisterInfo::materializeFrameBaseRegister(MachineBasicBlock *MBB,
                                                       unsigned BaseReg,
                                                       int FrameIdx,
                                                       int64_t Offset) const {
  MachineBasicBlock::iterator Ins = MBB->begin();
  DebugLoc DL; // Defaults to "unknown"
  if (Ins != MBB->end())
    DL = Ins->getDebugLoc();
  const MachineFunction &MF = *MBB->getParent();
  const AArch64InstrInfo *TII =
      MF.getSubtarget<AArch64Subtarget>().getInstrInfo();
  const MCInstrDesc &MCID = TII->get(AArch64::ADDXri);
  MachineRegisterInfo &MRI = MBB->getParent()->getRegInfo();
  MRI.constrainRegClass(BaseReg, TII->getRegClass(MCID, 0, this, MF));
  unsigned Shifter = AArch64_AM::getShifterImm(AArch64_AM::LSL, 0);

  BuildMI(*MBB, Ins, DL, MCID, BaseReg)
      .addFrameIndex(FrameIdx)
      .addImm(Offset)
      .addImm(Shifter);
}

void AArch64RegisterInfo::resolveFrameIndex(MachineInstr &MI, unsigned BaseReg,
                                            int64_t Offset) const {
  int Off = Offset; // ARM doesn't need the general 64-bit offsets
  unsigned i = 0;

  while (!MI.getOperand(i).isFI()) {
    ++i;
    assert(i < MI.getNumOperands() && "Instr doesn't have FrameIndex operand!");
  }
  const MachineFunction *MF = MI.getParent()->getParent();
  const AArch64InstrInfo *TII =
      MF->getSubtarget<AArch64Subtarget>().getInstrInfo();
  bool Done = rewriteAArch64FrameIndex(MI, i, BaseReg, Off, TII);
  assert(Done && "Unable to resolve frame index!");
  (void)Done;
}

void AArch64RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                              int SPAdj, unsigned FIOperandNum,
                                              RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected");

  MachineInstr &MI = *II;
  MachineBasicBlock &MBB = *MI.getParent();
  MachineFunction &MF = *MBB.getParent();
  const AArch64InstrInfo *TII =
      MF.getSubtarget<AArch64Subtarget>().getInstrInfo();
  const AArch64FrameLowering *TFI = static_cast<const AArch64FrameLowering *>(
      MF.getSubtarget().getFrameLowering());

  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  unsigned FrameReg;
  int Offset;

  // Special handling of dbg_value, stackmap and patchpoint instructions.
  if (MI.isDebugValue() || MI.getOpcode() == TargetOpcode::STACKMAP ||
      MI.getOpcode() == TargetOpcode::PATCHPOINT) {
    Offset = TFI->resolveFrameIndexReference(MF, FrameIndex, FrameReg,
                                             /*PreferFP=*/true);
    Offset += MI.getOperand(FIOperandNum + 1).getImm();
    MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, false /*isDef*/);
    MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
    return;
  }

  // Modify MI as necessary to handle as much of 'Offset' as possible
  Offset = TFI->resolveFrameIndexReference(MF, FrameIndex, FrameReg);
  if (rewriteAArch64FrameIndex(MI, FIOperandNum, FrameReg, Offset, TII))
    return;

  assert((!RS || !RS->isScavengingFrameIndex(FrameIndex)) &&
         "Emergency spill slot is out of reach");

  // If we get here, the immediate doesn't fit into the instruction.  We folded
  // as much as possible above.  Handle the rest, providing a register that is
  // SP+LargeImm.
  unsigned ScratchReg =
      MF.getRegInfo().createVirtualRegister(&AArch64::GPR64RegClass);
  emitFrameOffset(MBB, II, MI.getDebugLoc(), ScratchReg, FrameReg, Offset, TII);
  MI.getOperand(FIOperandNum).ChangeToRegister(ScratchReg, false, false, true);
}

namespace llvm {

unsigned AArch64RegisterInfo::getRegPressureLimit(const TargetRegisterClass *RC,
                                                  MachineFunction &MF) const {
  const TargetFrameLowering *TFI = MF.getSubtarget().getFrameLowering();
//moslem
int GPRreseredRegs=0;
int floatReseredRegs=0;
if (ReserveRegisters) {GPRreseredRegs=19;floatReseredRegs=19;}

//////
  switch (RC->getID()) {
  default:
    return 0;
  case AArch64::GPR32RegClassID:
  case AArch64::GPR32spRegClassID:
  case AArch64::GPR32allRegClassID:
  case AArch64::GPR64spRegClassID:
  case AArch64::GPR64allRegClassID:
  case AArch64::GPR64RegClassID:
  case AArch64::GPR32commonRegClassID:
  case AArch64::GPR64commonRegClassID:
    return 32 - 1                                // XZR/SP
           - (TFI->hasFP(MF) || TT.isOSDarwin()) // FP
           - (TT.isOSDarwin() || ReserveX18) // X18 reserved as platform register
	   - ReserveX3 // X3 is reserved for moslem
           - hasBasePointer(MF)           // X19
	   - GPRreseredRegs;		//reserved for moslem
  case AArch64::FPR8RegClassID:
  case AArch64::FPR16RegClassID:
  case AArch64::FPR32RegClassID:
  case AArch64::FPR64RegClassID:
  case AArch64::FPR128RegClassID:
    return 32 - floatReseredRegs;

  case AArch64::DDRegClassID:
  case AArch64::DDDRegClassID:
  case AArch64::DDDDRegClassID:
  case AArch64::QQRegClassID:
  case AArch64::QQQRegClassID:
  case AArch64::QQQQRegClassID:
    return 32;

  case AArch64::FPR128_loRegClassID:
    return 16;
  }
}

} // namespace llvm
