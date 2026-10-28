
#include <functional>
#include <vector>

/**
 * @brief A signal/slot mechanism for connecting and emitting callbacks.
 *
 * This class allows you to connect multiple callbacks (slots) that will be
 * invoked when the signal is emitted. It supports variadic arguments for
 * the callbacks.
 *
 * @tparam Args The types of the arguments passed to the callbacks.
 */ 
template<typename... Args>
class Signal {
public:
    using Callback = std::function<void(Args...)>;

    /**
     * @brief Connect a callback to the signal.
     *
     * @param callback The callback function to connect.
     */
    void connect(Callback callback) {
        callbacks.push_back(std::move(callback));
    }

    /**
     * @brief Emit the signal, invoking all connected callbacks.
     *
     * @param args The arguments to pass to the callbacks.
     */
    void emit(Args... args) const {
        for (const auto& callback : callbacks) {
            callback(args...);
        }
    }

private:
    std::vector<Callback> callbacks;
};
