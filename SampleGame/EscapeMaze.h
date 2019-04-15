#pragma once
#include"../KoaliEngine/include/Cube.h"
#include"../KoaliEngine/include/Quad.h"
#include"../KoaliEngine/include/SkyBox.h"
#include"../KoaliEngine/include/MaterialsManager.h"
#include"../KoaliEngine/include/ObjectModel.h"

class EscapeMaze
{
public:
	EscapeMaze(LPDIRECT3DDEVICE9 pDevice);
	~EscapeMaze();

	void init();
	void update();
	void render();



private:
	LPDIRECT3DDEVICE9 pd3dDevice;
	Quad *floor;
	Cube *wall[9];
	ObjectModel *player;
	Cube *playerTest;
	SkyBox *skybox;
	MaterialsManager *materialsManager;
};

