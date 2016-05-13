#ifndef FPSCOUNTER_H_
#define FPSCOUNTER_H_

/** @file
 * @brief Define an averaged FPS counter
 */

#include <chrono>

/** @brief Compute an averaged FPS.
 *
 * This helper class provides an automatically updated and averaged FPS count.
 * The value is averaged at each interval specified in the constructor. A typical
 * use of this class:
 * \code{.cpp}
 * FPSCounter counter( 1.5 ); // value refreshed every 1.5 second
 * while( running ) // main rendering loop
 * {
 *   float averageFPS = counter.getFPS();
 *   // averageFPS is guaranteed to remain the same for each interval
 *   // of 1.5 second. We can then display this value on screen: it will
 *   // slowly change such that we can still read it correctly.
 * }
 * \endcode
 */
class FPSCounter
{
public:
  /** @brief Build a FPS counter with a specific refresh interval.
   *
   * Create a FPS counter that will be refreshed every displayInterval
   * second(s).
   *
   * @param refreshInterval refresh interval, i.e. minimum duration for which
   * the value returned by this won't change. At the end of the interval, a
   * new FPS value is computed and stored to be returned.
   */
  FPSCounter( double refreshInterval = 2.0 );

  /** @brief Instance destructor.
   *
   * Instance destructor.
   */
  ~FPSCounter();

  /** @brief Get the averaged FPS count.
   *
   * This function updates the internal members of the instance, taking into
   * account the last time it was called. If the last refresh was enough time
   * ago, a new averaged fps count is computed. This is this averaged fps count
   * that is returned in all cases.
   *
   * @return The averaged FPS count.
   */
  float getFPS();

private:
  /** We use the system_clock of std::chrono to compute durations.
   * chrono is a nice addition to the c++ std, go have a look there:
   * http://www.cplusplus.com/reference/chrono/
   */
  typedef std::chrono::system_clock clock;
  /** We use a double precision duration type to have a precise FPS estimation.
   */
  typedef std::chrono::duration<double> Duration;
  /** Based on the clock and the duration types, we define a time point type
   * to store last events date.
   */
  typedef std::chrono::time_point< clock, Duration > TimePoint;

  /** Duration between two refresh of FPSCounter::m_fps. */
  Duration m_refreshInterval;
  /** Last time the FPSCounter::m_fps was refreshed. */
  TimePoint m_lastRefreshTime;
  /** Last time getFPS() was called. */
  TimePoint m_lastCallTime;
  /** The sum of the FPS computed at each call to getFPS() since last refresh. */
  double m_accumulatedFPS;
  /** Averaged FPS count stored such that getFPS() returns the same value between two refreshes.
   * When a refresh occurs, this value receives FPSCounter::m_accumulatedFPS / FPSCounter::m_numberOfSamples.*/
  float m_fps;
  /** The number of FPS computed by each call to getFPS() since last refresh. */
  unsigned int m_numberOfSamples;
};

#endif /* ATUIN_FPSCOUNTER_H_ */
