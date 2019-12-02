# Supporting PSA in Mbed OS

This document is still a work in progress. It describes the process of adding new PSA targets to Mbed OS and building [TF-M](https://git.trustedfirmware.org/trusted-firmware-m.git/tree/?h=feature-twincpu) for v8-M and dual-core targets. It also outlines the new directory structure created to support PSA on all targets.

*Glossary*  
PSA - Platform Security Architecture  
TF-M - Trusted Firmware M  
SPE - Secure Processing Environment  
NSPE - Non-Secure Processing Environment

# TODO
* Adding v8-M targets
* Running PSA compliance tests

# Adding new PSA targets
To help with the creation of new PSA targets, a couple of generic targets have been added to `targets/targets.json`.
* `PSA_Target` (Root level PSA target)
* `PSA_V7_M_NSPE` (Single v7-M NSPE generic target)
* `PSA_V7_M_SPE` (Single v7-M SPE generic target)
* `PSA_DUAL_V7_M_NSPE` (Dual-core NSPE generic target)
* `PSA_DUAL_V7_M_SPE` (Dual-core SPE generic target)
* `PSA_V8_M_NSPE` (v8-M NSPE generic target)
* `PSA_V8_M_SPE` (v8-M SPE generic target)

A single-core PSA target doesn't employ hardware isolation between the NSPE and the SPE. Therefore for single-core PSA targets only NSPE target is defined. PSA secure service emulation enables PSA API compatibility.

A dual-core PSA target will have at least two cores that are either Armv7-M or Armv6-M. One core will be used for the SPE and another for the NSPE. Hardware isolation between the cores enables PSA compliance. On dual-core targets, TF-M runs on the SPE and provides PSA services. Therefore, it is **MANDATORY** to define both the SPE and the NSPE target for dual-core PSA targets.

An Armv8-M PSA target employs hardware to isolate the NSPE from the SPE. On Armv8-M targets, TF-M runs on the SPE and provides PSA services. Therefore, it is **MANDATORY** to define both the SPE and the NSPE target for Armv8-M PSA targets.

The new PSA targets **MUST** `inherit` from the generic PSA target that corresponds to that target's architecture. Also, in case of dual-core and v8-M targets, SPE and NSPE targets **MUST** inherit from the respective SPE and NSPE PSA generic targets.

Example single-core PSA target:
```json
    "K64F": {
        "inherits": ["PSA_V7_M_NSPE", "Target"],
    }
```

Example dual-core PSA target:
```json
    "CY8CPROTO_064_SB_S": {
        "inherits": ["PSA_DUAL_V7_M_SPE"],
    },
    "CY8CPROTO_064_SB_NS": {
        "inherits": ["PSA_DUAL_V7_M_NSPE", "MCU_PSOC6_M4"],
    }
```

In addition, the new PSA NSPE target **MUST** `inherit` from `Target` which defines default target config options. The only exception is, if the new PSA target is inheriting from another generic target which inherits from `Target`.

Example:
```json
    "FAMILY_STM32": {
        "inherits": ["Target"],
        "public": false,
    }
    "NUCLEO_F429ZI": {
        "inherits": ["PSA_V7_M_NSPE", "FAMILY_STM32"],
    }
```

## Adding single-core PSA targets
Mbed OS's PSA service emulation provides PSA compatibility for single-core PSA targets. The following is taken from `targets/targets.json` and shows a PSA-enabled single-core target, `K64F`.

```json
    "K64F": {
        "inherits": ["PSA_V7_M_NSPE"],
        "supported_form_factors": ["ARDUINO"],
        "components_add": ["SD", "FLASHIAP"],
        "core": "Cortex-M4F",
        "supported_toolchains": ["ARM", "GCC_ARM", "IAR"],
        "extra_labels_add": [
            "Freescale",
            "MCUXpresso_MCUS",
            "KSDK2_MCUS",
            "FRDM",
            "KPSDK_MCUS",
            "KPSDK_CODE",
            "MCU_K64F",
            "Freescale_EMAC"
        ],
        "is_disk_virtual": true,
        "macros": ["CPU_MK64FN1M0VMD12", "FSL_RTOS_MBED", "MBED_SPLIT_HEAP"],
        "detect_code": ["0240"],
        "device_has_add": [
            "USTICKER",
            "LPTICKER",
            "RTC",
            "CRC",
            "ANALOGIN",
            "ANALOGOUT",
            "EMAC",
            "I2C",
            "I2CSLAVE",
            "INTERRUPTIN",
            "PORTIN",
            "PORTINOUT",
            "PORTOUT",
            "PWMOUT",
            "RESET_REASON",
            "SERIAL",
            "SERIAL_FC",
            "SERIAL_ASYNCH",
            "SLEEP",
            "SPI",
            "SPI_ASYNCH",
            "SPISLAVE",
            "STDIO_MESSAGES",
            "TRNG",
            "FLASH",
            "USBDEVICE",
            "WATCHDOG"
        ],
        "release_versions": ["2", "5"],
        "device_name": "MK64FN1M0xxx12",
        "bootloader_supported": true,
        "overrides": {
            "network-default-interface-type": "ETHERNET"
        }
    }
```

Please pay attention to config options `extra_labels_add` and `device_has_add`. If needed then a PSA target definition **MUST** use [extra_labels/device_has]`_add` or [extra_labels/device_has]`_remove` (not `extra_labels` or `device_has`) to add/remove either extra_labels or target capabilities. Also, use `feature_`[add/remove] to add/remove a feature.
Check [extra_labels](https://os.mbed.com/docs/mbed-os/v5.14/reference/adding-and-configuring-targets.html), [device_has](https://os.mbed.com/docs/mbed-os/v5.14/reference/adding-and-configuring-targets.html) and [features](https://os.mbed.com/docs/mbed-os/v5.14/reference/adding-and-configuring-targets.html) for more information.

## Adding dual-core PSA targets
A target can be categorized as a dual-core target if it has at least two cores that are either Armv7-M or Armv6-M. On dual-core PSA targets, TF-M runs on the SPE and provides PSA services. Therefore, it is **MANDATORY** to define both the SPE and the NSPE target for dual-core PSA targets.

The SPE target **MUST** contain the following attributes:

* inherits: PSA generic target PSA_DUAL_V7_M_SPE
* tfm_target_name: Target name in TF-M
* tfm_bootloader_supported: If TF-M bootloader is supported by the target. Values supported are "true" and "false"
* tfm_default_toolchain: Default TF-M toolchain supported. Values supported are "ARMCLANG" and "GNUARM"
* tfm_supported_toolchains: Supported TF-M toolchains. Values supported are "ARMCLANG" and "GNUARM"
* delivery_dir: The directory to which TF-M binary will be copied to

The following is taken from `targets/targets.json` and shows a PSA enabled dual-core target, `PSoC64`,

```json
    "CY8CPROTO_064_SB_S": {
        "inherits": ["PSA_DUAL_V7_M_SPE"],
        "tfm_target_name": "psoc64",
        "tfm_bootloader_supported": false,
        "tfm_default_toolchain": "GNUARM",
        "tfm_supported_toolchains": ["ARMCLANG", "GNUARM"],
        "delivery_dir": "TARGET_Cypress/TARGET_PSOC6/TARGET_CY8CPROTO_064_SB/prebuilt"
    },
    "CY8CPROTO_064_SB_NS": {
        "inherits": ["PSA_DUAL_V7_M_NSPE", "MCU_PSOC6_M4"],
        "components_remove": ["QSPIF"],
        "device_has_remove": ["QSPI"],
        "supported_form_factors": ["ARDUINO"],
        "extra_labels_add": ["PSOC6_01", "MXCRYPTO_01"],
        "macros_add": ["CYB06447BZI_D54",
                       "PSOC6_DYNSRM_DISABLE=1",
                       "CY_CFG_SYSCLK_WCO_ENABLED=1",
                       "SEMAPHORE"],
        "detect_code": ["1907"],
        "forced_reset_timeout": 5,
        "reset_method": "default",
        "post_binary_hook": {
            "function": "PSOC6Code.sign_image"
        }
    },
```

Please pay attention to the config options `extra_labels_add` and `device_has_remove`. If needed, a PSA target definition **MUST** use [extra_labels/device_has]`_add` or [extra_labels/device_has]`_remove` (not `extra_labels` or `device_has`) to add/remove either extra_labels or target capabilities. Also, use `feature_`[add/remove] to add/remove a feature.
Check [extra_labels](https://os.mbed.com/docs/mbed-os/v5.14/reference/adding-and-configuring-targets.html), [device_has](https://os.mbed.com/docs/mbed-os/v5.14/reference/adding-and-configuring-targets.html) and [features](https://os.mbed.com/docs/mbed-os/v5.14/reference/adding-and-configuring-targets.html) for more information.

## Adding Armv8-M PSA targets

#  Enabling PSA at application level
Having an entropy source is crucial for Mbed TLS and Mbed Crypto. The [document](https://os.mbed.com/docs/mbed-os/v5.14/porting/entropy-sources.html) talks about entropy and how to add an entropy source. Sometimes a target might not have a True Random Number Generator (TRNG), in that case the target will be configured as a non-PSA target in `targets/targets.json`. In that scenario, if an application wants to use that target as a PAS target then it is the responsibility of application to provide an entropy source and mark that target as PSA target at application level. The config option [target_overrides](https://os.mbed.com/docs/mbed-os/v5.14/reference/configuration.html) can be used to enable PSA for a target.

example mbed_app.json:
```json
"target_overrides": {
    "K64F": {
        "inherits": ["PSA_V7_M_NSPE"]
    }
}
```

# Building TF-M
For dual-core and Armv8-M PSA targets, TF-M runs on the SPE and provides PSA services. A python script `tools/psa/build_tfm.py` automates building TF-M and copying the binary to a predefined location defined by attribute `delivery_dir`.

Usage:
```console
python3 tools/psa/build_tfm.py
```
Supported options:
```console
usage: build_tfm.py [-h] [-m {CY8CPROTO_064_SB_S}] [-t {ARMCLANG,GNUARM}] [-d]
                    [-l] [--clone] [--commit] [--clean-build] [-v]

optional arguments:
  -h, --help            show this help message and exit
  -m {CY8CPROTO_064_SB_S}, --mcu {CY8CPROTO_064_SB_S}
                        Build for the given MCU
  -t {ARMCLANG,GNUARM}, --toolchain {ARMCLANG,GNUARM}
                        Build for the given toolchain (default is
                        tfm_default_toolchain)
  -d, --debug           Set build profile to debug
  -l, --list            Print supported TF-M secure targets
  --clone               Clone TF-M git repo and its dependencies
  --commit              Create a git commit for each platform
  --clean-build         Remove Cmake build directory
  -v, --verbose         Verbose output
```

If the python script is invoked without any options then TF-M will be built for all the supported targets and the binary will be copied to predefined location defined by attribute `delivery_dir`.

# PSA target directory structure

```console
features/FEATURE_PSA/
├── TARGET_TFM
│   ├── TARGET_TFM_V8M
│   └── TARGET_TFM_TWINCPU
└── TARGET_MBED_PSA_SRV
    ├── services
    │   ├── storage
    │   │   ├── ps
    │   │   ├── its
    │   │   └── common
    │   ├── platform
    │   └── attestation
    │       ├── qcbor
    │       │   ├── test
    │       │   ├── src
    │       │   └── inc
    │       ├── implementation
    │       │   └── tfm_impl
    │       │       └── t_cose
    │       │           ├── src
    │       │           └── inc
    │       └── emulation
    ├── inc
    │   └── psa
    └── TESTS
        ├── compliance_its
        │   ├── test_s010
        │   ├── test_s009
        │   ├── test_s008
        │   ├── test_s007
        │   ├── test_s006
        │   ├── test_s005
        │   ├── test_s004
        │   ├── test_s003
        │   ├── test_s002
        │   └── test_s001
        └── compliance_attestation
            └── test_a001
```
