#pragma once

#ifndef TRIO_H
#define TRIO_H

// TEMPLATE STRUCT trio
template<class _Ty1, class _Ty2, class _Ty3> struct trio
{	// store a trio of values
	typedef trio<_Ty1, _Ty2, _Ty3> _Myt;
	typedef _Ty1 first_type;
	typedef _Ty2 second_type;
	typedef _Ty3 third_type;

	trio()
		: first(_Ty1()), second(_Ty2()), third(_Ty3())
		{	// construct from defaults
		}

	trio(const _Ty1& _Val1, const _Ty2& _Val2, const _Ty3& _Val3)
		: first(_Val1), second(_Val2), third(_Val3)
		{	// construct from specified values
		}

	template<class _Other1, class _Other2, class _Other3>
		trio(const trio<_Other1, _Other2, _Other3>& _Right)
		: first(_Right.first), second(_Right.second), third(_Right.third)
		{	// construct from compatible trio
		}

	void swap(_Myt& _Right)
		{	// exchange contents with _Right
		std::swap(first, _Right.first);
		std::swap(second, _Right.second);
		std::swap(third, _Right.third);
		}

	_Ty1 first;	// the first stored value
	_Ty2 second;	// the second stored value
	_Ty3 third;	// the third stored value
};

		// trio TEMPLATE FUNCTIONS
template<class _Ty1, class _Ty2, class _Ty3> inline
	bool operator==(const trio<_Ty1, _Ty2, _Ty3>& _Left,
		const trio<_Ty1, _Ty2, _Ty3>& _Right)
	{	// test for trio equality
	return (_Left.first == _Right.first && _Left.second == _Right.second && _Left.third == _Right.third);
	}

template<class _Ty1, class _Ty2, class _Ty3> inline
	bool operator!=(const trio<_Ty1, _Ty2, _Ty3>& _Left,
		const trio<_Ty1, _Ty2, _Ty3>& _Right)
	{	// test for trio inequality
	return (!(_Left == _Right));
	}

template<class _Ty1, class _Ty2, class _Ty3> inline
	bool operator<(const trio<_Ty1, _Ty2, _Ty3>& _Left,
		const trio<_Ty1, _Ty2, _Ty3>& _Right)
	{	// test if _Left < _Right for trios
	return (_Left.first < _Right.first && _Left.second < _Right.second && _Left.third < _Right.third);
	}

template<class _Ty1, class _Ty2, class _Ty3> inline
	bool operator>(const trio<_Ty1, _Ty2, _Ty3>& _Left,
		const trio<_Ty1, _Ty2, _Ty3>& _Right)
	{	// test if _Left > _Right for trios
	return (_Right < _Left);
	}

template<class _Ty1, class _Ty2, class _Ty3> inline
	bool operator<=(const trio<_Ty1, _Ty2, _Ty3>& _Left,
		const trio<_Ty1, _Ty2, _Ty3>& _Right)
	{	// test if _Left <= _Right for trios
	return (!(_Right < _Left));
	}

template<class _Ty1, class _Ty2, class _Ty3> inline
	bool operator>=(const trio<_Ty1, _Ty2, _Ty3>& _Left,
		const trio<_Ty1, _Ty2, _Ty3>& _Right)
	{	// test if _Left >= _Right for trios
	return (!(_Left < _Right));
	}

template<class _Ty1, class _Ty2, class _Ty3> inline
	trio<_Ty1, _Ty2, _Ty3> make_trio(_Ty1 _Val1, _Ty2 _Val2, _Ty3 _Val3)
	{	// return trio composed from arguments
	return (trio<_Ty1, _Ty2, _Ty3>(_Val1, _Val2, _Val3));
	}

template<class _Ty1, class _Ty2, class _Ty3> inline
	void swap(trio<_Ty1, _Ty2, _Ty3>& _Left, trio<_Ty1, _Ty2, _Ty3>& _Right)
	{	// swap _Left and _Right trios
	_Left.swap(_Right);
	}

// TEMPLATE OPERATORS
namespace rel_ops_trio
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

/*#include <list>
typedef trio< std::wstring, int, HWND > CAnswer;
typedef std::list< CAnswer > CAnswerList;
typedef trio< std::wstring, std::wstring, CAnswerList > CTestItem;
typedef std::list< CTestItem > CTestItemList;*/

#endif
