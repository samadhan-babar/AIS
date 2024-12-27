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
 * @file CommandProcessor.hpp
 * @brief Provides functionality to process incoming commands in the Marine Automatic Identification System firmware.
 * @details The `CommandProcessor` class handles the initialization and processing of commands
 *          and events for the transponder firmware. It also includes functionality to switch to bootloader mode,
 *          manage One-Time Programmable (OTP) data, and interact with the Command Line Interface (CLI).
 * @version 1.0A
 * @date September 2024
 * @author Peter Antypas
 * @company Uluka Systems Pvt Ltd
 */

#ifndef SRC_COMMANDPROCESSOR_HPP_
#define SRC_COMMANDPROCESSOR_HPP_

#include "Events.hpp"

/**
 * @brief CommandProcessor class for handling commands and system events.
 * @details This class processes system commands and events in the MAIANA transponder firmware.
 *          It provides methods to manage bootloader transitions, OTP data manipulation,
 *          and other command-based interactions through a CLI.
 */
class CommandProcessor : public EventConsumer
{
public:
  /**
   * @brief Retrieves the singleton instance of the CommandProcessor.
   * @return A reference to the singleton `CommandProcessor` instance.
   */
  static CommandProcessor &instance();

  /**
   * @brief Initializes the CommandProcessor.
   * @details Sets up the necessary components or state for processing commands and events.
   */
  void init();

  /**
   * @brief Processes an incoming event.
   * @details Handles the event passed to the `CommandProcessor` and triggers command execution based on the event type.
   * @param e The event to process.
   */
  void processEvent(const Event &e);

private:
  /**
   * @brief Constructor for CommandProcessor.
   * @details This constructor is private to ensure the singleton pattern is enforced,
   *          preventing external instantiation.
   */
  CommandProcessor();

  /**
   * @brief Processes a given command string.
   * @param cmd The command string to be processed.
   * @details Parses and executes the provided command string.
   */
  void processCommand(const char *cmd);

  /**
   * @brief Jumps to the bootloader mode.
   * @details Executes the necessary steps to transfer control to the firmware's bootloader.
   */
  void jumpToBootloader();

  /**
   * @brief Dumps One-Time Programmable (OTP) data.
   * @details Outputs the OTP data for review or debugging purposes.
   */
  void dumpOTPData();

  /**
   * @brief Writes data to the OTP memory.
   * @param cmd The command string containing the data to be written to OTP.
   * @details Interprets the provided command and writes the corresponding data to OTP memory.
   */
  void writeOTPData(const std::string &cmd);

  /**
   * @brief Enters the Command Line Interface (CLI) mode.
   * @details Switches the system to CLI mode for direct command interaction.
   */
  void enterCLIMode();
  /*
   * viod ais_ts_peyload  is transfer the peyload
   *
   *
   *
   *
   */

//  void ais_tx_peyload();
};

#endif /* SRC_COMMANDPROCESSOR_HPP_ */
