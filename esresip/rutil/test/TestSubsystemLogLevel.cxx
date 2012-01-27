/* ***********************************************************************
   Copyright 2006-2007 Estacado Systems, LLC. All rights reserved.
 *********************************************************************** */

#include "rutil/Logger.hxx"
#include "TestSubsystemLogLevel.hxx"

#define RESIPROCATE_SUBSYSTEM Subsystem::TEST

using namespace resip;

void 
Noisy::outputLogMessages()
{
   StackLog(<< "  STACK");
   DebugLog(<< "  DEBUG");
   InfoLog(<<  "  INFO");
   CritLog(<<  "  CRIT");
}
