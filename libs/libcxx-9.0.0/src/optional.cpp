/*
-- Copyright (c) 2015-2019 Synopsys, Inc.  This Synopsys software and all
-- associated documentation are proprietary to Synopsys, Inc. and may only be
-- used pursuant to the terms and conditions of a written license agreement
-- with Synopsys, Inc.  All other use, reproduction, modification, or
-- distribution of the Synopsys software or the associated documentation is
-- strictly prohibited.
*/
//===------------------------ optional.cpp --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "optional"

namespace std
{

bad_optional_access::~bad_optional_access() _NOEXCEPT = default;

const char* bad_optional_access::what() const _NOEXCEPT {
  return "bad_optional_access";
  }

} // std


#include <experimental/__config>

//  Preserve std::experimental::bad_optional_access for ABI compatibility
//  Even though it no longer exists in a header file
_LIBCPP_BEGIN_NAMESPACE_EXPERIMENTAL

class _LIBCPP_EXCEPTION_ABI _LIBCPP_AVAILABILITY_BAD_OPTIONAL_ACCESS bad_optional_access
    : public std::logic_error
{
public:
	bad_optional_access() : std::logic_error("Bad optional Access") {}

//	Get the key function ~bad_optional_access() into the dylib
    virtual ~bad_optional_access() _NOEXCEPT;
};

bad_optional_access::~bad_optional_access() _NOEXCEPT = default;

_LIBCPP_END_NAMESPACE_EXPERIMENTAL
