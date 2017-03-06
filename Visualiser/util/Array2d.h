#pragma once

#include "util/log.h"

#include <cstring>

#include <functional>

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

    //Initialise the array to a value
    void init(T v) {
        for (int x = 0; x < m_width; x++) {
            for (int y = 0; y < m_height; y++) {
                this->get(x, y) = v;
            }
        }
    }

    /**
     * @brief Copy a section from another array to this
     * @param other Other array to copy from
     * @param ox    Initial X coord in other array
     * @param oy    Initial Y coord in other array
     * @param tx    Initial X coord in this array
     * @param ty    Initial Y coord in this array
     * @param w     Width of the area to copy (X length)
     * @param h     Height of the area to copy (Y length)
     */
    void copyFrom(const Array2D<T>& other, int ox, int oy, int tx, int ty, int w, int h);

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

    void forEach(std::function<void(int,int,T)> del) {
        for (int x = 0; x < m_width; x++) {
            for (int y = 0; y < m_height; y++) {
                del(x, y, this->get(x, y));
            }
        }
    }

    void copyFrom(const T* raw, size_t len) {
        memcpy(data, raw, len);
    }

private:
    const int m_width;
    const int m_height;

    T *data;

};

template <class T>
void Array2D<T>::copyFrom(const Array2D<T>& other, int ox, int oy, int tx, int ty, int w, int h)
{
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            get(tx + x, ty + y) = other.get(ox + x, oy + y);
        }
    }
}
