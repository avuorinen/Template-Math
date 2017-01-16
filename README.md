# Template-Math

## Example
``` C++

#include <Vector.h>

int main(int, char**)
{
	Math::Vector2 vector2(1, 2);
	Math::Vector3 vector3(1, 2, 3);
	Math::Vector4 vector4(1, 2, 3, 4);

	std::cout << "[2 1] + [1 2] = " << vector2 + Math::Vector2(2, 1) << std::endl;

	std::cout << "[1 2 3] - [1 2 0] = " << vector3 - vector2 << std::endl;

	std::cout << "[1 2 3] * 10 = " << vector3 * 10 << std::endl;
	
	// Size, Type
	Math::Vector<3, int> vector3i(3, 2, 1);

	// Dot Product
	float dot = vector3 * vector3i;
	// or
	dot = Math::Vector3::Dot(vector3, vector3i);

	std::cout << "[1 2 3] * [3 2 1] = " << dot << std::endl;

	Math::Vector3 cross3 = vector3.Cross(vector3i);
	std::cout << "[1 2 3] x [3 2 1] = " << cross3 << std::endl;

	float cross = vector2.Cross(Math::Vector2(2, 1));
	std::cout << "[1 2] x [2 1] = " << cross << std::endl;

	float lenght = vector3.Magnitude(); // Or cross.SqrtMagnitude();
	std::cout << "|[1 2 3]| = " << lenght << std::endl;

	Math::Vector3 normalized = vector3.Normalize();
	std::cout << "[1 2 3] normalized = " << normalized << std::endl;
  
  return 0;
}


```
