#pragma once

#include <cstddef>

template <typename T>
class Handler {
    public:
        operator T&() {
            return object_;
        }

    protected:
        T object_;
};
