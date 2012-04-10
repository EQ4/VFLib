//==============================================================================
// 
// Copyright (C) 2008 by Vinnie Falco, this file is part of VFLib.
// 
// Based on SOCI - The C++ Database Access Library:
//   http://soci.sourceforge.net/
// 
// This file is distributed under the following terms:
// 
// Boost Software License - Version 1.0, August 17th, 2003
// 
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
// 
// This file incorporates work covered by the following copyright
// and permission notice:
// 
// Copyright (C) 2004 Maciej Sobczak, Stephen Hutton, Mateusz Loskot,
// Pawel Aleksander Fedorynski, David Courtney, Rafal Bobrowski,
// Julian Taylor, Henning Basold, Ilia Barahovski, Denis Arnaud,
// Daniel Lidstr�m, Matthieu Kermagoret, Artyom Beilis, Cory Bennett,
// Chris Weed, Michael Davidsaver, Jakub Stachowski, Alex Ott, Rainer Bauer,
// Martin Muenstermann, Philip Pemberton, Eli Green, Frederic Chateau,
// Artyom Tonkikh, Roger Orr, Robert Massaioli, Sergey Nikulov,
// Shridhar Daithankar, S�ren Meyer-Eppler, Mario Valesco.
// 
//==============================================================================

namespace db {

namespace detail {

namespace {

template <typename T>
inline T& as(void* data)
{
  return *static_cast<T*>(data);
}

template <typename T>
static void integer_into(void* data, sqlite3_int64 v)
{
  *(static_cast<T*>(data)) = T(v);
}

}

standard_into_type::~standard_into_type()
{
}

void standard_into_type::bind (statement_imp& st, int& iCol)
{
  m_st = &st;
  m_iCol = iCol++;
}

void standard_into_type::do_into()
{
  int colType = sqlite3_column_type( m_st->m_stmt, m_iCol );

  if (colType == SQLITE_NULL)
  {
    // null encountered with no indicator
    if (!m_ind)
      Throw (Error().fail (__FILE__, __LINE__));

    *m_ind = i_null;
  }
  else
  {
    if (m_ind)
      *m_ind = i_ok;

    switch (colType)
    {
    case SQLITE_INTEGER:
      {
        sqlite3_int64 v = sqlite3_column_int64 (m_st->m_stmt, m_iCol);
        switch (m_type)
        {
        case x_bool:
          as<bool>(m_data) = v != 0;
          break;

        case x_char:  integer_into<char>(m_data,v); break;
        case x_short: integer_into<short>(m_data,v); break; 
        case x_int:   integer_into<int>(m_data,v); break;
        case x_long:  integer_into<long>(m_data,v); break;
        case x_int64: integer_into<int64>(m_data,v); break;

        case x_uchar:  integer_into<unsigned char>(m_data,v); break;
        case x_ushort: integer_into<unsigned short>(m_data,v); break; 
        case x_uint:   integer_into<unsigned int>(m_data,v); break;
        case x_ulong:  integer_into<unsigned long>(m_data,v); break;
        case x_uint64: integer_into<uint64>(m_data,v); break;

        case x_juceString:
          as <juce::String> (m_data) = String (v);
          break;

        default:
          Throw (Error().fail (__FILE__, __LINE__));
        }
      }
      break;

    case SQLITE_FLOAT:
      {
        double v = sqlite3_column_double (m_st->m_stmt, m_iCol);

        switch (m_type)
        {
        case x_float:
          as<float>(m_data) = static_cast<float>(v);
          break;

        case x_double:
          as<double>(m_data) = v;
          break;

        case x_juceString:
          as <juce::String> (m_data) = String (v);
          break;

        default:
          Throw (Error().fail (__FILE__, __LINE__));
        };
      }
      break;

    case SQLITE_TEXT:
      {
        switch (m_type)
        {
        case x_stdstring:
          {
            // excludes terminator
            int bytes = sqlite3_column_bytes( m_st->m_stmt, m_iCol );
            unsigned char const* v = sqlite3_column_text( m_st->m_stmt, m_iCol );
            std::string& result = as<std::string>(m_data);
            result.assign( reinterpret_cast<char const*>(v), bytes );
          }
          break;

        case x_stdwstring:
          Throw (Error().fail (__FILE__, __LINE__));
          break;

        case x_juceString:
          {
            // excludes terminator
            int bytes = sqlite3_column_bytes (m_st->m_stmt, m_iCol);

            const juce::CharPointer_UTF8::CharType* c = reinterpret_cast
              <const juce::CharPointer_UTF8::CharType*>
              (sqlite3_column_text (m_st->m_stmt, m_iCol));

            juce::String& s = as <juce::String> (m_data);
            s = juce::String (juce::CharPointer_UTF8 (c), juce::CharPointer_UTF8 (c + bytes));
          }
          break;

        default:
          {
            sqlite3_int64 v = sqlite3_column_int64( m_st->m_stmt, m_iCol );
            switch( m_type )
            {
            case x_bool:
              as<bool>(m_data) = v != 0;
              break;

            case x_char:  integer_into<char>(m_data,v); break;
            case x_short: integer_into<short>(m_data,v); break; 
            case x_int:   integer_into<int>(m_data,v); break;
            case x_long:  integer_into<long>(m_data,v); break;
            case x_int64: integer_into<int64>(m_data,v); break;

            case x_uchar:  integer_into<unsigned char>(m_data,v); break;
            case x_ushort: integer_into<unsigned short>(m_data,v); break; 
            case x_uint:   integer_into<unsigned int>(m_data,v); break;
            case x_ulong:  integer_into<unsigned long>(m_data,v); break;
            case x_uint64: integer_into<uint64>(m_data,v); break;

            default:
              Throw (Error().fail (__FILE__, __LINE__));
            }

          }
          break;
        };
      }
      break;

    case SQLITE_BLOB:
       Throw (Error().fail (__FILE__, __LINE__));

    default:
      Throw (Error().fail (__FILE__, __LINE__));
    };
  }

  convert_from_base();
}

}

}
