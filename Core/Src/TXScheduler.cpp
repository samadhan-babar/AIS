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
 * @file TXScheduler.cpp
 * @brief Implementation of the TXScheduler class for AIS message scheduling and transmission control.
 *
 * The TXScheduler is responsible for managing and scheduling transmission of AIS messages
 * (Position Reports and Static Data) based on GPS data and system configuration.
 * It interacts with RadioManager to schedule transmissions and EventQueue to handle various events.
 *
 * @author
 * @date
 * @version 1.0
 */

#include "TXScheduler.hpp"

#include "EventQueue.hpp"

#include "AISMessages.hpp"

#include "AODV_mesh.hpp"

#include "TXPacket.hpp"

#include "config.h"

#include <vector>

#include <string>

#include <sstream>

#include <stdlib.h>

#include "RadioManager.hpp"

#include "ChannelManager.hpp"

#include "TXErrors.h"

#include "printf_serial.h"

#include "bsp/bsp.hpp"



using namespace std;

/**
 * @brief Singleton instance of the TXScheduler class.
 *
 * @return TXScheduler& Returns the single instance of TXScheduler.
 */
TXScheduler & TXScheduler::instance() {
    static TXScheduler __instance;
    return __instance;
}

/**
 * @brief Constructor for TXScheduler.
 *
 * Registers the TXScheduler as an observer for GPS_FIX_EVENT, CLOCK_EVENT, and INTERROGATION_EVENT.
 * Initializes default values for various transmission channels and timings.
 */
TXScheduler::TXScheduler() {
    EventQueue::instance().addObserver(this, GPS_FIX_EVENT | CLOCK_EVENT | INTERROGATION_EVENT);
    mPositionReportChannel = CH_87;
    mStaticDataChannel = CH_87;
    mUTC = 0;
    mAvgSpeed = 0.0;
    mLast18Time = 0;
    mLast24Time = 0;
}

/**
 * @brief Initializes the TXScheduler.
 *
 * Loads station data and checks if the station is in CLI boot mode.
 * Reports the transmission status.
 */
void TXScheduler::init() {
    if (Configuration::instance().readStationData(mStationData)) {
        DBG("Successfully loaded Station Data \r\n");
    } else {
        DBG("Failed to read Station Data !!!\r\n");
    }

    bool cliBootMode = * (uint32_t * ) BOOTMODE_ADDRESS == CLI_FLAG_MAGIC;
    if (!cliBootMode)
        reportTXStatus();
}


// replace code "samadhan

void TXScheduler::queueMessage24(VHFChannel channel)
{
    #if REPORT_TX_SCHEDULING
    char sentence[48];
    #endif

    if (mStationData.magic != STATION_DATA_MAGIC)
        return;

    TXPacket * p2 = TXPacketPool::instance().newTXPacket(channel);
    if (!p2) {
        #if REPORT_TX_SCHEDULING
        sprintf(sentence, "$PAISCHTX,24A,%d*", TX_ALLOC_ERROR);
        sendNMEASentence(sentence);
        #endif
        return;
    }

    AISMessage24A msg2;
    msg2.encode(mStationData, * p2);
    RadioManager::instance().scheduleTransmission(p2);

    TXPacket * p3 = TXPacketPool::instance().newTXPacket(channel);
    if (!p3) {
        #if REPORT_TX_SCHEDULING
        sprintf(sentence, "$PAISCHTX,24B,%d*", TX_ALLOC_ERROR);
        sendNMEASentence(sentence);
        #endif
        return;
    }

    AISMessage24B msg3;
    msg3.encode(mStationData, * p3);
    RadioManager::instance().scheduleTransmission(p3);
}









time_t TXScheduler::positionReportTimeInterval()
{
    if (mAvgSpeed < 2.0)
        return MAX_MSG_18_TX_INTERVAL;

    return MIN_MSG_18_TX_INTERVAL;
}


/**
 * @brief Destructor for TXScheduler.
 */
TXScheduler::~TXScheduler() {}

/**
 * @brief Reports the transmission status by sending a NMEA sentence.
 *
 * This method checks various flags to determine if transmission is allowed and reports it.
 */
