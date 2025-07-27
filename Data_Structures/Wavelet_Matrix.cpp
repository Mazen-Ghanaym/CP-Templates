// @Mazen-Ghanaym
#include <bits/stdc++.h>
#include <immintrin.h>
using namespace std;
/*
    Wavelet Matrix
    use for 2d points queries 
    add point: O(log^2 n) -> x y w . add w to the point (x, y)
    ! sum in rectangle: O(log^2 n) -> x1 y1 x2 y2 -> sum in range [x1, x2) * [y1, y2)
    Memory: O(n log n)
    Note: This is an offline data structure, all possible point locations must be known before queries begin.
    This implementation uses a Fenwick Tree (Binary Indexed Tree) for fast updates and queries.
*/
/**
 * @brief A memory-efficient bit array that supports fast rank queries.
 * Rank queries count how many 0s or 1s appear before a given position.
 * rank1(i): How many 1s are there in the sequence up to (but not including) position i
 * rank0(i): How many 0s are there in the sequence up to (but not including) position i
 */
struct bit_vector {
    using u32 = uint32_t;
    using u64 = uint64_t;

    static constexpr u32 w = 64; // Word size (64 bits)
    vector<u64> block;
    vector<u32> count;
    u32 n, zeros;

    bit_vector() = default;
    bit_vector(int _n) { init(_n); }

    void init(int _n) {
        n = zeros = _n;
        block.assign(n / w + 1, 0);
        count.assign(block.size(), 0);
    }

    inline void set(u32 i) { block[i / w] |= (1ULL << (i % w)); }

    // Enables POPCNT instruction for fast bit counting
    __attribute__((target("popcnt"), optimize("O3,unroll-loops")))
    void build() {
        for (u32 i = 1; i < block.size(); ++i) {
            count[i] = count[i - 1] + _mm_popcnt_u64(block[i - 1]);
        }
        zeros = rank0(n);
    }

    inline u32 rank0(u32 i) const { return i - rank1(i); }

    // Enables BMI2 and POPCNT instructions for fast rank calculation
    __attribute__((target("bmi2,popcnt"), optimize("O3,unroll-loops")))
    inline u32 rank1(u32 i) const {
        return count[i / w] + _mm_popcnt_u64(_bzhi_u64(block[i / w], i % w));
    }
};


/**
 * @brief A Wavelet Matrix that supports adding values to points and querying rectangle sums.
 * This is an offline data structure: all possible point locations must be known before queries begin.
 *
 * @tparam S The type for coordinates (e.g., int).
 * @tparam T The type for weights/values (e.g., long long).
 */
template <typename S, typename T>
struct WaveletMatrix {
    using u32 = uint32_t;

    // A standard 1D Fenwick Tree (or Binary Indexed Tree)
    struct BIT {
        u32 N;
        vector<T> data;

        BIT() = default;
        BIT(int size) { init(size); }

        void init(int size) {
            N = size;
            data.assign(N + 1, 0);
        }

        void add(u32 k, T x) {
            for (++k; k <= N; k += k & -k) data[k] += x;
        }

        T prefix_sum(u32 k) const {
            T ret = T();
            for (; k > 0; k -= k & -k) ret += data[k];
            return ret;
        }

        // Sum over the half-open range [l, r)
        T range_sum(u32 l, u32 r) const {
            if (l >= r) return 0;
            return prefix_sum(r) - prefix_sum(l);
        }
    };

    using P = pair<S, S>;
    int n = 0, lg = 0;
    vector<bit_vector> bv;
    vector<BIT> bit;
    vector<P> ps; // All unique points, sorted by x then y
    vector<S> ys; // All unique y-coordinates, sorted

    WaveletMatrix() = default;

    // Collects a potential point location. Must be called before build().
    void add_point_location(S x, S y) {
        ps.emplace_back(x, y);
        ys.emplace_back(y);
    }

    // Builds the data structure from all collected point locations.
    void build() {
        sort(ps.begin(), ps.end());
        ps.erase(unique(ps.begin(), ps.end()), ps.end());
        n = ps.size();

        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());

        if (ys.empty()) return;

        vector<u32> cur(n), nxt(n);
        for (int i = 0; i < n; ++i) cur[i] = get_y_id(ps[i].second);

        lg = (ys.size() > 1) ? (log2(ys.size() - 1) + 1) : 1;

