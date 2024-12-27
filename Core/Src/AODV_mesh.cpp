/*
 * AODV_mesh.cpp
 *
 *  Created on: Sep 25, 2024
 *      Author: ishan
 */

#include "AODV_mesh.hpp"
#include "StationData.h"
#include "EventTypes.h"
#include "Events.hpp"
#include "EventQueue.hpp"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Configuration.hpp"
#include <bsp/bsp.hpp>
#include "GPS.hpp"

AODVmesh& AODVmesh::instance() {
	static AODVmesh __instance;
	return __instance;
}

void AODVmesh::init() {

}

AODVMessage::AODVMessage() {

}

AODVMessage::~AODVMessage() {
}

void AODVMessage::encode(const AODV_rreq_t &station, TXPacket &)
{
}

bool AODVMessage::decode(const RXPacket&) {
	// The base class method should never be called
	ASSERT(false);

	// This will never execute but the compiler is not smart enough to know it
	return false;
}

void AODVMessage::addBits(uint8_t *bitVector, uint16_t &size, uint32_t value,
		uint8_t numBits) {
	ASSERT(numBits > 0 && numBits <= 32);
	uint16_t pos = size;
	for (uint8_t bit = 0; bit < numBits; ++bit, value >>= 1) {
		bitVector[pos + numBits - bit - 1] = value & 1;
	}

	size += numBits;
}

void AODVMessage::putBits(uint8_t *bitVector, uint32_t value, uint8_t numBits) {
	// This is used for HDLC framing
	uint16_t pos = 0;
	for (uint8_t bit = 0; bit < numBits; ++bit, value >>= 1) {
		bitVector[pos++] = value & 0x01;
	}
}

void AODVMessage::reverseEachByte(uint8_t *bitVector, uint16_t size) {
	ASSERT(size % 8 == 0);
	for (uint16_t i = 0; i < size; i += 8) {
		for (uint8_t j = 0; j < 4; ++j) {
			swap(bitVector[i + j], bitVector[i + 7 - j]);
		}
	}
}

void AODVMessage::addString(uint8_t *bitVector, uint16_t &size,
		const string &value, uint8_t maxChars) {
	ASSERT(value.length() <= maxChars);
	ASSERT(maxChars < 30); // There should be no application for such long strings here
	char s[30];
	memset(s, 0, sizeof s);
	strncpy(s, value.c_str(), value.length());

	uint8_t buffer[32];
	for (uint8_t c = 0; c < maxChars; ++c) {
		uint8_t byte = s[c] >= 64 ? s[c] - 64 : s[c];
		buffer[c] = byte;
	}

	for (uint8_t c = 0; c < maxChars; ++c)
		addBits(bitVector, size, buffer[c], 6);
}

void AODVMessage::payloadToBytes(uint8_t *bitVector, uint16_t numBits,
		uint8_t *byteArray) {
	for (uint16_t i = 0; i < numBits; i += 8) {
		uint8_t byte = 0;
		for (uint8_t b = 0; b < 8; ++b) {
			byte |= (bitVector[i + b] << b);
		}
		byteArray[i / 8] = byte;
	}
}

void AODVMessage::finalize(uint8_t *payload, uint16_t &size, TXPacket &packet) {
	// Nothing we send exceeds 256 bits, including preambles and such. 40 bytes is more than enough.
	uint8_t bytes[40];

	// CRC-CCITT calculation
	payloadToBytes(payload, size, bytes);
	uint16_t crc = Utils::crc16(bytes, size / 8);
	uint8_t crcL = crc & 0x00ff;
	uint8_t crcH = (crc & 0xff00) >> 8;
	addBits(payload, size, crcL, 8);
	addBits(payload, size, crcH, 8);
	payloadToBytes(payload, size, bytes);

	// Encoding for transmission
	reverseEachByte(payload, size);
	bitStuff(payload, size);
	constructHDLCFrame(payload, size);
	nrziEncode(payload, size, packet);
	packet.pad();
}

void AODVMessage::bitStuff(uint8_t *buff, uint16_t &size) {
	uint16_t numOnes = 0;
	for (uint16_t i = 0; i < size; ++i) {
		switch (buff[i]) {
		case 0:
			numOnes = 0;
			break;
		case 1:
			++numOnes;
			if (numOnes == 5) {
				// Insert a 0 right after this one
				memmove(buff + i + 2, buff + i + 1, size - i - 1);
				buff[i + 1] = 0;
				++size;
			}
			break;
		default:
			ASSERT(false)
			;
		}
	}
}

