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
	m_content = new std::list<T>[n * m];
}

template<typename T>
Matrix<T>::~Matrix() {
	if (m_content != NULL) {
		delete [] m_content;
 	}
}

template<typename T>
std::list<T> & Matrix<T>::at(const int & i, const int & j)
{
	return m_content[i * m_numLine + j];
}

template<typename T>
const std::list<T> & Matrix<T>::at(const int & i, const int & j) const
{
	if (i < 0 || i >= m_numLine || j < 0 || j >= m_numCol) {
		throw std::invalid_argument("Invalid index");
	}
	return m_content[i * m_numLine + j];
}

template<typename T>
void Matrix<T>::add(const int & i, const int & j, const T & elt)
{
	if (i < 0 || i >= m_numLine || j < 0 || j >= m_numCol) {
		throw std::invalid_argument("Invalid index");
	}
	m_content[i * m_numLine + j].push_back(elt);
}

template<typename T>
const int Matrix<T>::getNumLine() const
{
	return m_numLine;
}

template<typename T>
const int Matrix<T>::getNumCol() const
{
	return m_numCol;
}

template<typename T>
void Matrix<T>::move(const T & elt, const int & iprev, const int & jprev, const int & inext, const int & jnext)
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

	if (it == at(iprev, jprev).end()) {
		throw std::invalid_argument("In function move : Element not in the initial list");
	}

	at(iprev, jprev).splice(it, at(inext, jnext));
}

#endif
