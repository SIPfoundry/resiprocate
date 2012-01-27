/* ***********************************************************************
   Copyright 2006-2007 Estacado Systems, LLC. All rights reserved.

   Portions of this code are copyright Estacado Systems. Its use is
   subject to the terms of the license agreement under which it has been
   supplied.
 *********************************************************************** */

#include <iostream>
#include "assert.h"

#include "rutil/DnsUtil.hxx"
#include "rutil/Logger.hxx"
#include "rutil/Inserter.hxx"
#include "rutil/Timer.hxx"

using namespace resip;
using namespace std;

#define RESIPROCATE_SUBSYSTEM Subsystem::TEST

int
main(int argc, char* argv[])
{
   Log::Level l = Log::Debug;
   Log::initialize(Log::Cout, l, argv[0]);
   
   {
      cerr << "Network Interfaces: " << endl << Inserter(DnsUtil::getInterfaces()) << endl << endl;
   }
   
   {
      Data addr("1:1");
      cerr << "!! "<< addr << endl;
      assert(DnsUtil::isIpV6Address(addr));
   }

   {
      Data addr("1:1:192.168.2.233");
      cerr << "!! "<< addr << endl;
      assert(DnsUtil::isIpV6Address(addr));
   }

   {
      Data addr("1:1:::::");
      cerr << "!! "<< addr << endl;
      assert(DnsUtil::isIpV6Address(addr));
   }

   {
      Data addr("1:1::::::168.192.2.233");
      cerr << "!! "<< addr << endl;
      assert(DnsUtil::isIpV6Address(addr));
   }

   {
      Data addr("5f1b:df00:ce3e:e200:20:800:2b37:6426");
      cerr << "!! "<< addr << endl;
      assert(DnsUtil::isIpV6Address(addr));
   }

   {
      Data addr("5f1b:df00:ce3e:e200:20:800:2b37:6426:121.12.131.12");
      cerr << "!! "<< addr << endl;
      assert(DnsUtil::isIpV6Address(addr));
   }

   {
      Data addr("192.168.2.233");
      cerr << "!! "<< addr << endl;
      assert(!DnsUtil::isIpV6Address(addr));
   }

   {
      Data addr("u@a.tv:1290");
      cerr << "!! "<< addr << endl;
      assert(!DnsUtil::isIpV6Address(addr));
   }

   {
      Data addr("::1");
      cerr << "!! "<< addr << endl;
      assert(DnsUtil::isIpV6Address(addr));
   }

   {
      Data addr("::");
      cerr << "!! "<< addr << endl;
      assert(DnsUtil::isIpV6Address(addr));
   }

   {
      Data addr("FF01::43");
      cerr << "!! "<< addr << endl;
      assert(DnsUtil::isIpV6Address(addr));
   }

   {
      Data c("apple:5060");
      Data addr(Data::Share, c.c_str(), 5);
      cerr << "!! " << addr << endl;
      assert(!DnsUtil::isIpV6Address(addr));
   }
   
   {
      Data addr(":zzz");
      if(DnsUtil::isIpV6Address(addr))
      {
         DnsUtil::canonicalizeIpV6Address(addr);
      }
   }

   {
      Data addr("255.255.255.255");
      UInt32 now(Timer::getTimeMs());
      for(long long int i=0; i<10000000LL; ++i)
      {
         assert(DnsUtil::isIpV4Address(addr));
      }
      cerr << "!! " << (UInt32)(Timer::getTimeMs() - now) << endl;
   }

   {
      Data addr("100.100.100.100");
      cerr << "!! "<< addr << endl;
      assert(DnsUtil::isIpV4Address(addr));
   }

   {
      Data addr("256.255.255.255");
      cerr << "!! "<< addr << endl;
      assert(!DnsUtil::isIpV4Address(addr));
   }

   {
      Data addr("255.256.255.255");
      cerr << "!! "<< addr << endl;
      assert(!DnsUtil::isIpV4Address(addr));
   }

   {
      Data addr("265.255.255.255");
      cerr << "!! "<< addr << endl;
      assert(!DnsUtil::isIpV4Address(addr));
   }

   {
      Data addr("300.255.255.255");
      cerr << "!! "<< addr << endl;
      assert(!DnsUtil::isIpV4Address(addr));
   }

   {
      Data addr("aaa.aaa.aaa.aaa");
      cerr << "!! "<< addr << endl;
      assert(!DnsUtil::isIpV4Address(addr));
   }

   cerr << "All OK" << endl;
}
/* ====================================================================
 * 
 * Portions of this file may fall under the following license. The
 * portions to which the following text applies are available from:
 * 
 *   http://www.resiprocate.org/
 * 
 * Any portion of this code that is not freely available from the
 * Resiprocate project webpages is COPYRIGHT ESTACADO SYSTEMS, LLC.
 * All rights reserved.
 * 
 * ====================================================================
 * The Vovida Software License, Version 1.0 
 * 
 * Copyright (c) 2000 Vovida Networks, Inc.  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. The names "VOCAL", "Vovida Open Communication Application Library",
 *    and "Vovida Open Communication Application Library (VOCAL)" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact vocal@vovida.org.
 *
 * 4. Products derived from this software may not be called "VOCAL", nor
 *    may "VOCAL" appear in their name, without prior written
 *    permission of Vovida Networks, Inc.
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL VOVIDA
 * NETWORKS, INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT DAMAGES
 * IN EXCESS OF $1,000, NOR FOR ANY INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * 
 * ====================================================================
 * 
 * This software consists of voluntary contributions made by Vovida
 * Networks, Inc. and many individuals on behalf of Vovida Networks,
 * Inc.  For more information on Vovida Networks, Inc., please see
 * <http://www.vovida.org/>.
 *
 */
