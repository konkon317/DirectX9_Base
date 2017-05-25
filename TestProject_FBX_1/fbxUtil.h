#pragma once

#include "Global.h"

class Model;


void ProbeNode(FbxNode* pNode,Model* pModel=nullptr,int depth=0);

bool IsMesh(FbxNode* pNode);