/*
 * Copyright (C) 2016 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _LIBUNWINDSTACK_REGS_GET_LOCAL_H
#define _LIBUNWINDSTACK_REGS_GET_LOCAL_H

namespace unwindstack {

#if defined(__arm__)

inline __attribute__((__always_inline__)) void AsmGetRegs(void* reg_data) {
  asm volatile(
      ".align 2\n"
      "bx pc\n"
      "nop\n"
      ".code 32\n"
      "stmia %[base], {r0-r12}\n"
      "add r2, %[base], #52\n"
      "mov r3, r13\n"
      "mov r4, r14\n"
      "mov r5, r15\n"
      "stmia r2, {r3-r5}\n"
      "orr %[base], pc, #1\n"
      "bx %[base]\n"
      : [ base ] "+r"(reg_data)
      :
      : "r2", "r3", "r4", "r5", "memory");
}

#elif defined(__aarch64__)

inline __attribute__((__always_inline__)) void AsmGetRegs(void* reg_data) {
  asm volatile(
      "1:\n"
      "stp x0, x1, [%[base], #0]\n"
      "stp x2, x3, [%[base], #16]\n"
      "stp x4, x5, [%[base], #32]\n"
      "stp x6, x7, [%[base], #48]\n"
      "stp x8, x9, [%[base], #64]\n"
      "stp x10, x11, [%[base], #80]\n"
      "stp x12, x13, [%[base], #96]\n"
      "stp x14, x15, [%[base], #112]\n"
      "stp x16, x17, [%[base], #128]\n"
      "stp x18, x19, [%[base], #144]\n"
      "stp x20, x21, [%[base], #160]\n"
      "stp x22, x23, [%[base], #176]\n"
      "stp x24, x25, [%[base], #192]\n"
      "stp x26, x27, [%[base], #208]\n"
      "stp x28, x29, [%[base], #224]\n"
      "str x30, [%[base], #240]\n"
      "mov x12, sp\n"
      "adr x13, 1b\n"
      "stp x12, x13, [%[base], #248]\n"
      : [base] "+r"(reg_data)
      :
      : "x12", "x13", "memory");
}

#elif defined(__loongarch__)

inline __attribute__((__always_inline__)) void AsmGetRegs(void* reg_data) {
  asm volatile(
      "1:\n"
      "st.d $ra, %[base], 8\n"
      "st.d $tp, %[base], 16\n"
      "st.d $sp, %[base], 24\n"
      "st.d $a0, %[base], 32\n"
      "st.d $a1, %[base], 40\n"
      "st.d $a2, %[base], 48\n"
      "st.d $a3, %[base], 56\n"
      "st.d $a4, %[base], 64\n"
      "st.d $a5, %[base], 72\n"
      "st.d $a6, %[base], 80\n"
      "st.d $a7, %[base], 88\n"
      "st.d $t0, %[base], 96\n"
      "st.d $t1, %[base], 104\n"
      "st.d $t2, %[base], 112\n"
      "st.d $t3, %[base], 120\n"
      "st.d $t4, %[base], 128\n"
      "st.d $t5, %[base], 136\n"
      "st.d $t6, %[base], 144\n"
      "st.d $t7, %[base], 152\n"
      "st.d $t8, %[base], 160\n"
      "st.d $r21, %[base], 168\n"
      "st.d $fp, %[base], 176\n"
      "st.d $s0, %[base], 184\n"
      "st.d $s1, %[base], 192\n"
      "st.d $s2, %[base], 200\n"
      "st.d $s3, %[base], 208\n"
      "st.d $s4, %[base], 216\n"
      "st.d $s5, %[base], 224\n"
      "st.d $s6, %[base], 232\n"
      "st.d $s7, %[base], 240\n"
      "st.d $s8, %[base], 248\n"
      "la $t1, 1b\n"
      "st.d $t1, %[base], 0\n"
      : [base] "+r"(reg_data)
      :
      : "t1", "memory");
}

#elif defined(__i386__) || defined(__x86_64__)

extern "C" void AsmGetRegs(void* regs);

#endif

inline __attribute__((__always_inline__)) void RegsGetLocal(Regs* regs) {
  AsmGetRegs(regs->RawData());
}


}  // namespace unwindstack

#endif  // _LIBUNWINDSTACK_REGS_GET_LOCAL_H
