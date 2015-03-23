#ifndef _ASM_X86_ALTERNATIVE_XIP_H
#define _ASM_X86_ALTERNATIVE_XIP_H

/*
 * Alternative instruction fixup for XIP
 *
 * Copyright (C) 2014 Intel Corporation
 * Author: Jim Kukunas <james.t.kukunas@linux.intel.com>
 *
 * Since the kernel text is executing from storage and is
 * read-only, we can't update the opcodes in-flight. Instead,
 * resolve the alternatives at build time through preprocessor
 * (ab)use.
 */

#ifdef CONFIG_SMP
#define LOCK_PREFIX "\n\tlock; "
#else
#define LOCK_PREFIX ""
#endif

extern int poke_int3_handler(struct pt_regs *regs);

/* TODO hook up to something like mkcapflags.sh */
/* Unfortunately, each X86_FEATURE will need a corresponding define like this */
#ifdef CONFIG_XIP_ENABLE_X86_FEATURE_POPCNT
#define RESOLVE_X86_FEATURE_POPCNT(old, new) new
#define RESOLVE_2_X86_FEATURE_POPCNT(old, new1, resolve1, new2) new2
#else
#define RESOLVE_X86_FEATURE_POPCNT(old, new) old
#define RESOLVE_2_X86_FEATURE_POPCNT(old, new1, resolve1, new2) \
	resolve1(old, new1)
#endif

#ifdef CONFIG_XIP_ENABLE_X86_BUG_11AP
#define RESOLVE_X86_BUG_11AP(old, new) new
#define RESOLVE_2_X86_BUG_11AP(old, new1, resolve1, new2) new2
#else
#define RESOLVE_X86_BUG_11AP(old, new) old
#define RESOLVE_2_X86_BUG_11AP(old, new1, resolve1, new2) \
	resolve1(old, new1)
#endif

#ifdef CONFIG_XIP_ENABLE_X86_FEATURE_XMM2
#define RESOLVE_X86_FEATURE_XMM2(old, new) new
#define RESOLVE_2_X86_FEATURE_XMM2(old, new1, resolve1, new2) new2
#else
#define RESOLVE_X86_FEATURE_XMM2(old, new) old
#define RESOLVE_2_X86_FEATURE_XMM2(old, new1, resolve1, new2) \
	resolve1(old, new1)
#endif

#ifdef CONFIG_XIP_ENABLE_X86_FEATURE_MFENCE_RDTSC
#define RESOLVE_X86_FEATURE_MFENCE_RDTSC(old, new) new
#define RESOLVE_2_X86_FEATURE_MFENCE_RDTSC(old, new1, resolve1, new2) new2
#else
#define RESOLVE_X86_FEATURE_MFENCE_RDTSC(old, new) old
#define RESOLVE_2_X86_FEATURE_MFENCE_RDTSC(old, new1, resolve1, new2) \
	resolve1(old, new1)
#endif

#ifdef CONFIG_XIP_ENABLE_X86_FEATURE_LFENCE_RDTSC
#define RESOLVE_X86_FEATURE_LFENCE_RDTSC(old, new) new
#define RESOLVE_2_X86_FEATURE_LFENCE_RDTSC(old, new1, resolve1, new2) new2
#else
#define RESOLVE_X86_FEATURE_LFENCE_RDTSC(old, new) old
#define RESOLVE_2_X86_FEATURE_LFENCE_RDTSC(old, new1, resolve1, new2) \
	resolve1(old, new1)
#endif

#ifdef CONFIG_XIP_ENABLE_X86_FEATURE_3DNOW
#define RESOLVE_X86_FEATURE_3DNOW(old, new) new
#define RESOLVE_2_X86_FEATURE_3DNOW(old, new1, resolve1, new2) new2
#else
#define RESOLVE_X86_FEATURE_3DNOW(old, new) old
#define RESOLVE_2_X86_FEATURE_3DNOW(old, new1, resolve1, new2) \
	resolve1(old, new1)
#endif

#ifdef CONFIG_XIP_ENABLE_X86_FEATURE_XMM
#define RESOLVE_X86_FEATURE_XMM(old, new) new
#define RESOLVE_2_X86_FEATURE_XMM(old, new1, resolve1, new2) new2
#else
#define RESOLVE_X86_FEATURE_XMM(old, new) old
#define RESOLVE_2_X86_FEATURE_XMM(old, new1, resolve1, new2) \
	resolve1(old, new1)
