//
// Copyright (C) OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_INTRUSIVE_PTR_
#define __INET_INTRUSIVE_PTR_

#include <iosfwd>

#define INET_ALWAYS_INLINE __attribute__((always_inline)) inline

namespace inet {

/**
 * A smart pointer that uses intrusive reference counting.
 *
 * Relies on unqualified calls to
 *     void intrusive_ptr_add_ref(T *p);
 *     void intrusive_ptr_release(T *p);
 * The object is responsible for destroying itself.
 */
template <class T> class intrusive_ptr
{
  private:
    T *p;

  public:
    INET_ALWAYS_INLINE constexpr intrusive_ptr() noexcept : p(0) { }

    INET_ALWAYS_INLINE intrusive_ptr(T *p, bool add_ref = true) : p(p) {
        if (p != 0 && add_ref) intrusive_ptr_add_ref(p);
    }

    template <class U>
    INET_ALWAYS_INLINE intrusive_ptr(intrusive_ptr<U> const & rhs) : p(rhs.get()) {
        if (p != 0) intrusive_ptr_add_ref(p);
    }

    INET_ALWAYS_INLINE intrusive_ptr(intrusive_ptr const & rhs) : p(rhs.p) {
        if (p != 0) intrusive_ptr_add_ref(p);
    }

    INET_ALWAYS_INLINE ~intrusive_ptr() {
        if (p != 0) intrusive_ptr_release(p);
    }

    template <class U>
    INET_ALWAYS_INLINE intrusive_ptr & operator=(intrusive_ptr<U> const & rhs) {
        intrusive_ptr(rhs).swap(*this);
        return *this;
    }

    INET_ALWAYS_INLINE intrusive_ptr(intrusive_ptr && rhs) noexcept : p(rhs.p) {
        rhs.p = 0;
    }

    INET_ALWAYS_INLINE intrusive_ptr & operator=(intrusive_ptr && rhs) noexcept {
        intrusive_ptr(static_cast<intrusive_ptr &&>(rhs)).swap(*this);
        return *this;
    }

    template <class U> friend class intrusive_ptr;

    template <class U>
    INET_ALWAYS_INLINE intrusive_ptr(intrusive_ptr<U> && rhs) : p(rhs.p) {
        rhs.p = 0;
    }

    template <class U>
    INET_ALWAYS_INLINE intrusive_ptr & operator=(intrusive_ptr<U> && rhs) noexcept {
        intrusive_ptr(static_cast<intrusive_ptr<U> &&>(rhs)).swap(*this);
        return *this;
    }

    INET_ALWAYS_INLINE intrusive_ptr & operator=(intrusive_ptr const & rhs) {
        intrusive_ptr(rhs).swap(*this);
        return *this;
    }

    INET_ALWAYS_INLINE intrusive_ptr & operator=(T *rhs) {
        intrusive_ptr(rhs).swap(*this);
        return *this;
    }

    INET_ALWAYS_INLINE void reset() {
        intrusive_ptr().swap(*this);
    }

    INET_ALWAYS_INLINE void reset(T *rhs) {
        intrusive_ptr(rhs).swap(*this);
    }

    INET_ALWAYS_INLINE void reset(T *rhs, bool add_ref) {
        intrusive_ptr(rhs, add_ref).swap(*this);
    }

    INET_ALWAYS_INLINE T *get() const noexcept { return p; }

    INET_ALWAYS_INLINE T *detach() noexcept {
        T *ret = p;
        p = 0;
        return ret;
    }

    INET_ALWAYS_INLINE T & operator*() const noexcept { return *p; }

    INET_ALWAYS_INLINE T *operator->() const noexcept { return p; }

    INET_ALWAYS_INLINE explicit operator bool() const noexcept { return p != 0; }

    INET_ALWAYS_INLINE void swap(intrusive_ptr & rhs) noexcept {
        T *tmp = p;
        p = rhs.p;
        rhs.p = tmp;
    }

    INET_ALWAYS_INLINE unsigned int use_count() const noexcept { return p->use_count(); }
};

template <class T, class U>
INET_ALWAYS_INLINE bool operator==(intrusive_ptr<T> const & a, intrusive_ptr<U> const & b) noexcept
{
    return a.get() == b.get();
}

template <class T, class U>
INET_ALWAYS_INLINE bool operator!=(intrusive_ptr<T> const & a, intrusive_ptr<U> const & b) noexcept
{
    return a.get() != b.get();
}

template <class T, class U>
INET_ALWAYS_INLINE bool operator==(intrusive_ptr<T> const & a, U *b) noexcept
{
    return a.get() == b;
}

template <class T, class U>
INET_ALWAYS_INLINE bool operator!=(intrusive_ptr<T> const & a, U *b) noexcept
{
    return a.get() != b;
}

template <class T, class U>
INET_ALWAYS_INLINE bool operator==(T *a, intrusive_ptr<U> const & b) noexcept
{
    return a == b.get();
}

template <class T, class U>
INET_ALWAYS_INLINE bool operator!=(T *a, intrusive_ptr<U> const & b) noexcept
{
    return a != b.get();
}

template <class T>
INET_ALWAYS_INLINE bool operator==(intrusive_ptr<T> const & p, std::nullptr_t) noexcept
{
    return p.get() == 0;
}

template <class T>
INET_ALWAYS_INLINE bool operator==(std::nullptr_t, intrusive_ptr<T> const & p) noexcept
{
    return p.get() == 0;
}

template <class T>
INET_ALWAYS_INLINE bool operator!=(intrusive_ptr<T> const & p, std::nullptr_t) noexcept
{
    return p.get() != 0;
}

template <class T>
INET_ALWAYS_INLINE bool operator!=(std::nullptr_t, intrusive_ptr<T> const & p) noexcept
{
    return p.get() != 0;
}

template <class T>
INET_ALWAYS_INLINE bool operator<(intrusive_ptr<T> const & a, intrusive_ptr<T> const & b) noexcept
{
    return std::less<T *>()(a.get(), b.get());
}

template <class T>
INET_ALWAYS_INLINE void swap(intrusive_ptr<T> & lhs, intrusive_ptr<T> & rhs) noexcept
{
    lhs.swap(rhs);
}

template <class T>
INET_ALWAYS_INLINE T *get_pointer(intrusive_ptr<T> const & p) noexcept
{
    return p.get();
}

template <class T, class U>
INET_ALWAYS_INLINE intrusive_ptr<T> static_pointer_cast(intrusive_ptr<U> const & p)
{
    return static_cast<T *>(p.get());
}

template <class T, class U>
INET_ALWAYS_INLINE intrusive_ptr<T> const_pointer_cast(intrusive_ptr<U> const & p)
{
    return const_cast<T *>(p.get());
}

template <class T, class U>
INET_ALWAYS_INLINE intrusive_ptr<T> dynamic_pointer_cast(intrusive_ptr<U> const & p)
{
    return dynamic_cast<T *>(p.get());
}

template <class Y>
INET_ALWAYS_INLINE std::ostream & operator<< (std::ostream & os, intrusive_ptr<Y> const & p)
{
    os << p.get();
    return os;
}

template <typename T>
class intrusive_ref_counter;

template <typename T>
INET_ALWAYS_INLINE void intrusive_ptr_add_ref(const intrusive_ref_counter<T> *p) noexcept;
template <typename T>
INET_ALWAYS_INLINE void intrusive_ptr_release(const intrusive_ref_counter<T> *p) noexcept;

template <typename T>
class intrusive_ref_counter
{
  private:
    mutable unsigned int c;

