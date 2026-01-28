#pragma once

#include <iostream>
#include <type_traits>
#include <utility>
#include <iterator>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <stack>
#include <queue>

using namespace std;

template <template <class...> class Primary, class T>
struct is_specialization_of : false_type {};
template <template <class...> class Primary, class... Args>
struct is_specialization_of<Primary, Primary<Args...>> : true_type {};

template <typename T> struct is_string : false_type {};
template<> struct is_string<string> : true_type {};

// is_pair
template <typename T> struct is_pair : false_type {};
template <typename A, typename B> struct is_pair<pair<A,B>> : true_type {};

// is_container
template <typename T, typename = void>
struct is_container : false_type {};

template<typename T>
struct is_container<T, void_t<
    decltype(declval<T>().begin()),
    decltype(declval<T>().end())
>> : integral_constant<bool, !is_string<T>::value> {};

// is_adapter
template <typename T>
struct is_adapter : bool_constant<
    is_specialization_of<queue, T>::value ||
    is_specialization_of<stack, T>::value ||
    is_specialization_of<priority_queue, T>::value
> {};

// Generic
template <typename T>
enable_if_t<!is_container<T>::value && !is_pair<T>::value && !is_adapter<T>::value, void>
printVar(const T& val) {
    cout << val;
}

// Pair
template <typename A, typename B>
void printVar(const pair<A, B>& p) {
    cout << "(";
    printVar(p.first);
    cout << ", ";
    printVar(p.second);
    cout << ")";
}

// Containers (Vector, Set)
template <typename T>
enable_if_t<is_container<T>::value, void>
printVar(const T& container) {
    for (auto it = container.begin(); it != container.end(); ++it) {
        printVar(*it);
        if (next(it) != container.end()) cout << ", ";
    }
}

// Map
template <typename K, typename V>
void printVar (const map<K, V>& m) {
    cout << endl;
    for (auto it = m.begin(); it != m.end(); ++it) {
        printVar(it->first);
        cout << ": ";
        printVar(it->second);
        if (next(it) != m.end()) cout << endl;
    }
}

// Adapters (queue, stack, priority_queue)
template <typename Adapter>
enable_if_t<is_adapter<Adapter>::value, void>
printVar(Adapter a) {
    // Queue
    if constexpr (is_specialization_of<std::queue, Adapter>::value) {
        bool first = true;
        while (!a.empty()) {
            if (!first) std::cout << ", ";
            printVar(a.front());
            a.pop();
            first = false;
        }
    } 

    // Stack & Priority Queue
    else if constexpr (is_specialization_of<std::stack, Adapter>::value || is_specialization_of<std::priority_queue, Adapter>::value) {
        bool first = true;
        while (!a.empty()) {
            if (!first) std::cout << ", ";
            printVar(a.top());
            a.pop();
            first = false;
        }
    }
}

#define DEBUG(x) do { \
    cout << #x << " = "; \
    printVar(x); \
    cout << endl; \
} while(0)
