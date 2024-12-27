// AODV_mesh.hpp
#ifndef AODV_MESH_HPP
#define AODV_MESH_HPP

#include <stdint.h>
#include <stdbool.h>
#include "TXPacket.hpp"
#include "RXPacket.hpp"
#include "StationData.h"
#include "EventTypes.h"
#include "Events.hpp"


// Constants
#define AODV_NET_DIAMETER 35         // Maximum number of hops
#define AODV_REQ_RETRIES 2           // Number of retries for request
#define AODV_REQ_TIMEOUT 2           // Timeout for request in seconds
#define AODV_ACTIVE_ROUTE_TIMEOUT 30000  // Timeout for active route (in milliseconds)
#define AODV_ROUTE_DELETE_TIMEOUT 1100   // Timeout for route deletion
#define AODV_LOCAL_ADD_TTL 2         // Local add TTL

#define MAX_AODV_TX_PACKET_SIZE 256 //definition of max packet size


// AODV Route Table Entry
typedef struct AODV_routing_table_entry {
	uint32_t destination_mmsi;    // Destination MMSI
	uint32_t next_hop_mmsi;       // Next hop MMSI
    uint32_t hop_count;         // Number of hops to the destination
    uint32_t lifetime;          // Route expiration or deletion time
    bool valid;                 // Valid route flag
    struct AODV_routing_table_entry *next;  // Pointer to the next route table entry (linked list)
} AODV_routing_table_entry_t;

// AODV Request (RREQ) message format
typedef struct {
    uint8_t type;               // AODV message type (AODV_REQUEST)
    uint8_t reserved;
    uint8_t hop_count;          // Number of hops from the source
    uint8_t req_id;            // Request ID
    uint32_t source_mmsi;         // Source MMSI
    uint32_t dest_mmsi;           // Destination MMSI
    float latitude;            // Source latitude
    float longitude;           // Source longitude
} AODV_rreq_t;

// AODV Acknowledgement (RREP) message format
typedef struct {
    uint8_t type;               // AODV message type (AODV_ACK)
    uint8_t reserved;
    uint8_t hop_count;          // Number of hops from the destination
    uint32_t dest_mmsi;           // Destination MMSI
    uint32_t origin_mmsi;         // Originator MMSI (source of the request)
    float latitude;            // Destination latitude
    float longitude;           // Destination longitude
} AODV_rrep_t;

// AODV Error (RERR) message format
typedef struct {
    uint8_t type;               // AODV message type (AODV_ERROR)
    uint8_t reserved;
    uint8_t dest_count;         // Number of unreachable destinations
    uint32_t *unreachable_dest;   // Array of unreachable destination MMSIs
} AODV_rerr_t;

class AODVmesh
{
public:

	 /**
	   * @brief Retrieves the singleton instance of the AODVmesh.
	   * @return A reference to the singleton `AODVmesh` instance.
	   */
	  static AODVmesh &instance();

	  void init();


    // Function Prototypes
    void aodv_init(); // Initialize the AODV protocol
    void aodv_handle_rreq(AODV_rreq_t *rreq); // Handle an incoming Request message (RREQ)
    void aodv_handle_rrep(AODV_rrep_t *rrep); // Handle an incoming Acknowledgement message (RREP)
    void aodv_handle_rerr(AODV_rerr_t *rerr); // Handle an incoming Error message (RERR)

    AODV_rreq_t aodv_whois(const std::string &); //initiate a RREQ
    void aodv_generate_rreq(uint32_t dest_mmsi, double latitude, double longitude); // Generate and send a Request (RREQ)
    void aodv_generate_rrep(AODV_rreq_t *rreq); // Generate and send an ACK in response to a Request (RREP)
    void aodv_generate_rerr(AODV_routing_table_entry_t *entry); // Generate and send an Error (RERR)
    AODV_routing_table_entry_t *aodv_find_route(uint32_t dest_mmsi); // Find a route in the routing table
    void aodv_update_route_lifetime(AODV_routing_table_entry_t *entry, uint32_t lifetime); // Update route lifetime

    // Function to estimate distance in nautical miles
    double aodv_calculate_distance(double lat1, double lon1, double lat2, double lon2);



};

class AODVMessage
{
public:

	AODVMessage();


  virtual ~AODVMessage();

  virtual bool decode(const RXPacket &packet);

  virtual void encode(const AODV_rreq_t &station, TXPacket &packet);



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

class AODVMessageRREP : public AODVMessage
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


	AODVMessageRREP();


  void encode(const AODV_rrep_t &data, TXPacket &packet);
};


class AODVMessageRREQ : public AODVMessage
{
public:

	AODVMessageRREQ();


  void encode(const AODV_rreq_t &data, TXPacket &packet);
};

class AODVMessageRERR : public AODVMessage
{
public:

	AODVMessageRERR();


  void encode(const AODV_rerr_t &data, TXPacket &packet);
};



#endif // AODV_MESH_HPP
