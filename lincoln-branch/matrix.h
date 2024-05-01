/* 'matrix.h' -- Lincoln Scheer -- April 2024 */

#ifndef LS_MATLIB_H
#define LS_MATLIB_H

#include <vector>
#include <stdexcept>
#include <type_traits>
#include <random>
#include <iostream>
#include <iomanip>

typedef unsigned int uint;

/**
 * @class Mat
 * @breif A class implementing a matrix type with operation overloads.
 * @tparam T The arithmatic type of the matrix.
 * 
 * This class provides functionality for dealing with matrices in C++
 * and the ability to use matrix operations.
*/
template <typename T>
class Mat {
	/* reject non-arithmatic types */
	static_assert(std::is_arithmetic<T>::value, "Numeric types only"); 

protected:
	uint m_rows;
	uint m_cols;
	std::vector<T> m_data;

public:
	Mat() = default;	/* Default constructor. */
	virtual ~Mat() {} /* Virtual Destructor */


	/**
	 * @brief Creates new instance of Mat of desired size.
	 * @param _rows The number of rows in the new matrix.
	 * @param _cols The number of columns in the new matrix.
	*/
	Mat(uint _rows, uint _cols);

	/**
	 * @brief Returns the transpose of this matrix.
	 * @return The new matrix.
	*/
	Mat transpose() const;

	/**
	 * @brief Prints the entries of this matrix.
	*/
	void print();

	/**
	 * @brief Returns a reference to the entry at the i-th row and the j-th
	 * column.
	 * @param i The row of the entry (1 <= i <= rows())
	 * @param j The column of the entry (1 <= j <= cols())
	 * @return A reference to the i-th row, j-th column's entry.
	*/
	T& operator()(uint i, uint j);

	/**
	 * @brief Returns a const reference to the entry at the i-th row 
	 * and the j-th column.
	 * @param i The row of the entry (1 <= i <= rows())
	 * @param j The column of the entry (1 <= j <= cols())
	 * @return A const reference to the i-th row, j-th columns entry.
	*/
	const T& operator()(uint i, uint j) const;

	/**
	 * @brief Returns the number of rows of the matrix.
	 * @return The number of rows.
	*/
	uint rows();

	/**
	 * @brief Returns the number of columns of the matrix.
	 * @return The number of columns.
	*/
	uint cols();

	/**
	 * @brief Matrix addition overload.
	 * @param mat The other matrix.
	 * @return The resulting Mat from the matrix addition.
	*/
	Mat operator+(Mat const& mat);

	/**
	 * @brief Scalar multiplication overload.
	 * @param scalar The other scalar.
	 * @return The resulting Mat from the scalar addition.
	*/
	Mat operator*(T const& scalar);

	/**
	 * @brief Matrix multiplication overload.
	 * @param mat The other matrix.
	 * @return The resulting Mat from the matrix multiplication.
	*/
	Mat operator*(Mat const& mat);

	/**
	 * @brief Matrix subtraction overload.
	 * @param mat The other matrix.
	 * @return The resulting Mat from the matrix multiplication.
	*/
	Mat operator-(Mat const& mat);
};

template <typename T>
class Random : public Mat<T> {

public:

	/**
	 * @brief Creates new instance of Mat of desired size with random
	 * entries between -1, and 1.
	 * @param _rows The number of rows in the new matrix.
	 * @param _cols The number of columns in the new matrix.
	 * @return The new matrix.
	*/
	Random(uint _rows, uint _cols);

	/**
	 * @brief Creates new instance of Mat of desired size with random
	 * entries between minVal and maxVal.
	 * @param _rows The number of rows in the new matrix.
	 * @param _cols The number of columns in the new matrix.
	 * @param minVal The minimum bound of random entries.
	 * @param maxVal The maximum bound of random entries.
	 * @return The new matrix.
	*/
	Random(uint _rows, uint _cols, const T& minVal, const T& maxVal);

};

template <typename T>
class Ones : public Mat<T> {

public:

	/**
	 * @brief Creates new instance of Mat of desired size with 1's as
	 * entries.
	 * @param _rows The number of rows in the new matrix.
	 * @param _cols The number of columns in the new matrix.
	 * @return The new matrix.
	*/
	Ones(uint _rows, uint _cols);
};

template <typename T>
class Zeros : public Mat<T> {

public:

	/**
	 * @brief Creates new instance of Mat of desired size with 0's as
	 * entries.
	 * @param _rows The number of rows in the new matrix.
	 * @param _cols The number of columns in the new matrix.
	 * @return The new matrix.
	*/
	Zeros(uint _rows, uint _cols);

};

template <typename T>
class Identity : public Mat<T> {

public:

	/**
	 * @brief Creates new instance of Mat as a square identity matrix of
	 * desired size.
	 * @param size The number of rows & columns of the new matrix.
	 * @return The new matrix.
	*/
	Identity(uint size);

};


// ****** DEFINITIONS ******

