/*
 * \brief   Fiasco utilities
 * \author  Christian Helmuth
 * \date    2006-04-11
 *
 * Is very practical now, but please keep the errors of the l4util pkg in mind.
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _CORE__INCLUDE__UTIL_H_
#define _CORE__INCLUDE__UTIL_H_

/* Genode includes */
#include <base/stdint.h>
#include <base/printf.h>
#include <rm_session/rm_session.h>
#include <util/touch.h>
#include <fiasco/thread_helper.h>

/* Fiasco includes */
namespace Fiasco {
#include <l4/sys/types.h>
#include <l4/sys/ipc.h>
#include <l4/sys/kdebug.h>
#include <l4/sys/ktrace.h>
}

namespace Genode {

	inline void log_event(const char *s)
	{
		Fiasco::fiasco_tbuf_log(s);
	}

	inline void log_event(const char *s, unsigned v1, unsigned v2, unsigned v3)
	{
		Fiasco::fiasco_tbuf_log_3val(s, v1, v2, v3);
	}

	inline void panic(const char *s)
	{
		using namespace Fiasco;
		outstring(s);
		enter_kdebug("> panic <");
	}

	inline void touch_ro(const void *addr, unsigned size)
	{
		using namespace Fiasco;
		unsigned char const volatile *bptr;
		unsigned char const          *eptr;

		bptr = (unsigned char const volatile *)(((unsigned)addr) & L4_PAGEMASK);
		eptr = (unsigned char const *)(((unsigned)addr + size - 1) & L4_PAGEMASK);
		for ( ; bptr <= eptr; bptr += L4_PAGESIZE)
			touch_read(bptr);
	}

	inline void touch_rw(const void *addr, unsigned size)
	{
		using namespace Fiasco;
		unsigned char volatile *bptr;
		unsigned char const    *eptr;

		bptr = (unsigned char volatile *)(((unsigned)addr) & L4_PAGEMASK);
		eptr = (unsigned char const *)(((unsigned)addr + size - 1) & L4_PAGEMASK);
		for (; bptr <= eptr; bptr += L4_PAGESIZE)
			touch_read_write(bptr);
	}

	inline addr_t trunc_page(addr_t addr)
	{
		using namespace Fiasco;
		return l4_trunc_page(addr);
	}

	inline addr_t round_page(addr_t addr)
	{
		using namespace Fiasco;
		return l4_round_page(addr);
	}

	inline addr_t round_superpage(addr_t addr)
	{
		using namespace Fiasco;
		return l4_round_superpage(addr);
	}

	constexpr size_t get_page_size()            { return L4_PAGESIZE;      }
	constexpr size_t get_page_size_log2()       { return L4_LOG2_PAGESIZE; }
	constexpr size_t get_super_page_size()      { return L4_SUPERPAGESIZE; }
	constexpr size_t get_super_page_size_log2() { return L4_LOG2_SUPERPAGESIZE; }

	inline void print_page_fault(const char *msg, addr_t pf_addr, addr_t pf_ip,
	                             Rm_session::Fault_type pf_type,
	                             unsigned long badge)
	{
		Native_thread_id tid;
		tid.raw = badge;
		printf("%s (%s pf_addr=%p pf_ip=%p from %x.%02x)\n", msg,
		       pf_type == Rm_session::WRITE_FAULT ? "WRITE" : "READ",
		       (void *)pf_addr, (void *)pf_ip,
		       (int)tid.id.task, (int)tid.id.lthread);
	}

	inline addr_t map_src_addr(addr_t core_local_addr, addr_t phys_addr) {
		return core_local_addr; }

	inline size_t constrain_map_size_log2(size_t size_log2) { return size_log2; }
}

#endif /* _CORE__INCLUDE__UTIL_H_ */
