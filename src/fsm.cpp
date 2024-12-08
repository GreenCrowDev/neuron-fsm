#include "neuron_fsm/fsm.h"
#include "neuron_fsm/errors.h"

namespace neuron_fsm {

FSM::FSM() {
}

FSM::~FSM() {
}

void FSM::register_state(const std::string &p_name, const std::shared_ptr<State> &p_state) {
	NFSM_ASSERT_RETURN_MSG(!_is_locked, "States cannot be registered while the FSM is locked.");
	NFSM_ASSERT_RETURN_MSG(!p_name.empty(), "State name cannot be empty.");
	auto result = states.emplace(p_name, StateNode(p_state));
	NFSM_ASSERT_RETURN_MSG(result.second, "State '" + p_name + "' is already registered.");
}

std::optional<std::shared_ptr<State>> FSM::get_state(const std::string &p_name) {
	auto state_node = _get_state_node(p_name);
	NFSM_ASSERT_RETURN_V_MSG(state_node, std::nullopt, "State '" + p_name + "' does not exist.");
	return state_node->get().state;
}

void FSM::add_transition(const std::string &p_state, const std::string &p_event, const std::string &p_target) {
	NFSM_ASSERT_RETURN_MSG(!_is_locked, "Cannot add transition while the FSM is locked.");
	NFSM_ASSERT_RETURN_MSG(!p_state.empty(), "State name cannot be empty.");

	auto state_node = _get_state_node(p_state);
	NFSM_ASSERT_RETURN_MSG(state_node, "State node '" + p_state + "' not found.");

	auto target = _get_state_node(p_target);
	NFSM_ASSERT_RETURN_MSG(target, "Target state node '" + p_target + "' not found.");

	state_node->get().add_transition(p_event, p_target);
}

std::optional<std::shared_ptr<State>> FSM::get_starting_state() const {
	NFSM_ASSERT_RETURN_V_MSG(starting_state != states.end(), std::nullopt, "Starting state is not set.");
	return starting_state->second.state;
}

void FSM::set_starting_state(const std::string &p_name) {
	NFSM_ASSERT_RETURN_MSG(!_is_locked, "FSM is locked.");
	NFSM_ASSERT_RETURN_MSG(!_is_running, "FSM is running.");
	auto it = states.find(p_name);
	NFSM_ASSERT_RETURN_MSG(it != states.end(), "State '" + p_name + "' does not exists.");
	starting_state = it;
}

void FSM::change_state(const std::string &p_name, const bool p_silent) {
	NFSM_ASSERT_RETURN_MSG(_is_locked, "FSM is not locked.");
	NFSM_ASSERT_RETURN_MSG(_is_running, "FSM is not running.");

	_transition_to(p_name);
}

std::optional<std::shared_ptr<State>> FSM::get_current_state() const {
	NFSM_ASSERT_RETURN_V_MSG(_is_locked, std::nullopt, "FSM is not locked.");
	NFSM_ASSERT_RETURN_V_MSG(_is_running, std::nullopt, "FSM is not running.");
	NFSM_ASSERT_RETURN_V_MSG(current_state != states.end(), std::nullopt, "Current state is not set.");
	return current_state->second.state;
}

void FSM::start() {
	NFSM_ASSERT_RETURN_MSG(_is_locked, "FSM is not locked.");
	NFSM_ASSERT_RETURN_MSG(!_is_running, "FSM is already running.");
	NFSM_ASSERT_RETURN_MSG(starting_state != states.end(), "Starting state is not set.");

	_is_running = true;
	current_state = starting_state;
	current_state->second.state->on_enter();
}

void FSM::stop(const bool p_silent) {
	NFSM_ASSERT_RETURN_MSG(_is_locked, "FSM is not locked.");
	NFSM_ASSERT_RETURN_MSG(_is_running, "FSM is not running.");

	auto state = get_current_state().value_or(nullptr);
	NFSM_ASSERT_RETURN_MSG(state, "No current state is set.");

	if (!p_silent) {
		state->on_exit();
	}

	current_state = states.end();
	_is_running = false;
}

void FSM::play() {
	NFSM_ASSERT_RETURN_MSG(!_is_running, "FSM is already running.");
	_is_running = true;
}

void FSM::pause() {
	NFSM_ASSERT_RETURN_MSG(_is_running, "FSM is not running.");
	_is_running = false;
}

void FSM::reset() {
	NFSM_ASSERT_RETURN_MSG(_is_locked, "FSM is not locked.");
	current_state = states.end();
	_is_running = false;
}

void FSM::lock() {
	NFSM_ASSERT_RETURN_MSG(!_is_locked, "FSM is already locked.");
	NFSM_ASSERT_RETURN_MSG(starting_state != states.end(), "FSM cannot be locked if a starting state has not been set.");
	_is_locked = true;
}

void FSM::unlock() {
	NFSM_ASSERT_RETURN_MSG(!_is_running, "FSM is running and cannot be unlocked.");
	NFSM_ASSERT_RETURN_MSG(_is_locked, "FSM is already unlocked.");
	_is_locked = false;
}

void FSM::tick(double p_delta) const {
	NFSM_ASSERT_RETURN_MSG(_is_locked, "FSM is not locked.");
	NFSM_ASSERT_RETURN_MSG(_is_running, "FSM is not running.");

	auto state = get_current_state().value_or(nullptr);
	NFSM_ASSERT_RETURN_MSG(state, "No current state is set.");
	state->on_process(p_delta);
}

void FSM::process_event(const std::string &p_event) {
	NFSM_ASSERT_RETURN_MSG(_is_locked, "FSM is not locked.");
	NFSM_ASSERT_RETURN_MSG(_is_running, "FSM is not running.");

	auto next_state = current_state->second.get_transition_target(p_event);
	NFSM_ASSERT_RETURN_MSG(next_state, "No transition for event '" + p_event + "' exists in state '" + current_state->first + "'.");

	_transition_to(next_state.value());
}

void FSM::StateNode::add_transition(const std::string &p_event, const std::string &p_target) {
	NFSM_ASSERT_RETURN_MSG(!p_event.empty(), "Event name cannot be empty.");
	NFSM_ASSERT_RETURN_MSG(!p_target.empty(), "Target name cannot be empty.");
	auto result = transitions.emplace(p_event, p_target);
	NFSM_ASSERT_RETURN_MSG(result.second, "Transition '" + p_event + "' already exists.");
}

std::optional<std::string> FSM::StateNode::get_transition_target(const std::string &p_event) const {
	auto it = transitions.find(p_event);
	NFSM_ASSERT_RETURN_V_MSG(it != transitions.end(), std::nullopt, "Transition '" + p_event + "' does not exists.");
	return it->second;
}

void FSM::_set_current_state(const std::string &p_name) {
	auto it = states.find(p_name);
	NFSM_ASSERT_RETURN_MSG(it != states.end(), "State '" + p_name + "' does not exists.");
	current_state = it;
}

std::optional<std::reference_wrapper<FSM::StateNode>> FSM::_get_state_node(const std::string &p_name) {
	NFSM_ASSERT_RETURN_V_MSG(!p_name.empty(), std::nullopt, "State name cannot be empty.");
	auto it = states.find(p_name);
	NFSM_ASSERT_RETURN_V_MSG(it != states.end(), std::nullopt, "State node '" + p_name + "' does not exist.");
	return std::ref(it->second);
}

void FSM::_transition_to(const std::string &p_name) {
	auto state = get_current_state().value_or(nullptr);
	NFSM_ASSERT_RETURN_MSG(state, "No current state is set.");
	state->on_exit();

	_set_current_state(p_name);

	state = get_current_state().value_or(nullptr);
	NFSM_ASSERT_RETURN_MSG(state, "No current state is set.");
	state->on_enter();
}

} // namespace neuron_fsm
