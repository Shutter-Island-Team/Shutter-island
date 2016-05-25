/**
 *  @file		StateType.hpp
 *  @brief		Contain the enumerate for the state of the boids
 *  @author		HAMMEN Maxence
 *	@author		KACHER Ilyes
 *  @author		LY Mickael
 *	@author		STOFFEL Mathieu
 *  @version	0.5
 *  @date		05/24/16
 */
#ifndef STATE_TYPE_HPP
#define STATE_TYPE_HPP

/**
 * @enum StateType
 * @brief Record the different type of behavior
 */
enum StateType {
	TEST_STATE,
	WALK_STATE,
	STAY_STATE,
	SLEEP_STATE,
	FLEE_STATE,
	FIND_FOOD_STATE,
	ATTACK_STATE,
	EAT_STATE,
	LOST_STATE,
	FIND_WATER_STATE,
	DRINK_STATE,
	MATE_STATE
};

#endif