  public:
    INET_ALWAYS_INLINE intrusive_ref_counter() noexcept : c(0) { }
    INET_ALWAYS_INLINE intrusive_ref_counter(intrusive_ref_counter const&) noexcept : c(0) { }

    INET_ALWAYS_INLINE unsigned int use_count() const noexcept { return c; }

    INET_ALWAYS_INLINE intrusive_ref_counter& operator= (intrusive_ref_counter const&) noexcept { return *this; }

    INET_ALWAYS_INLINE intrusive_ptr<T> shared_from_this() { return intrusive_ptr<T>(static_cast<T *>(this)); }

  protected:
    INET_ALWAYS_INLINE ~intrusive_ref_counter() = default;

    friend void intrusive_ptr_add_ref<T>(const intrusive_ref_counter<T> *p) noexcept;
    friend void intrusive_ptr_release<T>(const intrusive_ref_counter<T> *p) noexcept;
};

template <typename T>
INET_ALWAYS_INLINE void intrusive_ptr_add_ref(const intrusive_ref_counter<T> *p) noexcept
{
    ++(p->c);
}

template <typename T>
INET_ALWAYS_INLINE void intrusive_ptr_release(const intrusive_ref_counter<T> *p) noexcept
{
    if (--(p->c) == 0)
        delete static_cast<const T*>(p);
}

} // namespace inet

#endif  // #ifndef __INET_INTRUSIVE_PTR_
