#include "TeleportSystem.h"

TeleportSystem::TeleportSystem()
{
	std::array<std::pair<glm::vec3, glm::vec3>, 9> positions =
	{
		std::make_pair(glm::vec3(9.7801485, 14.863845, -304.78183),
					   glm::vec3(35.64512,-174.74876,-308.76395)),

		std::make_pair(glm::vec3(10.37517,19.467102,-428.13168),
					   glm::vec3(23.842197,-176.02649,-506.9026)),

		std::make_pair(glm::vec3(9.424553,20.513628,-551.026),
					   glm::vec3(20.026344,-173.36757,-723.678)),

		std::make_pair(glm::vec3(12.962267,10.982218,-666.49585),
					   glm::vec3(17.512012,-174.62334,-917.03394)),

		std::make_pair(glm::vec3(-64.99592,27.617996,-709.5072),
					   glm::vec3(-42.084026,-176.44987,-1019.4506)),

		std::make_pair(glm::vec3(-146.69879,20.236301,-667.29083),
					   glm::vec3(-128.31462,-175.0184,-914.53455)),

		std::make_pair(glm::vec3(-146.52676,15.648167,-552.51),
					   glm::vec3(-123.68435,-176.2005,-714.96747)),

		std::make_pair(glm::vec3(-136.55302,15.825421,-432.32208),
					   glm::vec3(-119.534386,-175.43192,-499.6635)),
				
		std::make_pair(glm::vec3(-135.90807,16.439344,-306.9898),
					   glm::vec3(-116.718864,-177.81673,-306.48254))
	};
	
	mTimer.setDimensionOfTime(Dimension::SECONDS);

	std::shared_ptr<Primitive> cube = std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	std::weak_ptr<Primitive> cubeWeak = cube;

	mDoors.reserve(9);

	mDoors.emplace(TypeDoor::FLASHLIGHT, std::make_pair(std::make_unique<Mesh>(cubeWeak),
													    std::make_unique<Mesh>(cubeWeak)));
	mDoors.emplace(TypeDoor::SPOTLIGHT, std::make_pair(std::make_unique<Mesh>(cubeWeak),
													   std::make_unique<Mesh>(cubeWeak)));
	mDoors.emplace(TypeDoor::POINT, std::make_pair(std::make_unique<Mesh>(cubeWeak),
												   std::make_unique<Mesh>(cubeWeak)));
	mDoors.emplace(TypeDoor::DIRECTIONAL, std::make_pair(std::make_unique<Mesh>(cubeWeak),
										  std::make_unique<Mesh>(cubeWeak)));
	mDoors.emplace(TypeDoor::MAIN, std::make_pair(std::make_unique<Mesh>(cubeWeak),
												  std::make_unique<Mesh>(cubeWeak)));
	mDoors.emplace(TypeDoor::AREA, std::make_pair(std::make_unique<Mesh>(cubeWeak),
												  std::make_unique<Mesh>(cubeWeak)));
	mDoors.emplace(TypeDoor::EMISSIVE, std::make_pair(std::make_unique<Mesh>(cubeWeak),
													  std::make_unique<Mesh>(cubeWeak)));
	mDoors.emplace(TypeDoor::MULTIPLE, std::make_pair(std::make_unique<Mesh>(cubeWeak),
													  std::make_unique<Mesh>(cubeWeak)));
	mDoors.emplace(TypeDoor::ATTENNUATION, std::make_pair(std::make_unique<Mesh>(cubeWeak),
														  std::make_unique<Mesh>(cubeWeak)));

	uint32_t counter = 0;
	for (auto& [key, value] : mDoors)
	{
		value.first->setPos(positions[counter].first);
		value.second->setPos(positions[counter].second);
		counter++;
	}
}

void TeleportSystem::turnOnTeleport(bool pTP)
{
	mTPisTurnedOn = pTP;
}

Mesh& TeleportSystem::getMeshOfUpDoor(TypeDoor pTypeDoor)
{
	return *mDoors[pTypeDoor].first;
}

Mesh& TeleportSystem::getMeshOfDownDoor(TypeDoor pTypeDoor)
{
	return *mDoors[pTypeDoor].second;
}

void TeleportSystem::update(Player& pPlayer)
{
	if (mTPisTurnedOn && !mTimer.isRunning())
	{
		glm::vec3 charPos = pPlayer.getPos();
		for (auto& [key, value] : mDoors)
		{
			if (mCollider.areCollided(*value.first, pPlayer.getHitbox()))
			{
				std::cout << "i collide!\n";
				pPlayer.freezePlayer(true);
				pPlayer.setPos(value.second->getPos());
				pPlayer.freezePlayer(false);
				mTimer.startTimer();
			}
			if (mCollider.areCollided(*value.second, pPlayer.getHitbox()))
			{
				std::cout << "i collide!\n";
				pPlayer.freezePlayer(true);
				pPlayer.setPos(value.first->getPos());
				pPlayer.freezePlayer(false);
				mTimer.startTimer();
			}
		}
	}
	if (mTimer.isRunning() && mTimer.getDeltaTime(false) >= 3.0f)
		mTimer.stopTimer();
}
