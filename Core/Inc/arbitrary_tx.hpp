/*
 * arbitrary_tx.hpp
 *
 *  Created on: Dec 19, 2024
 *      Author: ubuntu
 */

#ifndef INC_ARBITRARY_TX_HPP_
#define INC_ARBITRARY_TX_HPP_




#include <stdint.h>
#include <stdbool.h>
#include "TXPacket.hpp"
#include "RXPacket.hpp"
#include "StationData.h"
#include "EventTypes.h"
#include "Events.hpp"
#include"TXScheduler.hpp"


// Constants

#define MAX_AODV_TX_PACKET_SIZE 256 //definition of max packet size


// AODV Route Table Entry


// AODV Request (RREQ) message format
typedef struct
{
    char msg;               // AODV message type (AODV_REQUEST)
                 // Source longitude
} arbitrary_tx_t;

// AODV Acknowledgement (RREP) message format


// AODV Error (RERR) message format

class arbitrary_tx
{
public:

	 /**
	   * @brief Retrieves the singleton instance of the AODVmesh.
	   * @return A reference to the singleton `AODVmesh` instance.
	   */
	  static arbitrary_tx &instance();

	  void init();


    // Function Prototypes


    arbitrary_tx_t abr_tx(const std::string &); //initiate a RREQ
    void arbitrary_tx_t_generate_rreq(uint32_t dest_mmsi, double latitude, double longitude); // Generate and send a Request (RREQ)
   // void arbitrary_tx_t_generate_rrep(AODV_rreq_t *rreq); // Generate and send an ACK in response to a Request (RREP)
   // void arbitrary_tx_t_generate_rerr(AODV_routing_table_entry_t *entry); // Generate and send an Error (RERR)
   // AODV_routing_table_entry_t *aodv_find_route(uint32_t dest_mmsi); // Find a route in the routing table
   // void arbitrary_tx_t_update_route_lifetime(AODV_routing_table_entry_t *entry, uint32_t lifetime); // Update route lifetime

    // Function to estimate distance in nautical miles
    double aodv_calculate_distance(double lat1, double lon1, double lat2, double lon2);



};

class arb_msg
{
public:

	arb_msg();
	~arb_msg();




  virtual bool decode(const arbitrary_tx_t &packet);

  virtual void encode(const arbitrary_tx_t & station, TXPacket & packet);



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

class arbitory : public arb_msg
{
public:
	uint8_t type;               // AODV message type (AODV_REQUEST)
	uint8_t reserved;			//reserved for future use.
	uint8_t hop_count;          // Number of hops from the source
	uint8_t req_id;            // Request ID
	uint32_t source_mmsi;         // Source MMSI
	uint32_t dest_mmsi;           // Destination MMSI
	float latitude;            // Source latitude
	float longitude;           // Source longitude


	arbitory();


  void encode(const AODV_rrep_t &data, TXPacket &packet);
};


class arb_msg_1 : public arb_msg
{
public:

	arb_msg_1();


  void encode(const AODV_rreq_t &data, TXPacket &packet);
};

class arb_msg_1A : public arb_msg
{
public:

	arb_msg_1A();


  void encode(const AODV_rerr_t &data, TXPacket &packet);
};







#endif /* INC_ARBITRARY_TX_HPP_ */