void TXScheduler::reportTXStatus() {
    bool hwSwitchOff = bsp_is_tx_disabled();
    bool softSwitch = Configuration::instance().isTXEnabled();
    bool hasStation = true; // Set to true for now

    Event * e = EventPool::instance().newEvent(PROPR_NMEA_SENTENCE);
    if (!e)
        return;

    sprintf(e -> nmeaBuffer.sentence, "$PAITXCFG,%d,%d,%d,%d,%d*", bsp_is_tx_hardwired(), !hwSwitchOff, softSwitch, hasStation, softSwitch && hasStation);
    Utils::completeNMEA(e -> nmeaBuffer.sentence);
    EventQueue::instance().push(e);
}

/**
 * @brief Checks if transmission is allowed based on hardware and software switches.
 *
 * @return true if transmission is allowed, false otherwise.
 */
bool TXScheduler::isTXAllowed() {
    bool hwSwitchOff = bsp_is_tx_disabled();
    if (hwSwitchOff)
        return false;

    bool softSwitch = Configuration::instance().isTXEnabled();
    bool hasStation = Configuration::instance().isStationDataProvisioned();

    return softSwitch && hasStation;
}

/**
 * @brief Processes various system events (GPS fix, clock updates, and interrogations).
 *
 * This method handles the different events like GPS_FIX_EVENT, CLOCK_EVENT, and INTERROGATION_EVENT.
 * It schedules the appropriate AIS message transmissions based on the event type.
 *
 * @param e The event to process.
 */
void TXScheduler::processEvent(const Event & e) {
    switch (e.type) {
    case GPS_FIX_EVENT:
        mLastGPSFix = e.gpsFix;

        if (!ChannelManager::instance().channelsDetermined() || !RadioManager::instance().initialized() || mUTC == 0 || !isTXAllowed())
            return;

        static float alpha = 0.2;
        mAvgSpeed = mAvgSpeed * (1.0 - alpha) + mLastGPSFix.speed * alpha;

        if (mUTC - mLast18Time > positionReportTimeInterval()) {
            queueMessage18(mPositionReportChannel);
            mPositionReportChannel = RadioManager::instance().alternateChannel(mPositionReportChannel);
            mLast18Time = mUTC;
        }

        if (mUTC - mLast24Time > MSG_24_TX_INTERVAL) {
            queueMessage24(mStaticDataChannel);
            mStaticDataChannel = RadioManager::instance().alternateChannel(mStaticDataChannel);
            mLast24Time = mUTC;
        }
        break;

    case CLOCK_EVENT:
        if (mUTC == 0) {
            mLast18Time = e.clock.utc - MAX_MSG_18_TX_INTERVAL / 2;
            mLast24Time = e.clock.utc - MSG_24_TX_INTERVAL / 2;
        }
        mUTC = e.clock.utc;
        break;

    case INTERROGATION_EVENT:
        if (e.interrogation.messageType == 18)
            queueMessage18(e.interrogation.channel);
        if (e.interrogation.messageType == 24)
            queueMessage24(e.interrogation.channel);
        break;
//    case AODV_REQUEST:
//        queueAODVRREQ(mPositionReportChannel);
//        mPositionReportChannel = RadioManager::instance().alternateChannel(mPositionReportChannel);
//        break;
    default:
        break;
    }
}

/**
 * @brief Sends a custom NMEA sentence to the event queue.
 *
 * @param sentence The NMEA sentence to send.
 */
void TXScheduler::sendNMEASentence(const char * sentence) {
    Event * e = EventPool::instance().newEvent(PROPR_NMEA_SENTENCE);
    if (!e)
        return;

    strlcpy(e -> nmeaBuffer.sentence, sentence, sizeof e -> nmeaBuffer.sentence);
    Utils::completeNMEA(e -> nmeaBuffer.sentence);
    EventQueue::instance().push(e);
}

/**
 * @brief Queues an AIS message of type 18 (Position Report).
 *
 * If station data is valid, it creates a TXPacket and schedules transmission via the RadioManager.
 *
 * @param channel The VHF channel to use for the transmission.
 */
