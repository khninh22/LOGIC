#ifndef THUATTOAN_H
#define THUATTOAN_H

#include <iostream>
#include <string>
using namespace std;

// ============================
//  MẢNG ĐỘNG TỰ CÀI ĐẶT
// ============================

template <typename T>
class MangDong {
private:
    T* data;           // con trỏ tới mảng dữ liệu
    int kichThuoc;     // số phần tử hiện có
    int sucChua;       // dung lượng tối đa hiện tại

    void moRong() {
        int moi = (sucChua == 0) ? 2 : sucChua * 2;
        T* temp = new T[moi];
        for (int i = 0; i < kichThuoc; i++)
            temp[i] = data[i];
        delete[] data;
        data = temp;
        sucChua = moi;
    }

public:
    // 1) Ctor mặc định + đủ tham số
    MangDong(int sucChuaBanDau = 5)
        : data(new T[sucChuaBanDau > 0 ? sucChuaBanDau : 5]),
        kichThuoc(0),
        sucChua(sucChuaBanDau > 0 ? sucChuaBanDau : 5) {}

    // 2) Copy ctor (deep copy)
    MangDong(const MangDong& o)
        : data(new T[o.sucChua]),
        kichThuoc(o.kichThuoc),
        sucChua(o.sucChua) {
        for (int i = 0; i < kichThuoc; ++i) data[i] = o.data[i];
    }

    // 3) Copy assignment (deep copy + self-assign guard)
    MangDong& operator=(const MangDong& o) {
        if (this == &o) return *this;
        T* newData = new T[o.sucChua];
        for (int i = 0; i < o.kichThuoc; ++i) newData[i] = o.data[i];
        delete[] data;
        data = newData;
        kichThuoc = o.kichThuoc;
        sucChua   = o.sucChua;
        return *this;
    }

    // 4) Destructor
    ~MangDong() { delete[] data; }

    void them(const T& phanTu) {
        if (kichThuoc >= sucChua)
            moRong();
        data[kichThuoc++] = phanTu;
    }

    int getKichThuoc() const {
        return kichThuoc;
    }

    T& operator[](int i) {
        return data[i];
    }

    const T& operator[](int i) const {
        return data[i];
    }

    bool xoaTheoViTri(int index) {
        if (index < 0 || index >= kichThuoc)
            return false;
        for (int i = index; i < kichThuoc - 1; i++)
            data[i] = data[i + 1];
        kichThuoc--;
        return true;
    }
};

// ============================
//  HÀM TÌM KIẾM / KIỂM TRA TRÙNG
// ============================

// tìm phần tử theo mã (dùng cho class có getMaKH() hoặc getMaSan())
template <typename T>
int timTheoMa(const MangDong<T>& ds, const string& ma) {
    for (int i = 0; i < ds.getKichThuoc(); i++) {
        if ((ds[i].getMaKH() == ma) || (ds[i].getMaSan() == ma))
            return i;
    }
    return -1;
}

// kiểm tra mã trùng
template <typename T>
bool maDaTonTai(const MangDong<T>& ds, const string& ma) {
    return timTheoMa(ds, ma) != -1;
}

// ============================
//  HÀM SẮP XẾP (BUBBLE SORT)
// ============================

template <typename T>
void sapXep(MangDong<T>& ds, bool (*soSanh)(const T&, const T&)) {
    int n = ds.getKichThuoc();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (soSanh(ds[j + 1], ds[j])) {
                T temp = ds[j];
                ds[j] = ds[j + 1];
                ds[j + 1] = temp;
            }
        }
    }
}

// ============================
//  QUEUE (HÀNG ĐỢI) - TỰ CÀI ĐẶT
// ============================

template <typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val) : data(val), next(nullptr) {}
    };
    
    Node* front;    // đầu hàng đợi
    Node* rear;     // cuối hàng đợi
    int count;      // số phần tử

public:
    Queue() : front(nullptr), rear(nullptr), count(0) {}
    
    ~Queue() {
        while (front) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
    }

    // Copy constructor
    Queue(const Queue& other) : front(nullptr), rear(nullptr), count(0) {
        Node* curr = other.front;
        while (curr) {
            enqueue(curr->data);
            curr = curr->next;
        }
    }

    // Copy assignment
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            while (front) {
                Node* temp = front;
                front = front->next;
                delete temp;
            }
            front = rear = nullptr;
            count = 0;
            
            Node* curr = other.front;
            while (curr) {
                enqueue(curr->data);
                curr = curr->next;
            }
        }
        return *this;
    }
    
    void enqueue(const T& val) {
        Node* newNode = new Node(val);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        count++;
    }
    
    bool dequeue(T& outVal) {
        if (isEmpty()) return false;
        Node* temp = front;
        outVal = front->data;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
        count--;
        return true;
    }
    
    bool peek(T& outVal) const {
        if (isEmpty()) return false;
        outVal = front->data;
        return true;
    }
    
    bool isEmpty() const { return front == nullptr; }
    int size() const { return count; }
};

// ============================
//  STACK (NGĂN XẾP) - TỰ CÀI ĐẶT
// ============================

template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val) : data(val), next(nullptr) {}
    };
    
    Node* top;
    int count;

