#pragma once

#include <vector>
#include <string>
#include <iostream>

template <class T> class Matrix
{
public:
	Matrix<T>() : rows(0), cols(0) {};
	Matrix(const Matrix<T>& other) : rows(other.rows), cols(other.cols), elements(other.elements) {};
	Matrix<T>(int rows, int cols) : rows(rows), cols(cols), elements(rows * cols, T(0)) {};
	Matrix<T>(int rows, int cols, const std::vector<T>& elements) : rows(rows), cols(cols), elements(elements) {};
	Matrix<T>(int rows, int cols, const T value) : rows(rows), cols(cols), elements(rows * cols, value) {};
	T operator () (int row, int col) const {
		return elements[row * cols + col];
	};

	Matrix<T> operator + (const Matrix<T>& other) {
		if (rows == other.rows && cols == other.cols)
		{
			Matrix<T> result(rows, cols);
			for (int i = 0; i < rows * cols; i++)
			{
				result.elements[i] = elements[i] + other.elements[i];
			}
			return result;
		}
		else
		{
			throw std::exception("Matrix dimensions must agree.");
		}
	};

	Matrix<T> operator - (const Matrix<T>& other) {
		if (rows == other.rows && cols == other.cols)
		{
			Matrix<T> result(rows, cols);
			for (int i = 0; i < rows * cols; i++)
			{
				result.elements[i] = elements[i] - other.elements[i];
			}
			return result;
		}
		else
		{
			throw std::exception("Matrix dimensions must agree.");
		}
	};

	// create == operator
	bool operator == (const Matrix<T>& other) {
		if (rows == other.rows && cols == other.cols)
		{
			for (int i = 0; i < rows * cols; i++)
			{
				if (elements[i] != other.elements[i])
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	};


	Matrix<T> operator * (const Matrix<T>& other) {
		if (cols == other.rows)
		{
			Matrix<T> result(rows, other.cols);
			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < other.cols; j++)
				{
					T sum = 0;
					for (int k = 0; k < cols; k++)
					{
						sum += elements[i * cols + k] * other.elements[k * other.cols + j];
					}
					result.elements[i * other.cols + j] = sum;
				}
			}
			return result;
		}
		else
		{
			throw std::exception("Matrix dimensions must agree.");
		}
	};

	Matrix<T> operator * (const T& scalar) {
		Matrix<T> result(rows, cols);
		for (int i = 0; i < rows * cols; i++)
		{
			result.elements[i] = elements[i] * scalar;
		}
		return result;
	};

	Matrix<T> operator *= (const T& scalar) {
		for (int i = 0; i < rows * cols; i++)
		{
			elements[i] *= scalar;
		}
		return *this;
	};


	void set(int row, int col, T val)
	{
		if (row < 0 || row >= rows || col < 0 || col >= cols) {
			throw std::out_of_range("Index out of range");
		}
		elements[row * cols + col] = val;
	};


	Matrix<T> inverse()
	{
		if (rows == cols)
		{
			Matrix<T> tmp(*this);

			Matrix<T> result(rows, cols);
			for (int i = 0; i < rows; i++)
			{
				result.elements[i * cols + i] = 1;
			}
			for (int i = 0; i < rows; i++)
			{
				T pivot = tmp.elements[i * cols + i];
				for (int j = 0; j < cols; j++)
				{
					tmp.elements[i * cols + j] /= pivot;
					result.elements[i * cols + j] /= pivot;
				}
				for (int j = 0; j < rows; j++)
				{
					if (i != j)
					{
						T factor = tmp.elements[j * cols + i];
						for (int k = 0; k < cols; k++)
						{
							tmp.elements[j * cols + k] -= factor * tmp.elements[i * cols + k];
							result.elements[j * cols + k] -= factor * result.elements[i * cols + k];
						}
					}
				}
			}
			return result;
		}
		else
		{
			throw std::exception("Matrix dimensions must agree.");
		}
	};


	Matrix<T> getSubMatrix(int startRow, int startCol, int rows, int columns)
	{
		if (startRow + rows <= this->rows && startCol + columns <= this->cols)
		{
			Matrix<T> result(rows, columns);
			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < columns; j++)
				{
					result.elements[i * columns + j] = elements[(startRow + i) * this->cols + startCol + j];
				}
			}
			return result;
		}
		else
		{
			throw std::exception("Matrix dimensions must agree.");
		}
	};

	std::vector<T> getRow(int row)
	{
		if (row < 0 || row >= rows)
		{
			throw std::out_of_range("Index out of range");
		}
		std::vector<T> result(cols);
		for (int i = 0; i < cols; i++)
		{
			result[i] = elements[row * cols + i];
		}
		return result;
	};

	std::vector<T> getColumn(int col)
	{
		if (col < 0 || col >= cols)
		{
			throw std::out_of_range("Index out of range");
		}
		std::vector<T> result(rows);
		for (int i = 0; i < rows; i++)
		{
			result[i] = elements[i * cols + col];
		}
		return result;
	};


	void printMatrix()
	{
		for (int i = 0; i < rows; i++)
		{
			std::cout << "[";
			for (int j = 0; j < cols; j++)
			{
				std::cout << (std::string)(elements[i * cols + j]);
				if (j < cols - 1)
					std::cout << " ";
			}
			std::cout << "]" << std::endl;
		}
		std::cout << "-----------------------------------" << std::endl;
	};

	bool gaussElimimination()
	{
		if (rows <= cols)
		{
			T zeroVal = T(0);
			for (int r = 1; r < rows; r++)
			{
				bool NonZeroFound = false;

				T fp = elements[(r - 1) * cols + r - 1];
				T fpInv = T(1) / fp;
				//if (fp != 1)
				//{
				//	for (int i = r-1; i < cols; i++)
				//	{
				//		elements[(r - 1) * cols + i] /= fp;
				//	}
				//}

				for (int i = r; i < rows; i++)
				{
					T factor = elements[i * cols + r - 1] * fpInv;
					for (int j = r - 1; j < cols; j++)
					{
						if (
							(elements[i * cols + j] -= factor * elements[(r - 1) * cols + j])
							!= zeroVal)
						{
							NonZeroFound = true;
						}
					}
				}
				if (! NonZeroFound)
				{
					return false;
				}
			
				//printMatrix();
			}
			// continue to form a diagonal matrix
			for (int r = rows - 1; r > 0; r--)
			{
				for (int i = r - 1; i >= 0; i--)
				{
					T factor = elements[i * cols + r] / elements[r * cols + r];
					for (int j = r; j < cols; j++)
					{
						elements[i * cols + j] -= factor * elements[r * cols + j];
					}
				}
				//printMatrix();
			}
			// continue to form a identity matrix
			for (int r = 0; r < rows; r++)
			{
				T factor = elements[r * cols + r];
				elements[r * cols + r] /= factor;
				for (int i = rows; i < cols; i++)
				{
					elements[r * cols + i] /= factor;
				}
			}
			//printMatrix();
		}
		else
		{
			throw std::exception("Matrix dimensions must agree.");
		}
		return true;
	}

	int rows;
	int cols;
	std::vector<T> elements;
};

