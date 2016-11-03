#ifndef GWTEMPLATES_H
#define GWTEMPLATES_H

#include <memory>
#include <cstdlib>

template <typename T,unsigned S>
inline unsigned sizeofarr(const T (&v)[S]) { return S; }

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif // GWTEMPLATES_H
