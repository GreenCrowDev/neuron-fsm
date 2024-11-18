#ifndef NEURON_FSM_STATE_H
#define NEURON_FSM_STATE_H

#include "macros.h"
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace neuron_fsm {

class State {
	NFSM_DECLARE_STATE_BEHAVIOUR(enter)
	NFSM_DECLARE_STATE_BEHAVIOUR(process)
	NFSM_DECLARE_STATE_BEHAVIOUR(exit)
};

} // namespace neuron_fsm

#endif // NEURON_FSM_STATE_H
