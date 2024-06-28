#ifndef LOX_BOX_HPP
#define LOX_BOX_HPP

#include <memory>

namespace lox{
    template<typename T>
    //implementation from https://www.foonathan.net/2022/05/recursive-variant-box/
    class Box{
        public:
        Box() = default;
        Box(T&& obj) : data(new T(std::move(obj))){}
        Box(const T &obj) : data(new T(obj)){}

        Box(const Box& other) : Box(*other.data) {}

        Box &operator=(const Box& other){
            *data = *other.data;
            return *this;
        }

        Box(Box&& other) = default;
        Box &operator=(Box &&other) = default;

        template<typename Arg, typename... Args>
        Box(Arg arg, Args&&... args){
            using k = typename std::remove_const<typename std::remove_reference<Arg>::type>;
            if constexpr (std::is_same_v<k, Box> and sizeof...(args) == 0u) {
                data = std::make_unique<T>(*arg);
            } else{
                data = std::make_unique<T>(arg, std::forward<Args>(args)...);
            }
        }

        T &operator*() { return *data; }
        const T &operator*() const { return *data; }

        T *operator->() { return data.get(); }
        const T *operator->() const { return data.get(); }
        ~Box() = default;
        private:
        std::unique_ptr<T> data;
    };
};


#endif