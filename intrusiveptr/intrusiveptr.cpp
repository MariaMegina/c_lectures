#include <utility>
#include <cassert>
#include <iostream>

template <typename T>
class TRefCounter {
public:
    TRefCounter() : refCount(0) {}

    void AddRef() {
        ++refCount;
    }

    void ReleaseRef() {
        assert(refCount > 0);
        --refCount;
    }

    int RefCount() const {
        return refCount;
    }

private:
    int refCount;
};

template <typename T>
class TIntrusivePtr {
public:
    TIntrusivePtr() : ptr(nullptr) {}

    explicit TIntrusivePtr(T* rawPtr) : ptr(rawPtr) {
        if (ptr) {
            ptr->AddRef();
        }
    }

    TIntrusivePtr(const TIntrusivePtr& other) : ptr(other.ptr) {
        if (ptr) {
            ptr->AddRef();
        }
    }

    TIntrusivePtr(TIntrusivePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    TIntrusivePtr& operator=(const TIntrusivePtr& other) {
        if (this != &other) {
            Reset();
            ptr = other.ptr;
            if (ptr) {
                ptr->AddRef();
            }
        }
        return *this;
    }

    TIntrusivePtr& operator=(TIntrusivePtr&& other) noexcept {
        if (this != &other) {
            Reset();
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    ~TIntrusivePtr() {
        Reset();
    }

    void Reset(T* newPtr = nullptr) {
        if (ptr) {
            ptr->ReleaseRef();
            if (ptr->RefCount() == 0) {
                delete ptr;
            }
        }
        ptr = newPtr;
        if (ptr) {
            ptr->AddRef();
        }
    }
    
    void Reset(TIntrusivePtr&& other) noexcept {
    	if (ptr != other.ptr) {
            Reset();
            ptr = other.ptr;
            other.ptr = nullptr;
    	}
    }


    T* Release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    T* Get() const {
        return ptr;
    }

    int UseCount() const {
        return ptr ? ptr->RefCount() : 0;
    }

    T* operator->() const {
        return ptr;
    }

    T& operator*() const {
        return *ptr;
    }

    explicit operator bool() const {
        return ptr != nullptr;
    }

    bool operator==(const TIntrusivePtr& other) const {
        return ptr == other.ptr;
    }

    bool operator!=(const TIntrusivePtr& other) const {
        return ptr != other.ptr;
    }

private:
    T* ptr;
};

template <typename T, typename... Args>
TIntrusivePtr<T> MakeIntrusive(Args&&... args) {
    return TIntrusivePtr<T>(new T(std::forward<Args>(args)...));
}

class TDoc : public TRefCounter<TDoc> {
public:
    void Print() const {
        std::cout << "Document object\n";
    }
};

int main() {
    TIntrusivePtr<TDoc> ptr;
    ptr = MakeIntrusive<TDoc>();
    std::cout << "RefCount: " << ptr.UseCount() << "\n"; // RefCount == 1

    TIntrusivePtr<TDoc> ptr2 = ptr; // Copy
    std::cout << "RefCount: " << ptr.UseCount() << "\n"; // RefCount == 2

    TIntrusivePtr<TDoc> ptr3 = MakeIntrusive<TDoc>();
    ptr3.Reset(ptr2.Get());
    std::cout << "RefCount: " << ptr.UseCount() << "\n"; // RefCount == 3

    ptr3.Reset();
    std::cout << "RefCount: " << ptr.UseCount() << "\n"; // RefCount == 2

    ptr3.Reset(std::move(ptr2));
    std::cout << "RefCount: " << ptr.UseCount() << "\n"; // RefCount == 2

    static_assert(sizeof(TDoc*) == sizeof(ptr));
}

