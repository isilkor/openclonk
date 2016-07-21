/*
 * OpenClonk, http://www.openclonk.org
 *
 * Copyright (c) 1998-2000, Matthes Bender
 * Copyright (c) 2001-2009, RedWolf Design GmbH, http://www.clonk.de/
 * Copyright (c) 2009-2016, The OpenClonk Team and contributors
 *
 * Distributed under the terms of the ISC license; see accompanying file
 * "COPYING" for details.
 *
 * "Clonk" is a registered trademark of Matthes Bender, used with permission.
 * See accompanying file "TRADEMARK" for details.
 *
 * To redistribute this file separately, substitute the full license texts
 * for the above references.
 */

/* This header is included first from every source file. It serves three purposes:
   - PlatformAbstraction.h
   - Common utility functionality that's used everywhere
   - Speeding up the compilation by precompiling this header
All of our headers are designed to be used with C4Include.h included before and
don't need to include this file or any of the files it includes. */

#ifndef INC_C4Include
#define INC_C4Include

#include "platform/PlatformAbstraction.h"

#define DEBUGREC_SCRIPT
#define DEBUGREC_START_FRAME 0
#define DEBUGREC_PXS
#define DEBUGREC_OBJCOM
#define DEBUGREC_MATSCAN
#define DEBUGREC_MENU
#define DEBUGREC_OCF
//#define DEBUGREC_RECRUITMENT

// solidmask debugging
//#define SOLIDMASK_DEBUG

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cerrno>
#include <climits>

#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <regex>
namespace re = std;

#include "lib/Standard.h"
#include "C4Prototypes.h"
#include "lib/C4Real.h"
#include "lib/StdBuf.h"
#include "platform/StdFile.h"
#include "c4group/C4Language.h"
#include "lib/C4Log.h"
#include "config/C4Reloc.h"
#include "config/C4Config.h"

#include "game/C4Game.h"

#endif // INC_C4Include
