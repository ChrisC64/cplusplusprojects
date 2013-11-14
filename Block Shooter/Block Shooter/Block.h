#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>
#include "PlayingField.h"
using namespace std;
class Block : public PlayingField
{
private:
	/////////////////////////////////////////////////////////////////////////
	// 3D Mesh Objects
	/////////////////////////////////////////////////////////////////////////
	ID3DXMesh*			m_pBoxMesh;

	D3DXVECTOR3 Position;

	float mb_fCenterX, mb_fCenterY;

public:
	///////////////////////////
	//Constructor/Destructors//
	///////////////////////////
	Block();
	~Block();

	void Init();

	void Update();

	void Draw();

};
#endif BLOCK_H