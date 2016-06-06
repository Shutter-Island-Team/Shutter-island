#include "../../include/boids2D/BoidType.hpp"

std::ostream& operator<< (std::ostream &out, const BoidType &boidType)
{
	switch(boidType) {
		case RABBIT:
			out << "Rabbit";
			break;
		case WOLF:
			out << "Wolf";
			break;
		default:
			out << "UNKNOWN";
			break;
	}

    return out;	
}