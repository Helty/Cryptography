#include "TestCheckFlatDistribution.h"

std::vector<Point> getPoints(std::string NumSequence)
{
	std::vector<Point> points;
	for (size_t i = 0; i < NumSequence.size() - 1; i += 1)
	{
		std::string pair = NumSequence.substr(i, 2);
		Point point = { (uint16_t)(pair[0] - '0'), (uint16_t)(pair[1] - '0') };
		points.push_back(point);
	}
	return points;
}
void StartCheckFlatDistribution(std::string const& NumSequence)
{
	std::vector<Point> points = getPoints(NumSequence);
	std::cout << "CheckFlatDistribution: ";
	for (Point const& point : points)
	{
		std::cout << point.x << "," << point.y << "; ";
	}
	std::cout << std::endl;
}