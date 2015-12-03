/***

   RollRat Software Project.
   Copyright (C) 2015. rollrat. All Rights Reserved.

File name:
   
   Sorting.h

Purpose:

	RollRat Library

Author:

   10-25-2015:   HyunJun Jeong, Creation

***/

#ifndef _SORTING_9bf1541fdf7efd41b7b39543fd870ac4_
#define _SORTING_9bf1541fdf7efd41b7b39543fd870ac4_

#include "WString.h"

namespace Utility {

	//
	//	알고리즘: Introspective Sort
	//	요구: >, < 연산자
	//	기본적인 성능은 quicksort이며 최악의 경우는 heap과 같다.
	//	이 알고리즘은 <algorithm>::std::sort와 같습니다.
	//	WString의 경우 연산자가 포함되어있으나 String은 없다.
	//	WString 성능 보고:
	//		이 알고리즘을 사용하면 WString의 소멸자 호출이 불가피하여
	//		치명적인 정렬 오류나 메모리 누수가 발생합니다.
	//		또한 이 경우 조잡한 String_Destruct_Off를 이용하여,
	//		강제로 소멸자 동작을 막아야하기 때문에 상당히 비 효율적입니다.
	//		따라서 이 클래스는 WString에 대해 특수화 되어있습니다.
	//
	template<typename type>
	class Sorting
	{
		size_t m_size;
		type*  source;

	public:
		
		Sorting(type* m_what, size_t size)
			: source(m_what)
			, m_size(size)
		{
			IntroSort(0, m_size - 1, flog2() * 2);
		}

	private:

		int flog2()
		{
			size_t tsz = m_size;
			int ret = 0;
			for ( ; tsz; tsz >>= 1)
				ret++;
			return ret;
		}

		void Swap(int t1, int t2)
		{
			if (t1 != t2)
			{
				type x = source[t1];
				source[t1] = source[t2];
				source[t2] = x;
			}
		}

		void SwapCompare(int t1, int t2)
		{
			if (t1 != t2)
			{
				if (source[t1] > source[t2])
				{
					type x = source[t1];
					source[t1] = source[t2];
					source[t2] = x;
				}
			}
		}

		// l: first, h:last
		void IntroSort(int l, int h, int depth)
		{
			while (l < h)
			{
				// 작은 부분에 한해선 삽입정렬 사용
				int partition = h - l + 1;

				// 16 32 64 128 테스트 결과
				// 특정 배열에선 128이 가장 빠른 결과를 보임
				// 하지만 32가 평균으로 가장 빠름
				// 따라서 32로 설정
				if (partition <= 32)
				{
					if (partition == 1)
						return;
					if (partition == 2)
					{
						SwapCompare(l, h);
						return;
					}
					if (partition == 3)
					{
						SwapCompare(l, h - 1);
						SwapCompare(l, h);
						SwapCompare(h - 1, h);
						return;
					}

					InsertionSort(l, h);
					return;
				}

				if (depth == 0)
				{
					HeapSort(l, h);
					return;
				}
				depth--;

				int p = pivot(l, h);
				IntroSort(p + 1, h, depth);
				h = p - 1;
			}
		}

		int pivot(int l, int h)
		{
			int mid = l + (h - l) >> 1;

			SwapCompare(l, mid);
			SwapCompare(l, h);
			SwapCompare(mid, h);

			type pivot = source[mid];
			Swap(mid, h - 1);

			int left = l, right = h - 1;

			while (left < right)
			{
				while (source[++left] < pivot)
					;
				while (pivot < source[--right])
					;

				if (left >= right)
					break;

				Swap(left, right);
			}

			Swap(left, h - 1);
			return left;
		}

		void InsertionSort(int l, int h)
		{
			int i, j;
			type t;
			for (i = l; i < h; i++)
			{
				j = i;
				t = source[i + 1];
				while (j >= l && source[j] > t)
				{
					source[j + 1] = source[j];
					j--;
				}
				source[j + 1] = t;
			}
		}

