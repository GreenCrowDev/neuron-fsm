#ifndef NEURON_FSM_MACROS_H
#define NEURON_FSM_MACROS_H

#include "neuron_fsm/errors.h"
#include <functional>
#include <string>
#include <unordered_map>

// State behaviour macros.

#define FSM_STATE_BEHAVIOUR(name)                                                                               \
private:                                                                                                        \
	std::unordered_map<std::string, std::function<void()>> on_##name##_callbacks;                               \
                                                                                                                \
public:                                                                                                         \
	const std::unordered_map<std::string, std::function<void()>> &get_on_##name##_callbacks() const {           \
		return on_##name##_callbacks;                                                                           \
	}                                                                                                           \
	void add_on_##name##_callback(const std::string &p_name, const std::function<void()> &p_callback) {         \
		NFSM_ASSERT_RETURN_MSG(!p_name.empty(), "Callback name cannot be empty.");                              \
		auto result = on_##name##_callbacks.emplace(p_name, p_callback);                                        \
		NFSM_ASSERT_RETURN_MSG(result.second, "Callback '" + p_name + "' is already registered.");              \
	}                                                                                                           \
	void remove_on_##name##_callback(const std::string &p_name) {                                               \
		NFSM_ASSERT_RETURN_MSG(!p_name.empty(), "Callback name cannot be empty.");                              \
		auto it = on_##name##_callbacks.find(p_name);                                                           \
		NFSM_ASSERT_RETURN_MSG(it != on_##name##_callbacks.end(), "Callback '" + p_name + "' does not exist."); \
		on_##name##_callbacks.erase(it);                                                                        \
	}                                                                                                           \
	void on_##name() const {                                                                                    \
		for (const auto &pair : on_##name##_callbacks) {                                                        \
			pair.second();                                                                                      \
		}                                                                                                       \
	}                                                                                                           \
	void clear_on_##name##_callbacks() {                                                                        \
		on_##name##_callbacks.clear();                                                                          \
	}

#define FSM_STATE_BEHAVIOUR_1_ARG(name, type_0)                                                                 \
private:                                                                                                        \
	std::unordered_map<std::string, std::function<void(type_0)>> on_##name##_callbacks;                         \
                                                                                                                \
public:                                                                                                         \
	const std::unordered_map<std::string, std::function<void(type_0)>> &get_on_##name##_callbacks() const {     \
		return on_##name##_callbacks;                                                                           \
	}                                                                                                           \
	void add_on_##name##_callback(const std::string &p_name, const std::function<void(type_0)> &p_callback) {   \
		NFSM_ASSERT_RETURN_MSG(!p_name.empty(), "Callback name cannot be empty.");                              \
		auto result = on_##name##_callbacks.emplace(p_name, p_callback);                                        \
		NFSM_ASSERT_RETURN_MSG(result.second, "Callback '" + p_name + "' is already registered.");              \
	}                                                                                                           \
	void remove_on_##name##_callback(const std::string &p_name) {                                               \
		NFSM_ASSERT_RETURN_MSG(!p_name.empty(), "Callback name cannot be empty.");                              \
		auto it = on_##name##_callbacks.find(p_name);                                                           \
		NFSM_ASSERT_RETURN_MSG(it != on_##name##_callbacks.end(), "Callback '" + p_name + "' does not exist."); \
		on_##name##_callbacks.erase(it);                                                                        \
	}                                                                                                           \
	void on_##name(type_0 arg_0) const {                                                                        \
		for (const auto &pair : on_##name##_callbacks) {                                                        \
			pair.second(arg_0);                                                                                 \
		}                                                                                                       \
	}                                                                                                           \
	void clear_on_##name##_callbacks() {                                                                        \
		on_##name##_callbacks.clear();                                                                          \
	}

#endif // NEURON_FSM_MACROS_H
