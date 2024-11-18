#ifndef NEURON_FSM_MACROS_H
#define NEURON_FSM_MACROS_H

// StateBehaviour

#define NFSM_DECLARE_STATE_BEHAVIOUR(name)                                     \
private:                                                                       \
	std::vector<std::function<void()>> on_##name##_callbacks;                  \
                                                                               \
public:                                                                        \
	const std::vector<std::function<void()>> get_on_##name##_callback() const; \
	void add_on_##name##_callback(const std::function<void()> &p_callback);    \
	void on_##name() const;                                                    \
	void clear_on_##name##_callbacks();

#define NFSM_DEFINE_STATE_BEHAVIOUR(class_name, name)                                       \
	const std::vector<std::function<void()>> class_name::get_on_##name##_callback() const { \
		return on_##name##_callbacks;                                                       \
	}                                                                                       \
	void class_name::add_on_##name##_callback(const std::function<void()> &p_callback) {    \
		on_##name##_callbacks.push_back(p_callback);                                        \
	}                                                                                       \
	void class_name::on_##name() const {                                                    \
		for (const auto &callback : on_##name##_callbacks) {                                \
			callback();                                                                     \
		}                                                                                   \
	}                                                                                       \
	void class_name::clear_on_##name##_callbacks() {                                        \
		on_##name##_callbacks.clear();                                                      \
	}

#endif // NEURON_FSM_MACROS_H
