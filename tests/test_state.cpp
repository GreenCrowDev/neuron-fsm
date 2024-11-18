#include "doctest.h"
#include "neuron_fsm/state.h"

TEST_CASE("State construction.") {
	neuron_fsm::State state;

	// State has 0 callbacks.
	CHECK(state.get_on_enter_callback().size() == 0);
	CHECK(state.get_on_process_callback().size() == 0);
	CHECK(state.get_on_exit_callback().size() == 0);

	bool entered = false;
	bool processed = false;
	bool exited = false;

	state.add_on_enter_callback([&]() { entered = true; });
	state.add_on_process_callback([&]() { processed = true; });
	state.add_on_exit_callback([&]() { exited = true; });

	CHECK(state.get_on_enter_callback().size() == 1);
	CHECK(state.get_on_process_callback().size() == 1);
	CHECK(state.get_on_exit_callback().size() == 1);

	state.on_enter();
	state.on_process();
	state.on_exit();

	CHECK(entered);
	CHECK(processed);
	CHECK(exited);
}