template <typename T>
Mat<T>::Mat(uint _rows, uint _cols) : m_rows(_rows), m_cols(_cols), m_data(_rows* _cols) {}

template <typename T>
uint Mat<T>::rows() { return m_rows; }

template <typename T>
uint Mat<T>::cols() { return m_cols; }

template <typename T>
T& Mat<T>::operator()(uint i, uint j) {
	i--;
	j--;
	if (i < 0 || j < 0)
		throw std::out_of_range("Matrix index out of bounds");
	return m_data[i * m_cols + j];
}

template <typename T>
const T& Mat<T>::operator()(uint i, uint j) const {
	i--;
	j--;
	if (i < 0 || j < 0)
		throw std::out_of_range("Matrix index out of bounds");
	return m_data[i * m_cols + j];
}

template <typename T>
Mat<T> Mat<T>::operator+(Mat const& mat) {
	if (m_cols != mat.m_cols || m_rows != mat.m_rows)
		throw std::logic_error("Matrix dimensions incompatible");

	Mat res(m_rows, m_cols);
	for (uint i = 1; i < m_rows + 1; i++) {
		for (uint j = 1; j < m_cols + 1; j++) {
			res(i, j) = (*this)(i, j) + mat(i, j);
		}
	}
	return res;
}

template <typename T>
Mat<T> Mat<T>::operator*(T const& scalar) {
	Mat res(m_rows, m_cols);
	for (uint i = 1; i < m_rows; i++) {
		for (uint j = 1; j < m_cols; j++) {
			res(i, j) = (*this)(i, j) * scalar;
		}
	}
	return res;
}

template <typename T>
Mat<T> Mat<T>::operator*(Mat const& mat) {

	if (m_rows != mat.m_cols)
		throw std::logic_error("Matrix dimensions incompatible");

	Mat res(m_rows, mat.m_cols);
	for (uint i = 1; i < m_rows + 1; i++) {
		for (uint j = 1; j < mat.m_cols + 1; j++) {
			res(i, j) = 0;
			for (uint k = 1; k < m_cols + 1; k++) {
				res(i, j) += (*this)(i, k) * mat(k, j);
			}
		}
	}

	return res;
}

template <typename T>
Mat<T> Mat<T>::operator-(Mat const& mat) {
	if (m_cols != mat.m_cols || m_rows != mat.m_rows)
		throw std::logic_error("Matrix dimensions incompatible");

	Mat res = mat;
	res = res * -1;
	res = res + *this;
	return res;
}

template <typename T>
Mat<T> Mat<T>::transpose() const {
	Mat res(m_rows, m_cols);

	for (uint i = 1; i < m_rows + 1; i++) {
		for (uint j = 1; j < m_cols + 1; j++) {
			res(i, j) = (*this)(j, i);
		}
	}
	return res;
}

template <typename T>
void Mat<T>::print() {
	for (uint i = 1; i < m_rows + 1; i++) {
		std::cout << "[\t";
		for (uint j = 1; j < m_cols + 1; j++) {
			std::cout << std::fixed << std::setprecision(2) << (*this)(i, j) << "\t\t";
		}
		std::cout << "]\n";
	}
}

template <typename T>
Random<T>::Random(uint _rows, uint _cols) : Mat<T>(_rows, _cols) {
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_real_distribution<T> distribution(-1, 1); // 0 Mean, +/-1 Amplitude
	for (uint i = 1; i < _rows + 1; i++) {
		for (uint j = 1; j < _cols + 1; j++) {
			(*this)(i, j) = distribution(generator);
		}
	}
}

template <typename T>
Random<T>::Random(uint _rows, uint _cols, const T& minVal, const T& maxVal) : Mat<T>(_rows, _cols) {
	if (minVal >= maxVal)
		throw std::logic_error("Minimum value cannot be greater than maximum value");
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_real_distribution<T> distribution(minVal, maxVal); // 0 Mean, +/-1 Amplitude
	for (uint i = 1; i < _rows + 1; i++) {
		for (uint j = 1; j < _cols + 1; j++) {
			(*this)(i, j) = distribution(generator);
		}
	}
}

template <typename T>
Ones<T>::Ones(uint _rows, uint _cols) : Mat<T>(_rows, _cols) {
	for (uint i = 1; i < _rows + 1; i++) {
		for (uint j = 1; j < _cols + 1; j++) {
			(*this)(i, j) = 1;
		}
	}
}

template <typename T>
Zeros<T>::Zeros(uint _rows, uint _cols) : Mat<T>(_rows, _cols) {
	for (uint i = 1; i < _rows + 1; i++) {
		for (uint j = 1; j < _cols + 1; j++) {
			(*this)(i, j) = 0;
		}
	}
}

template <typename T>
Identity<T>::Identity(uint size) : Mat<T>(size, size) {
	for (uint i = 1; i < size + 1; i++) {
		(*this)(i, i) = 1;
	}
}

#endif /* LS_MATLIB_H */