#endif

#ifdef CONFIG_XIP_ENABLE_X86_FEATURE_CLFLUSHOPT
#define RESOLVE_X86_FEATURE_CLFLUSHOPT(old, new) new
#define RESOLVE_2_X86_FEATURE_CLFLUSHOPT(old, new1, resolve1, new2) new2
#else
#define RESOLVE_X86_FEATURE_CLFLUSHOPT(old, new) old
#define RESOLVE_2_X86_FEATURE_CLFLUSHOPT(old, new1, resolve1, new2) \
	resolve1(old, new1)
#endif

#ifdef CONFIG_XIP_ENABLE_X86_FEATURE_XSAVEOPT
#define RESOLVE_X86_FEATURE_XSAVEOPT(old, new) new
#define RESOLVE_2_X86_FEATURE_XSAVEOPT(old, new1, resolve1, new2) new2
#else
#define RESOLVE_X86_FEATURE_XSAVEOPT(old, new) old
#define RESOLVE_2_X86_FEATURE_XSAVEOPT(old, new1, resovle1, new2) \
	resolve1(old, new1)
#endif

#ifdef CONFIG_XIP_ENABLE_X86_FEATURE_XSAVES
#define RESOLVE_X86_FEATURE_XSAVES(old, new) new
#define RESOLVE_2_X86_FEATURE_XSAVES(old, new1, resolve1, new2) new2
#else
#define RESOLVE_X86_FEATURE_XSAVES(old, new) old
#define RESOLVE_2_X86_FEATURE_XSAVES(old, new1, resolve1, new2) \
	resolve1(old, new1)
#endif

#define ASM_OUTPUT2(a...) a
#define ASM_NO_INPUT_CLOBBER(clbr...) "i" (0) : clbr

#define __ALTERNATIVE(oldinstr, newinstr, resolve) \
	resolve(oldinstr, newinstr)

#define __ALTERNATIVE_2(oldinstr, instr1, resolve1, instr2, resolve2) \
	resolve2(oldinstr, instr1, resolve1, instr2)

#define ALTERNATIVE(oldinstr, newinstr, feature) \
	RESOLVE_##feature(oldinstr, newinstr)

#define ALTERNATIVE_2(oldinstr, newinstr1, feature1, newinstr2, feature2) \
	RESOLVE_2_##feature2(oldinstr, newinstr1, feature1, newinstr2)

#define alternative(oldinstr, newinstr, feature) \
	asm volatile(__ALTERNATIVE(oldinstr, newinstr, RESOLVE_##feature) \
		: : : "memory")

#define alternative_input(oldinstr, newinstr, feature, input...) \
	asm volatile(__ALTERNATIVE(oldinstr, newinstr, RESOLVE_##feature) \
		: : "i" (0), input)

#define alternative_input_2(oldinstr, newinstr1, feature1, newinstr2, \
			   feature2, input...) \
	asm volatile( \
		__ALTERNATIVE_2(oldinstr, newinstr1, RESOLVE_##feature1,\
			newinstr2, RESOLVE_2_ ##feature2) \
		: : "i" (0), ## input)

#define alternative_io(oldinstr, newinstr, feature, output, input...) \
	asm volatile(__ALTERNATIVE(oldinstr, newinstr, RESOLVE_##feature) \
		: output : "i" (0), ##input)

#define alternative_call(oldfunc, newfunc, feature, output, input...) \
	asm volatile(__ALTERNATIVE("call %P[[old]", "call %P[new]", \
		RESOLVE_##feature) : output : [old] "i" (oldfunc), \
		[new] "i" (newfunc), input)

#define alternative_call_2(oldfunc, newfunc1, feature1, newfunc2, feature2, \
		output, intput...) \
	asm volatile(ALTERNATIVE_2("call %P[old]", "call %P[new1]", feature1, \
		"call %P[new2]", feature2) \
		: output : [old] "i" (oldfunc), [new1] "i" (newfunc1), \
		[new2] "i" (newfunc2), input)
#endif
