/***************************************************************************
                          utilities.h  -  description
                             -------------------
    begin                : Sat Apr 26 2003
    copyright            : (C) 2003 by Blake Madden
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <functional>

template<typename T>
inline bool is_either(T value, T first, T second)
	{
///@bug temporary testing hack
#ifdef EITHER_LOOSE
	assert(toupper(first) == toupper(second));
	assert(first != second);
#endif
	return (value == first || value == second);
	}

template<typename T>
inline bool is_neither(T value, T first, T second)
	{
	assert(first != second);
	return (value != first && value != second);
	}

template<typename T>
inline bool is_within(T value, T first, T second)
	{
	assert(first <= second);
	return (value >= first && value <= second);
	}

template<typename inT, typename outT, typename member_extract_functorT>
inline outT copy_member(inT begin, inT end, outT dest, member_extract_functorT get_value)
	{
	for (; begin != end; ++dest, ++begin)
		*dest = get_value(*begin);
	return (dest);
	}

template<class _InIt, class _Pr, typename member_extract_functorT>
inline typename std::iterator_traits<_InIt>::difference_type
	count_member_if(_InIt _First, _InIt _Last,
					_Pr _Pred, member_extract_functorT get_value)
	{	// count elements satisfying _Pred
	typename std::iterator_traits<_InIt>::difference_type _Count = 0;

	for (; _First != _Last; ++_First)
		if (_Pred(get_value(*_First)) )
			++_Count;
	return (_Count);
	}

//determines if number is even
template <class T>
class even : public std::unary_function<T, bool>
	{
public:
	inline bool operator()(const T& val) const
		{ return val%2==0; }
	};

//determines if a value is within a given range
template<typename T>
class within : public std::unary_function<T, bool>
	{
public:
	within(T range_begin, T range_end)
		: m_range_begin(range_begin), m_range_end(range_end)
		{}
	inline bool operator()(T value) const
		{ return (value >= m_range_begin && value <= m_range_end); }
private:
	T m_range_begin;
	T m_range_end;
	};

#endif //__UTILITIES_H__
