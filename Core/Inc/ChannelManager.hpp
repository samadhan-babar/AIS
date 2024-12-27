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
 * @file ChannelManager.hpp
 * @brief Manages the active AIS VHF channels in the Marine Automatic Identification System firmware.
 * @details This class is responsible for determining and managing the two AIS channels (A and B) that the device uses for communication.
 *          It handles the assignment of channels based on events and maintains state information for channel A and channel B.
 * @version 1.0A
 * @date September 2024
 * @author Peter Antypas
 * @company Uluka Systems Pvt Ltd
 */

#ifndef CHANNELMANAGER_HPP_
#define CHANNELMANAGER_HPP_

#include "AISChannels.h"
#include "Events.hpp"

/**
 * @brief ChannelManager is responsible for managing the current AIS channels.
 * @details This class inherits from `EventConsumer` to handle system events related to channel management.
 *          It provides access to the two active AIS channels and monitors whether the channels have been determined.
 */
class ChannelManager : public EventConsumer
{
public:
  /**
   * @brief Retrieves the singleton instance of the ChannelManager.
   * @return A reference to the singleton `ChannelManager` instance.
   */
  static ChannelManager &instance();

  /**
   * @brief Destructor for ChannelManager.
   */
  virtual ~ChannelManager();

  /**
   * @brief Processes events related to channel management.
   * @details This method handles system events passed to the ChannelManager, determining their relevance to AIS channel management.
   * @param e The event to be processed.
   */
  void processEvent(const Event &e);

  /**
   * @brief Gets the current AIS channel A.
   * @return A reference to the `ais_channel` representing AIS channel A.
   */
  const ais_channel & channelA();

  /**
   * @brief Gets the current AIS channel B.
   * @return A reference to the `ais_channel` representing AIS channel B.
   */
  const ais_channel &channelB();

  /**
   * @brief Checks if both AIS channels have been determined.
   * @return True if both channels have been determined, false otherwise.
   */
  bool channelsDetermined();

private:
  /**
   * @brief Private constructor for ChannelManager to implement the singleton pattern.
   */
  ChannelManager();

  uint8_t mChannelA; /**< Holds the ordinal value of AIS channel A. */
  uint8_t mChannelB; /**< Holds the ordinal value of AIS channel B. */
};

#endif /* CHANNELMANAGER_HPP_ */