void AODVMessage::constructHDLCFrame(uint8_t *buff, uint16_t &size) {

	// Make room for 35 bits at the front
	memmove(buff + 35, buff, size);
	size += 35;
	putBits(buff, 0xFF, 3);            // 3 ramp bits. That's all we can afford.
	putBits(buff + 3, 0b010101010101010101010101, 24); // 24 training bits (ramp will actually continue during the first 1-2)
	putBits(buff + 27, 0x7e, 8);                          // HDLC start flag

	// Now append the end marker and ramp-down bits
	addBits(buff, size, 0x7e, 8);                       // HDLC stop flag
	addBits(buff, size, 0x00, 3);                       // Ramp down
}

void AODVMessage::nrziEncode(uint8_t *buff, uint16_t &size, TXPacket &packet) {
	uint8_t prevBit = 1;        // Arbitrarily starting with 1
	packet.addBit(prevBit);

	for (uint16_t i = 0; i < size; ++i) {
		if (buff[i] == 0) {
			packet.addBit(!prevBit);
			prevBit = !prevBit;
		} else {
			packet.addBit(prevBit);
		}
	}

	// The TXPacket is now populated with the sequence of bits that need to be sent
}

///////////////////////////////////////////////////////////////////////////////
//
// AODV RREQ Message
//
///////////////////////////////////////////////////////////////////////////////

AODVMessageRREQ::AODVMessageRREQ() {
}

void AODVMessageRREQ::encode(const AODV_rreq_t &message_packet,
		TXPacket &packet) {
	/* Following information is contained in an AODV Request
	 * bit length are mentioned in brackets
	 * type (8)
	 * reserved (8)
	 * hop_count (8)
	 * req_id (8)
	 * source_mmsi (32)
	 * dest_mmsi (32)
	 * latitude (32) Converted to uint32
	 * longitude (32) Converted to unit32
	 */

	AODVMessage::encode(message_packet, packet);

	// TODO: Perhaps this shouldn't live on the stack?
	uint8_t payload[MAX_AODV_TX_PACKET_SIZE];
	uint16_t size = 0;
	uint32_t value;

	packet.setMessageType("28");

	value = message_packet.type;
	addBits(payload, size, value, 8);   // Message Type Indicator

	value = message_packet.reserved;
	addBits(payload, size, value, 8);   // Reserved for future use

	value = message_packet.source_mmsi;
	addBits(payload, size, value, 32);  // Source MMSI

	value = message_packet.dest_mmsi;
	addBits(payload, size, value, 32);  // Destination MMSI

	value = message_packet.hop_count;
	addBits(payload, size, value, 8);   // Hop Count

	value = message_packet.req_id;
	addBits(payload, size, value, 8);  // Request Id

	value = Utils::coordinateToUINT32(message_packet.longitude);
	addBits(payload, size, value, 32);  // Longitude

	value = Utils::coordinateToUINT32(message_packet.latitude);
	addBits(payload, size, value, 32);  // Latitude

#if DEV_MODE
  ASSERT(size == 160);
#endif

	finalize(payload, size, packet);
}

AODV_rreq_t AODVmesh::aodv_whois(const std::string &s) {

	AODV_rreq_t RREQ={0};

	string params = s.substr(8);

	if (params.empty())
		return RREQ;

	// Check if the input string is exactly 9 characters
	if (params.length() != 9) {
		// Invalid MMSI length
		// Return an empty RREQ or handle the error (alternatively, throw an exception in a more robust design)
		return RREQ;
	}

	// Check if all characters are digits
	for (char c : params) {
		if (!isdigit(c)) {
			// Invalid MMSI (non-digit character found)
			// Return an empty RREQ or handle the error
			return RREQ;
		}
	}

	// Convert the string to a number
	uint32_t dest_mmsi = strtoul(params.c_str(), nullptr, 10);

	// Check if the MMSI number is within the valid range
	if (dest_mmsi < 1 || dest_mmsi > 999999999) {
		// Invalid MMSI number
		// Return an empty RREQ or handle the error
		return RREQ;
	}

	// If all checks pass, the MMSI is valid
	RREQ.dest_mmsi = dest_mmsi;

	// read station data to get own station information
	StationData d;
	bsp_read_station_data(&d);

	RREQ.source_mmsi = d.mmsi;  // Retrieve the MMSI number from the station data

	// get own position from GPS data
	RREQ.latitude = GPS::instance().lat();
	RREQ.longitude = GPS::instance().lng();  // This should be longitude instead of latitude

	// Set other fields of the RREQ message
	RREQ.type = 0;         // Default message type
	RREQ.hop_count = 0;    // This is the first message, so hop count is 0
	RREQ.req_id = 0;       // Default request ID
	RREQ.reserved = 0;     // Reserved field, set to 0

	// Return the generated RREQ message
	return RREQ;
}

