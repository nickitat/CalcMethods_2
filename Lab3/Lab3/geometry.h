#pragma once

#include <vector>
#include <utility>
using std::hash;
using std::vector;

class geometry {

public:

	struct point {

		int xi, yj;

		point() {}
		point(int xi, int yj) : xi(xi), yj(yj) {}

		friend point operator+(const point &p1, const point &p2) { return point(p1.xi + p2.xi, p1.yj + p2.yj); }
		friend point operator-(const point &p1, const point &p2) { return point(p1.xi - p2.xi, p1.yj - p2.yj); }
		friend bool operator==(const point &p1, const point &p2) { return p1.xi == p2.xi && p1.yj == p2.yj; }

		int length_sq() const { return xi*xi + yj*yj; }

	};

	geometry(vector<point> bound) : bound(bound) {}

	bool belong_hull(const point &p) {

		for (int i = 0; i < bound.size() - 1; ++i) {
			int curent_check = classify(bound[i], bound[i + 1], p);
			if (!(curent_check == RIGHT || curent_check == BELONG))
				return false;
		}
		return true;

	}

	bool belong_boundary(const point &p) {

		for (int i = 0; i < bound.size() - 1; ++i) {
			int curent_check = classify(bound[i], bound[i + 1], p);
			if (curent_check == BELONG)
				return true;
		}
		return false;

	}

private:

	vector<point> bound;

	enum { RIGHT, LEFT, BELONG, BEHIND, BEYOND };

	int classify(const point &p1, const point &p2, const point &p3) {

		point a = p2 - p1;
		point b = p3 - p1;
		int cross = a.xi * b.yj - b.xi * a.yj;
		if (cross > 0) return LEFT;
		if (cross < 0) return RIGHT;
		if ((a.xi * b.xi < 0) || (a.yj * b.yj < 0)) return BEHIND;
		if (a.length_sq() < b.length_sq()) return BEYOND;
		return BELONG;

	}

};

template<>
struct hash<geometry::point>
{
	size_t operator()(const geometry::point &pnt) const
	{
		return (pnt.xi * 31 + pnt.yj) % ((int)1e9 + 7);
	}
};