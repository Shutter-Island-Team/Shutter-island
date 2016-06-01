/**
 *  @file      Matrix.tpp
 *  @brief     Implementation of Matrix. See Matrix.h 
 *  @author    HAMMEN Maxence
 *  @version   1.0
 *  @date      06/01/16
 */

#ifndef MATRIX_TPP
#define MATRIX_TPP

#include <iostream>

template<typename T>
Matrix<T>::Matrix( int n, int m ) {
	if (m < 0 || n < 0) {
    	throw std::invalid_argument("Dimension of matrix must be positive");
	}
	m_numLine = n;
	m_numCol = m;
	m_content = new std::vector<T>[n * m];
}

template<typename T>
Matrix<T>::~Matrix() {
	std::cerr << "Lulz" << std::endl;
	for (int i = 0; i < m_numLine * m_numCol; ++i) {
		
	}
}

template<typename T>
std::vector<T> & Matrix<T>::at(const int & i, const int & j)
{
	return m_content[i * m_numLine + j];
}

template<typename T>
const std::vector<T> & Matrix<T>::at(const int & i, const int & j) const
{
	return m_content[i * m_numLine + j];
}

template<typename T>
void Matrix<T>::push_back(const int & i, const int & j, const T & elt)
{
	m_content[i * m_numLine + j].push_back(elt);
}

template<typename T>
int Matrix<T>::getNumLine() const
{
	return m_numLine;
}

template<typename T>
int Matrix<T>::getNumCol() const
{
	return m_numCol;
}

template<typename T>
int Matrix<T>::getNumElt() const 
{
	int res = 0;
	for (int i = 0; i < m_numCol; ++i) {
		for (int j = 0; j < m_numLine; ++j) {
			res += m_content[i * m_numLine + j].size();
		}
	}
	return res;
}

#endif
