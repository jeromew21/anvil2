/**
 * @file tween.hpp
 * @brief Tweening library for smooth value interpolation with easing functions.
 * TODO: this needs LOTS of improvement
 * First of all, it doesn't obey syntax conventions
 */
#pragma once

#include <vector>
#include <functional>
#include <cmath>

// Easing functions
namespace Easing {
    inline float Linear(const float t) { return t; }

    inline float EaseInQuad(const float t) { return t * t; }

    inline float EaseOutQuad(const float t) { return t * (2.0f - t); }

    inline float EaseInOutQuad(const float t) {
        return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
    }

    inline float EaseInCubic(const float t) { return t * t * t; }

    inline float EaseOutCubic(const float t) {
        return 1.0f + (t - 1.0f) * (t - 1.0f) * (t - 1.0f);;
    }

    inline float EaseInOutCubic(const float t) {
        return t < 0.5f ? 4.0f * t * t * t : (t - 1.0f) * (2.0f * t - 2.0f) * (2.0f * t - 2.0f) + 1.0f;
    }

    inline float EaseInSine(const float t) { return 1.0f - cosf(t * 1.57079632679f); }

    inline float EaseOutSine(const float t) { return sinf(t * 1.57079632679f); }

    inline float EaseInOutSine(const float t) { return -(cosf(3.14159265359f * t) - 1.0f) / 2.0f; }
}

using EasingFunc = std::function<float(float)>;

class Tween {
public:
    float *target;

    float start_value;

    float end_value;

    float duration;

    float elapsed;

    EasingFunc easing;

    std::function<void()> on_complete;

    bool finished;

    Tween(float *ptr, float start, float end, float dur, EasingFunc ease = Easing::Linear);

    void update(float dt);

    void set_on_complete(const std::function<void()> &callback);
};

class TweenManager {
    // TODO: this could theoretically be a problem. Consider switching to vector of unique_ptr
    // Every time calling CreateTween*, we could be invalidating other Tween references.
    // This probably won't be a problem unless we're creating Tweens from different threads or
    // doing stuff out of order, but still a problem nonetheless.
    std::vector<Tween> tween_list;

public:
    Tween *create_tween(float *target, float end_value, float duration,
                       EasingFunc easing = Easing::Linear);

    Tween *create_tween_from_to(float *target, float start_value, float end_value,
                             float duration, EasingFunc easing = Easing::Linear);

    void update(float delta_time);

    void clear();

    static TweenManager& get_instance();
};
