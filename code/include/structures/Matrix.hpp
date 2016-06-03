/**
 *  @file      Matrix.hpp
 *  @brief     Implementation of a matrix 
 *  @author    HAMMEN Maxence
 *  @version   1.0
 *  @date      06/01/16
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <list>
#include <stdexcept>

/**
 * @class Matrix
 * @brief Class to manipulate an actual Matrix of Triangles
 */
 
template<typename T>
class Matrix
{
 public:
  /**
   * @brief     Creates a Matrix with a given size
   * @param[in] n Number of lines
   * @param[in] m Number of columns
   */
  Matrix<T>( int n, int m );

  /**
   * @brief Destructor
   */
  ~Matrix<T>();

  std::list<T> & at(const int & i, const int & j);

  const std::list<T> & at(const int & i, const int & j) const;

  void push_back(const int & i, const int & j, const T & elt);

  int getNumLine() const;

  int getNumCol() const;

  int getNumElt() const;

  void move(const T & elt, const int & iprev, const int & jprev, const int & inext, const int & jnext);


 private:
  std::list<T> *m_content; ///< Container of in the class
  int m_numLine;
  int m_numCol;
};

#include "../../src/structures/Matrix.tpp"

#endif
