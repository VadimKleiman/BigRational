#ifndef SHARED_BUFFER_H
#define SHARED_BUFFER_H

#include <cstddef>
#include <cstdint>

struct shared_buffer {
    explicit shared_buffer(size_t data_size);

    shared_buffer(const shared_buffer &src);

    shared_buffer &operator=(shared_buffer src);

    shared_buffer(shared_buffer &&src);

    ~shared_buffer();

    int64_t *get_data();

    const int64_t *get_data() const;

    size_t get_size() const;

    void push();

    void pop();

private:
    struct shared;
    shared *shared_;
};

#endif //SHARED_BUFFER_H
