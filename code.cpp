// nxxhoang - the dreamer
#include<bits/stdc++.h>
using namespace std;

struct Student {
    string name;
    double score;
    Student(string _name, double _score) : name(_name), score(_score) {}

    bool operator<(const Student& other) const { return score < other.score; }
    bool operator>(const Student& other) const { return score > other.score; }
    bool operator==(const Student& other) const { return score == other.score && name == other.name; }
    bool operator<=(const Student& other) const { return *this < other || *this == other; }
    bool operator>=(const Student& other) const { return *this > other || *this == other; }
    Student operator+(const Student& other) const { return Student(name, score + other.score); }
};
static double cur = 0;

Student getMax(const Student& a, const Student& b) {cur = 0; return (a.score > b.score) ? a : b; }

Student getMin(const Student& a, const Student& b) {cur = 1000; return (a.score < b.score) ? a : b; }

Student getSum(const Student& a, const Student& b) { return a + b; }

class SegmentTree {
private:
    vector<Student> tree;
    vector<Student> lazy;
    function<Student(const Student&, const Student&)> Function;

    void build(int idx, int l, int r) {
        if (l == r) {
            string name;
            double score;
            cin >> name >> score;
            tree[idx] = Student(name, score);
            return;
        }

        int mid = (l + r) / 2;
        build(2 * idx + 1, l, mid);
        build(2 * idx + 2, mid + 1, r);

        tree[idx] = Function(tree[2 * idx + 1], tree[2 * idx + 2]);
    }

public:
    SegmentTree(int n, function<Student(const Student&, const Student&)> func) : Function(func) {
        tree.resize(4 * n, Student("", cur));
        lazy.resize(4 * n, Student("", cur));
        build(0, 0, n - 1);
    }

    void pushLazy(int idx, int l, int r) {
        if (lazy[idx].score != 0) {
            tree[idx].score = lazy[idx].score;
            if (l < r) {
                lazy[2 * idx + 1].score = lazy[idx].score;
                lazy[2 * idx + 2].score = lazy[idx].score;
            }
            lazy[idx].score = 0;
        }
    }

    void update(int idx, int l, int r, int x, int y, double val) {
        pushLazy(idx, l, r);
        if (y < l || r < x) return;

        if (l >= x && r <= y) {
            lazy[idx].score = val;
            pushLazy(idx, l, r);
            return;
        }

        int mid = (l + r) / 2;
        update(2 * idx + 1, l, mid, x, y, val);
        update(2 * idx + 2, mid + 1, r, x, y, val);
        tree[idx] = Function(tree[2 * idx + 1], tree[2 * idx + 2]);
    }

    Student query(int idx, int l, int r, int x, int y) {
        pushLazy(idx, l, r);
        if (r < x || y < l) return Student("", cur);

        if (l >= x && r <= y) return tree[idx];

        int mid = (l + r) / 2;
        return Function(query(2 * idx + 1, l, mid, x, y), query(2 * idx + 2, mid + 1, r, x, y));
    }
};


int main() {
    int n, q;
    cin >> n >> q;

    unordered_map<string, function<Student(const Student&, const Student&)>> operations = {
        {"max", getMax},
        {"min", getMin},
        {"sum", getSum}
    };

    string operationType;
    cin >> operationType;

    SegmentTree st(n, operations[operationType]);

    while(q--) {
        int typE, left, right;
        cin >> typE >> left >> right;
        if (typE == 1) {
            double x;
            cin >> x;
            st.update(0, 0, n - 1, left - 1, right - 1, x);
        } else {
            Student stu = st.query(0, 0, n - 1, left - 1, right - 1);
            cout << std::left << setw(10) << stu.name << std::right << setw(5) << stu.score << endl;
        }
    }
    return 0;
}
