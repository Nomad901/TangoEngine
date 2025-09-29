#pragma once
#include <iostream>
#include <format>
#include <unordered_map>
#include <memory>

#include "Collider.h"
#include "Model.h"
#include "Timer.h"
#include "Player.h"

enum class TypeDoor
{
	FLASHLIGHT = 0,
	SPOTLIGHT = 1,
	POINT = 2,
	DIRECTIONAL = 3,
	MAIN = 4,
	AREA = 5,
	EMISSIVE = 6,
	MULTIPLE = 7,
	ATTENNUATION = 8
};

class TeleportSystem
{
public:
	TeleportSystem();

	void turnOnTeleport(bool pTP);
	Mesh& getMeshOfUpDoor(TypeDoor pTypeDoor);
	Mesh& getMeshOfDownDoor(TypeDoor pTypeDoor);

	void update(Player& pPlayer);

private:
	bool mTPisTurnedOn{ true };
	std::unordered_map<TypeDoor, std::pair<std::unique_ptr<Mesh>, std::unique_ptr<Mesh>>> mDoors;
	Collider mCollider;
	Timer mTimer;
};

//	FlashLight pos (first - up room. second - down room):
//	Character pos: 9.7801485,14.863845,-304.78183
//	Character pos: 36.64512,-174.74876,-308.76395
//	spotlight:
//	Character pos: 10.37517,19.467102,-428.13168
//	Character pos: 24.842197,-176.02649,-506.9026
//	point:
//	Character pos: 9.424553,20.513628,-551.026
//	Character pos: 21.026344,-173.36757,-723.678
//	directional:
//	Character pos: 12.962267,10.982218,-666.49585
//	Character pos: 18.512012,-174.62334,-917.03394
//	main:
//	Character pos: -64.99592,27.617996,-709.5072
//	Character pos: -42.084026,-176.44987,-1019.4506
//	area:
//	Character pos: -146.69879,20.236301,-667.29083
//	Character pos: -129.31462,-175.0184,-914.53455
//	emissive:
//	Character pos: -146.52676,15.648167,-552.51
//	Character pos: -124.68435,-176.2005,-714.96747
//	multiple:
//	Character pos: -136.55302,15.825421,-432.32208
//	Character pos: -120.534386,-175.43192,-499.6635
//	attenuation:
//	Character pos: -135.90807,16.439344,-306.9898
//	Character pos: -117.718864,-177.81673,-306.48254
