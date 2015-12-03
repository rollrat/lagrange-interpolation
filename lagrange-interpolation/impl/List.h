/***

   RollRat Software Project.
   Copyright (C) 2015. rollrat. All Rights Reserved.

File name:
   
   List.h

Purpose:

	RollRat Library

Author:

   10-18-2015:   HyunJun Jeong, Creation

***/

#ifndef _LIST_9bf1541fdf7efd41b7b39543fd870ac4_
#define _LIST_9bf1541fdf7efd41b7b39543fd870ac4_

#include <type_traits>

namespace Utility {
	
	// 리스트 할당자
	// 향후 기본 할당자로 변경될 예정임
	template<typename type>
	class CollectionAllocator
	{
	public:
		typedef CollectionAllocator<type> this_type;
		typedef size_t size_type;
		typedef typename type value_type;
		typedef typename type* pointer;
		typedef typename type& reference;
		typedef typename const type* const_pointer;
		typedef typename const type& const_reference;

		template<typename other_type>
		struct rebind
		{
			typedef CollectionAllocator<other_type> other;
		};

		CollectionAllocator() throw() // noexcept
		{
		}

		CollectionAllocator(const CollectionAllocator&) throw()
		{
		}

		template<typename other_type>
		CollectionAllocator(const CollectionAllocator<other_type>&) throw()
		{
		}

		~CollectionAllocator() throw()
		{
		}

		pointer address(reference ref) const throw()
		{
			return std::addressof(ref);
		}

		const_pointer address(const_reference cref) const throw()
		{
			return std::addressof(cref);
		}

		pointer allocate(size_t size, const void* = 0)
		{
			if ( size > this->max_size() )
				throw "bad alloc"; // check

			return static_cast<type *> (new type[size * sizeof(type)]);
		}

		void deallocate(pointer ptr, size_t)
		{
			delete ptr;
		}

		template<typename up, typename ... _Args>
		void construct(up* p, _Args&&... args)
		{
			new ((void *)p) up(std::forward<_Args>(args) ...);
		}

		template<typename up>
		void destroy(up* ptr)
		{
			ptr->~up();
		}

		void construct(pointer ptr, const type& val)
		{
			new ((void *) ptr) type(val);
		}

		void destroy(pointer ptr)
		{
			ptr->~type();
		}

		size_type max_size() const throw()
		{
			return size_type(-1) / sizeof(type);
		}

	};

	//----------------------------------

	struct CollectionContainer;

	struct CollectionProxy
	{
		CollectionProxy()
			: cont(nullptr)
		{
		}

		CollectionContainer *cont;
	};

	struct CollectionContainer
	{
		CollectionContainer()
			: proxy(nullptr)
		{
		}

		CollectionContainer(const CollectionContainer&)
			: proxy(nullptr)
		{
		}

		~CollectionContainer() throw()
		{
		}

		CollectionProxy *proxy;
	};

	//----------------------------------

	template<typename node_type, class allocator>
	struct ListBase
	{
		typedef typename allocator::template rebind<node_type>::other altype;

		typedef typename allocator::pointer tptr;
		typedef typename allocator::template rebind<tptr>::other alptrtype;
	};

	// node value_type cast from type
	template<typename bind, typename type>
	struct ListTransfer
		: public CollectionContainer
	{
		typedef typename type::size_type size_type;
		typedef typename type::value_type value_type;
		typedef typename type::pointer pointer;
		typedef typename type::reference reference;
		typedef typename type::const_pointer const_pointer;
		typedef typename type::const_reference const_reference;
		typedef typename bind::value_type node_type;
		typedef typename bind::pointer node_pointer;
		typedef typename bind::reference node_reference;

		node_pointer head;
		node_pointer last;
		node_pointer tail;
	};

	template<typename base_type>
	class ListAlloc
		: public ListTransfer<typename base_type::altype, 
							typename base_type::altype::value_type>
	{
	public:
		typename base_type::altype alval;
		typedef typename base_type::altype altype;

		ListAlloc(const altype& al = altype())
			: alval(al)
		{
			typename altype::template rebind<CollectionProxy>::other alproxy(this->alval);
			this->proxy = alproxy.allocate(1);
			alproxy.construct(this->proxy, CollectionProxy());
		}

		~ListAlloc()
		{
			typename altype::template rebind<CollectionProxy>::other alproxy(this->alval);
			alproxy.destroy(this->proxy);
			alproxy.deallocate(this->proxy, 1);
			this->proxy = nullptr;
		}

		altype& get()
		{
			return this->alval;
		}

		const altype& get() const
		{
			return this->alval;
		}

	};

	//----------------------------------

	template<typename type>
	class ListNode final
	{
	public:
		typedef ListNode<type> this_type;
		typedef size_t size_type;
		typedef typename type value_type;
		typedef typename type* pointer;
		typedef typename type& reference;
		typedef typename const type* const_pointer;
		typedef typename const type& const_reference;

		this_type* next;
		this_type* prev;
		value_type data;
	};

