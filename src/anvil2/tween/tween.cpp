#include "tween.hpp"

#include <utility>
#include <algorithm>

Tween::Tween(float *ptr, float start, float end, float dur, EasingFunc ease) : target(ptr), start_value(start),
                                                                               end_value(end), duration(dur),
                                                                               elapsed(0.0f), easing(std::move(ease)),
                                                                               finished(false) {
    *target = start_value;
}

void Tween::update(const float dt) {
    if (finished) return;

    elapsed += dt;
    const float u = std::clamp(elapsed / duration, 0.0f, 1.0f);  // Use float, not double

    const float eased = easing(u);
    *target = start_value + (end_value - start_value) * eased;

    if (u >= 1.0f) {  // Use 1.0f for consistency
        *target = end_value;  // Force exact end value
        finished = true;
        if (on_complete) on_complete();
    }
}

void TweenManager::update(float delta_time) {
    for (auto& t : tween_list)
        t.update(delta_time);

    // std::erase_if(tween_list, [](const Tween& t){ return t.finished; });
    tween_list.erase(std::remove_if(tween_list.begin(), tween_list.end(), [](const Tween& t){ return t.finished; }), tween_list.end());
}

void Tween::set_on_complete(const std::function<void()> &callback) {
    on_complete = callback;
}

Tween *TweenManager::create_tween(float *target, float end_value, float duration, EasingFunc easing) {
    tween_list.emplace_back(target, *target, end_value, duration, easing);
    return &tween_list.back();
}

Tween *TweenManager::create_tween_from_to(float *target, float start_value, float end_value, float duration,
                                       EasingFunc easing) {
    tween_list.emplace_back(target, start_value, end_value, duration, easing);
    return &tween_list.back();
}

// should this do something?
void TweenManager::clear() {
    tween_list.clear();
}

TweenManager &TweenManager::get_instance() {
    static TweenManager inst;
    return inst;
}
