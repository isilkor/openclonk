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

# Locate the GNU Multi-Precision Arithmetic Library.
# This module defines a new target named "gmp" which you can link to.

find_path(GMP_INCLUDE_DIR NAMES gmp.h)
find_library(GMP_LIBRARY NAMES gmp)
mark_as_advanced(GMP_INCLUDE_DIR GMP_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GMP
	FOUND_VAR GMP_FOUND
	REQUIRED_VARS GMP_LIBRARY GMP_INCLUDE_DIR
	)

if(GMP_FOUND)
	add_library(gmp UNKNOWN IMPORTED)
	set_property(TARGET gmp PROPERTY IMPORTED_LOCATION "${GMP_LIBRARY}")
	set_property(TARGET gmp PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${GMP_INCLUDE_DIR}")
endif()
