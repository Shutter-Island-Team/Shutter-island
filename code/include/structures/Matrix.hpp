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
 * @brief Structure of matrix of list
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
  Matrix<T>( size_t n, size_t m );

  /**
   * @brief Destructor
   */
  ~Matrix<T>();

  /**
   * @brief     Getter of the reference of the list of the matrix giving a position
   * @Warning   The function does not check if the position is valid
   * @param[in] i Number of the line
   * @param[in] j Number of the column 
   * @return    Returns a reference of the list at the giving position
   */
  std::list<T> & at(const unsigned int & i, const unsigned int & j);

  /**
   * @brief     Getter of the list of the matrix giving a position
   * @Warning   The function does not check if the position is valid
   * @param[in] i Number of the line
   * @param[in] j Number of the column
   * @return    Return the list of element at the giving position
   */
  const std::list<T> & at(const unsigned int & i, const unsigned int & j) const;

  /**
   * @brief     Add an element in the list of the matrix at the giving position
   * @param[in] i   Number of the line
   * @param[in] j   Number of the column
   * @param[in] elt Value of the element to be added
   */
  void add(const unsigned int & i, const unsigned int & j, const T & elt);

  /**
   * @brief   Getter of the number of line
   * @return  Number of line of the matrix
   */
  const size_t getNumLine() const;

  /**
   * @brief   Getter of the number of column
   * @return  Number of column of the matrix
   */
  const size_t getNumCol() const;

  /**
   * @brief     Remove an element from the position (iprev, jprev) and add it to (inext, jnext)
   * @param[in] elt Element to add
   * @param[in] iprev index of the previous line
   * @param[in] jprev index of the previous column
   * @param[in] inext index of the next line
   * @param[in] jnext index of the next line
   */
  void move(const T & elt, const unsigned int & iprev, const unsigned int & jprev, const unsigned int & inext, const unsigned int & jnext);

  const std::list<T> merge(const unsigned int & i, const unsigned int & j) const;

 private:
  const size_t m_numLine;
  const size_t m_numCol;
  std::list<T> *m_content; ///< Container of in the class
};

#include "../../src/structures/Matrix.tpp"

#endif
