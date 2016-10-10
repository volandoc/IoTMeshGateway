#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <cstdlib>

template <typename T,unsigned S>
inline unsigned sizeofarr(const T (&v)[S]) { return S; }

#endif // GWTEMPLATES_H
