#include"../include/MaterialsManager.h"

MaterialsManager::MaterialsManager()
{
	WHITE_MTRL = new KoaliMaterial(d3d::WHITE, d3d::WHITE, d3d::WHITE, d3d::BLACK, 2.0f);
	RED_MTRL = new KoaliMaterial(d3d::RED, d3d::RED, d3d::RED, d3d::BLACK, 2.0f);
}


MaterialsManager::~MaterialsManager()
{
}
