#pragma once

#ifndef QUAD_H
#define QUAD_H

#include "trio_template.h"

// TEMPLATE STRUCT quad
template<class _Ty1, class _Ty2, class _Ty3, class _Ty4> struct quad
{	// store a quad of values
	typedef quad<_Ty1, _Ty2, _Ty3,_Ty4> _Myt;
	typedef _Ty1 first_type;
	typedef _Ty2 second_type;
	typedef _Ty3 third_type;
	typedef _Ty4 fourth_type;

	quad()
		: first(_Ty1()), second(_Ty2()), third(_Ty3()), fourth(_Ty4())
		{	// construct from defaults
		}

	quad(const _Ty1& _Val1, const _Ty2& _Val2, const _Ty3& _Val3, const _Ty4& _Val4)
		: first(_Val1), second(_Val2), third(_Val3), fourth(_Val4)
		{	// construct from specified values
		}

	template<class _Other1, class _Other2, class _Other3, class _Other4>
		quad(const quad<_Other1, _Other2, _Other3, _Other4>& _Right)
		: first(_Right.first), second(_Right.second), third(_Right.third), fourth(_Right.fourth)
		{	// construct from compatible quad
		}

	void swap(_Myt& _Right)
		{	// exchange contents with _Right
		std::swap(first, _Right.first);
		std::swap(second, _Right.second);
		std::swap(third, _Right.third);
		std::swap(fourth, _Right.fourth);
		}

	_Ty1 first;	// the first stored value
	_Ty2 second;	// the second stored value
	_Ty3 third;	// the third stored value
	_Ty4 fourth;	// the fourth stored value
};

		// quad TEMPLATE FUNCTIONS
template<class _Ty1, class _Ty2, class _Ty3, class _Ty4> inline
	bool operator==(const quad<_Ty1, _Ty2, _Ty3, _Ty4>& _Left,
		const quad<_Ty1, _Ty2, _Ty3, _Ty4>& _Right)
	{	// test for quad equality
	return (_Left.first == _Right.first && _Left.second == _Right.second && _Left.third == _Right.third && _Left.fourth == _Right.fourth);
	}

template<class _Ty1, class _Ty2, class _Ty3, class _Ty4> inline
	bool operator!=(const quad<_Ty1, _Ty2, _Ty3, _Ty4>& _Left,
		const quad<_Ty1, _Ty2, _Ty3, _Ty4>& _Right)
	{	// test for quad inequality
	return (!(_Left == _Right));
	}

template<class _Ty1, class _Ty2, class _Ty3, class _Ty4> inline
	bool operator<(const quad<_Ty1, _Ty2, _Ty3, _Ty4>& _Left,
		const quad<_Ty1, _Ty2, _Ty3, _Ty4>& _Right)
	{	// test if _Left < _Right for quads
	return (_Left.first < _Right.first && _Left.second < _Right.second && _Left.third < _Right.third && _Left.fourth < _Right.fourth);
	}

template<class _Ty1, class _Ty2, class _Ty3, class _Ty4> inline
	bool operator>(const quad<_Ty1, _Ty2, _Ty3, _Ty4>& _Left,
		const quad<_Ty1, _Ty2, _Ty3, _Ty4>& _Right)
	{	// test if _Left > _Right for quads
	return (_Right < _Left);
	}

template<class _Ty1, class _Ty2, class _Ty3, class _Ty4> inline
	bool operator<=(const quad<_Ty1, _Ty2, _Ty3, _Ty4>& _Left,
		const quad<_Ty1, _Ty2, _Ty3, _Ty4>& _Right)
	{	// test if _Left <= _Right for quads
	return (!(_Right < _Left));
	}

template<class _Ty1, class _Ty2, class _Ty3, class _Ty4> inline
	bool operator>=(const quad<_Ty1, _Ty2, _Ty3, _Ty4>& _Left,
		const quad<_Ty1, _Ty2, _Ty3, _Ty4>& _Right)
	{	// test if _Left >= _Right for quads
	return (!(_Left < _Right));
	}

template<class _Ty1, class _Ty2, class _Ty3, class _Ty4> inline
	quad<_Ty1, _Ty2, _Ty3, _Ty4> make_quad(_Ty1 _Val1, _Ty2 _Val2, _Ty3 _Val3, _Ty4 _Val4)
	{	// return quad composed from arguments
	return (quad<_Ty1, _Ty2, _Ty3, _Ty4>(_Val1, _Val2, _Val3, _Val4));
	}

template<class _Ty1, class _Ty2, class _Ty3, class _Ty4> inline
	void swap(quad<_Ty1, _Ty2, _Ty3, _Ty4>& _Left, quad<_Ty1, _Ty2, _Ty3, _Ty4>& _Right)
	{	// swap _Left and _Right quads
	_Left.swap(_Right);
	}

// TEMPLATE OPERATORS
namespace rel_ops_quad
{	// nested namespace to hide relational operators from std
template<class _Ty> inline
	bool  operator!=(const _Ty& _Left, const _Ty& _Right)
	{	// test for inequality, in terms of equality
	return (!(_Left == _Right));
	}

template<class _Ty> inline
	bool operator>(const _Ty& _Left, const _Ty& _Right)
	{	// test if _Left > _Right, in terms of operator<
	return (_Right < _Left);
	}

template<class _Ty> inline
	bool operator<=(const _Ty& _Left, const _Ty& _Right)
	{	// test if _Left <= _Right, in terms of operator<
	return (!(_Right < _Left));
	}

template<class _Ty> inline
	bool operator>=(const _Ty& _Left, const _Ty& _Right)
	{	// test if _Left >= _Right, in terms of operator<
	return (!(_Left < _Right));
	}
}

#include <list>
typedef trio< std::wstring, int, HWND > CAnswer;
typedef std::list< CAnswer > CAnswerList;
typedef quad< std::wstring, std::wstring, CAnswerList, int > CTestItem;
typedef std::list< CTestItem > CTestItemList;

#endif
