#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <iostream>
#include <stdexcept>

template<typename T>
class MyVector {
private:
    T* data;           // Con trỏ tới mảng dữ liệu
    size_t capacity_;  // Dung lượng hiện tại
    size_t size_;      // Số phần tử hiện tại

    // Hàm helper để resize mảng
    void resize_array(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        
        // Copy dữ liệu cũ sang mảng mới
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = data[i];
        }
        
        // Giải phóng bộ nhớ cũ
        delete[] data;
        
        // Cập nhật con trỏ và capacity
        data = new_data;
        capacity_ = new_capacity;
    }

public:
    // Constructor mặc định
    MyVector() : data(nullptr), capacity_(0), size_(0) {}
    
    // Constructor với kích thước ban đầu
    MyVector(size_t initial_size) : capacity_(initial_size), size_(initial_size) {
        data = new T[capacity_];
        // Khởi tạo các phần tử với giá trị mặc định
        for (size_t i = 0; i < size_; ++i) {
            data[i] = T();
        }
    }
    
    // Constructor với kích thước và giá trị khởi tạo
    MyVector(size_t initial_size, const T& value) : capacity_(initial_size), size_(initial_size) {
        data = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data[i] = value;
        }
    }
    
    // Copy constructor
    MyVector(const MyVector& other) : capacity_(other.capacity_), size_(other.size_) {
        data = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data[i] = other.data[i];
        }
    }
    
    // Assignment operator
    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            delete[] data;
            
            capacity_ = other.capacity_;
            size_ = other.size_;
            data = new T[capacity_];
            
            for (size_t i = 0; i < size_; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    // Destructor
    ~MyVector() {
        delete[] data;
    }
    
    // Thêm phần tử vào cuối vector
    void push_back(const T& value) {
        if (size_ == capacity_) {
            // Tăng capacity lên gấp đôi hoặc bằng 1 nếu capacity = 0
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            resize_array(new_capacity);
        }
        data[size_++] = value;
    }
    
    // Xóa phần tử cuối
    void pop_back() {
        if (size_ > 0) {
            --size_;
        }
    }
    
    // Truy cập phần tử theo index (có kiểm tra bounds)
    T& at(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    
    const T& at(size_t index) const {
        if (index >= size_) {
                    throw std::out_of_range("Index out of range");
                }
                return data[index];
            }
            
            // Operator [] (không kiểm tra bounds)
        T& operator[](size_t index) {
            return data[index];
    }
    
    const T& operator[](size_t index) const {
        return data[index];
    }
    
    // Lấy phần tử đầu tiên
    T& front() {
        if (size_ == 0) {
            throw std::runtime_error("Vector is empty");
        }
        return data[0];
    }
    
    const T& front() const {
        if (size_ == 0) {
            throw std::runtime_error("Vector is empty");
        }
        return data[0];
    }
    
    // Lấy phần tử cuối cùng
    T& back() {
        if (size_ == 0) {
            throw std::runtime_error("Vector is empty");
        }
        return data[size_ - 1];
    }
    
    const T& back() const {
        if (size_ == 0) {
            throw std::runtime_error("Vector is empty");
        }
        return data[size_ - 1];
    }
    
    // Kiểm tra vector có rỗng không
    bool empty() const {
        return size_ == 0;
    }
    
    // Lấy kích thước hiện tại
    size_t size() const {
        return size_;
    }
    
    // Lấy dung lượng hiện tại
    size_t capacity() const {
        return capacity_;
    }
    
    // Xóa tất cả phần tử
    void clear() {
        size_ = 0;
    }
    
    // Đặt lại kích thước vector
    void resize(size_t new_size) {
        if (new_size > capacity_) {
            resize_array(new_size);
        }
        
        // Nếu new_size lớn hơn size hiện tại, khởi tạo các phần tử mới
        for (size_t i = size_; i < new_size; ++i) {
            data[i] = T();
        }
        
        size_ = new_size;
    }
    
    // Đặt lại kích thước với giá trị khởi tạo
    void resize(size_t new_size, const T& value) {
        if (new_size > capacity_) {
            resize_array(new_size);
        }
        
        // Khởi tạo các phần tử mới với giá trị được chỉ định
        for (size_t i = size_; i < new_size; ++i) {
            data[i] = value;
        }
        
        size_ = new_size;
    }
    
    // Dự trữ dung lượng
    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            resize_array(new_capacity);
        }
    }
    
    // Chèn phần tử tại vị trí index
    void insert(size_t index, const T& value) {
        if (index > size_) {
            throw std::out_of_range("Index out of range");
        }
        
        if (size_ == capacity_) {
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            resize_array(new_capacity);
        }
        
        // Dịch chuyển các phần tử sang phải
        for (size_t i = size_; i > index; --i) {
            data[i] = data[i - 1];
        }
        
        data[index] = value;
        ++size_;
    }
// Xóa phần tử tại vị trí index
    void erase(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        
        // Dịch chuyển các phần tử sang trái
        for (size_t i = index; i < size_ - 1; ++i) {
            data[i] = data[i + 1];
        }
        
        --size_;
    }
    
    // Iterator đơn giản
    class iterator {
    private:
        T* ptr;
    public:
        iterator(T* p) : ptr(p) {}
        
        T& operator*() { return *ptr; }
        T* operator->() { return ptr; }
        
        iterator& operator++() { ++ptr; return *this; }
        iterator operator++(int) { iterator temp = *this; ++ptr; return temp; }
        
        iterator& operator--() { --ptr; return *this; }
        iterator operator--(int) { iterator temp = *this; --ptr; return temp; }
        
        bool operator==(const iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }
    };
    
    iterator begin() { return iterator(data); }
    iterator end() { return iterator(data + size_); }
    
    // In vector
    void print() const {
        std::cout << "[";
        for (size_t i = 0; i < size_; ++i) {
            std::cout << data[i];
            if (i < size_ - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
};

#endif // MY_VECTOR_H