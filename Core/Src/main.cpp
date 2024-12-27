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

#include "config.h"
#include "RadioManager.hpp"
#include "RXPacketProcessor.hpp"
#include "DataTerminal.hpp"
#include "TXScheduler.hpp"
#include "GPS.hpp"
#include "LEDManager.hpp"
#include "CommandProcessor.hpp"
#include "bsp/bsp.hpp"
#include "AODV_mesh.hpp"



void jump_to_bootloader()
{
  typedef void (*pFunction)(void);
  pFunction systemBootloader;

  /**
   * System bootloader for L4xx series resides at 0x1fff0000,
   * so the first 4 bytes contain the stack pointer and the next 4 contain the
   * program counter
   */
  systemBootloader = (pFunction) (*((uint32_t *)(0x1fff0004)));
  uint32_t *pp = (uint32_t*)0x1fff0000;
  uint32_t msp = *pp;
  __set_MSP(msp);

  // That's it, jump!
  systemBootloader();
}

void mainLoop()
{
  bool cliBootMode = *(uint32_t*)BOOTMODE_ADDRESS == CLI_FLAG_MAGIC;

  EventPool::instance().init();
  EventQueue::instance().init();
  Configuration::instance().init();
  DataTerminal::instance().init();
  CommandProcessor::instance().init();
  AODVmesh::instance().init();
  RXPacketProcessor packetProcessor;
  GPS::instance().init();
  TXPacketPool::instance().init();
  TXScheduler::instance().init();
  RadioManager::instance().init();
  LEDManager::instance().init();

  if ( !cliBootMode )
    {
      if ( RadioManager::instance().initialized() )
        {
          GPS::instance().enable();
          RadioManager::instance().start();
        }
    }
  else
    {
//      DataTerminal::instance().write("\r\n\r\nCLI mode. Send the 'reboot' command or cycle power to exit.\r\n");
    }

  *(uint32_t*)BOOTMODE_ADDRESS = 0;

#if ENABLE_WDT
  bsp_start_wdt();
#endif



  while (1)
    {
      EventQueue::instance().dispatch();
#if ENABLE_WDT
      bsp_refresh_wdt();
#endif
      __WFI();
    }
}


int main(void)
{
  if ( *(uint32_t*)BOOTMODE_ADDRESS == DFU_FLAG_MAGIC )
    {
      *(uint32_t*)BOOTMODE_ADDRESS = 0;
      jump_to_bootloader();
    }

  // This is for debugging imprecise bus faults
  //*(uint8_t *)0xe000ed08 |= 2;

  /*************************|   TIP : 2 Unique Device ID 96 bit   |***********************/
  uint32_t (*unique_id_1) = (uint32_t*)(0x1FFF7590 ); // BASE address
  uint32_t (*unique_id_2) = (uint32_t*)(0x1FFF7594 ); // BASE address + 0x04 offset
  uint32_t (*unique_id_3) = (uint32_t*)(0x1FFF7598 ); // BASE address + 0x08 0ffset
  /***************************************************************************************/
  (*unique_id_3>>24) & 0xFF;
  (*unique_id_3>>16) & 0xFF;
  (*unique_id_3>>8) & 0xFF;
  (*unique_id_3) & 0xFF;

  (*unique_id_2>>24) & 0xFF;
  (*unique_id_2>>16) & 0xFF;
  (*unique_id_2>>8) & 0xFF;
  (*unique_id_2) & 0xFF;

  (*unique_id_1>>24) & 0xFF;
  (*unique_id_1>>16) & 0xFF;
  (*unique_id_1>>8) & 0xFF;
  (*unique_id_1) & 0xFF;

  bsp_hw_init();
  mainLoop();
  return 1;
}