void TXScheduler::queueMessage18(VHFChannel channel) {
    #if REPORT_TX_SCHEDULING
    char sentence[48];
    #endif

    if (mStationData.magic != STATION_DATA_MAGIC)
        return;

    TXPacket * p1 = TXPacketPool::instance().newTXPacket(channel);
    if (!p1) {
        #if REPORT_TX_SCHEDULING
        sprintf(sentence, "$PAISCHTX,18,%d*", TX_ALLOC_ERROR);
        sendNMEASentence(sentence);
        #endif
        return;
    }

    AISMessage18 msg;
    msg.latitude = mLastGPSFix.lat;
    msg.longitude = mLastGPSFix.lng;
    msg.sog = mLastGPSFix.speed;
    msg.cog = mLastGPSFix.cog;
    msg.utc = mLastGPSFix.utc;
    msg.encode(mStationData, * p1);

    RadioManager::instance().scheduleTransmission(p1);
}

void TXScheduler::queueAODVRREQ(VHFChannel channel, const AODV_rreq_t &RREQrequest) {
    #if REPORT_TX_SCHEDULING
    char sentence[48];
    #endif

    if (mStationData.magic != STATION_DATA_MAGIC)
        return;

    TXPacket * p1 = TXPacketPool::instance().newTXPacket(channel);
    if (!p1) {
        #if REPORT_TX_SCHEDULING
        sprintf(sentence, "$PAISCHTX,AODV,%d*", TX_ALLOC_ERROR);
        sendNMEASentence(sentence);
        #endif
        return;
    }

    AODVMessageRREQ msg;

    msg.encode(RREQrequest, * p1);

    RadioManager::instance().scheduleTransmission(p1);
}



void TXScheduler::arbitrary_tx(VHFChannel channel, const arbitrary_tx_t & RREQrequest)  //(VHFChannel channel, const arbitrary_tx_t &);
 {
    #if REPORT_TX_SCHEDULING
    char sentence[48];
    #endif

    if (mStationData.magic != STATION_DATA_MAGIC)
        return;

    TXPacket * p1 = TXPacketPool::instance().newTXPacket(channel);
    if (!p1) {
        #if REPORT_TX_SCHEDULING
        sprintf(sentence, "$PAISCHTX,arbitrary ,%d*", TX_ALLOC_ERROR);
        sendNMEASentence(sentence);
        #endif
        return;
    }


 }


/**
 * @brief Queues an AIS message of type 24 (Static Data Report).
 *
 * This function handles both the 24A and 24B types of the message.
 *
 * @param channel The VHF channel to use for the transmission.
 */
//void TXScheduler::queueMessage24(VHFChannel channel)
//{
//    #if REPORT_TX_SCHEDULING
//    char sentence[48];
//    #endif
//
//    if (mStationData.magic != STATION_DATA_MAGIC)
//        return;
//
//    TXPacket * p2 = TXPacketPool::instance().newTXPacket(channel);
//    if (!p2) {
//        #if REPORT_TX_SCHEDULING
//        sprintf(sentence, "$PAISCHTX,24A,%d*", TX_ALLOC_ERROR);
//        sendNMEASentence(sentence);
//        #endif
//        return;
//    }
//
//    AISMessage24A msg2;
//    msg2.encode(mStationData, * p2);
//    RadioManager::instance().scheduleTransmission(p2);
//
//    TXPacket * p3 = TXPacketPool::instance().newTXPacket(channel);
//    if (!p3) {
//        #if REPORT_TX_SCHEDULING
//        sprintf(sentence, "$PAISCHTX,24B,%d*", TX_ALLOC_ERROR);
//        sendNMEASentence(sentence);
//        #endif
//        return;
//    }
//
//    AISMessage24B msg3;
//    msg3.encode(mStationData, * p3);
//    RadioManager::instance().scheduleTransmission(p3);
//}
//
//

/**
 * @brief Determines the time interval for position reports based on the vessel's speed.
 *
 * If the speed is less than 2 knots, a longer interval is used; otherwise, a shorter interval is applied.
 *
 * @return time_t The time interval for position report transmissions.
 */
