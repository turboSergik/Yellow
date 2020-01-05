#ifndef THREADSAFEBITSET_HPP
#define THREADSAFEBITSET_HPP
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <type_traits>
#include <algorithm>
#include <utility>
#include <array>

template <size_t N>
class ThreadSafeBitset {
    typedef std::integral_constant<size_t, N / 32 + (N % 32 > 0)> arraySize;
    std::array<std::pair<uint32_t, std::mutex>, arraySize::value> data;
    // std::mutex mutexes[arraySize::value];
    
    template <class Func>
    decltype(std::declval<Func>()(std::declval<size_t>(), 
                                  std::declval<size_t>()))
    bitOperation(size_t, Func);
    
    template <class Func>
    ThreadSafeBitset<N> & forEach(Func);
    
public:
    // zero initializetion
    ThreadSafeBitset();
    // fill with bool init
    ThreadSafeBitset(bool);

    ThreadSafeBitset<N> & set(size_t);
    ThreadSafeBitset<N> & set(bool, size_t);   
    ThreadSafeBitset<N> & set();
    ThreadSafeBitset<N> & reset(size_t);
    ThreadSafeBitset<N> & reset();
    ThreadSafeBitset<N> & flip(size_t);
    
    bool get(size_t);
    bool unsafeGet(size_t);
    void unsafeSetNumber(size_t, uint32_t);
    std::mutex & getMutex(size_t);
    
    size_t size();
    
    
};

template<size_t N>
ThreadSafeBitset<N>::ThreadSafeBitset(bool initBool) {
    uint32_t filledValue = initBool ? UINT32_MAX : 0;
    std::for_each(data.begin(), data.end(), 
                  [filledValue](std::pair<uint32_t, std::mutex> & pair) {
        pair.first = filledValue;
    });
}

template<size_t N>
ThreadSafeBitset<N>::ThreadSafeBitset() : ThreadSafeBitset(false) {
    
}

template <size_t N>
template<class Func>
decltype(std::declval<Func>()(std::declval<size_t>(), 
                              std::declval<size_t>()))
ThreadSafeBitset<N>::bitOperation(size_t index, Func f) {
    size_t actualIndex = index / 32;
    size_t numberOfBit = index % 32;
    data[actualIndex].second.lock();
    decltype(std::declval<Func>()(std::declval<size_t>(), 
                                  std::declval<size_t>()))
            result = f(actualIndex, numberOfBit);
    data[actualIndex].second.unlock();
    return result;
}


template<size_t N>
ThreadSafeBitset<N> & ThreadSafeBitset<N>::set(size_t index) {
    
    return bitOperation(index, [this](size_t actualIndex, size_t numberOfBit) -> ThreadSafeBitset<N> & {
        this->data[actualIndex].first |= (1 << numberOfBit);
        return *this;
    });    
}

template<size_t N>
ThreadSafeBitset<N> & ThreadSafeBitset<N>::reset(size_t index) {
    return bitOperation(index, [this](size_t actualIndex, size_t numberOfBit) -> ThreadSafeBitset<N> & {
        this->data[actualIndex].first &= ~(1 << numberOfBit);
        return *this;
    });    
}

template<size_t N>
bool ThreadSafeBitset<N>::get(size_t index) {
    return bitOperation(index, [this](size_t actualIndex, size_t numberOfBit) -> bool {
        bool result = (this->data[actualIndex].first >> numberOfBit) & 1;
        return result;
    });
}

template<size_t N>
bool ThreadSafeBitset<N>::unsafeGet(size_t index) {
    return (this->data[index / 32].first >> (index % 32)) & 1;
}

template<size_t N>
void ThreadSafeBitset<N>::unsafeSetNumber(size_t i, uint32_t val) {
    data[i].first = val;
}

template<size_t N>
std::mutex & ThreadSafeBitset<N>::getMutex(size_t index) {
    return data[index].second;
}

template<size_t N>
ThreadSafeBitset<N> & ThreadSafeBitset<N>::flip(size_t index) {
    return bitOperation(index, [this](size_t actualIndex, size_t numberOfBit) -> ThreadSafeBitset<N> & {
        bool value = (this->data[actualIndex].first >> numberOfBit) & 1;
        if (value) {
            this->data[actualIndex].first &= ~(1 << numberOfBit);
        } else {
            this->data[actualIndex].first |= (1 << numberOfBit);            
        }
        return *this;
    });
}

template<size_t N>
ThreadSafeBitset<N> & ThreadSafeBitset<N>::set(bool value, size_t index) {
    if (value) {
        return set(index);
    } else {
        return reset(index);
    }
}

template<size_t N>
ThreadSafeBitset<N> & ThreadSafeBitset<N>::set() {
    return this->forEach([](uint32_t & item) {
        item = UINT32_MAX;
    });
}

template<size_t N>
ThreadSafeBitset<N> & ThreadSafeBitset<N>::reset() {
    return this->forEach([](uint32_t & item) {
        item = 0;
    });
}

template<size_t N>
template<class Func>
ThreadSafeBitset<N> & ThreadSafeBitset<N>::forEach(Func f) {
    std::for_each(data.begin(), data.end(), 
                  [&f](std::pair<uint32_t, std::mutex> & pair) {
        pair.second.lock();
        f(pair.first);
        pair.second.unlock();
    });
    return *this;
}

#endif // THREADSAFEBITSET_HPP
