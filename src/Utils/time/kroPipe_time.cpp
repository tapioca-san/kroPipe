
#include "kroPipe_time.hpp"

namespace KP {
namespace UTILS {

std::pair<uint, float> getTimeFraction(std::vector<float>& times, float& deltaTime) {
	uint segment = 0;
	while (deltaTime > times[segment])
		segment++;
	float start = times[segment - 1];
	float end = times[segment];
	float fraction = (deltaTime - start) / (end - start);
	return {segment, fraction};
}

} // namespace UTILS
} // namespace KP


