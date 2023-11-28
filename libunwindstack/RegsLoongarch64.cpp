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
  fn("pc", regs_[LOONGARCH64_REG_PC]);
  fn("ra", regs_[LOONGARCH64_REG_RA]);
  fn("tp", regs_[LOONGARCH64_REG_TP]);
  fn("sp", regs_[LOONGARCH64_REG_SP]);
  fn("a0", regs_[LOONGARCH64_REG_A0]);
  fn("a1", regs_[LOONGARCH64_REG_A1]);
  fn("a2", regs_[LOONGARCH64_REG_A2]);
  fn("a3", regs_[LOONGARCH64_REG_A3]);
  fn("a4", regs_[LOONGARCH64_REG_A4]);
  fn("a5", regs_[LOONGARCH64_REG_A5]);
  fn("a6", regs_[LOONGARCH64_REG_A6]);
  fn("a7", regs_[LOONGARCH64_REG_A7]);
  fn("t0", regs_[LOONGARCH64_REG_T0]);
  fn("t1", regs_[LOONGARCH64_REG_T1]);
  fn("t2", regs_[LOONGARCH64_REG_T2]);
  fn("t3", regs_[LOONGARCH64_REG_T3]);
  fn("t4", regs_[LOONGARCH64_REG_T4]);
  fn("t5", regs_[LOONGARCH64_REG_T5]);
  fn("t6", regs_[LOONGARCH64_REG_T6]);
  fn("t7", regs_[LOONGARCH64_REG_T7]);
  fn("t8", regs_[LOONGARCH64_REG_T8]);
  fn("r21", regs_[LOONGARCH64_REG_R21]);
  fn("fp", regs_[LOONGARCH64_REG_FP]);
  fn("s0", regs_[LOONGARCH64_REG_S0]);
  fn("s1", regs_[LOONGARCH64_REG_S1]);
  fn("s2", regs_[LOONGARCH64_REG_S2]);
  fn("s3", regs_[LOONGARCH64_REG_S3]);
  fn("s4", regs_[LOONGARCH64_REG_S4]);
  fn("s5", regs_[LOONGARCH64_REG_S5]);
  fn("s6", regs_[LOONGARCH64_REG_S6]);
  fn("s7", regs_[LOONGARCH64_REG_S7]);
  fn("s8", regs_[LOONGARCH64_REG_S8]);
}

Regs* RegsLoongarch64::Read(void* remote_data) {
  loongarch64_user_regs* user = reinterpret_cast<loongarch64_user_regs*>(remote_data);

  RegsLoongarch64* regs = new RegsLoongarch64();
  memcpy(regs->RawData(), &user->regs[0], LOONGARCH64_REG_MAX * sizeof(uint64_t));
  //uint64_t* reg_data = reinterpret_cast<uint64_t*>(regs->RawData());
  return regs;
}

Regs* RegsLoongarch64::CreateFromUcontext(void* ucontext) {
  loongarch64_ucontext_t* loongarch64_ucontext = reinterpret_cast<loongarch64_ucontext_t*>(ucontext);

  RegsLoongarch64* regs = new RegsLoongarch64();
  memcpy(regs->RawData(), &loongarch64_ucontext->uc_mcontext.regs[0], LOONGARCH64_REG_MAX * sizeof(uint64_t));
  return regs;
}

bool RegsLoongarch64::StepIfSignalHandler(uint64_t elf_offset, Elf* elf, Memory* process_memory) {
  uint64_t data;
  Memory* elf_memory = elf->memory();
  // Read from elf memory since it is usually more expensive to read from
  // process memory.
  if (!elf_memory->ReadFully(elf_offset, &data, sizeof(data))) {
    return false;
  }
  // Look for the kernel sigreturn function.
  // __kernel_rt_sigreturn:
  // li a7, __NR_rt_sigreturn
  // scall

  const uint8_t li_scall[] = {0x93, 0x08, 0xb0, 0x08, 0x73, 0x00, 0x00, 0x00};
  if (memcmp(&data, &li_scall, 8) != 0) {
    return false;
  }

  // SP + sizeof(siginfo_t) + uc_mcontext offset + PC offset.
  if (!process_memory->ReadFully(regs_[LOONGARCH64_REG_SP] + 0x80 + 0xb0 + 0x00, regs_.data(),
                                 sizeof(uint64_t) * (LOONGARCH64_REG_MAX))) {
    return false;
  }
  return true;
}

Regs* RegsLoongarch64::Clone() {
  return new RegsLoongarch64(*this);
}

}  // namespace unwindstack
