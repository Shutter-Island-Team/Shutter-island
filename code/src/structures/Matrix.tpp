/**
 *  @file      Matrix.tpp
 *  @brief     Implementation of Matrix. See Matrix.h 
 *  @author    HAMMEN Maxence
 *  @version   1.0
 *  @date      06/01/16
 */

#ifndef MATRIX_TPP
#define MATRIX_TPP

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
	for (int i = 0; i < m_numLine * m_numCol; ++i) {
		m_content[i].clear();
	}
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

#endif