		void DownHeap(int i, int n, int l)
		{
			type d = source[l + i - 1];
			int ch;
			while (i <= n >> 1)
			{
				ch = i << 1;
				if (ch < n && source[l + ch - 1] < source[l + ch])
					ch++;
				if (!(d < source[l + ch - 1]))
					break;
				source[l + i - 1] = source[l + ch - 1];
				i = ch;
			}
			source[l + i - 1] = d;
		}

		// O(n*log(n))
		void HeapSort(int l, int h)
		{
			int n = h - l + 1;
			for (int i = n >> 1; i >= 1; i--)
			{
				DownHeap(i, n, l);
			}
			for (int i = n; i > 1; i--)
			{
				Swap(l, l + i - 1);
				DownHeap(1, i - 1, l);
			}
		}

	};

	//
	//	WString에 특화된 Sorting클래스
	//	특수화를 안한 것 보다 6배나 빠르다.
	//
	template<>
	class Sorting<WString>
	{
		size_t m_size;
		WString*  source;

	public:
		
		Sorting(WString* m_what, size_t size)
			: source(m_what)
			, m_size(size)
		{
			IntroSort(0, m_size - 1, flog2() * 2);
		}

	private:

		int flog2()
		{
			size_t tsz = m_size;
			int ret = 0;
			for ( ; tsz; tsz >>= 1)
				ret++;
			return ret;
		}

		void Swap(int t1, int t2)
		{
			if (t1 != t2)
			{
				source[t1].Swap(source[t2]);
			}
		}

		void SwapCompare(int t1, int t2)
		{
			if (t1 != t2)
			{
				if (source[t1] > source[t2])
				{
					source[t1].Swap(source[t2]);
				}
			}
		}

		void IntroSort(int l, int h, int depth)
		{
			while (l < h)
			{
				int partition = h - l + 1;

				if (partition <= 32)
				{
					if (partition == 1)
						return;
					if (partition == 2)
					{
						SwapCompare(l, h);
						return;
					}
					if (partition == 3)
					{
						SwapCompare(l, h - 1);
						SwapCompare(l, h);
						SwapCompare(h - 1, h);
						return;
					}

					InsertionSort(l, h);
					return;
				}

				if (depth == 0)
				{
					HeapSort(l, h);
					return;
				}
				depth--;

				int p = pivot(l, h);
				IntroSort(p + 1, h, depth);
				h = p - 1;
			}
		}

		int pivot(int l, int h)
		{
			int mid = l + ((h - l) >> 1);

			SwapCompare(l, mid);
			SwapCompare(l, h);
			SwapCompare(mid, h);

			WString pivot;
			pivot.Clone(source[mid]);
			Swap(mid, h - 1);

			int left = l, right = h - 1;

			while (left < right)
			{
				while (source[++left] < pivot)
					;
				while (pivot < source[--right])
					;

				if (left >= right)
					break;

				Swap(left, right);
			}

			Swap(left, h - 1);
			return left;
		}

		void InsertionSort(int l, int h)
		{
			int i, j;
			WString t;
			for (i = l; i < h; i++)
			{
				j = i;
				t.Clone(source[i + 1]);
				while (j >= l && source[j] > t)
				{
					source[j + 1].Clone(source[j]);
					j--;
				}
				source[j + 1].Clone(t);
			}
		}

		void DownHeap(int i, int n, int l)
		{
			WString d;
			d.Clone(source[l + i - 1]);
			int ch;
			while (i <= n >> 1)
			{
				ch = i << 1;
				if (ch < n && source[l + ch - 1] < source[l + ch])
					ch++;
				if (!(d < source[l + ch - 1]))
					break;
				source[l + i - 1].Clone(source[l + ch - 1]);
				i = ch;
			}
			source[l + i - 1].Clone(d);
		}

