#ifndef NEURON_FSM_ERRORS_H
#define NEURON_FSM_ERRORS_H

#include <iostream>

// Abnormally terminate the program.
#ifdef _MSC_VER
#define NFSM_GENERATE_TRAP() __debugbreak()
#else
#define NFSM_GENERATE_TRAP() __builtin_trap()
#endif

#define NFSM_INTERNAL_PRINT_ERROR(err, cond, msg)                     \
	std::cerr << err << "Assertion failed: \"" #cond "\" is false.\n" \
			  << msg << "\n"                                          \
			  << "File: " << __FILE__ << "(" << __LINE__ << ")\n";

// The following macros check a condition. If it fails, they print a message and crash the program.

#define NFSM_ASSERT_MSG(cond, msg)                      \
	if (!(cond)) {                                      \
		NFSM_INTERNAL_PRINT_ERROR("FATAL: ", cond, msg) \
		std::cerr.flush();                              \
		NFSM_GENERATE_TRAP();                           \
	} else                                              \
		((void)0)

#define NFSM_ASSERT(cond) NFSM_ASSERT_MSG(cond, "")

// The following macros check a condition. If it fails, they print a message, then return or continue.

#define NFSM_INTERNAL_ASSERT_ACT(cond, act, msg) \
	if (!(cond)) {                               \
		NFSM_INTERNAL_PRINT_ERROR("", cond, msg) \
		act;                                     \
	} else                                       \
		((void)0)

#define NFSM_ASSERT_RETURN(cond) NFSM_INTERNAL_ASSERT_ACT(cond, return, "")
#define NFSM_ASSERT_RETURN_MSG(cond, msg) NFSM_INTERNAL_ASSERT_ACT(cond, return, msg)
#define NFSM_ASSERT_RETURN_V(cond, retval) NFSM_INTERNAL_ASSERT_ACT(cond, return retval, "")
#define NFSM_ASSERT_RETURN_V_MSG(cond, retval, msg) NFSM_INTERNAL_ASSERT_ACT(cond, return retval, msg)
#define NFSM_ASSERT_CONTINUE(cond) NFSM_INTERNAL_ASSERT_ACT(cond, continue, "")
#define NFSM_ASSERT_CONTINUE_MSG(cond, msg) NFSM_INTERNAL_ASSERT_ACT(cond, continue, msg)

#endif // NEURON_FSM_ERRORS_H