public:
    Stack() : top(nullptr), count(0) {}
    
    ~Stack() {
        while (top) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
    }

    // Copy constructor
    Stack(const Stack& other) : top(nullptr), count(0) {
        if (other.top) {
            // Đảo ngược để giữ đúng thứ tự
            Stack temp;
            Node* curr = other.top;
            while (curr) {
                temp.push(curr->data);
                curr = curr->next;
            }
            while (!temp.isEmpty()) {
                T val;
                temp.pop(val);
                push(val);
            }
        }
    }

    // Copy assignment
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            while (top) {
                Node* temp = top;
                top = top->next;
                delete temp;
            }
            top = nullptr;
            count = 0;
            
            if (other.top) {
                Stack temp;
                Node* curr = other.top;
                while (curr) {
                    temp.push(curr->data);
                    curr = curr->next;
                }
                while (!temp.isEmpty()) {
                    T val;
                    temp.pop(val);
                    push(val);
                }
            }
        }
        return *this;
    }
    
    void push(const T& val) {
        Node* newNode = new Node(val);
        newNode->next = top;
        top = newNode;
        count++;
    }
    
    bool pop(T& outVal) {
        if (isEmpty()) return false;
        Node* temp = top;
        outVal = top->data;
        top = top->next;
        delete temp;
        count--;
        return true;
    }
    
    bool peek(T& outVal) const {
        if (isEmpty()) return false;
        outVal = top->data;
        return true;
    }
    
    bool isEmpty() const { return top == nullptr; }
    int size() const { return count; }
};

// ============================
//  HASHMAP (BẢN ĐỒ BĂNG) - TỰ CÀI ĐẶT
// ============================

template <typename K, typename V>
class HashMap {
private:
    struct Entry {
        K key;
        V value;
        Entry* next;
        Entry(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };
    
    static const int BUCKET_SIZE = 101;  // Số lượng bucket (số nguyên tố)
    Entry* buckets[BUCKET_SIZE];
    int count;
    
    // Hàm băm cho string
    int hash(const string& key) const {
        unsigned long h = 0;
        for (size_t i = 0; i < key.length(); i++) {
            h = h * 31 + key[i];
        }
        return h % BUCKET_SIZE;
    }
    
    // Hàm băm chung (dùng cho các kiểu khác)
    int hashKey(const K& key) const {
        return hash(to_string(key));
    }

public:
    HashMap() : count(0) {
        for (int i = 0; i < BUCKET_SIZE; i++) {
            buckets[i] = nullptr;
        }
    }
    
    ~HashMap() {
        clear();
    }

    // Copy constructor
    HashMap(const HashMap& other) : count(0) {
        for (int i = 0; i < BUCKET_SIZE; i++) {
            buckets[i] = nullptr;
        }
        for (int i = 0; i < BUCKET_SIZE; i++) {
            Entry* curr = other.buckets[i];
            while (curr) {
                put(curr->key, curr->value);
                curr = curr->next;
            }
        }
    }

    // Copy assignment
    HashMap& operator=(const HashMap& other) {
        if (this != &other) {
            clear();
            for (int i = 0; i < BUCKET_SIZE; i++) {
                Entry* curr = other.buckets[i];
                while (curr) {
                    put(curr->key, curr->value);
                    curr = curr->next;
                }
            }
        }
        return *this;
    }
    
    void put(const K& key, const V& value) {
        int idx = hashKey(key);
        Entry* curr = buckets[idx];
        
        // Nếu key đã tồn tại, cập nhật value
        while (curr) {
            if (curr->key == key) {
                curr->value = value;
                return;
            }
            curr = curr->next;
        }
        
        // Thêm entry mới
        Entry* newEntry = new Entry(key, value);
        newEntry->next = buckets[idx];
        buckets[idx] = newEntry;
        count++;
    }
    
    bool get(const K& key, V& outValue) const {
        int idx = hashKey(key);
        Entry* curr = buckets[idx];
        
        while (curr) {
            if (curr->key == key) {
                outValue = curr->value;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }
    
    bool contains(const K& key) const {
        int idx = hashKey(key);
        Entry* curr = buckets[idx];
        
        while (curr) {
            if (curr->key == key) return true;
            curr = curr->next;
        }
        return false;
    }
    
    bool remove(const K& key) {
        int idx = hashKey(key);
        Entry* curr = buckets[idx];
        Entry* prev = nullptr;
        
        while (curr) {
            if (curr->key == key) {
                if (prev) {
                    prev->next = curr->next;
                } else {
                    buckets[idx] = curr->next;
                }
                delete curr;
                count--;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }
    
    void clear() {
        for (int i = 0; i < BUCKET_SIZE; i++) {
            Entry* curr = buckets[i];
            while (curr) {
                Entry* temp = curr;
                curr = curr->next;
                delete temp;
            }
            buckets[i] = nullptr;
        }
        count = 0;
    }
    
    int size() const { return count; }
    bool isEmpty() const { return count == 0; }
    
    // Lấy tất cả keys
    MangDong<K> getAllKeys() const {
        MangDong<K> keys;
        for (int i = 0; i < BUCKET_SIZE; i++) {
            Entry* curr = buckets[i];
            while (curr) {
                keys.them(curr->key);
                curr = curr->next;
            }
        }
        return keys;
    }
};

// Template specialization cho hash(string)
template <>
inline int HashMap<string, int>::hashKey(const string& key) const {
    return hash(key);
}

template <>
inline int HashMap<string, double>::hashKey(const string& key) const {
    return hash(key);
}

template <>
inline int HashMap<string, string>::hashKey(const string& key) const {
    return hash(key);
}

#endif
