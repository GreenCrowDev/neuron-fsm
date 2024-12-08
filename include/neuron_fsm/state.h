#ifndef NEURON_FSM_STATE_H
#define NEURON_FSM_STATE_H

#include "macros.h"

namespace neuron_fsm {

class State {
	FSM_STATE_BEHAVIOUR(enter)
	FSM_STATE_BEHAVIOUR_1_ARG(process, double)
	FSM_STATE_BEHAVIOUR(exit)
};

} // namespace neuron_fsm

#endif // NEURON_FSM_STATE_H
