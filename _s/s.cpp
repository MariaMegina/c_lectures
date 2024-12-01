#include <utility>
#include <type_traits>
#include <iostream>

template <char... c>
struct TString {
    static constexpr char str[] = {c..., '\0'};

    static constexpr const char* Data() {
        return str;
    }
};


template <char... c>
constexpr char TString<c...>::str[];

template <typename T1, typename T2>
struct TStringConcat;

template <char... c1, char... c2>
struct TStringConcat<TString<c1...>, TString<c2...>> {
    using type = TString<c1..., c2...>;
};

template <char... c1, char... c2>
constexpr auto operator+(TString<c1...>, TString<c2...>) {
    return typename TStringConcat<TString<c1...>, TString<c2...>>::type{};
}

template <char... c1, char... c2>
constexpr bool operator==(TString<c1...>, TString<c2...>) {
    return std::is_same<TString<c1...>, TString<c2...>>::value;
}

template <typename CharT, CharT... c>
constexpr auto operator"" _s() {
    return TString<c...>{};
}

int main() {
    constexpr auto hello = "hello"_s + " world"_s;
    static_assert(hello == "hello world"_s);
}
