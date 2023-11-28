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
  LOONGARCH64_REG_PC,
  LOONGARCH64_REG_RA,
  LOONGARCH64_REG_TP,
  LOONGARCH64_REG_SP,
  LOONGARCH64_REG_A0,
  LOONGARCH64_REG_A1,
  LOONGARCH64_REG_A2,
  LOONGARCH64_REG_A3,
  LOONGARCH64_REG_A4,
  LOONGARCH64_REG_A5,
  LOONGARCH64_REG_A6,
  LOONGARCH64_REG_A7,
  LOONGARCH64_REG_T0,
  LOONGARCH64_REG_T1,
  LOONGARCH64_REG_T2,
  LOONGARCH64_REG_T3,
  LOONGARCH64_REG_T4,
  LOONGARCH64_REG_T5,
  LOONGARCH64_REG_T6,
  LOONGARCH64_REG_T7,
  LOONGARCH64_REG_T8,
  LOONGARCH64_REG_R21,
  LOONGARCH64_REG_FP,
  LOONGARCH64_REG_S0,
  LOONGARCH64_REG_S1,
  LOONGARCH64_REG_S2,
  LOONGARCH64_REG_S3,
  LOONGARCH64_REG_S4,
  LOONGARCH64_REG_S5,
  LOONGARCH64_REG_S6,
  LOONGARCH64_REG_S7,
  LOONGARCH64_REG_S8,
  LOONGARCH64_REG_MAX,
};

}  // namespace unwindstack

#endif  // _LIBUNWINDSTACK_MACHINE_LOONGARCH64_H
