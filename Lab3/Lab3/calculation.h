#pragma once

#include <algorithm>
#include "sparse_matrix.h"

class calculation {

public:

	double calc(sparse_matrix matrix) {

		vectord y_prev(matrix.size());
		for (int i = 0; i < y_prev.size(); ++i) {
			y_prev[i] = 1;
		}
		normalize(y_prev);

		const double EPS = 1e-3;
		double lambda = 1e18;
		while (true) {

			vectord yk = matrix.mult(y_prev);
			double new_lambda = dot(yk, y_prev); 
			normalize(yk);

			//double new_lambda = dot(yk, y_prev);
			/*double new_lambda = yk[0] / y_prev[0];
			for (int i = 1; i < yk.size(); ++i) {
			new_lambda = std::max(new_lambda, yk[i] / y_prev[i]);
			}*/

			if (abs(new_lambda - lambda) < EPS) break;
			lambda = new_lambda;
			y_prev = yk;

		}

		return lambda;

	}

	double dot(vectord &v1, vectord &v2) {

		assert(v1.size() == v2.size());
		double result = 0;
		for (int i = 0; i < v1.size(); ++i) {
			result += v1[i] * v2[i];
		}
		return result;

	}

	void normalize(vectord &v) {

		double norm = 0;
		for (int i = 0; i < v.size(); ++i) {

			norm += v[i] * v[i];

		}
		norm = sqrt(norm);
		for (int i = 0; i < v.size(); ++i) {

			v[i] /= norm;

		}

	}

};