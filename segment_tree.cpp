#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

class segment_tree {
    long long *sum;
    long long *prefix_sum;
    int *add_value;
    const int size;
public:
    segment_tree(int max_size, const std::vector<int> &a, const std::vector<int> &b);
    ~segment_tree();

    inline void relax_sum(int vertex);
    inline void relax_push(int vertex, int left, int right, int change);
    inline long long sum_segment(int left, int right);
    inline void push(int vertex, int left, int right);
    inline void build(int vertex, int left, int right, const std::vector<int> &a);
    inline void update(int vertex, int left, int right, int query_left, int query_right);
    inline long long sum_range(int vertex, int left, int right, int query_left, int query_right);
};

inline void segment_tree::relax_sum(int vertex) {
    sum[vertex] = sum[2 * vertex] + sum[2 * vertex + 1];
}

inline void segment_tree::build(int vertex, int left, int right, const std::vector<int> &a) {
    add_value[vertex] = 0;
    if (left == right) {
        sum[vertex] = a[left];
        return;
    }
    build(2 * vertex, left, (left + right) / 2, a);
    build(2 * vertex + 1, (left + right) / 2 + 1, right, a);
    relax_sum(vertex);
}

segment_tree::segment_tree(int max_size, const std::vector<int> &a, const std::vector<int> &b) : size(8 * max_size) {
    prefix_sum = new long long[size];
    sum = new long long[size];
    add_value = new int[size];
    for (int i = 0; i < max_size; ++i) {
        prefix_sum[i] = (i > 0 ? prefix_sum[i - 1] : 0) + b[i];
    }
    build(1, 0, max_size - 1, a);
}

segment_tree::~segment_tree() {
    delete [] sum, prefix_sum, add_value;
}

inline long long segment_tree::sum_segment(int left, int right) {
    long long ans = prefix_sum[right] - (left > 0 ? prefix_sum[left - 1] : 0);
    return ans;
}

inline void segment_tree::relax_push(int vertex, int left, int right, int change) {
    add_value[vertex] += change;
    sum[vertex] += change * sum_segment(left, right);
}

inline void segment_tree::push(int vertex, int left, int right) {
    relax_push(2 * vertex, left, (left + right) / 2, add_value[vertex]);
    relax_push(2 * vertex + 1, (left + right) / 2 + 1, right, add_value[vertex]);
    add_value[vertex] = 0;
}


inline void segment_tree::update(int vertex, int left, int right, int query_left, int query_right) {
    if (query_left > query_right) {
        return;
    }
    push(vertex, left, right);
    if (left == query_left && right == query_right) {
        relax_push(vertex, left, right, 1);
        return; 
    }
    update(2 * vertex, left, (left + right) / 2, query_left, std::min(query_right, (left + right) / 2));
    update(2 * vertex + 1, (left + right) / 2 + 1, right, std::max(query_left, (left + right) / 2 + 1), query_right);
    relax_sum(vertex);
}

inline long long segment_tree::sum_range(int vertex, int left, int right, int query_left, int query_right) {
    if (query_left > query_right) {
        return 0;
    }
    push(vertex, left, right);
    if (left == query_left && right == query_right) {
        return sum[vertex];
    }
    return sum_range(2 * vertex, left, (left + right) / 2, query_left, std::min(query_right, (left + right) / 2)) +
        sum_range(2 * vertex + 1, (left + right) / 2 + 1, right, std::max(query_left, (left + right) / 2 + 1), query_right);
}

int main() {
    std::ios_base::sync_with_stdio(0);
    int n;
    std::cin >> n;
    std::vector<int> a(n), b(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }
    for (int i = 0; i < n; ++i) {
        std::cin >> b[i];
    }
    segment_tree tree(n, a, b);
    int m;
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        char type;
        int left, right;
        std::cin >> type >> left >> right;
        if (type == '+') {
            tree.update(1, 0, n - 1, left, right);
        } else {
            std::cout << tree.sum_range(1, 0, n - 1, left, right) << std::endl;
        }
    }    
    return 0;
}