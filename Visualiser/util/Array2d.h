#pragma once

#include "util/log.h"

template <class T>
/**
 * @brief 2 dimensional array class
 */
class Array2D {
public:
    Array2D(int width, int height)
        : m_width(width), m_height(height)
    {
        data = new T[m_width * m_height];
    }

    ~Array2D() {
        delete [] data;
    }

    T &get(int x, int y) const {
        S_ASSERT(x >= 0 && x < m_width && y >= 0 && y < m_height, "Array2D index out of bounds")
        return data[y * m_width + x];
    }

    int width() const {
        return m_width;
    }

    int height() const {
        return m_height;
    }

private:
    const int m_width;
    const int m_height;

    T *data;

};
