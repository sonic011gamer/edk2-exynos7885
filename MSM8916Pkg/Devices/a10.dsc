[Defines]
  PLATFORM_NAME                  = MSM8916Pkg
  PLATFORM_GUID                  = 28f1a3bf-193a-47e3-a7b9-5a435eaab2ee
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010019
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = MSM8916Pkg/MSM8916Pkg.fdf

!include MSM8916Pkg/MSM8916Pkg.dsc

[PcdsFixedAtBuild.common]
  # System Memory (1.5GB)
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x60000000
  
  # Framebuffer (720x1280)
  gMSM8916PkgTokenSpaceGuid.PcdMipiFrameBufferAddress|0x0ec000000
  gMSM8916PkgTokenSpaceGuid.PcdMipiFrameBufferWidth|720
  gMSM8916PkgTokenSpaceGuid.PcdMipiFrameBufferHeight|1520
  gMSM8916PkgTokenSpaceGuid.PcdMipiFrameBufferVisibleWidth|720
  gMSM8916PkgTokenSpaceGuid.PcdMipiFrameBufferVisibleHeight|1520
