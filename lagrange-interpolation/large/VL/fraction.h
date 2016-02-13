/***

   RollRat Software Project.
   Copyright (C) 2015. rollrat. All Rights Reserved.

File name:
   
   fraction.h

Author:

   02-13-2016:   HyunJun Jeong, Creation

***/

#ifndef _FRACTION_
#define _FRACTION_

#include "biginteger.h"

template<int I>
class basic_fraction
{
	typedef basic_fraction<I> this_type;
	typedef basic_integer<I> integer_operation;
	integer_operation high;
	integer_operation low;

public:

	basic_fraction(std::string wstr)
		: high(wstr)
		, low(1)
	{
	}

	basic_fraction(std::string wstr1, std::string wstr2)
		: high(wstr1)
		, low(wstr2)
	{
		reduction();
	}
	
	this_type& operator=(std::string wstr)
	{
		high = wstr;
		low = 1;
		return *this;
	}

	this_type& operator=(const this_type& refer)
	{
		high = refer.high;
		low = refer.low;
		return *this;
	}

	this_type& operator*=(const this_type& refer)
	{
		integer_operation hit = refer.high * high;
		integer_operation lot = refer.low * low;

		integer_operation gcd = hit.abs().gcd(lot.abs());

		high = hit / gcd;
		low = lot / gcd;

		return *this;
	}

	this_type operator*(const this_type& refer) const
	{
		this_type tmp = *this;
		return tmp *= refer;
	}

	this_type& operator+=(const this_type& refer)
	{
		integer_operation lcm = low.abs().lcm(refer.low.abs());

		high = high * lcm / low + refer.high * lcm / refer.low;
		low = lcm;

		return *this;
	}

	this_type operator+(const this_type& refer) const
	{
		this_type tmp = *this;
		return tmp += refer;
	}

	void reverse()
	{
		high.swap(low);
	}
	
	void reduction()
	{
		integer_operation gcd = high.abs().gcd(low.abs());
		high /= gcd;
		low /= gcd;
	}

	std::string operator*()
	{
		std::string build;

		if ( high.sign() != low.sign() )
			build.append("-");

		reduction();
		
		build.append(*high.abs());

		if ( low != "1" )
		{
			build.append("/");
			build.append(*low.abs());
		}

		return build;
	}

	bool sign() const
	{
		if ( high.sign() != low.sign() )
			return true;
		else
			return false;
	}

};

typedef basic_fraction<256> fraction;

#endif