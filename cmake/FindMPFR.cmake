# OpenClonk, http://www.openclonk.org
#
# Copyright (c) 2017, The OpenClonk Team and contributors
#
# Distributed under the terms of the ISC license; see accompanying file
# "COPYING" for details.
#
# "Clonk" is a registered trademark of Matthes Bender, used with permission.
# See accompanying file "TRADEMARK" for details.
#
# To redistribute this file separately, substitute the full license texts
# for the above references.

# Locate the GNU MPFR Library.
# This module defines a new target named "mpfr" which you can link to.

find_path(MPFR_INCLUDE_DIR NAMES mpfr.h)
find_library(MPFR_LIBRARY NAMES mpfr)
get_filename_component(__MPFR_LIBRARY_DIR "${MPFR_LIBRARY}" PATH)
mark_as_advanced(MPFR_INCLUDE_DIR MPFR_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MPFR
	FOUND_VAR MPFR_FOUND
	REQUIRED_VARS MPFR_LIBRARY MPFR_INCLUDE_DIR
	)

set(__mpfr_forward_args)
if(MPFR_FIND_QUIETLY)
	list(APPEND __mpfr_forward_args QUIET)
endif()
if(MPFR_FIND_REQUIRED)
	list(APPEND __mpfr_forward_args REQUIRED)
endif()

find_package(GMP ${__mpfr_forward_args})

if(MPFR_FOUND AND GMP_FOUND)
	add_library(mpfr UNKNOWN IMPORTED)
	set_property(TARGET mpfr PROPERTY IMPORTED_LOCATION "${MPFR_LIBRARY}")
	set_property(TARGET mpfr PROPERTY INTERFACE_LINK_LIBRARIES "gmp")
	set_property(TARGET mpfr PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${MPFR_INCLUDE_DIR}")
endif()
