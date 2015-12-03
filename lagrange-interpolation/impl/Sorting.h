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
	//	�˰���: Introspective Sort
	//	�䱸: >, < ������
	//	�⺻���� ������ quicksort�̸� �־��� ���� heap�� ����.
	//	�� �˰����� <algorithm>::std::sort�� �����ϴ�.
	//	WString�� ��� �����ڰ� ���ԵǾ������� String�� ����.
	//	WString ���� ����:
	//		�� �˰����� ����ϸ� WString�� �Ҹ��� ȣ���� �Ұ����Ͽ�
	//		ġ������ ���� ������ �޸� ������ �߻��մϴ�.
	//		���� �� ��� ������ String_Destruct_Off�� �̿��Ͽ�,
	//		������ �Ҹ��� ������ ���ƾ��ϱ� ������ ����� �� ȿ�����Դϴ�.
	//		���� �� Ŭ������ WString�� ���� Ư��ȭ �Ǿ��ֽ��ϴ�.
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
				// ���� �κп� ���ؼ� �������� ���
				int partition = h - l + 1;

				// 16 32 64 128 �׽�Ʈ ���
				// Ư�� �迭���� 128�� ���� ���� ����� ����
				// ������ 32�� ������� ���� ����
				// ���� 32�� ����
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
	//	WString�� Ưȭ�� SortingŬ����
	//	Ư��ȭ�� ���� �� ���� 6�質 ������.
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
	//	WString�� ���� �����ϴ� ���� �� ���� ����
	//	�׷� ��찡 �ſ� �幰�� ������ Ư��ȭ���� �ʾҽ��ϴ�.
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
	//	����ü ���������� ĳ���� �� ���İ����� �� �ϴ�.
	//
}

#endif