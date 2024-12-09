#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "neuron_fsm/fsm.h"
#include "neuron_fsm/state.h"

TEST_CASE("FSM initializazion.") {
	neuron_fsm::FSM fsm;

	// Check initial state of the FSM.
	CHECK_FALSE(fsm.is_locked());
	CHECK_FALSE(fsm.is_running());

	// Register a state.
	auto state = std::make_shared<neuron_fsm::State>();
	std::string state_name = "Start";
	fsm.register_state(state_name, state);

	// Check if the state was registered.
	auto registered_state = fsm.get_state(state_name);
	CHECK(registered_state.has_value());
	CHECK(registered_state.value() == state);

	// Set the starting state.
	fsm.set_starting_state(state_name);
	CHECK(fsm.get_starting_state_name() == state_name);
	CHECK(fsm.get_starting_state() == state);
}

TEST_CASE("FSM behaviour.") {
	neuron_fsm::FSM fsm;

	SUBCASE("FSM cannot be locked if a starting state has not been set.") {
		fsm.lock();
		CHECK_FALSE(fsm.is_locked());
	}

	// Create and set starting state.
	auto start_state = std::make_shared<neuron_fsm::State>();
	std::string start_state_name = "Start";
	fsm.register_state(start_state_name, start_state);
	fsm.set_starting_state(start_state_name);

	// Create and register a new state.
	auto new_state = std::make_shared<neuron_fsm::State>();
	std::string new_state_name = "New State";
	fsm.register_state(new_state_name, new_state);

	// Add transition between the starting state and the new state.
	const std::string start_to_new_event = "Start to New";
	fsm.add_transition(start_state_name, start_to_new_event, new_state_name);

	SUBCASE("FSM cannot be started if it is not locked.") {
		fsm.start();
		CHECK_FALSE(fsm.is_running());
	}

	SUBCASE("FSM locked behaviour.") {
		fsm.lock();
		CHECK(fsm.is_locked());

		// States cannot be registered while the FSM is locked.
		auto another_state = std::make_shared<neuron_fsm::State>();
		const std::string another_state_name = "Another State";
		fsm.register_state(another_state_name, another_state);
		CHECK_FALSE(fsm.get_state(another_state_name));

		// FSM running behaviour.

		fsm.start();
		CHECK(fsm.is_running());
		CHECK(fsm.get_current_state_name() == start_state_name);
		CHECK(fsm.get_current_state() == start_state);

		// Change state through an event.
		fsm.process_event(start_to_new_event);
		CHECK(fsm.get_current_state_name() == new_state_name);
		CHECK(fsm.get_current_state() == new_state);

		// FSM manual state changing.
		fsm.change_state(start_state_name);
		CHECK(fsm.get_current_state_name() == start_state_name);
		CHECK(fsm.get_current_state() == start_state);

		fsm.pause();
		CHECK_FALSE(fsm.is_running());

		fsm.play();
		CHECK(fsm.is_running());

		fsm.stop();
		CHECK_FALSE(fsm.is_running());
		CHECK_FALSE(fsm.get_current_state_name());
		CHECK_FALSE(fsm.get_current_state());

		fsm.start();
		CHECK(fsm.is_running());

		fsm.reset();
		CHECK_FALSE(fsm.is_running());
		CHECK_FALSE(fsm.get_current_state_name());
		CHECK_FALSE(fsm.get_current_state());

		fsm.unlock();
		CHECK_FALSE(fsm.is_locked());
	}
}
