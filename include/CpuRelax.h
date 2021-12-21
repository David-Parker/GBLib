// Portable CPU relax macro. Uses PAUSE instruction on x86 and YIELD for ARM, NOP for all other architectures.
// See: http://cr.openjdk.java.net/~dchuyko/8186670/yield/spinwait.html

#if defined(_MSC_VER)
	#include <windows.h>
#endif

#define instr_noop (void)0

// MSVC
#if defined(_MSC_VER)
	#define cpu_relax() YieldProcessor();
// GCC & Clang
#elif defined(__GNUC__) || defined(__clang__)
	// ARM
	#if (defined(__ARM_ARCH) || defined(__TARGET_ARCH_ARM) || \
	    defined(__TARGET_ARCH_THUMB) || defined(_M_ARM) || \
	    defined(__arm__) || defined(__arm64) || defined(__thumb__) || \
	    defined(_M_ARM64) || defined(__aarch64__) || defined(__AARCH64EL__) || \
	    defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || \
	    defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || \
	    defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || \
	    defined(__ARM_ARCH_6KZ__) || defined(__ARM_ARCH_6T2__) || \
	    defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__) || \
	    defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_4__))
		
		// Only ARMv6k+ has yield instruction 
		#if (defined(__ARM_ARCH_6K__) || \
				defined(__ARM_ARCH_6Z__) || \
				defined(__ARM_ARCH_6ZK__) || \
				defined(__ARM_ARCH_6T2__) || \
				defined(__ARM_ARCH_7__) || \
				defined(__ARM_ARCH_7A__) || \
				defined(__ARM_ARCH_7R__) || \
				defined(__ARM_ARCH_7M__) || \
				defined(__ARM_ARCH_7S__) || \
				defined(__ARM_ARCH_8A__) || \
				defined(__aarch64__))
			#define cpu_relax() asm volatile ("yield" ::: "memory");
		#else
			#define cpu_relax() asm volatile ("nop" ::: "memory");
        #endif
    // x86
    #elif (defined(i386) || defined(__i386__) || \
			defined(__i486__) || defined(__i586__) || \
			defined(__i686__) || defined(__i386) || \
			defined(_M_IX86) || defined(_X86_) || \
			defined(__THW_INTEL__) || defined(__I86__) || \
			defined(__INTEL__) || defined(__x86_64) || defined(__x86_64__) || \
    		defined(__amd64__) || defined(__amd64) || \
    		defined(_M_X64))
		#define cpu_relax() asm volatile ("pause" ::: "memory");
	#else
		#define cpu_relax() instr_noop;
	#endif
// Unsupported Compiler
#else
	#define cpu_relax() instr_noop;
#endif