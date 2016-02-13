/***

   RollRat Software Project.
   Copyright (C) 2015. rollrat. All Rights Reserved.

File name:
   
   Interpolation.h

Purpose:

   RollRat Library

Author:

   11-01-2015:   HyunJun Jeong, Creation

***/

#ifndef _INTERPLATION_9bf1541fdf7efd41b7b39543fd870ac4_
#define _INTERPLATION_9bf1541fdf7efd41b7b39543fd870ac4_

#include <cmath>

#include "WStringBuilder.h"
#include "List.h"
#include "Sorting.h"

#include "VL\fraction.h"

namespace Utility {
	
	template<typename type>
	class LagrangeInterpolation // ��׶��� ���׽�, ������
	{
		List<type> tar;	// x
		List<type> val;	// y
		type *tar_arr;
		type *val_arr;

	public:

		void AddValue(type x, type y)
		{
			tar.Append(x);
			val.Append(y);
		}

		WString GetInterpolation(bool formal = false)
		{
			tar_arr = tar.ToArray();
			val_arr = val.ToArray();

			SortingWith<type, type> sorting(tar_arr, val_arr, tar.Size());

			type *facto = new type[tar.Size()]; // factorization coefficient
			for (int i = 0; i < tar.Size(); i++)
				facto[i] = 0;

			for (int i = 0; i < tar.Size(); i++)
			{
				type *tmpfacto = new type[tar.Size()];

				// ���� ���� ����� ���
				double mul = 1.0f;
				for (int j = 0; j < tar.Size(); j++)
				{
					tmpfacto[j] = 0;
					if (i != j)
					{
						mul *= tar_arr[i] - tar_arr[j];
					}
				}
				mul = val_arr[i] / mul;
				tmpfacto[0] = 1;

				// �μ����ؽ� ��� ���
				for (int j = 0, pure = 1; j < tar.Size(); j++)
				{
					if (i != j)
					{
						// 3x+4 ��� ���� �ִٰ� �ϸ�
						// 3x^2+4x+4 ���� �ٲ۴ٰ� �����ϸ� ��(����� ����)
						for (int k = pure - 1; k >= 0; k--)
						{
							tmpfacto[k+1] = tmpfacto[k];
						}

						// 3x^2+4x+0 (����� ����)
						tmpfacto[0] = 0;

						// 3x+4���� x-3�� ���ϴ� ��쿣
						//  |3x^2 + 4x + 0
						// +|      -9x  -12
						//  --------------
						//   3x^2  -5x  -12
						// �� ���ȴ�.
						// ��, n�� ���׽��� ������� n-1�� ����ϸ� �ȴ�.
						for (int k = 0; k < pure; k++)
						{
							tmpfacto[k] += tmpfacto[k + 1] * -tar_arr[j];
						}

						// ��� ��ġ ex) pure�� 1�̸� x�� ����� 3�̸� x^3�� ����� ����Ŵ
						pure++;
					}
				}

				// ���մ��׽Ŀ� ���Ѵ�.
				for (int k = 0; k < tar.Size(); k++)
				{
					facto[k] += tmpfacto[k] * mul;
				}

				delete[] tmpfacto;
			}
			
			WStringBuilder wsb;

			// �� �����
			for (int k = tar.Size() - 1; k >= 0; k--)
			{
				wsb.Append(WString(facto[k]));
				if (formal == false)
				{
					for (int t = 0; t < k; t++)
					{
						wsb.Append(L"*x");
					}
				}
				else
				{
					if (k > 0)
					{
						wsb.Append(L"x^");
						wsb.Append(k);
					}
				}
				if (k)
				{
					if (formal == false)
					{
						wsb.Append(L"+");
					}
					else if (facto[k-1] > 0)
					{
						wsb.Append(L"+");
					}
				}
			}

			return wsb.ToString();
		}

