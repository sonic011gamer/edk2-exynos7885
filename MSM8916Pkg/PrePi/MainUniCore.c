/** @file

  Copyright (c) 2011, ARM Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "PrePi.h"

  void setFBcolor(char* colors) {
    char* base = (char*)0x0ec000000ull;
    for (int i = 0; i < 0x00800000; i += 4) {
        base[i] = colors[0];      // Blue component
        base[i + 1] = colors[1];  // Green component
        base[i + 2] = colors[2];  // Red component
        base[i + 3] = 255;        // Full opacity
    }
}
VOID
PrimaryMain (
  IN  UINTN   UefiMemoryBase,
  IN  UINTN   StacksBase,
  IN  UINT64  StartTimeStamp
  )
{

  char colors[3] = {0, 255, 0}; // Green color (RGB format)
  setFBcolor(colors);
  PrePiMain (UefiMemoryBase, StacksBase, StartTimeStamp);

  // We must never return
  ASSERT (FALSE);
}

VOID
SecondaryMain (
  IN  UINTN  MpId
  )
{
  char colors[3] = {0, 0, 255}; // Red color (RGB format)
  setFBcolor(colors);
  // We must never get into this function on UniCore system
  ASSERT (FALSE);
}
