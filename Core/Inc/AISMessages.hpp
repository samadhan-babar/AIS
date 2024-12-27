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
 * @file AISMessages.hpp
 * @brief Defines AIS message types and encoding/decoding logic for the Marine Automatic Identification System firmware.
 * @details This file provides the core AIS message handling classes, including encoding and decoding functions for
 *          various AIS message types such as position reports and station metadata.
 * @version 1.0A
 * @date September 2024
 * @author Peter Antypas
 * @company Uluka Systems Pvt Ltd
 */

#ifndef AISMESSAGES_HPP_
#define AISMESSAGES_HPP_

#include "TXPacket.hpp"
#include "RXPacket.hpp"
#include <time.h>
#include <string>
#include "StationData.h"

// These are the AIS messages that this unit will actually work with

/**
 * @brief Default communication state for AIS message 18, used by all class B "CS" stations.
 * Because Class B “CS” does not use any Communication state information, the communication state
 * field in Message 18 should be filled with the default value “1100000000000000110” and the
 * communication state selector flag field filled with “1”. This is because a Class B” CS” station
 * by default reports sync state 3 and does not report “number of received stations” .Therefore
 * it will not be used as sync source for other stations.
 * Refer - Section 4.3.3.5 of Rec. ITU-R M.1371-4
 */
#define DEFAULT_COMM_STATE 0b1100000000000000110

/**
 * @brief Base class for handling AIS messages.
 * @details Provides common attributes and methods for encoding and decoding AIS messages.
 *          Derived classes implement specific AIS message types.
 */
class AISMessage
{
public:
  /**
   * @brief Constructor for AISMessage.
   */
  AISMessage();

  /**
   * @brief Destructor for AISMessage.
   */
  virtual ~AISMessage();

  /**
   * @brief Decodes a received AIS packet into this message.
   * @param packet The RXPacket containing the received AIS message data.
   * @return True if decoding was successful, false otherwise.
   */
  virtual bool decode(const RXPacket &packet);

  /**
   * @brief Encodes this AIS message into a transmit packet.
   * @param station The station data to be included in the message.
   * @param packet The TXPacket to store the encoded message.
   */
  virtual void encode(const StationData &station, TXPacket &packet);

  /**
   * @brief Gets the type of this AIS message.
   * @return The message type as a uint8_t.
   */
  uint8_t type() const;

  /**
   * @brief Gets the repeat indicator value of this AIS message.
   * @return The repeat indicator value.
   */
  uint8_t repeatIndicator() const;

  /**
   * @brief Gets the MMSI (Maritime Mobile Service Identity) of this AIS message.
   * @return The MMSI as a uint32_t.
   */
  uint32_t mmsi() const;

protected:
  uint8_t mType;    /**< AIS message type. */
  uint8_t mRI;      /**< Repeat indicator. */
  uint32_t mMMSI;   /**< MMSI (Maritime Mobile Service Identity). */

protected:
  void appendCRC(uint8_t *buff, uint16_t &size);
  void addBits(uint8_t *buff, uint16_t &size, uint32_t value, uint8_t numBits);
  void putBits(uint8_t *buff, uint32_t value, uint8_t numBits);
  void addString(uint8_t *buff, uint16_t &size, const std::string &name, uint8_t maxChars);
  void finalize(uint8_t *buff, uint16_t &size, TXPacket &packet);

private:
  void bitStuff(uint8_t *buff, uint16_t &size);
  void constructHDLCFrame(uint8_t *buff, uint16_t &size);
  void nrziEncode(uint8_t *buff, uint16_t &size, TXPacket &packet);
  void payloadToBytes(uint8_t *bitVector, uint16_t numBits, uint8_t *bytes);
  void reverseEachByte(uint8_t *bitVector, uint16_t numBits);
};

/**
 * @brief Class for handling AIS message 15 (interrogation).
 * @details This message type is received from shore stations and decoded.
 */
class AISMessage15: public AISMessage
{
public:
  /**
   * @brief Structure for storing interrogation target details.
   */
  typedef struct {
    uint32_t mmsi;       /**< MMSI of the target station. */
    uint8_t  messageType; /**< Interrogation message type. */
  } InterrogationTarget;

  InterrogationTarget targets[3]; /**< Array of interrogation targets. */

  /**
   * @brief Constructor for AISMessage15.
   */
  AISMessage15();

  /**
   * @brief Decodes a received packet containing AIS message 15.
   * @param packet The RXPacket containing the received AIS message 15 data.
   * @return True if decoding was successful, false otherwise.
   */
  bool decode(const RXPacket &packet);
};

/**
 * @brief Class for handling AIS message 18 (position report for class B vessels).
 * @details This message type is transmitted by class B AIS transponders, containing position, speed, and other details.
 */
class AISMessage18 : public AISMessage
{
public:
  float latitude;   /**< Latitude of the vessel. */
  float longitude;  /**< Longitude of the vessel. */
  float sog;        /**< Speed over ground (SOG) in knots. */
  float cog;        /**< Course over ground (COG) in degrees. */
  time_t utc;       /**< UTC timestamp of the report. */

  /**
   * @brief Constructor for AISMessage18.
   */
  AISMessage18();

  /**
   * @brief Encodes AIS message 18 into a transmit packet.
   * @param data The station data to be included in the message.
   * @param packet The TXPacket to store the encoded message.
   */
  void encode(const StationData &data, TXPacket &packet);
};

/**
 * @brief Class for handling AIS message 24A (station metadata part A).
 * @details This message type transmits part A of the station's metadata.
 */
class AISMessage24A : public AISMessage
{
public:
  /**
   * @brief Constructor for AISMessage24A.
   */
  AISMessage24A();

  /**
   * @brief Encodes AIS message 24A into a transmit packet.
   * @param data The station data to be included in the message.
   * @param packet The TXPacket to store the encoded message.
   */
  void encode(const StationData &data, TXPacket &packet);
};

/**
   * @brief Class for handling AIS message 24B (station metadata part B).
   * @details This message type transmits part B of the station's metadata.
   */
class AISMessage24B : public AISMessage
{
public:
  /**
   * @brief Constructor for AISMessage24B.
   */
  AISMessage24B();

  /**
   * @brief Encodes AIS message 24B into a transmit packet.
   * @param data The station data to be included in the message.
   * @param packet The TXPacket to store the encoded message.
   */
  void encode(const StationData &data, TXPacket &packet);
};

/*
 *   @brief AIS_peyload class defind and inheritance AISMessage class in public mode
 *
 *
 * */

class AIS_peyload :public AISMessage
{
	  typedef struct {
	    uint32_t mmsi;       /**< MMSI of the target station. */
	    uint8_t  messageType; /**< Interrogation message type. */
	  } InterrogationTarget;

	  InterrogationTarget targets[3]; /**< Array of interrogation targets. */

	/*
	 *
	 *
	 *  this  constrator and deconstractor AIS_peyload
	 */

	AIS_peyload();
	~AIS_peyload();

	void encode(const StationData &data, TXPacket &packet);
	bool decode(const RXPacket &packet);
};

#endif /* AISMESSAGES_HPP_ */
