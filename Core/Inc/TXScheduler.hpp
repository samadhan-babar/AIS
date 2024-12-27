/*
 * TXScheduler.hpp
 *
 *  Created on: Dec 12, 2015
 *      Author: peter
 */

#ifndef TXSCHEDULER_HPP_
#define TXSCHEDULER_HPP_

#include "Events.hpp"
#include  <time.h>
#include "ObjectPool.hpp"
#include "AISChannels.h"
#include "Configuration.hpp"
#include "AODV_mesh.hpp"
#include "arbitrary_tx.hpp"



class TXScheduler : public EventConsumer
{
public:
  static TXScheduler &instance();
  void init();
  void processEvent(const Event &event);
  void startTXTesting();
  void queueMessage18(VHFChannel channel);
  void queueMessage24(VHFChannel channel);
  void queueAODVRREQ(VHFChannel channel, const AODV_rreq_t &);
  void arbitrary_tx(VHFChannel channel, const arbitrary_tx_t & RREQrequest);  //(VHFChannel channel, const arbitrary_tx_t &);



  void reportTXStatus();
  bool isTXAllowed();
private:
  TXScheduler ();
  virtual ~TXScheduler ();
  time_t positionReportTimeInterval();
  void sendNMEASentence(const char *sentence);
private:
  VHFChannel mPositionReportChannel;
  VHFChannel mStaticDataChannel;
  time_t mUTC;
  time_t mLast18Time;
  time_t mLast24Time;
  float mAvgSpeed = 0.0f;
  StationData mStationData = {0};
  AODV_rreq_t AODVRREQrequest = {0};
  GPSFix mLastGPSFix;
};

#endif /* TXSCHEDULER_HPP_ */
