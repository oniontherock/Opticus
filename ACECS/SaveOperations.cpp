#include "SaveOperations.hpp"


// BaseLevelPtr / GameLevel Out
std::ofstream& operator<< (std::ofstream& str, GameLevel& item) {
	str.write(asByte(&item.idPlayer), sizeof(item.idPlayer));
	str.write(asByte(&item.firstRun), sizeof(item.firstRun));
	str.write(asByte(&item.saveTest), sizeof(item.saveTest));

	return str;
}
// BaseLevelPtr / GameLevel in
std::ifstream& operator>> (std::ifstream& str, GameLevel& item) {

	str.read(asByte(&item.idPlayer), sizeof(item.idPlayer));
	str.read(asByte(&item.firstRun), sizeof(item.firstRun));
	str.read(asByte(&item.saveTest), sizeof(item.saveTest));

	return str;
}