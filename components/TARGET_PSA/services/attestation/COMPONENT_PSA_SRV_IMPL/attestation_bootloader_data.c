/*
* Copyright (c) 2018-2019 ARM Limited. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "attestation_bootloader_data.h"

/* Temporary Boodloader data - contains temp mandatory claims */
__attribute__((aligned(4)))
const uint8_t temp_ram_page_data[] = {
    0x16, 0x20, 0xB0, 0x00,                         //shared_data_tlv_header
    0x83, 0x11, 0x0C, 0x00,                         // SW_TYPE
    0x4E, 0x53, 0x50, 0x45, 0x5F, 0x53, 0x50, 0x45,
    0x80, 0x11, 0x0A, 0x00,                         //SW_VERSION
    0x31, 0x2E, 0x31, 0x2E, 0x31, 0x31,
    0x82, 0x11, 0x06, 0x00,                         //SW_EPOCH
    0x00, 0x00,
    0x88, 0x11, 0x24, 0x00,                         //SW_MEASURE_VALUE
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
    0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
    0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0x81, 0x11, 0x24, 0x00,                         //SW_SIGNER_ID
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
    0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
    0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0x89, 0x11, 0x0A, 0x00,                         //SW_MEASURE_TYPE
    0x53, 0x48, 0x41, 0x32, 0x35, 0x36,
    0x00, 0x10, 0x24, 0x00,                        //TLV_MINOR_IAS_BOOT_SEED
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
    0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
    0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    0x01, 0x10, 0x16, 0x00,                         //TLV_MINOR_IAS_HW_VERSION
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x31, 0x32
};

/* Temporary Implementation ID data: mandatory claim represents the original
** implementation signer of the attestation key and identifies the contract
** between the report and verification */
#define TEMP_IMPL_ID_DATA_SIZE (32u)

#define TEMP_IMPL_ID_DATA  0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, \
                           0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, \
                           0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, \
                           0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF

const uint8_t impl_id_data[TEMP_IMPL_ID_DATA_SIZE] = {TEMP_IMPL_ID_DATA};