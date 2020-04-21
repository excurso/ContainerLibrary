/******************************************************************************
ContainerLibrary - A C++ library extending the functionality of some STL containers
Copyright (C) 2019-2020 Waldemar Zimpel <hspp@utilizer.de>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/


#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define DECL_EXPORT __declspec(dllexport)
    #define DECL_IMPORT __declspec(dllimport)
#else
    #define DECL_EXPORT __attribute__((visibility("default")))
    #define DECL_IMPORT __attribute__((visibility("default")))
#endif

#ifdef CONTAINER_LIBRARY
#  define CONTAINER_EXPORT DECL_EXPORT
#else
#  define CONTAINER_EXPORT DECL_IMPORT
#endif

#include <algorithm>
#include <functional>
#include <vector>
using namespace std;

/// Container which is derived from vector
template<class T>
class CONTAINER_EXPORT DataContainer : public vector<T>
{
public:
    using vector<T>::vector;

	DataContainer(size_t reserve_size);

    inline void
    appendElement(const T &element),
    removeElement(const T &element),

    iterateAll(const function<void(const T &element)> &) const;

    inline bool
    hasElement(const T &element);

    void
    removeElement(const function<bool(const T &element)> &),
    removeElements(const function<bool(const T &element)> &),
    iterateAllConditional(const function<bool(const T &element)> &) const,
    iterateAllReverse(const function<void(const T &element)> &) const;

    typename DataContainer<T>::iterator
    find(const T &element);
};

template<class T>
DataContainer<T>::DataContainer(const size_t reserve_size)
{
	this->reserve(reserve_size);
}

template<class T>
typename DataContainer<T>::iterator
DataContainer<T>::
find(const T &element)
{
    return std::find(this->begin(), this->end(), element);
}

template<class T>
inline void
DataContainer<T>::
appendElement(const T &element)
{
    this->emplace_back(element);
}

template<class T>
inline void
DataContainer<T>::
removeElement(const T &element)
{
    this->erase(this->find(element));
}

template<class T>
void
DataContainer<T>::
removeElement(const function<bool(const T &element)> &function)
{
    // The callback function has to return true to remove the element and
    // stop iteration

    iterateAllConditional([&](const T &element) -> bool {
        if (function(element)) {
            removeElement(element);

            // Stop iteration by returning false
            return false;
        }

        // Iterate through elements by returning true
        return true;
    });
}

template<class T>
void
DataContainer<T>::
removeElements(const function<bool(const T &element)> &function)
{
    // The callback function has to return true to remove the element.
    // Iteration does't stop until the end.

    iterateAll([&](const T &element) -> void {
        if (function(element))
            removeElement(element);
    });
}

template<class T>
inline bool
DataContainer<T>::
hasElement(const T &element)
{
    return this->find(element) != this->end();
}

template<class T>
inline void
DataContainer<T>::
iterateAll(const function<void(const T &element)> &function) const
{
    iterateAllConditional([&function](const T &element) -> bool {
        function(element); return true;
    });
}

template<class T>
void
DataContainer<T>::
iterateAllConditional(const function<bool(const T &element)> &function) const
{
    // The callback function has to return true to continue iteration.
    // If false is returned, iteration will be stopped.

    if (!this->empty()) {
        uint64_t size = this->size();

        for (auto itr = this->begin(); itr != this->end(); ++itr) {
            // If the callback function returns false, abort iteration.
            if (!function(*itr)) return;
            // If current element has been removed, decrement and continue.
            if (size != this->size()) { --size; --itr; }
        }
    }
}

template<class T>
void
DataContainer<T>::
iterateAllReverse(const function<void(const T &element)> &function) const
{
    if (!this->empty())
        for (auto itr = this->rbegin(); itr != this->rend(); ++itr)
            function(*itr);
}

#endif // DATACONTAINER_H