	template<typename type, class allocator = CollectionAllocator<ListNode<type>>>
	class List
		: public ListAlloc<ListBase<ListNode<type>, allocator>>
	{
		typedef ListAlloc<ListBase<ListNode<type>, allocator>> base_type;

		typedef typename base_type::altype altype;
		
		typedef typename base_type::size_type size_type;
		typedef typename base_type::value_type value_type;
		typedef typename base_type::pointer pointer;
		typedef typename base_type::reference reference;
		typedef typename base_type::const_pointer const_pointer;
		typedef typename base_type::const_reference const_reference;
		typedef typename base_type::node_type node_type;
		typedef typename base_type::node_pointer node_pointer;
		typedef typename base_type::node_reference node_reference;

		size_type size;

	public:

		List()
			: base_type()
			, size(0)
		{
			__Init();
		}

		~List()
		{
			if (size > 0)
			{
				__Dispose();
			}
			else
			{
				this->get().deallocate(this->head, 1);
				this->get().deallocate(this->tail, 1);
			}
		}

		void Append(const value_type& val)
		{
			_Append(val);
		}
		void Append(value_type&& val)
		{
			_Append(std::forward<value_type> (val));
		}

		// 첫 번째 요소에 삽입
		void Serve(const value_type& val)
		{
			_Insert(0, val);
		}
		void Serve(value_type&& val)
		{
			_Insert(0, val);
		}

		void Insert(const size_type starts, const value_type& val)
		{
			_Insert(starts, val);
		}
		void Insert(const size_type starts, value_type&& val)
		{
			_Insert(starts, std::forward<value_type> (val));
		}
		
		template<typename func>
		void Each(func& function)
		{
			node_pointer nptpos = this->head->next;

			for (size_type i = 0; i < this->size; i++, nptpos = nptpos->next)
			{
				function(nptpos->data);
			}
		}

		template<typename func>
		void EachBreak(func& function)
		{
			node_pointer nptpos = this->head->next;

			for (size_type i = 0; i < this->size; i++, nptpos = nptpos->next)
			{
				if (function(nptpos->data) == true)
					return;
			}
		}


		bool Exist(const value_type& val)
		{
			node_pointer nptpos = this->head->next;

			for (size_type i = 0; i < this->size; i++, nptpos = nptpos->next)
			{
				if (nptpos->data == val)
					return true;
			}

			return false;
		}

		value_type ExistGet(const value_type& val)
		{
			node_pointer nptpos = this->head->next;

			for (size_type i = 0; i < this->size; i++, nptpos = nptpos->next)
			{
				if (nptpos->data == val)
					return true;
			}

			return false;
		}

		size_type Size() const
		{
			return this->size;
		}

		pointer ToArray()
		{
			pointer ptr = new value_type[this->size];
			node_pointer nptpos = this->head->next;

			for (size_type i = 0; i < this->size; i++, nptpos = nptpos->next)
			{
				ptr[i] = nptpos->data;
			}

			return ptr;
		}

	private:
		
		template<class... other_type>
		void _Append(other_type&&... val)
		{
			node_pointer npt = this->get().allocate(1);
			this->get().construct(std::addressof(npt->data),
				std::forward<other_type>(val)...);
			__ConnectBack(npt);
			this->size++;
		}

		template<class... other_type>
		void _Insert(const size_type starts, other_type&&... val)
		{
			node_pointer nptpos = this->head->next;

			for (size_type i = 0; i < starts && nptpos != tail; i++, nptpos = nptpos->next)
				;

			node_pointer npt = this->get().allocate(1);
			this->get().construct(std::addressof(npt->data),
				std::forward<other_type>(val)...);
			__ConnectWith(nptpos->prev, npt, nptpos);
			this->size++;
		}

		void __Dispose()
		{
			node_pointer npt = this->head->next;
			node_pointer nptnext = this->head->next->next;
			altype alval(this->get());

			for (size_type i = 0; i < this->size; i++)
			{
				nptnext = npt->next;
				alval.destroy(std::addressof(npt->data));
				this->get().deallocate(npt, 1);
				npt = nptnext;
			}

			this->get().deallocate(this->head, 1);
			this->get().deallocate(this->tail, 1);
		}

		void __ConnectBack(node_pointer& nptptr)
		{
			this->last->next = nptptr;
			nptptr->prev = this->last;
			nptptr->next = this->tail;
			this->tail->prev = nptptr;
			this->last = nptptr;
		}

		void __ConnectWith(node_pointer& left, node_pointer& middle, node_pointer& right)
		{
			left->next = middle;
			middle->prev = left;
			middle->next = right;
			right->prev = middle;
		}

		void __Init()
		{
			this->head = this->get().allocate(1);
			this->tail = this->get().allocate(1);

			// Connecting
			this->head->next = this->tail;
			this->tail->prev = this->head;
			this->head->prev = nullptr;
			this->tail->next = nullptr;

			this->last = this->head;
		}

	};

}

#endif