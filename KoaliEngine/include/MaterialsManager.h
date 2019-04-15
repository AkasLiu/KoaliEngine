#pragma once
#include"KoaliMaterial.h"

class MaterialsManager
{
private:
	MaterialsManager();
	MaterialsManager(const MaterialsManager &);
	//MaterialsManager& operator=(const MaterialsManager &);
	~MaterialsManager();
	static MaterialsManager myInstance;

public:
	static MaterialsManager* getInstance()
	{
		return &myInstance;
	}

public:
	KoaliMaterial *WHITE_MTRL;
	KoaliMaterial *RED_MTRL;
};