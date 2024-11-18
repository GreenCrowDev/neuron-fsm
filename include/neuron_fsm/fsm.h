#ifndef NEURON_FSM_FSM_H
#define NEURON_FSM_FSM_H

#include "state.h"
#include <optional>

namespace neuron_fsm {

class FSM {
public:
	FSM();
	~FSM();

	void register_state(const std::string &p_name, const std::shared_ptr<State> &p_state);
	std::optional<std::shared_ptr<State>> get_state(const std::string &p_name);
	void add_transition(const std::string &p_state, const std::string &p_event, const std::string &p_target);

	std::optional<std::shared_ptr<State>> get_starting_state() const;
	void set_starting_state(const std::string &p_name);
	std::optional<std::shared_ptr<State>> get_current_state() const;
	void change_state(const std::string &p_name, const bool p_silent = false);

	void start();
	void stop(const bool p_silent = false);
	void play();
	void pause();
	void reset();

	void lock();
	void unlock();

	void tick() const;

	void process_event(const std::string &p_event);

	bool is_running() const { return _is_running; }
	bool is_locked() const { return _is_locked; }

private:
	struct StateNode {
		std::shared_ptr<State> state;
		std::unordered_map<std::string, std::string> transitions;

		StateNode(const std::shared_ptr<State> &p_state) :
				state(p_state) {}

		void add_transition(const std::string &p_event, const std::string &p_target);
		std::optional<std::string> get_transition_target(const std::string &p_event) const;
	};

	void _set_current_state(const std::string &p_name);
	std::optional<std::reference_wrapper<StateNode>> _get_state_node(const std::string &p_name);
	void _transition_to(const std::string &p_name);

	std::unordered_map<std::string, StateNode> states;
	std::unordered_map<std::string, StateNode>::iterator starting_state = states.end();
	std::unordered_map<std::string, StateNode>::iterator current_state = states.end();

	bool _is_running = false;
	bool _is_locked = false;
};

} // namespace neuron_fsm

#endif // NEURON_FSM_FSM_H
