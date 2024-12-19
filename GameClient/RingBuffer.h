#pragma once
//
//template <typename T, size_t size>
//class RingBufferIterator {
//public:
//#ifdef _HAS_CXX20
//    using iterator_concept = std::random_access_iterator;
//#endif
//
//    using value_type = T;
//    using difference_type = std::ptrdiff_t;
//    using pointer = T*;
//    using reference = T&;
//    using iterator_category = std::random_access_iterator_tag;
//    using index_type = size_t;
//
//public:
//    RingBufferIterator() noexcept { };
//
//    RingBufferIterator(pointer ptr, size_t offset = 0) noexcept {
//
//    }
//
//    RingBufferIterator(const RingBufferIterator& other) noexcept {
//
//    }
//
//    RingBufferIterator(RingBufferIterator&& other) noexcept {
//
//    }
//
//    RingBufferIterator& operator=(const RingBufferIterator& other) noexcept {
//        return *this;
//    }
//
//    RingBufferIterator& operator=(RingBufferIterator&& other) noexcept {
//        return *this;
//    }
//
//    ~RingBufferIterator() noexcept { }
//
//public:
//    RingBufferIterator operator++(int) {
//        RingBufferIterator ret{ mPtr };
//        ++mPtr;
//        return ret;
//    }
//
//    void operator++() {
//        ++mPtr;
//    }
//
//    RingBufferIterator operator--(int) {
//        RingBufferIterator ret{ mPtr };
//        --mPtr;
//        return ret;
//    }
//
//    void operator--() {
//        --mPtr;
//    }
//
//    bool operator==(const RingBufferIterator& other) {
//        return mPtr == other.mPtr;
//    }
//
//private:
//    T* mPtr{ nullptr };
//    
//};
//
//// FIFO
//// Two pointer
//template <typename T, size_t size>
//class RingBuffer {
//public:
//    using value_type = T;
//    using pointer = T*;
//    using reference = T&;
//
//    using iterator = RingBufferIterator<T, size>;
//    using reverse_iterator = std::reverse_iterator<iterator>;
//
//    using index_t = size_t;
//
//public:
//
//
//private:
//    std::array<T, size> mBuffer{ };
//    index_t mRead{ };
//    index_t mWrite{ };
//};