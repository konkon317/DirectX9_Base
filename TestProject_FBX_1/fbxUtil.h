#pragma once


#include <fbxsdk.h>
#include <iostream>

void ProbeNode(FbxNode* pNode,int depth=0);

bool IsMesh(FbxNode* pNode);