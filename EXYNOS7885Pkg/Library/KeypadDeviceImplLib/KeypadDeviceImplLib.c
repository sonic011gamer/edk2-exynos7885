#include <PiDxe.h>

#include <Library/LKEnvLib.h>
#include <Library/QcomPm8x41Lib.h>
#include <Library/QcomGpioTlmmLib.h>
#include <Library/KeypadDeviceImplLib.h>
#include <Library/KeypadDeviceHelperLib.h>
#include <Protocol/KeypadDevice.h>

typedef enum {
  KEY_DEVICE_TYPE_UNKNOWN,

  KEY_DEVICE_TYPE_GPA,
  KEY_DEVICE_TYPE_GPIO
} KEY_DEVICE_TYPE;

typedef struct {
  KEY_CONTEXT      EfiKeyContext;
  BOOLEAN          IsValid;
  KEY_DEVICE_TYPE  DeviceType;
} KEY_CONTEXT_PRIVATE;

STATIC KEY_CONTEXT_PRIVATE KeyContextPower;
STATIC KEY_CONTEXT_PRIVATE KeyContextVolumeUp;
STATIC KEY_CONTEXT_PRIVATE KeyContextVolumeDown;

STATIC KEY_CONTEXT_PRIVATE* KeyList[] = {
  &KeyContextPower,
  &KeyContextVolumeUp,
  &KeyContextVolumeDown
};

STATIC
VOID
KeypadInitializeKeyContextPrivate (
  KEY_CONTEXT_PRIVATE  *Context
  )
{
  Context->IsValid = FALSE;
  Context->DeviceType = KEY_DEVICE_TYPE_UNKNOWN;
}

STATIC
KEY_CONTEXT_PRIVATE*
KeypadKeyCodeToKeyContext (
  UINT32 KeyCode
  )
{
  if (KeyCode == 114){
    return &KeyContextVolumeDown;
  }else if (KeyCode == 115){
    return &KeyContextVolumeUp;
  }else if (KeyCode == 116){
    return &KeyContextPower;
  }else
    return NULL;
}

RETURN_STATUS
EFIAPI
KeypadDeviceImplConstructor (
  VOID
  )
{
  UINTN                          Index;
  KEY_CONTEXT_PRIVATE            *StaticContext;

  // Reset all keys
  for (Index=0; Index<ARRAY_SIZE(KeyList); Index++) {
    KeypadInitializeKeyContextPrivate (KeyList[Index]);
  }

  // Configure keys

  // Vol Up (115) , Camera Splash (766) and Camera Focus (528)
  // go through TLMM GPIO
  StaticContext = KeypadKeyCodeToKeyContext(115);
  StaticContext->DeviceType = KEY_DEVICE_TYPE_GPA;
  StaticContext->IsValid = TRUE;

  // Vol Down (114) and Power On (116) on through PMIC PON
  StaticContext = KeypadKeyCodeToKeyContext(114);
  StaticContext->DeviceType = KEY_DEVICE_TYPE_GPA;
  StaticContext->IsValid = TRUE;

  StaticContext = KeypadKeyCodeToKeyContext(116);
  StaticContext->DeviceType = KEY_DEVICE_TYPE_GPA;
  StaticContext->IsValid = TRUE;

  return RETURN_SUCCESS;
}

EFI_STATUS EFIAPI KeypadDeviceImplReset (KEYPAD_DEVICE_PROTOCOL *This)
{
  LibKeyInitializeKeyContext(&KeyContextPower.EfiKeyContext);
  KeyContextPower.EfiKeyContext.KeyData.Key.UnicodeChar = CHAR_CARRIAGE_RETURN;

  LibKeyInitializeKeyContext(&KeyContextVolumeUp.EfiKeyContext);
  KeyContextVolumeUp.EfiKeyContext.KeyData.Key.ScanCode = SCAN_UP;

  LibKeyInitializeKeyContext(&KeyContextVolumeDown.EfiKeyContext);
  KeyContextVolumeDown.EfiKeyContext.KeyData.Key.ScanCode = SCAN_DOWN;

  return EFI_SUCCESS;
}

EFI_STATUS KeypadDeviceImplGetKeys (KEYPAD_DEVICE_PROTOCOL *This, KEYPAD_RETURN_API *KeypadReturnApi, UINT64 Delta)
{
  UINT8    GpioStatus;
  BOOLEAN  IsPressed;
  UINTN    Index;

  for (Index=0; Index<ARRAY_SIZE(KeyList); Index++) {
    KEY_CONTEXT_PRIVATE *Context = KeyList[Index];

    // check if this is a valid key
    if (Context->IsValid == FALSE)
      continue;

    // get status
    if (Context->DeviceType == KEY_DEVICE_TYPE_GPA) {
      GpioStatus = gGpioTlmm->Get(Context->Gpio);
    }
    else {
      continue;
    }
    IsPressed = FALSE;

    // update key status
    IsPressed = TRUE;
    LibKeyUpdateKeyStatus(&Context->EfiKeyContext, KeypadReturnApi, IsPressed, Delta);
  }

  return EFI_SUCCESS;
}
