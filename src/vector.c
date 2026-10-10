/* vector.c —— 你要实现的地方 */

#include "vector.h"
/* 初始化：申请一块能装 capacity 个 int 的缓冲区，size() 为 0
 * 调用前 v 不得持有尚未释放的缓冲区；字段无需预先清零，初始化不读取旧值
 * capacity 为 0 时不分配内存，三个指针置为 NULL，返回 0
 * capacity 最大为 SIZE_MAX / sizeof(int)
 * 超过上限或内存分配失败时返回 -1，三个指针置为 NULL，否则返回 0
 * 时间复杂度：O(1)
 */
int vector_init(vector *v, size_t capacity) {

    v->data = NULL;
    v->cap = NULL;
    v->end = NULL;

    if(capacity > SIZE_MAX / sizeof(int)) {
        return -1;
    }
    if(capacity == 0) {
        return 0;
    }

    v->data = malloc(capacity * sizeof(int));
    if(v->data == NULL){
        return -1;
    }
    v -> cap = v->data + capacity;
    v -> end = v->data;
    return 0;
}

void vector_destroy(vector *v) {
    free(v->data);
    v->data = NULL;
    v->cap = NULL;
    v->end = NULL;
}

size_t size(const vector *v) {
    if(v->end == NULL) {
        return 0;
    }
    return (v->end - v->data);
}

size_t capacity(const vector *v) {
    if(v->cap == NULL) {
        return 0;
    }
    return (v->cap - v->data);
}

int empty(const vector *v) {
    if(v->data == NULL) {
        return 1;
    }
    return (v->end == v->data) ? 1 : 0;
}
/* 读取下标 index 处的元素，写入 *out
 * index >= size() 时返回 -1 且不写入 *out，成功返回 0
 * 时间复杂度：O(1)
 */
int get(const vector *v, size_t index, int *out) {
    if(index >= size(v)) {
        return -1;
    }
    *out = v->data[index];
    return 0;
}

int set(vector *v, size_t index, int value) {
    if(index >= size(v)) {
        return -1;
    }
    v->data[index] = value;
    return 0;
}

int front(const vector *v, int *out) {
    if(empty(v)) {
        return -1;
    }
    *out = v->data[0];
    return 0;
}

int back(const vector *v, int *out) {
    if(empty(v)) {
        return -1;
    }
    *out = v->data[size(v) - 1];
    return 0;
}
/* 在尾部追加一个元素，size() 加一，容量不够时自动扩容
 * 容量足够时容量不变；容量为 0 时增至 1，否则严格增至原容量的 2 倍
 * 两倍容量超过 SIZE_MAX / sizeof(int) 或分配失败时返回 -1，不修改 vector
 * 成功返回 0
 * 时间复杂度：不扩容时 O(1)，扩容时 O(size())
 */
int push_back(vector *v, int value) {
    if(size(v) == capacity(v)) {
        if(size(v) == 0){
            if(reserve(v, 1) == -1) {
                return -1;
            }
            v->end = v->end + 1;
            v->data[0] = value;
            
            return 0;
        }
        if(reserve(v, capacity(v) * 2) == -1) {
            return -1;
        }
        v->end = v->end + 1;
        v->data[size(v) - 1] = value;
        
        return 0;
    }

    v->end = v->end + 1;
    v->data[size(v) - 1] = value;
    

    return 0;
}

int pop_back(vector *v, int *out) {
    if(empty(v)) {
        return -1;
    }
    *out = v->data[size(v) - 1];
    v->end = v->end - 1;
    return 0;
}

int reserve(vector *v, size_t new_capacity) {
    if(new_capacity > SIZE_MAX / sizeof(int)) {
        return -1;
    }
    if(new_capacity <= capacity(v)) {
        return 0;
    }
    size_t current_size = size(v);
    int *p = realloc(v->data, new_capacity * sizeof(int));
    if(p == NULL) {
        return -1;
    }
    v->data = p;
    v->end = v->data + current_size;
    v->cap = v->data + new_capacity;
    return 0;
}

int shrink_to_fit(vector *v) {
    if(size(v) == 0) {
        free(v ->data);
        v->data = NULL;
        v->cap = NULL;
        v->end = NULL;
        return 0;
    }
    size_t current_size = size(v);
    int *p = realloc(v->data, current_size * sizeof(int));
    if(p == NULL) {
        return -1;
    }

    v->data = p;
    v->end = v->data + current_size;
    v->cap = v->end;
    return 0;
}

void clear(vector *v) {
    v->end = v->data;
}

