/*
* Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TRINITY_DEFINE_H
#define TRINITY_DEFINE_H

#include "CompilerDefs.h"

#if WAR_COMPILER == WAR_COMPILER_GNU
#  if !defined(__STDC_FORMAT_MACROS)
#    define __STDC_FORMAT_MACROS
#  endif
#  if !defined(__STDC_CONSTANT_MACROS)
#    define __STDC_CONSTANT_MACROS
#  endif
#  if !defined(_GLIBCXX_USE_NANOSLEEP)
#    define _GLIBCXX_USE_NANOSLEEP
#  endif
#  if defined(HELGRIND)
#    include <valgrind/helgrind.h>
#    undef _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE
#    undef _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER
#    define _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(A) ANNOTATE_HAPPENS_BEFORE(A)
#    define _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(A)  ANNOTATE_HAPPENS_AFTER(A)
#  endif
#endif

#include <cstddef>
#include <cinttypes>
#include <climits>

#define WAR_LITTLEENDIAN 0
#define WAR_BIGENDIAN    1

#if !defined(WAR_ENDIAN)
#  if defined (BOOST_BIG_ENDIAN)
#    define WAR_ENDIAN WAR_BIGENDIAN
#  else
#    define WAR_ENDIAN WAR_LITTLEENDIAN
#  endif
#endif

#if WAR_PLATFORM == WAR_PLATFORM_WINDOWS
#  define WAR_PATH_MAX 260
#  define _USE_MATH_DEFINES
#  ifndef DECLSPEC_NORETURN
#    define DECLSPEC_NORETURN __declspec(noreturn)
#  endif //DECLSPEC_NORETURN
#  ifndef DECLSPEC_DEPRECATED
#    define DECLSPEC_DEPRECATED __declspec(deprecated)
#  endif //DECLSPEC_DEPRECATED
#else // WAR_PLATFORM != WAR_PLATFORM_WINDOWS
#  define WAR_PATH_MAX PATH_MAX
#  define DECLSPEC_NORETURN
#  define DECLSPEC_DEPRECATED
#endif // WAR_PLATFORM

#if !defined(COREDEBUG)
#  define WAR_INLINE inline
#else //COREDEBUG
#  if !defined(WAR_DEBUG)
#    define WAR_DEBUG
#  endif //WAR_DEBUG
#  define WAR_INLINE
#endif //!COREDEBUG

#if WAR_COMPILER == WAR_COMPILER_GNU
#  define ATTR_NORETURN __attribute__((__noreturn__))
#  define ATTR_PRINTF(F, V) __attribute__ ((__format__ (__printf__, F, V)))
#  define ATTR_DEPRECATED __attribute__((__deprecated__))
#else //WAR_COMPILER != WAR_COMPILER_GNU
#  define ATTR_NORETURN
#  define ATTR_PRINTF(F, V)
#  define ATTR_DEPRECATED
#endif //WAR_COMPILER == WAR_COMPILER_GNU

#ifdef WAR_API_USE_DYNAMIC_LINKING
#  if WAR_COMPILER == WAR_COMPILER_MICROSOFT
#    define WAR_API_EXPORT __declspec(dllexport)
#    define WAR_API_IMPORT __declspec(dllimport)
#  elif WAR_COMPILER == WAR_COMPILER_GNU
#    define WAR_API_EXPORT __attribute__((visibility("default")))
#    define WAR_API_IMPORT
#  else
#    error compiler not supported!
#  endif
#else
#  define WAR_API_EXPORT
#  define WAR_API_IMPORT
#endif

#if !defined(WAR_ENDIAN)
#  if defined (BOOST_BIG_ENDIAN)
#    define WAR_ENDIAN WAR_BIGENDIAN
#  else
#    define WAR_ENDIAN WAR_LITTLEENDIAN
#  endif
#endif

#ifdef WAR_API_EXPORT_COMMON
#  define WAR_COMMON_API WAR_API_EXPORT
#else
#  define WAR_COMMON_API WAR_API_IMPORT
#endif

#ifdef WAR_API_EXPORT_PROTO
#  define WAR_PROTO_API WAR_API_EXPORT
#else
#  define WAR_PROTO_API WAR_API_IMPORT
#endif

#ifdef WAR_API_EXPORT_DATABASE
#  define WAR_DATABASE_API WAR_API_EXPORT
#else
#  define WAR_DATABASE_API WAR_API_IMPORT
#endif

#ifdef WAR_API_EXPORT_SHARED
#  define WAR_SHARED_API WAR_API_EXPORT
#else
#  define WAR_SHARED_API WAR_API_IMPORT
#endif

#ifdef WAR_API_EXPORT_GAME
#  define WAR_GAME_API WAR_API_EXPORT
#else
#  define WAR_GAME_API WAR_API_IMPORT
#endif

#define UI64FMTD "%" PRIu64
#define UI64LIT(N) UINT64_C(N)

#define SI64FMTD "%" PRId64
#define SI64LIT(N) INT64_C(N)

#define SZFMTD "%" PRIuPTR


typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;

#endif //TRINITY_DEFINE_H
