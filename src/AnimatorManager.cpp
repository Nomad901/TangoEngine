#include "AnimatorManager.h"

AnimatorManager::AnimatorManager()
{
	mAnimMeshesStorage.reserve(20);
}

AnimatorManager::AnimatorManager(std::string_view pName, std::unique_ptr<Animator> pAnimator)
{
	mAnimMeshesStorage.reserve(20);
	mAnimMeshesStorage.emplace(std::string(pName), std::move(pAnimator));
}

Animator* AnimatorManager::getAnimator(std::string_view pName)
{
	std::string name = std::string(pName);
	if (mAnimMeshesStorage.contains(name))
		return mAnimMeshesStorage[name].get();

	std::cout << std::format("The storage doesnt contain the mesh with this name: {}!\n", name);
	return nullptr;
}

void AnimatorManager::pushAnimator(std::string_view pName, std::unique_ptr<Animator> pAnimator)
{
	mAnimMeshesStorage.insert_or_assign(std::string(pName), std::move(pAnimator));
}

void AnimatorManager::eraseAnimator(std::string_view pName)
{
	std::string name = std::string(pName);
	if (mAnimMeshesStorage.contains(name))
		mAnimMeshesStorage.erase(name);
	else 
		std::cout << std::format("The storage doesnt contain the mesh with this name: {}!\n", name);
}

bool AnimatorManager::isExist(std::string_view pName)
{
	return mAnimMeshesStorage.contains(std::string(pName));
}

auto AnimatorManager::getAnimMeshesStorage() -> std::unordered_map<std::string, std::unique_ptr<Animator>>&
{
	return mAnimMeshesStorage;
}
