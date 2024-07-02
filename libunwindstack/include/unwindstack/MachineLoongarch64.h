/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef _LIBUNWINDSTACK_MACHINE_LOONGARCH64_H
#define _LIBUNWINDSTACK_MACHINE_LOONGARCH64_H

#include <stdint.h>

namespace unwindstack {

enum Loongarch64Reg : uint16_t {
  //LOONGARCH64_REG_R0 = 0, //zero
  LOONGARCH64_REG_PC,
  LOONGARCH64_REG_RA,  // RA
  LOONGARCH64_REG_R2,  // TP
  LOONGARCH64_REG_SP,  // SP
  LOONGARCH64_REG_R4,  // A0
  LOONGARCH64_REG_R5,  // A1
  LOONGARCH64_REG_R6,  // A2
  LOONGARCH64_REG_R7,  // A3
  LOONGARCH64_REG_R8,  // A4
  LOONGARCH64_REG_R9,  // A5
  LOONGARCH64_REG_R10, // A6
  LOONGARCH64_REG_R11, // A7
  LOONGARCH64_REG_R12, // T0
  LOONGARCH64_REG_R13, // T1
  LOONGARCH64_REG_R14, // T2
  LOONGARCH64_REG_R15, // T3
  LOONGARCH64_REG_R16, // T4
  LOONGARCH64_REG_R17, // T5
  LOONGARCH64_REG_R18, // T6
  LOONGARCH64_REG_R19, // T7
  LOONGARCH64_REG_R20, // T8
  LOONGARCH64_REG_R21,
  LOONGARCH64_REG_R22, // FP
  LOONGARCH64_REG_R23, // S0
  LOONGARCH64_REG_R24, // S1
  LOONGARCH64_REG_R25, // S2
  LOONGARCH64_REG_R26, // S3
  LOONGARCH64_REG_R27, // S4
  LOONGARCH64_REG_R28, // S5
  LOONGARCH64_REG_R29, // S6
  LOONGARCH64_REG_R30, // S7
  LOONGARCH64_REG_R31, // S8
  LOONGARCH64_REG_MAX,
};

}  // namespace unwindstack

#endif  // _LIBUNWINDSTACK_MACHINE_LOONGARCH64_H
