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


#ifndef HASHTABLE_H
#define HASHTABLE_H

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

#include <functional>
#include <unordered_map>
using namespace std;

/// Container which is derived from unordered_map
template<class T1, class T2>
class CONTAINER_EXPORT HashTable final : public unordered_map<T1, T2>
{
public:
    using unordered_map<T1, T2>::unordered_map;

    inline void
    appendElement(const T1 &key, const T2 &value),
    removeElement(const T1 &key);

    T2
    &setElement(T1 key, T2 &element);

    inline void
    iterateAll(function<void(T1 &)> function) const;
};

template<class T1, class T2>
inline void
HashTable<T1, T2>::
appendElement(const T1 &key, const T2 &value)
{
    this->emplace(key, value);
}

template<class T1, class T2>
inline void
HashTable<T1, T2>::
removeElement(const T1 &key)
{
    this->erase(this->find(key));
}

template<class T1, class T2>
T2 &
HashTable<T1, T2>::
setElement(const T1 key, T2 &element)
{
    // If key exists, let element point to the existing element,
    // else insert new element

    const auto &found = this->find(key);

    if (found != this->end()) {
        element = found->second;
    } else {
        this->emplace(key, element);
    }

    // Return reference to already existing or newly inserted element.
    return element;
}

template<class T1, class T2>
inline void
HashTable<T1, T2>::
iterateAll(function<void(T1 &key)> function) const
{
    for_each(this->begin(), this->end(), function);
}

#endif // HASHTABLE_H