        bv.assign(lg, n);
        bit.assign(lg, n);

        for (int h = lg - 1; h >= 0; --h) {
            for (int i = 0; i < n; ++i) {
                if ((cur[i] >> h) & 1) bv[h].set(i);
            }
            bv[h].build();

            // Stably partition `cur` into `nxt` based on the h-th bit
            auto it0 = nxt.begin();
            auto it1 = nxt.begin() + bv[h].zeros;
            for (int i = 0; i < n; ++i) {
                if ((cur[i] >> h) & 1) *it1++ = cur[i];
                else *it0++ = cur[i];
            }
            swap(cur, nxt);
        }
    }

    // Returns the compressed ID for an x-coordinate.
    int get_x_id(S x) const {
        auto it = lower_bound(ps.begin(), ps.end(), make_pair(x, numeric_limits<S>::min()));
        return distance(ps.begin(), it);
    }

    // Returns the compressed ID for a y-coordinate.
    int get_y_id(S y) const {
        auto it = lower_bound(ys.begin(), ys.end(), y);
        return distance(ys.begin(), it);
    }

    // Adds a value to a point.
    void add(S x, S y, T val) {
        auto it = lower_bound(ps.begin(), ps.end(), make_pair(x, y));
        if (it == ps.end() || *it != make_pair(x, y)) return;
        int i = distance(ps.begin(), it);

        u32 y_val = get_y_id(y);
        for (int h = lg - 1; h >= 0; --h) {
            bool go_right = (y_val >> h) & 1;
            
            if (go_right) {
                i = bv[h].rank1(i) + bv[h].zeros;
            } else {
                i = bv[h].rank0(i);
            }
            bit[h].add(i, val);
        }
    }
    
    // Internal helper to sum points with x-id in [l, r) and y-id < upper_y_id.
    T sum_internal(int l, int r, u32 upper_y_id) const {
        if (l >= r) return 0;
        T res = 0;
        for (int h = lg - 1; h >= 0; --h) {
            bool go_right = (upper_y_id >> h) & 1;

            int l0 = bv[h].rank0(l), r0 = bv[h].rank0(r);
            if (go_right) {
                res += bit[h].range_sum(l0, r0);
                l = bv[h].rank1(l) + bv[h].zeros;
                r = bv[h].rank1(r) + bv[h].zeros;
            } else {
                l = l0;
                r = r0;
            }
        }
        return res;
    }

    // Queries the sum of weights in the rectangle [L, R) x [D, U).
    T sum(S L, S D, S R, S U) const {
        int l_id = get_x_id(L), r_id = get_x_id(R);
        u32 d_id = get_y_id(D), u_id = get_y_id(U);
        return sum_internal(l_id, r_id, u_id) - sum_internal(l_id, r_id, d_id);
    }
};
struct point {
    int x, y;
    long long weight;
};
struct query{
    int type; // 0 for add, 1 for sum
    int x1, y1, x2, y2; // Coordinates for add or sum
    long long weight; // Weight for add queries
};
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    WaveletMatrix<int, long long> wm;

    int N, Q;
    cin >> N >> Q;

    vector<point> initial_points(N);
    vector<query> queries(Q);

    // 1. Read all initial points and queries to collect all possible coordinates.
    for (int i = 0; i < N; ++i) {
        cin >> initial_points[i].x >> initial_points[i].y >> initial_points[i].weight;
        wm.add_point_location(initial_points[i].x, initial_points[i].y);
    }
    for (int i = 0; i < Q; ++i) {
        cin >> queries[i].type;
        if (queries[i].type == 0) { // Add query
            cin >> queries[i].x1 >> queries[i].y1 >> queries[i].weight;
            wm.add_point_location(queries[i].x1, queries[i].y1);
        } else { // Sum query
            cin >> queries[i].x1 >> queries[i].y1 >> queries[i].x2 >> queries[i].y2;
        }
    }

    // 2. Build the data structure.
    wm.build();

    // 3. Add the initial weights.
    for (const auto& p : initial_points) {
        wm.add(p.x, p.y, p.weight);
    }

    // 4. Process queries chronologically.
    for (const auto& q : queries) {
        if (q.type == 0) {
            wm.add(q.x1, q.y1, q.weight);
        } else {
            cout << wm.sum(q.x1, q.y1, q.x2, q.y2) << "\n";
        }
    }

    return 0;
}