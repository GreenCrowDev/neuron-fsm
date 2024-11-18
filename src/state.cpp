#include "neuron_fsm/state.h"
#include "neuron_fsm/errors.h"

namespace neuron_fsm {

State::State() {
}

NFSM_DEFINE_STATE_BEHAVIOUR(State, enter)
NFSM_DEFINE_STATE_BEHAVIOUR(State, process)
NFSM_DEFINE_STATE_BEHAVIOUR(State, exit)

} // namespace neuron_fsm
