//------------------------------------------------------------------------------
//   TumanakoVC - Electric Vehicle and Motor control software
//   Copyright (C) 2009 Philip Court <philip@greenstage.co.nz>
//
//   This file is part of TumanakoVC.
//
//   TumanakoVC is free software: you can redistribute it and/or modify
//   it under the terms of the GNU Lesser General Public License as published
//   by the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   TumanakoVC is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU Lesser General Public License for more details.
//
//   You should have received a copy of the GNU Lesser General Public License
//   along with TumanakoVC.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#ifndef TUMANAKO_INVERTER_HPP
#define TUMANAKO_INVERTER_HPP

#include "tumanako_global.hpp"
#include "STM32_interface.hpp"

/*----------------------------- Private typedef ------------------------------*/
typedef enum {FWD, REV, NET} Direction_T;
typedef enum {RunState_IDLE, RunState_READY, RunState_RUN, RunState_ERROR} RunState_T;
typedef enum {PreCharge_OK, PreCharge_PHASE1_ERROR, PreCharge_PHASE2_ERROR, PreCharge_CONTACTOR_FEEDBACK_ERROR} PreCharge_T;

/*----------------------------- Public define -------------------------------*/
#define TK_FORWARD true
#define TK_REVERSE false
#define TK_ON true
#define TK_OFF false

class TumanakoInverter {

 public:
//Constructor
  TumanakoInverter();

// Get status of the contactors (requires ignition on first)
  bool getContactorsEngaged(void);

// Get status of the driving direction
// TRUE = forwards
// FALSE = backwards
// should have double logic (i.e. two bits to check?)  If not consistant shut
// down or stay in fwd...
  Direction_T getDirection(void);

  void checkVehcileControlInputs();
  void doIt(void);
  void stateMachineDo(void);
  
  void dashboard();

 private:
  void init();
  PreCharge_T doPrecharge(void);
  void flash();  //Flash the green RUN LED
  void delay(int multiple); //delay (multiple times)
  void turnOff();  //handle IGN key off
  bool busVoltageOK(void);

  unsigned long mLoopTime; 
  MyTimer mRunTimer;
  signed short mMotorRPM;
  signed short mAbsoluteMotorRPM;
  unsigned short mFlux;  //Rotor flux set point
  short mAcceleratorRef;  //actually this represents +ve and -ve torque
  short mAcceleratorRefSmooth; //smooths value we are sending to IFOC
  short mAccelRetardCount;  //limits max requested torque (can occur during over temperature events etc)
  short mSpeedBasedTorqueLimit;  //limits max torque based on motor RPM
  unsigned short mRawAcceleratorRef;  //from ADC
  long mBusCurrent;
  long mBusCurrentAvg;
  Direction_T mDirection;
  short mMotorTemp;
  short mPowerStageTemp;
  bool  mTorqueLimitOccured;
  STM32Interface mSTM32;
  RunState_T mState;
  bool mOldIGN; //used for debounce TODO update to use filter
  bool mOldStart; //used for debounce TODO update to use filter
  short mPrevAccRef; //used to detect violent direction change
  bool mFlashRunLED;  //TODO encapsulate flash logic
  short mAcceleratorMIN;  //Min expected value
  short mAcceleratorMAX;  //Max expected value
  short mCountMinThrottleError;
  short mCountMaxThrottleError;
  unsigned short mMotorStallError;
  unsigned short mDirectionError;  //Digital Input logic
  MyTimer mLastFlashTimer;
  //string mErrorString;  //TODO used to report error when inverter shutdown initiated due to falt
};

#endif // TUMANAKO_INVERTER_HPP
