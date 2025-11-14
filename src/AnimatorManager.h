#pragma once
#include <unordered_map>

#include "Animator.h"

class AnimatorManager
{
public:
	AnimatorManager();
	AnimatorManager(std::string_view pName, std::unique_ptr<Animator> pAnimator);

	auto getAnimator(std::string_view pName) -> Animator*;
	
	auto pushAnimator(std::string_view pName, std::unique_ptr<Animator> pAnimator) -> void;
	auto eraseAnimator(std::string_view pName) -> void;
	auto isExist(std::string_view pName) -> bool;

	auto getAnimMeshesStorage() -> std::unordered_map<std::string, std::unique_ptr<Animator>>&;

private:
	std::unordered_map<std::string, std::unique_ptr<Animator>> mAnimMeshesStorage;
};