		WString GetInterpolationNoCalc()
		{
			tar_arr = tar.ToArray();
			val_arr = val.ToArray();

			SortingWith<type, type> sorting(tar_arr, val_arr, tar.Size());

			WStringBuilder wsb;

			for (int i = 0; i < tar.Size(); i++)
			{
				for (int j = 0; j < tar.Size(); j++)
				{
					if (i != j)
					{
						wsb.Append(L"(x-");
						wsb.Append(WString(tar_arr[j]));
						wsb.Append(L")");
						if (j != tar.Size() - 1 &&
							!(i == tar.Size() - 1 && j == tar.Size() - 2))
						{
							wsb.Append(L"*");
						}
					}
				}
				wsb.Append(L"/(");
				bool checked = false;
				for (int j = 0; j < tar.Size(); j++)
				{
					if (i != j)
					{
						wsb.Append(L"(");
						wsb.Append(WString(tar_arr[i]));
						wsb.Append(L"-");
						wsb.Append(WString(tar_arr[j]));
						wsb.Append(L")");
						if (j != tar.Size() - 1 &&
							!(!checked && j == tar.Size() - 2) )
						{
							wsb.Append(L"*");
						}
					}
					else
					{
						checked = true;
					}
				}
				wsb.Append(L")*");
				wsb.Append(WString(val_arr[i]));
				if (i != tar.Size() - 1)
				{
					wsb.Append(L"+");
				}
			}

			return wsb.ToString();
		}

	};
	
	template<typename type>
	class LagrangeInterpolationInt // ��׶��� ���׽�, ������(������)
	{

		//class VisualizeFraction
		//{
		//public:

		//	VisualizeFraction(type wh)
		//		: hi(wh)
		//		, lo(1)
		//	{
		//	}
		//	
		//	VisualizeFraction(type wh, type lo)
		//		: hi(wh)
		//		, lo(lo)
		//	{
		//		reduction();
		//	}
		//	
		//	VisualizeFraction()
		//		: VisualizeFraction(0)
		//	{
		//	}

		//	~VisualizeFraction()
		//	{
		//	}

		//	VisualizeFraction operator=(type set)
		//	{
		//		hi = set;
		//		lo = 1;
		//		return *this;
		//	}

		//	VisualizeFraction operator=(const VisualizeFraction& refer)
		//	{
		//		hi = refer.hi;
		//		lo = refer.lo;
		//		return *this;
		//	}

		//	VisualizeFraction operator*(const VisualizeFraction& refer)
		//	{
		//		type hit = refer.hi * hi;
		//		type lot = refer.lo * lo;

		//		type gcd = this->gcd(abs(hit), abs(lot));
		//		
		//		return VisualizeFraction(hit / gcd, lot / gcd);
		//	}

		//	VisualizeFraction operator*(type t)
		//	{
		//		return VisualizeFraction(hi * t, lo);
		//	}

		//	VisualizeFraction operator+(const VisualizeFraction& refer)
		//	{
  //              type lcm = this->lcm(abs(refer.lo), abs(lo));
		//		return VisualizeFraction(hi * lcm / lo + refer.hi * lcm / refer.lo, lcm);
		//	}

		//	WString operator*()
		//	{
		//		WStringBuilder wsb;
		//		reduction();

		//		if ((hi > 0 && lo < 0) || (hi < 0 && lo > 0))
		//			wsb.Append(L"-");

		//		wsb.Append(WString(abs(hi)));
		//		
		//		if (abs(lo) != 1)
		//		{
		//			wsb.Append(L"/");
		//			wsb.Append(WString(abs(lo)));
		//		}
		//		return wsb.ToString();
		//	}
		//	
		//	type gcd(type t1, type t2)
		//	{
		//		type t;
		//		if (t1 < t2)
		//			std::swap(t1, t2);
		//		while (t2)
		//		{
		//			t = t1 % t2;
		//			t1 = t2;
		//			t2 = t;
		//		}
		//		return t1;
		//	}

		//	type lcm(type t1, type t2)
		//	{
		//		//
		//		//                t1   *   t2
		//		//  LCM(t1, t2) = -----------
		//		//                GCD(t1, t2)
		//		//
		//		return t1 * t2 / gcd(t1, t2);
		//	}

