#include "calculation.h"
#include "geometry.h"
#include "sparse_matrix.h"

#include <cassert>
#include <functional>
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

typedef function<double(double, double)> func;

class solution {

	typedef geometry::point point;

public:

	int m;
	double a, b;
	func f, phi;
	vector<point> nodes;
	unordered_map<point, int> mapping;
	sparse_matrix matrix;
	vectord F;

	solution(int m, double a, double b, func f, func phi) : m(m), a(a), b(b), f(f), phi(phi) {

		build_nodes();
		build_system();

	}

	bool belong_boundary(const point &p) {

		//geometry geom(vector<point> { { 0, m / 2 }, { 0, m }, { m / 2, m }, { m, m / 2 }, { m, 0 }, { m / 2, 0 }, { 0, m / 2 } });
		geometry geom(vector<point> { {0, 0}, { 0, m },{ m, m },  { m ,0 }, { 0, 0 } });
		return geom.belong_boundary(p);

	}

	bool belong_domain(const point &p) { // in closure

		//geometry geom(vector<point>{ { 0, m / 2 }, { 0, m }, { m / 2, m }, { m, m / 2 }, { m, 0 }, { m / 2, 0 }, { 0, m / 2 } });
		geometry geom(vector<point> { {0, 0}, { 0, m }, { m, m }, { m ,0 }, { 0, 0 } });
		return geom.belong_hull(p);

	}

	void build_nodes() {

		for (int yj = 0; yj <= m; ++yj) {
			for (int xi = 0; xi <= m; ++xi) {

				point curent(xi, yj);
				if (belong_domain(curent)) {
					mapping[curent] = nodes.size();
					nodes.push_back(curent);
				}

			}
		}

	}

	void build_system() {

		matrix.resize(nodes.size());
		F.resize(nodes.size());

		double h = 1.0 / m;
		double R = 2.0 * (a + b) / (h*h);
		double S = -a / (h*h), T = -b / (h*h);

		for (int i = 0; i < nodes.size(); ++i) {
			point curent(nodes[i].xi, nodes[i].yj);
			if (belong_domain(curent)) {
				if (belong_boundary(curent)) {
					matrix.add_elem(i, i, 1);
					F[i] = phi(curent.xi / m, curent.yj / m);
				}
				else {
					matrix.add_elem(i, i, R);
					F[i] = f(curent.xi / m, curent.yj / m);
					vector<point> delta = { { 1, 0 },{ -1, 0 },{ 0, 1 },{ 0, -1 } };
					for (int j = 0; j < delta.size(); ++j) {
						point neighbour = curent + delta[j];
						if (belong_domain(neighbour)) {
							assert(mapping.count(neighbour) > 0);
							int neighbour_num = mapping[neighbour];
							matrix.add_elem(i, neighbour_num, (curent.xi == neighbour.xi ? T : S));
						}
					}
				}
			}
		}

		//matrix.print();

	}

	const sparse_matrix& get_matrix() const {

		return matrix;

	}

};

int main() {

	//solution sol(4, 0.75, 1.25, [](double x, double y) { return -4 * (x*x + 3 * y*y); }, [](double x, double y) { return (x*x - y*y)*(x*x - y*y); });
	solution sol(10, 1, 1, [](double, double) { return 0; }, [](double, double) { return 0; });
	for (int i = 0; i <= 4; ++i) {
		for (int j = 0; j <= 4; ++j) {
			cout << sol.belong_domain(geometry::point(j, i)) << " ";
		}
		cout << endl;
	}

	calculation calc;
	sol.matrix.decrease(780);
	cout << calc.calc(sol.get_matrix()) << endl;

	system("pause");
	return 0;
}