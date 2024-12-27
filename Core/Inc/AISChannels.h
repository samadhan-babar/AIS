/*
  Copyright (c) 2016-2020 Peter Antypas

  This file is part of the MAIANA™ transponder firmware.

  The firmware is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>
*/

/**
 * @file AISChannels.h
 * @brief Defines AIS VHF channel mappings and utilities for Marine Automatic Identification System firmware.
 * @details This file contains definitions for ITU-designated marine VHF channels and their mappings to internal RFIC channels.
 *          It provides structures and functions to map ITU channel numbers to their corresponding RFIC channel ordinals,
 *          as well as reference frequency information.
 * @version 1.0A
 * @date September 2024

 */

#ifndef AISCHANNELS_H_
#define AISCHANNELS_H_

#include <inttypes.h>

/**
 * @brief Structure to define a marine VHF AIS channel.
 * @details This structure associates an ITU channel number with an internal RFIC channel ordinal,
 *          its designation ('A', 'B', or '?'), and its reference frequency in MHz.
 */
typedef struct {
  uint8_t itu;              /**< The ITU channel number. */
  uint8_t ordinal;          /**< The zero-based index as defined in radio_config.h. */
  char designation;         /**< Channel designation: 'A', 'B', or unknown ('?'). */
  float frequency;          /**< The channel frequency in MHz. */
} ais_channel;

/**
 * @brief Enumeration of supported VHF channels.
 * @details These channels correspond to ITU-defined VHF channels commonly used in AIS transponders.
 */
typedef enum {
  CH_18 = 0, /**< VHF Channel 18 */
  CH_78,     /**< VHF Channel 78 */
  CH_19,     /**< VHF Channel 19 */
  CH_79,     /**< VHF Channel 79 */
  CH_20,     /**< VHF Channel 20 */
  CH_80,     /**< VHF Channel 80 */
  CH_21,     /**< VHF Channel 21 */
  CH_81,     /**< VHF Channel 81 */
  CH_22,     /**< VHF Channel 22 */
  CH_82,     /**< VHF Channel 82 */
  CH_23,     /**< VHF Channel 23 */
  CH_83,     /**< VHF Channel 83 */
  CH_24,     /**< VHF Channel 24 */
  CH_84,     /**< VHF Channel 84 */
  CH_25,     /**< VHF Channel 25 */
  CH_85,     /**< VHF Channel 85 */
  CH_26,     /**< VHF Channel 26 */
  CH_86,     /**< VHF Channel 86 */
  CH_27,     /**< VHF Channel 27 */
  CH_87,     /**< VHF Channel 87 */
  CH_28,     /**< VHF Channel 28 */
  CH_88      /**< VHF Channel 88 */
} VHFChannel;

/**
 * @brief Array of AIS VHF channel definitions.
 * @details This array maps ITU channels to internal RFIC ordinals and reference frequencies. Channels 87 and 88 are
 *          designated as the default 'A' and 'B' channels, respectively.
 */
static const ais_channel AIS_CHANNELS[] = {
    {18, 0, '?', 161.500},
    {78, 1, '?', 161.525},
    {19, 2, '?', 161.550},
    {79, 3, '?', 161.575},
    {20, 4, '?', 161.600},
    {80, 5, '?', 161.625},
    {21, 6, '?', 161.650},
    {81, 7, '?', 161.675},
    {22, 8, '?', 161.700},
    {82, 9, '?', 161.725},
    {23, 10, '?', 161.750},
    {83, 11, '?', 161.775},
    {24, 12, '?', 161.800},
    {84, 13, '?', 161.825},
    {25, 14, '?', 161.850},
    {85, 15, '?', 161.875},
    {26, 16, '?', 161.900},
    {86, 17, '?', 161.925},
    {27, 18, '?', 161.950},
    {87, 19, 'A', 161.975}, /**< Default channel A */
    {28, 20, '?', 162.000},
    {88, 21, 'B', 162.025}  /**< Default channel B */
};

/**
 * @brief Macro to convert an ITU channel number to its corresponding RFIC ordinal.
 * @param C The ITU channel number.
 * @return The RFIC channel ordinal corresponding to the given ITU channel.
 */
#define ITU_TO_ORDINAL(C) (C < 78 ? (C-18)*2 : (C-78)*2+1)

/**
 * @brief Macro to convert an RFIC ordinal to its corresponding ITU channel number.
 * @param O The RFIC channel ordinal.
 * @return The ITU channel number corresponding to the given ordinal.
 */
#define ORDINAL_TO_ITU(O) (AIS_CHANNELS[O].itu)

#endif /* AISCHANNELS_H_ */


/* -*-c++-*- */
