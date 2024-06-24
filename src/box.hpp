#ifndef LOX_BOX_HPP
#define LOX_BOX_HPP

#include <memory>

namespace lox{
    template<typename T>
    //implementation from https://www.foonathan.net/2022/05/recursive-variant-box/
    class Box{
        public:
        Box(T&& obj) : data(new T(std::move(obj))){}
        Box(const T &obj) : data(new T(obj)){}
        Box(const Box& other) : Box(*other.data) {}
        Box &operator=(const Box&other){
            *data = *other.data;
            return *this;
        }
        template<typename ... Args>
        Box(Args&&... args): data(new T(std::forward<Args>(args)...)){}

        ~Box() = default;
        T &operator*() { return *data; }
        const T &operator*() const { return *data; }

        T *operator->() { return data.get(); }
        const T *operator->() const { return data.get(); }

        Box(Box &&other) : Box(std::move(*other.data)) {}
        Box &operator=(Box &&other)
        {
            *data = std::move(*other.data);
            return *this;
        }

        private:
        std::unique_ptr<T> data;
    };
};


#endif