		// O(n*log(n))
		void HeapSort(int l, int h)
		{
			int n = h - l + 1;
			for (int i = n >> 1; i >= 1; i--)
			{
				DownHeap(i, n, l);
			}
			for (int i = n; i > 1; i--)
			{
				Swap(l, l + i - 1);
				DownHeap(1, i - 1, l);
			}
		}

	};
	
	template<typename type, typename atype>
	class SortingWith
	{
		size_t m_size;
		type  *source;
		atype  *another;
		
	public:
		
		SortingWith(type* m_what, atype* another, size_t size)
			: source(m_what)
			, m_size(size)
			, another(another)
		{
			IntroSort(0, m_size - 1, flog2() * 2);
		}

	private:

		int flog2()
		{
			size_t tsz = m_size;
			int ret = 0;
			for ( ; tsz; tsz >>= 1)
				ret++;
			return ret;
		}

		void Swap(int t1, int t2)
		{
			if (t1 != t2)
			{
				type x = source[t1];
				source[t1] = source[t2];
				source[t2] = x;

				atype y = another[t1];
				another[t1] = another[t2];
				another[t2] = y;
			}
		}

		void SwapCompare(int t1, int t2)
		{
			if (t1 != t2)
			{
				if (source[t1] > source[t2])
				{
					type x = source[t1];
					source[t1] = source[t2];
					source[t2] = x;

					atype y = another[t1];
					another[t1] = another[t2];
					another[t2] = y;
				}
			}
		}

		// l: first, h:last
		void IntroSort(int l, int h, int depth)
		{
			while (l < h)
			{
				int partition = h - l + 1;
				if (partition <= 32)
				{
					if (partition == 1)
						return;
					if (partition == 2)
					{
						SwapCompare(l, h);
						return;
					}
					if (partition == 3)
					{
						SwapCompare(l, h - 1);
						SwapCompare(l, h);
						SwapCompare(h - 1, h);
						return;
					}

					InsertionSort(l, h);
					return;
				}

				if (depth == 0)
				{
					HeapSort(l, h);
					return;
				}
				depth--;

				int p = pivot(l, h);
				IntroSort(p + 1, h, depth);
				h = p - 1;
			}
		}

		int pivot(int l, int h)
		{
			int mid = l + (h - l) / 2;

			SwapCompare(l, mid);
			SwapCompare(l, h);
			SwapCompare(mid, h);

			type pivot = source[mid];
			Swap(mid, h - 1);

			int left = l, right = h - 1;

			while (left < right)
			{
				while (source[++left] < pivot)
					;
				while (pivot < source[--right])
					;

				if (left >= right)
					break;

				Swap(left, right);
			}

			Swap(left, h - 1);
			return left;
		}

		void InsertionSort(int l, int h)
		{
			int i, j;
			type t;
			atype ta;
			for (i = l; i < h; i++)
			{
				j = i;
				t = source[i + 1];
				ta = another[i + 1];
				while (j >= l && source[j] > t)
				{
					source[j + 1] = source[j];
					another[j + 1] = another[j];
					j--;
				}
				source[j + 1] = t;
				another[j + 1] = ta;
			}
		}

		void DownHeap(int i, int n, int l)
		{
			type d = source[l + i - 1];
			atype da = another[l + i - 1];
			int ch;
			while (i <= n / 2)
			{
				ch = i * 2;
				if (ch < n && source[l + ch - 1] < source[l + ch])
					ch++;
				if (!(d < source[l + ch - 1]))
					break;
				source[l + i - 1] = source[l + ch - 1];
				another[l + i - 1] = another[l + ch - 1];
				i = ch;
			}
			source[l + i - 1] = d;
			another[l + i - 1] = da;
		}

		void HeapSort(int l, int h)
		{
			int n = h - l + 1;
			for (int i = n / 2; i >= 1; i--)
			{
				DownHeap(i, n, l);
			}
			for (int i = n; i > 1; i--)
			{
				Swap(l, l + i - 1);
				DownHeap(1, i - 1, l);
			}
		}

	};

