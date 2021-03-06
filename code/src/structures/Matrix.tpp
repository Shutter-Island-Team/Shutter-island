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
Matrix<T>::Matrix( size_t n, size_t m ) : m_numLine(n), m_numCol(m) {
	if (m < 0 || n < 0) {
    	throw std::invalid_argument("Dimension of matrix must be positive");
	}
	m_content = new std::list<T>[n * m];
}

template<typename T>
Matrix<T>::~Matrix() {
	if (m_content != NULL) {
		delete [] m_content;
 	}
}

template<typename T>
std::list<T> & Matrix<T>::at(const unsigned int & i, const unsigned int & j)
{
	return m_content[i * m_numLine + j];
}

template<typename T>
const std::list<T> & Matrix<T>::at(const unsigned int & i, const unsigned int & j) const
{
	if (i < 0 || i >= m_numLine || j < 0 || j >= m_numCol) {
		throw std::invalid_argument("Invalid index");
	}
	return m_content[i * m_numLine + j];
}

template<typename T>
void Matrix<T>::add(const unsigned int & i, const unsigned int & j, const T & elt)
{
	if (i < 0 || i >= m_numLine || j < 0 || j >= m_numCol) {
		throw std::invalid_argument("Invalid index");
	}
	m_content[i * m_numLine + j].push_back(elt);
}

template<typename T>
const size_t Matrix<T>::getNumLine() const
{
	return m_numLine;
}

template<typename T>
const size_t Matrix<T>::getNumCol() const
{
	return m_numCol;
}

template<typename T>
const std::list<T> Matrix<T>::merge(const unsigned int & i, const unsigned int & j) const
{
	std::list<T> res;
	for (unsigned int iloop = i-1; iloop <= i+1; ++iloop) {
		for (unsigned int jloop = j-1; jloop <= j+1; ++jloop) {
			res.insert(res.end(), at(iloop, jloop).begin(), at(iloop,jloop).end());
		}
	}
	return res;
}

template<typename T>
void Matrix<T>::move(const T & elt, const unsigned int & iprev, const unsigned int & jprev, const unsigned int & inext, const unsigned int & jnext)
{
	if (iprev < 0 || iprev >= m_numLine || jprev < 0 || jprev >= m_numCol) {
		throw std::invalid_argument("Invalid index");
	}
	if (inext < 0 || inext >= m_numLine || jnext < 0 || jnext >= m_numCol) {
		throw std::invalid_argument("Invalid index");
	}

	typename std::list<T>::iterator it = at(iprev, jprev).begin();
	bool found = false;

	while ( !found && it != at(iprev, jprev).end() ) {
		if ( *it == elt ) {
			found = true;
		} else {
			it++;
		}
	}

	if (it == at(iprev, jprev).end() ) {
		throw std::invalid_argument("Did not find elt in initial list");
	} 

	at(inext, jnext).splice(at(inext, jnext).begin(), at(iprev, jprev), it);
}

template<typename T>
std::ostream& operator<< (std::ostream &out, const std::list<T> &l)
{
	out << l.size();
	return out;
}

template<typename T>
std::ostream& operator<< (std::ostream &out, const Matrix<T> &mat)
{
	for (unsigned int i = 0; i < mat.getNumLine(); ++i) {
		for (unsigned int j = 0; j < mat.getNumCol(); ++j) {
			if (mat.at(i,j).size() != 0) {			
				out << "(" << i << "," << j << "): " << mat.at(i, j) << " | ";
			}
		}
	}
	return out;
}


#endif
