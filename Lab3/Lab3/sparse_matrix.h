#pragma once



#include <iostream>
using std::cout;
using std::endl;


#include <cassert>
#include <vector>

using std::vector;

typedef vector<double> vectord;

class sparse_matrix {

	struct item {

		int column;
		double value;

		item() {}
		item(int column, double value) : column(column), value(value) {}

	};

	vector<vector<item>> data;

public:

	void add_elem(int row, int column, double value) {

		assert(row >= 0 && row < data.size());
		for (int i = 0; i < data[row].size(); ++i) {
			if (data[row][i].column == column) {
				data[row][i].value += value;
				return;
			}
		}
		data[row].push_back(item(column, value));

	}

	void decrease(double value) {

		for (int i = 0; i < data.size(); ++i) {
			for (int j = 0; j < data[i].size(); ++j) {
				if(i == data[i][j].column)
					data[i][j].value -= value;
			}
		}

	}

	vectord mult(vectord &vec) {

		vectord result(data.size(), 0);
		for (int i = 0; i < data.size(); ++i) {
			for (int j = 0; j < data[i].size(); ++j) {
				double value = data[i][j].value;
				int column = data[i][j].column;
				result[i] += value * vec[column];
			}
		}
		return result;

	}

	void print() {

		for (int i = 0; i < data.size(); ++i) {

			for (int j = 0; j < data[i].size(); ++j) {

				cout << "( " << data[i][j].column << " " << data[i][j].value << " ) ";

			}
			cout << endl;

		}

	}

	int size() const {

		return data.size();

	}

	void resize(int n) {

		data.resize(n);

	}

};