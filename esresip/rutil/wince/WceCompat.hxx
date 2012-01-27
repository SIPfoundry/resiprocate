/* ***********************************************************************
   Copyright 2006-2007 Estacado Systems, LLC. All rights reserved.
 *********************************************************************** */

#ifndef __WCECOMPAT_H
#define __WCECOMPAT_H

#ifdef UNDER_CE

#include <cassert>
namespace resip
{
wchar_t* ToWString(const char *str);
void FreeWString(wchar_t* wstr);


}	//	namespace resip

#endif UNDER_CE

#endif __WCECOMPAT_H