		//	type val()
		//	{
		//		return hi / lo;
		//	}

		//	void reverse()
		//	{
		//		std::swap(hi, lo);
		//	}

		//	void reduction()
		//	{
		//		type gcd = this->gcd(abs(hi), abs(lo));
		//		hi /= gcd;
		//		lo /= gcd;
		//	}

		//	type hi; // numerator
		//	type lo; // denominator
		//};

		List<type> tar;	// x
		List<type> val;	// y
		type *tar_arr;
		type *val_arr;

	public:
		
		void AddValue(type x, type y)
		{
			tar.Append(x);
			val.Append(y);
		}

		WString GetInterpolation(bool formal = false)
		{
			tar_arr = tar.ToArray();
			val_arr = val.ToArray();

			SortingWith<type, type> sorting(tar_arr, val_arr, tar.Size());

			fraction **facto = new fraction*[tar.Size()]; // factorization coefficient
			for (int i = 0; i < tar.Size(); i++)
			{
				facto[i] = new fraction("0");
				//*facto[i] = 0;
			}

			for (int i = 0; i < tar.Size(); i++)
			{
				fraction **tmpfacto = new fraction*[tar.Size()];
				for (int j = 0; j < tar.Size(); j++)
					tmpfacto[j] = new fraction("0");

				// ���� ���� ����� ���
				fraction mul("1");
				for (int j = 0; j < tar.Size(); j++)
				{
					//*tmpfacto[j] = 0;
					if (i != j)
					{
						mul = mul * std::to_string(tar_arr[i] - tar_arr[j]);
					}
				}
				mul.reverse();
				mul = mul * std::to_string(val_arr[i]);
				*tmpfacto[0] = "1";

				// �μ����ؽ� ��� ���
				for (int j = 0, pure = 1; j < tar.Size(); j++)
				{
					if (i != j)
					{
						// 3x+4 ��� ���� �ִٰ� �ϸ�
						// 3x^2+4x+4 ���� �ٲ۴ٰ� �����ϸ� ��(����� ����)
						for (int k = pure - 1; k >= 0; k--)
						{
							*tmpfacto[k+1] = *tmpfacto[k];
						}

						// 3x^2+4x+0 (����� ����)
						*tmpfacto[0] = "0";

						// 3x+4���� x-3�� ���ϴ� ��쿣
						//  |3x^2 + 4x + 0
						// +|      -9x  -12
						//  --------------
						//   3x^2  -5x  -12
						// �� ���ȴ�.
						// ��, n�� ���׽��� ������� n-1�� ����ϸ� �ȴ�.
						for (int k = 0; k < pure; k++)
						{
							*tmpfacto[k] = *tmpfacto[k] + *tmpfacto[k + 1] * std::to_string(-tar_arr[j]);
						}

						// ��� ��ġ ex) pure�� 1�̸� x�� ����� 3�̸� x^3�� ����� ����Ŵ
						pure++;
					}
				}

				// ���մ��׽Ŀ� ���Ѵ�.
				for (int k = 0; k < tar.Size(); k++)
				{
					*facto[k] = *facto[k] + *tmpfacto[k] * mul;
				}

				for (int k = 0; k < tar.Size(); k++)
					delete tmpfacto[k];
				delete[] tmpfacto;
			}
			
			WStringBuilder wsb;

			// �� �����
			for (int k = tar.Size() - 1; k >= 0; k--)
			{
				wsb.Append(**facto[k]);
				if (formal == false)
				{
					for (int t = 0; t < k; t++)
					{
						wsb.Append(L"*x");
					}
				}
				else
				{
					if (k > 0)
					{
						wsb.Append(L"x^");
						wsb.Append(k);
					}
				}
				if (k)
				{
					if (formal == false)
					{
						wsb.Append(L"+");
					}
					//else if (facto[k-1]->val() > 0)
					else if (facto[k-1]->sign() == false)
					{
						wsb.Append(L"+");
					}
				}
			}

			return wsb.ToString();
		}

	};
}

#endif