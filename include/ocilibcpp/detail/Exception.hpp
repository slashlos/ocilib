/*
 * OCILIB - C Driver for Oracle (C Wrapper for Oracle OCI)
 *
 * Website: http://www.ocilib.net
 *
 * Copyright (c) 2007-2020 Vincent ROGIER <vince.rogier@ocilib.net>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "ocilibcpp/types.hpp"

namespace ocilib
{

inline Exception::Exception() noexcept
    : _what(nullptr),
    _pStatement(nullptr),
    _pConnnection(nullptr),
    _row(0),
    _type(static_cast<ExceptionType::Type>(0)),
    _errLib(0),
    _errOracle(0)
{

}

inline Exception::Exception(OCI_Error *err) noexcept
    : _what(nullptr),
    _pStatement(OCI_ErrorGetStatement(err)),
    _pConnnection(OCI_ErrorGetConnection(err)),
    _row(OCI_ErrorGetRow(err)),
    _type(static_cast<ExceptionType::Type>(OCI_ErrorGetType(err))),
    _errLib(OCI_ErrorGetInternalCode(err)),
    _errOracle(OCI_ErrorGetOCICode(err))
{
    SetWhat(OCI_ErrorGetString(err));
}

inline Exception::Exception(const Exception& other) noexcept : Exception(nullptr)
{
    SetWhat(other._what);
}

inline Exception::~Exception() noexcept
{
    delete [] _what;
}

inline Exception& Exception::operator = (const Exception& other) noexcept
{
    if (this != &other)
    {
        _what = ostrdup(other._what);
    }

    return *this;
}

inline void Exception::SetWhat(const otext* value) noexcept
{
    if (!value)
    {
        return;
    }

    const size_t len = ostrlen(value);
    _what = new (std::nothrow) otext[len + 1];
    if (_what)
    {
        memcpy(_what, value, (len + 1) * (sizeof(otext)));
    }
}

inline const char * Exception::what() const noexcept
{
    return _what;
}

inline ostring Exception::GetMessage() const
{
    const otext* str = what();

    return str ? str : ostring{};
}

inline Exception::ExceptionType Exception::GetType() const
{
    return _type;
}

inline int Exception::GetOracleErrorCode() const
{
    return _errOracle;
}

inline int Exception::GetInternalErrorCode() const
{
    return _errLib;
}

inline Statement Exception::GetStatement() const
{
    return Statement(_pStatement, nullptr);
}

inline Connection Exception::GetConnection() const
{
    return Connection(_pConnnection, nullptr);
}

inline unsigned int Exception::GetRow() const
{
    return _row;
}

}
