/** @file

  Copyright (c) 2011-2017, ARM Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "PrePi.h"

#include <AArch64/AArch64.h>

/**
  Architecture specific initialization routine.
**/
VOID
ArchInitialize (
  VOID
  )
{
  if (ArmReadCurrentEL () == AARCH64_EL2) {
    // Trap General Exceptions. All exceptions that would be routed to EL1 are routed to EL2
    ArmWriteHcr (ARM_HCR_TGE);
  }
}