	//
	//	WString을 같이 정렬하는 경우는 본 적이 없고
	//	그럴 경우가 매우 드물기 때문에 특수화하지 않았습니다.
	//
	template<typename atype>
	class SortingWith<WString, atype>
	{
		size_t m_size;
		WString  *source;
		atype  *another;
		
	public:
		
		SortingWith(WString* m_what, atype* another, size_t size)
			: source(m_what)
			, m_size(size)
			, another(another)
		{
			IntroSort(0, m_size - 1, flog2() * 2);
		}

	private:

		int flog2()
		{
			size_t tsz = m_size;
			int ret = 0;
			for ( ; tsz; tsz >>= 1)
				ret++;
			return ret;
		}

		void Swap(int t1, int t2)
		{
			if (t1 != t2)
			{
				source[t1].Swap(source[t2]);

				atype y = another[t1];
				another[t1] = another[t2];
				another[t2] = x;
			}
		}

		void SwapCompare(int t1, int t2)
		{
			if (t1 != t2)
			{
				if (source[t1] > source[t2])
				{
					source[t1].Swap(source[t2]);

					atype y = another[t1];
					another[t1] = another[t2];
					another[t2] = y;
				}
			}
		}

		// l: first, h:last
		void IntroSort(int l, int h, int depth)
		{
			while (l < h)
			{
				int partition = h - l + 1;
				if (partition <= 32)
				{
					if (partition == 1)
						return;
					if (partition == 2)
					{
						SwapCompare(l, h);
						return;
					}
					if (partition == 3)
					{
						SwapCompare(l, h - 1);
						SwapCompare(l, h);
						SwapCompare(h - 1, h);
						return;
					}

					InsertionSort(l, h);
					return;
				}

				if (depth == 0)
				{
					HeapSort(l, h);
					return;
				}
				depth--;

				int p = pivot(l, h);
				IntroSort(p + 1, h, depth);
				h = p - 1;
			}
		}

		int pivot(int l, int h)
		{
			int mid = l + (h - l) / 2;

			SwapCompare(l, mid);
			SwapCompare(l, h);
			SwapCompare(mid, h);

			WString pivot;
			pivot.Clone(source[mid]);
			Swap(mid, h - 1);

			int left = l, right = h - 1;

			while (left < right)
			{
				while (source[++left] < pivot)
					;
				while (pivot < source[--right])
					;

				if (left >= right)
					break;

				Swap(left, right);
			}

			Swap(left, h - 1);
			return left;
		}

		void InsertionSort(int l, int h)
		{
			int i, j;
			WString t;
			atype ta;
			for (i = l; i < h; i++)
			{
				j = i;
				t.Clone(source[i + 1]);
				ta = another[i + 1];
				while (j >= l && source[j] > t)
				{
					source[j + 1].Clone(source[j]);
					another[j + 1] = another[j];
					j--;
				}
				source[j + 1].Clone(t);
				another[j + 1] = ta;
			}
		}

		void DownHeap(int i, int n, int l)
		{
			WString d;
			d.Clone(ource[l + i - 1]);
			atype da = another[l + i - 1];
			int ch;
			while (i <= n / 2)
			{
				ch = i * 2;
				if (ch < n && source[l + ch - 1] < source[l + ch])
					ch++;
				if (!(d < source[l + ch - 1]))
					break;
				source[l + i - 1].Clone(source[l + ch - 1]);
				another[l + i - 1] = another[l + ch - 1];
				i = ch;
			}
			source[l + i - 1].Clone(d);
			another[l + i - 1] = da;
		}

		void HeapSort(int l, int h)
		{
			int n = h - l + 1;
			for (int i = n / 2; i >= 1; i--)
			{
				DownHeap(i, n, l);
			}
			for (int i = n; i > 1; i--)
			{
				Swap(l, l + i - 1);
				DownHeap(1, i - 1, l);
			}
		}

	};

	//
	//	구조체 오프셋으로 캐스팅 후 정렬가능할 듯 하다.
	//
}

#endif