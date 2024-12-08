#include "doctest.h"
#include "neuron_fsm/state.h"

TEST_CASE("State construction.") {
	neuron_fsm::State state;

	// State has 0 callbacks.
	CHECK(state.get_on_enter_callbacks().size() == 0);
	CHECK(state.get_on_process_callbacks().size() == 0);
	CHECK(state.get_on_exit_callbacks().size() == 0);

	bool entered = false;
	bool processed = false;
	bool exited = false;

	// Add callbacks.
	state.add_on_enter_callback("0", [&]() { entered = true; });
	state.add_on_process_callback("1", [&](double) { processed = true; });
	state.add_on_exit_callback("2", [&]() { exited = true; });

	CHECK(state.get_on_enter_callbacks().size() == 1);
	CHECK(state.get_on_process_callbacks().size() == 1);
	CHECK(state.get_on_exit_callbacks().size() == 1);

	// Call callbacks.
	state.on_enter();
	state.on_process(0.1);
	state.on_exit();

	CHECK(entered);
	CHECK(processed);
	CHECK(exited);

	// Remove callbacks.
	state.remove_on_enter_callback("0");
	state.remove_on_process_callback("1");
	state.remove_on_exit_callback("2");

	CHECK(state.get_on_enter_callbacks().size() == 0);
	CHECK(state.get_on_process_callbacks().size() == 0);
	CHECK(state.get_on_exit_callbacks().size() == 0);
}
