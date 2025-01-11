/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>
#include <string.h>

#include <functional>

#include <unwindstack/Elf.h>
#include <unwindstack/MachineLoongarch64.h>
#include <unwindstack/MapInfo.h>
#include <unwindstack/Memory.h>
#include <unwindstack/RegsLoongarch64.h>
#include <unwindstack/UcontextLoongarch64.h>
#include <unwindstack/UserLoongarch64.h>

namespace unwindstack {

RegsLoongarch64::RegsLoongarch64()
    : RegsImpl<uint64_t>(LOONGARCH64_REG_MAX, Location(LOCATION_REGISTER, LOONGARCH64_REG_RA)) {}

ArchEnum RegsLoongarch64::Arch() {
  return ARCH_LOONGARCH64;
}

uint64_t RegsLoongarch64::pc() {
  return regs_[LOONGARCH64_REG_PC];
}

uint64_t RegsLoongarch64::sp() {
  return regs_[LOONGARCH64_REG_SP];
}

void RegsLoongarch64::set_pc(uint64_t pc) {
  regs_[LOONGARCH64_REG_PC] = pc;
}

void RegsLoongarch64::set_sp(uint64_t sp) {
  regs_[LOONGARCH64_REG_SP] = sp;
}

bool RegsLoongarch64::SetPcFromReturnAddress(Memory*) {
  uint64_t ra = regs_[LOONGARCH64_REG_RA];
  if (regs_[LOONGARCH64_REG_PC] == ra) {
    return false;
  }

  regs_[LOONGARCH64_REG_PC] = ra;
  return true;
}

void RegsLoongarch64::IterateRegisters(std::function<void(const char*, uint64_t)> fn) {
  //fn("zero", regs_[LOONGARCH64_REG_R0]);
  fn("pc", regs_[LOONGARCH64_REG_PC]);
  fn("ra", regs_[LOONGARCH64_REG_RA]);
  fn("tp", regs_[LOONGARCH64_REG_R2]);
  fn("sp", regs_[LOONGARCH64_REG_SP]);
  fn("r4", regs_[LOONGARCH64_REG_R4]);
  fn("r5", regs_[LOONGARCH64_REG_R5]);
  fn("r6", regs_[LOONGARCH64_REG_R6]);
  fn("r7", regs_[LOONGARCH64_REG_R7]);
  fn("r8", regs_[LOONGARCH64_REG_R8]);
  fn("r9", regs_[LOONGARCH64_REG_R9]);
  fn("r10", regs_[LOONGARCH64_REG_R10]);
  fn("r11", regs_[LOONGARCH64_REG_R11]);
  fn("r12", regs_[LOONGARCH64_REG_R12]);
  fn("r13", regs_[LOONGARCH64_REG_R13]);
  fn("r14", regs_[LOONGARCH64_REG_R14]);
  fn("r15", regs_[LOONGARCH64_REG_R15]);
  fn("r16", regs_[LOONGARCH64_REG_R16]);
  fn("r17", regs_[LOONGARCH64_REG_R17]);
  fn("r18", regs_[LOONGARCH64_REG_R18]);
  fn("r19", regs_[LOONGARCH64_REG_R19]);
  fn("r20", regs_[LOONGARCH64_REG_R20]);
  fn("r21", regs_[LOONGARCH64_REG_R21]);
  fn("r22", regs_[LOONGARCH64_REG_R22]);
  fn("r23", regs_[LOONGARCH64_REG_R23]);
  fn("r24", regs_[LOONGARCH64_REG_R24]);
  fn("r25", regs_[LOONGARCH64_REG_R25]);
  fn("r26", regs_[LOONGARCH64_REG_R26]);
  fn("r27", regs_[LOONGARCH64_REG_R27]);
  fn("r28", regs_[LOONGARCH64_REG_R28]);
  fn("r29", regs_[LOONGARCH64_REG_R29]);
  fn("r30", regs_[LOONGARCH64_REG_R30]);
  fn("r31", regs_[LOONGARCH64_REG_R31]);
}

Regs* RegsLoongarch64::Read(void* remote_data) {
  loongarch64_user_regs* user = reinterpret_cast<loongarch64_user_regs*>(remote_data);

  RegsLoongarch64* regs = new RegsLoongarch64();
  uint64_t* reg_data = reinterpret_cast<uint64_t*>(regs->RawData());
  memcpy((void*)reg_data, &user->regs[0], LOONGARCH64_REG_MAX * sizeof(uint64_t));
  // use the storage of reg zero to store PC
  reg_data[LOONGARCH64_REG_PC] = user->pad[LOONGARCH_EF_PC];
  return regs;
}

Regs* RegsLoongarch64::CreateFromUcontext(void* ucontext) {
  loongarch64_ucontext_t* loongarch64_ucontext = reinterpret_cast<loongarch64_ucontext_t*>(ucontext);

  RegsLoongarch64* regs = new RegsLoongarch64();
  uint64_t* reg_data = reinterpret_cast<uint64_t*>(regs->RawData());
  memcpy((void*)reg_data, &loongarch64_ucontext->uc_mcontext.sc_regs[0], LOONGARCH64_REG_MAX * sizeof(uint64_t));
  reg_data[LOONGARCH64_REG_PC] = loongarch64_ucontext->uc_mcontext.sc_pc;
  return regs;
}

bool RegsLoongarch64::StepIfSignalHandler(uint64_t elf_offset, Elf* elf, Memory* process_memory) {
  uint64_t data;
  // Read from elf memory since it is usually more expensive to read from
  // process memory.
  if (!elf->memory()->ReadFully(elf_offset, &data, sizeof(data))) {
    return false;
  }
  // Look for the kernel sigreturn function.
  // __vdso_rt_sigreturn:

  // 0x03822c0b li a7,0x8b
  // 0x002b0000 syscall 0
  if (data != 0x002b000003822c0bULL) {
    return false;
  }

  // SP + sizeof(siginfo_t) + uc_mcontext offset
  uint32_t kOffsetSpToSigcontext = 0x80 + 0xb0;
  if (!process_memory->ReadFully(regs_[LOONGARCH64_REG_SP] + kOffsetSpToSigcontext, regs_.data(),
                                 sizeof(uint64_t) * (LOONGARCH64_REG_MAX))) {
    return false;
  }
  return true;
}

Regs* RegsLoongarch64::Clone() {
  return new RegsLoongarch64(*this);
}

}  // namespace unwindstack
