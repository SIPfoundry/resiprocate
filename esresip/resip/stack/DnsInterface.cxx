/* ***********************************************************************
   Copyright 2006-2007 Estacado Systems, LLC. All rights reserved.

   Portions of this code are copyright Estacado Systems. Its use is
   subject to the terms of the license agreement under which it has been
   supplied.
 *********************************************************************** */

#if defined(HAVE_CONFIG_H)
#include "resip/stack/config.hxx"
#endif

#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#ifndef __CYGWIN__
#  include <netinet/in.h>
#  include <arpa/nameser.h>
#  include <resolv.h>
#endif
#endif

#include "rutil/compat.hxx"
#include "rutil/Logger.hxx"
#include "rutil/BaseException.hxx"
#include "rutil/Socket.hxx"

#include "rutil/dns/DnsStub.hxx"
#include "rutil/dns/RRVip.hxx"
#include "resip/stack/DnsInterface.hxx"
#include "rutil/dns/DnsHandler.hxx"
#include "resip/stack/DnsResult.hxx"

//#include "rutil/dns/ExternalDnsFactory.hxx"
#include "rutil/WinLeakCheck.hxx"


using namespace resip;

#define RESIPROCATE_SUBSYSTEM resip::Subsystem::DNS

DnsInterface::DnsInterface(DnsStub& dnsStub) : 
   mDnsStub(dnsStub)
{

#ifdef USE_DNS_VIP
   mDnsStub.setResultTransform(&mVip);
#endif
}

DnsInterface::~DnsInterface()
{
}

static Data UdpNAPTRType("SIP+D2U");
static Data TcpNAPTRType("SIP+D2T");
static Data SctpNAPTRType("SIP+D2S");
static Data TlsNAPTRType("SIPS+D2T");
static Data DtlsNAPTRType("SIPS+D2U");
static Data Sctp_TlsNAPTRType("SIPS+D2S");
void 
DnsInterface::addTransportType(TransportType type, IpVersion version)
{
   mSupportedTransports.push_back(std::make_pair(type, version));
   switch (type)
   {
      case UDP:
         mSupportedNaptrs.insert(UdpNAPTRType);
         break;
      case TCP:
         mSupportedNaptrs.insert(TcpNAPTRType);
         break;
      case SCTP:
         mSupportedNaptrs.insert(SctpNAPTRType);
         break;
      case TLS:
         mSupportedNaptrs.insert(TlsNAPTRType);
         break;
      case DTLS:
         mSupportedNaptrs.insert(DtlsNAPTRType);
         break;         
      default:
         assert(0);
         break;
   }
}

bool
DnsInterface::isSupported(const Data& service)
{
   return mSupportedNaptrs.count(service) != 0;
}

bool
DnsInterface::isSupported(TransportType t, IpVersion version)
{
   return std::find(mSupportedTransports.begin(), mSupportedTransports.end(), std::make_pair(t, version)) != mSupportedTransports.end();
}

bool
DnsInterface::isSupportedProtocol(TransportType t)
{
   for (TransportMap::const_iterator i=mSupportedTransports.begin(); i != mSupportedTransports.end(); ++i)
   {
      if (i->first == t)
      {
         return true;
      }
   }
   return false;
}

int DnsInterface::supportedProtocols()
{
   return (int)mSupportedTransports.size();
}


DnsResult*
DnsInterface::createDnsResult(DnsHandler* handler)
{
   DnsResult* result = new DnsResult(*this, mDnsStub, mVip, handler);
   return result;
}

void 
DnsInterface::lookup(DnsResult* res, const Uri& uri)
{
   res->lookup(uri, mDnsStub.getEnumSuffixes());   
}

void 
DnsInterface::poke()
{
   mDnsStub.poke();
}

// DnsResult* 
// DnsInterface::lookup(const Via& via, DnsHandler* handler)
// {
//    assert(0);

//    //DnsResult* result = new DnsResult(*this);
//    return NULL;
// }

//?dcm? -- why is this here?
DnsHandler::~DnsHandler()
{
}

/* moved to DnsStub.
void 
DnsInterface::lookupRecords(const Data& target, unsigned short type, DnsRawSink* sink)
{
   mDnsProvider->lookup(target.c_str(), type, this, sink);
}

void 
DnsInterface::handleDnsRaw(ExternalDnsRawResult res)
{
   reinterpret_cast<DnsRawSink*>(res.userData)->onDnsRaw(res.errorCode(), res.abuf, res.alen);
   mDnsProvider->freeResult(res);
}
*/

//  Copyright (c) 2003, Jason Fischl 
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

