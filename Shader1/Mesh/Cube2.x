xof 0303txt 0032

template XSkinMeshHeader {
 <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
 WORD nMaxSkinWeightsPerVertex;
 WORD nMaxSkinWeightsPerFace;
 WORD nBones;
}

template VertexDuplicationIndices {
 <b8d65549-d7c9-4995-89cf-53a9a8b031e3>
 DWORD nIndices;
 DWORD nOriginalVertices;
 array DWORD indices[nIndices];
}

template SkinWeights {
 <6f0d123b-bad2-4167-a0d0-80224f25fabb>
 STRING transformNodeName;
 DWORD nWeights;
 array DWORD vertexIndices[nWeights];
 array FLOAT weights[nWeights];
 Matrix4x4 matrixOffset;
}


Frame Frame_SCENE_ROOT {

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Frame Frame1_Cube2 {

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }

  Mesh Mesh_Cube2 {
   108;
   -5.000000;-5.800000;-5.000000;,
   -5.000000;-5.800000;-5.000000;,
   -5.000000;-5.800000;-5.000000;,
   -5.000000;-5.800000;-5.000000;,
   -5.000000;-5.800000;-5.000000;,
   5.000000;-5.800000;-5.000000;,
   5.000000;-5.800000;-5.000000;,
   5.000000;-5.800000;-5.000000;,
   5.000000;-5.800000;-5.000000;,
   5.000000;-5.800000;5.000000;,
   5.000000;-5.800000;5.000000;,
   5.000000;-5.800000;5.000000;,
   5.000000;-5.800000;5.000000;,
   -5.000000;-5.800000;5.000000;,
   -5.000000;-5.800000;5.000000;,
   -5.000000;-5.800000;5.000000;,
   -5.000000;-5.800000;5.000000;,
   -5.000000;-5.000000;-5.800000;,
   -5.000000;-5.000000;-5.800000;,
   -5.000000;-5.000000;-5.800000;,
   -5.000000;-5.000000;-5.800000;,
   -5.000000;-5.000000;-5.800000;,
   5.000000;-5.000000;-5.800000;,
   5.000000;-5.000000;-5.800000;,
   5.000000;-5.000000;-5.800000;,
   5.000000;-5.000000;-5.800000;,
   5.800000;-5.000000;-5.000000;,
   5.800000;-5.000000;-5.000000;,
   5.800000;-5.000000;-5.000000;,
   5.800000;-5.000000;-5.000000;,
   5.800000;-5.000000;5.000000;,
   5.800000;-5.000000;5.000000;,
   5.800000;-5.000000;5.000000;,
   5.800000;-5.000000;5.000000;,
   5.000000;-5.000000;5.800000;,
   5.000000;-5.000000;5.800000;,
   5.000000;-5.000000;5.800000;,
   5.000000;-5.000000;5.800000;,
   -5.000000;-5.000000;5.800000;,
   -5.000000;-5.000000;5.800000;,
   -5.000000;-5.000000;5.800000;,
   -5.000000;-5.000000;5.800000;,
   -5.800000;-5.000000;5.000000;,
   -5.800000;-5.000000;5.000000;,
   -5.800000;-5.000000;5.000000;,
   -5.800000;-5.000000;5.000000;,
   -5.800000;-5.000000;-5.000000;,
   -5.800000;-5.000000;-5.000000;,
   -5.800000;-5.000000;-5.000000;,
   -5.800000;-5.000000;-5.000000;,
   -5.800000;-5.000000;-5.000000;,
   -5.000000;5.000000;-5.800000;,
   -5.000000;5.000000;-5.800000;,
   -5.000000;5.000000;-5.800000;,
   -5.000000;5.000000;-5.800000;,
   5.000000;5.000000;-5.800000;,
   5.000000;5.000000;-5.800000;,
   5.000000;5.000000;-5.800000;,
   5.000000;5.000000;-5.800000;,
   5.000000;5.000000;-5.800000;,
   -5.800000;5.000000;-5.000000;,
   -5.800000;5.000000;-5.000000;,
   -5.800000;5.000000;-5.000000;,
   -5.800000;5.000000;-5.000000;,
   -5.000000;5.800000;-5.000000;,
   -5.000000;5.800000;-5.000000;,
   -5.000000;5.800000;-5.000000;,
   -5.000000;5.800000;-5.000000;,
   5.000000;5.800000;-5.000000;,
   5.000000;5.800000;-5.000000;,
   5.000000;5.800000;-5.000000;,
   5.000000;5.800000;-5.000000;,
   5.000000;5.800000;-5.000000;,
   5.800000;5.000000;-5.000000;,
   5.800000;5.000000;-5.000000;,
   5.800000;5.000000;-5.000000;,
   5.800000;5.000000;-5.000000;,
   5.800000;5.000000;-5.000000;,
   5.800000;5.000000;5.000000;,
   5.800000;5.000000;5.000000;,
   5.800000;5.000000;5.000000;,
   5.800000;5.000000;5.000000;,
   5.800000;5.000000;5.000000;,
   5.000000;5.800000;5.000000;,
   5.000000;5.800000;5.000000;,
   5.000000;5.800000;5.000000;,
   5.000000;5.800000;5.000000;,
   5.000000;5.800000;5.000000;,
   5.000000;5.000000;5.800000;,
   5.000000;5.000000;5.800000;,
   5.000000;5.000000;5.800000;,
   5.000000;5.000000;5.800000;,
   5.000000;5.000000;5.800000;,
   -5.000000;5.000000;5.800000;,
   -5.000000;5.000000;5.800000;,
   -5.000000;5.000000;5.800000;,
   -5.000000;5.000000;5.800000;,
   -5.000000;5.000000;5.800000;,
   -5.000000;5.800000;5.000000;,
   -5.000000;5.800000;5.000000;,
   -5.000000;5.800000;5.000000;,
   -5.000000;5.800000;5.000000;,
   -5.000000;5.800000;5.000000;,
   -5.800000;5.000000;5.000000;,
   -5.800000;5.000000;5.000000;,
   -5.800000;5.000000;5.000000;,
   -5.800000;5.000000;5.000000;,
   -5.800000;5.000000;5.000000;;
   48;
   3;78,88,83;,
   3;17,55,22;,
   3;26,79,30;,
   3;38,89,93;,
   3;46,103,60;,
   3;64,84,68;,
   3;5,18,23;,
   3;6,31,9;,
   3;13,34,39;,
   3;0,42,47;,
   3;19,61,51;,
   3;56,65,69;,
   3;24,73,27;,
   3;74,85,80;,
   3;32,90,35;,
   3;94,86,98;,
   3;43,95,104;,
   3;62,99,66;,
   3;1,20,48;,
   3;7,25,28;,
   3;10,33,36;,
   3;14,40,44;,
   3;52,63,67;,
   3;57,75,70;,
   3;2,11,15;,
   3;96,105,100;,
   3;58,71,76;,
   3;81,87,91;,
   3;97,101,106;,
   3;3,49,21;,
   3;2,8,11;,
   3;17,53,55;,
   3;26,77,79;,
   3;38,37,89;,
   3;46,45,103;,
   3;64,102,84;,
   3;5,4,18;,
   3;6,29,31;,
   3;13,12,34;,
   3;0,16,42;,
   3;19,50,61;,
   3;56,54,65;,
   3;24,59,73;,
   3;74,72,85;,
   3;32,82,90;,
   3;94,92,86;,
   3;43,41,95;,
   3;62,107,99;;

   MeshNormals {
    108;
    -0.707107;-0.707107;0.000000;,
    0.577350;0.577350;0.577350;,
    0.000000;-1.000000;0.000000;,
    -0.577350;-0.577350;-0.577350;,
    0.000000;-0.707107;-0.707107;,
    0.000000;-0.707107;-0.707107;,
    0.707107;-0.707107;0.000000;,
    0.577350;-0.577350;-0.577350;,
    0.000000;-1.000000;0.000000;,
    0.707107;-0.707107;0.000000;,
    0.577350;-0.577350;0.577350;,
    0.000000;-1.000000;0.000000;,
    0.000000;-0.707107;0.707107;,
    0.000000;-0.707107;0.707107;,
    -0.577350;-0.577350;0.577350;,
    0.000000;-1.000000;0.000000;,
    -0.707107;-0.707107;0.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;-0.707107;-0.707107;,
    -0.707107;0.000000;-0.707107;,
    0.577350;0.577350;0.577350;,
    -0.577350;-0.577350;-0.577350;,
    0.000000;0.000000;-1.000000;,
    0.000000;-0.707107;-0.707107;,
    0.707107;0.000000;-0.707107;,
    0.577350;-0.577350;-0.577350;,
    1.000000;0.000000;0.000000;,
    0.707107;0.000000;-0.707107;,
    0.577350;-0.577350;-0.577350;,
    0.707107;-0.707107;0.000000;,
    1.000000;0.000000;0.000000;,
    0.707107;-0.707107;0.000000;,
    0.707107;0.000000;0.707107;,
    0.577350;-0.577350;0.577350;,
    0.000000;-0.707107;0.707107;,
    0.707107;0.000000;0.707107;,
    0.577350;-0.577350;0.577350;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;-0.707107;0.707107;,
    -0.577350;-0.577350;0.577350;,
    -0.707107;0.000000;0.707107;,
    -0.707107;-0.707107;0.000000;,
    -0.707107;0.000000;0.707107;,
    -0.577350;-0.577350;0.577350;,
    -1.000000;0.000000;0.000000;,
    -1.000000;0.000000;0.000000;,
    -0.707107;-0.707107;0.000000;,
    0.577350;0.577350;0.577350;,
    -0.577350;-0.577350;-0.577350;,
    -0.707107;0.000000;-0.707107;,
    -0.707107;0.000000;-0.707107;,
    -0.577350;0.577350;-0.577350;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.707107;-0.707107;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.707107;-0.707107;,
    -0.577350;-0.577350;0.577350;,
    0.577350;0.577350;-0.577350;,
    0.707107;0.000000;-0.707107;,
    -1.000000;0.000000;0.000000;,
    -0.707107;0.000000;-0.707107;,
    -0.707107;0.707107;0.000000;,
    -0.577350;0.577350;-0.577350;,
    0.000000;1.000000;0.000000;,
    0.000000;0.707107;-0.707107;,
    -0.707107;0.707107;0.000000;,
    -0.577350;0.577350;-0.577350;,
    0.000000;1.000000;0.000000;,
    0.000000;0.707107;-0.707107;,
    -0.577350;-0.577350;0.577350;,
    0.577350;0.577350;-0.577350;,
    0.707107;0.707107;0.000000;,
    0.707107;0.000000;-0.707107;,
    0.707107;0.707107;0.000000;,
    -0.577350;-0.577350;0.577350;,
    0.577350;0.577350;-0.577350;,
    1.000000;0.000000;0.000000;,
    -0.577350;-0.577350;-0.577350;,
    1.000000;0.000000;0.000000;,
    0.707107;0.707107;0.000000;,
    0.577350;0.577350;0.577350;,
    0.707107;0.000000;0.707107;,
    -0.577350;-0.577350;-0.577350;,
    0.000000;1.000000;0.000000;,
    0.707107;0.707107;0.000000;,
    0.000000;0.707107;0.707107;,
    0.577350;0.577350;0.577350;,
    -0.577350;-0.577350;-0.577350;,
    0.000000;0.000000;1.000000;,
    0.707107;0.000000;0.707107;,
    0.577350;0.577350;0.577350;,
    0.000000;0.707107;0.707107;,
    0.000000;0.000000;1.000000;,
    0.000000;0.707107;0.707107;,
    -0.707107;0.000000;0.707107;,
    0.577350;-0.577350;-0.577350;,
    -0.577350;0.577350;0.577350;,
    0.000000;0.707107;0.707107;,
    -0.707107;0.707107;0.000000;,
    0.577350;-0.577350;-0.577350;,
    -0.577350;0.577350;0.577350;,
    0.000000;1.000000;0.000000;,
    -1.000000;0.000000;0.000000;,
    -0.707107;0.000000;0.707107;,
    0.577350;-0.577350;-0.577350;,
    -0.577350;0.577350;0.577350;,
    -0.707107;0.707107;0.000000;;
    48;
    3;78,88,83;,
    3;17,55,22;,
    3;26,79,30;,
    3;38,89,93;,
    3;46,103,60;,
    3;64,84,68;,
    3;5,18,23;,
    3;6,31,9;,
    3;13,34,39;,
    3;0,42,47;,
    3;19,61,51;,
    3;56,65,69;,
    3;24,73,27;,
    3;74,85,80;,
    3;32,90,35;,
    3;94,86,98;,
    3;43,95,104;,
    3;62,99,66;,
    3;1,20,48;,
    3;7,25,28;,
    3;10,33,36;,
    3;14,40,44;,
    3;52,63,67;,
    3;57,75,70;,
    3;2,11,15;,
    3;96,105,100;,
    3;58,71,76;,
    3;81,87,91;,
    3;97,101,106;,
    3;3,49,21;,
    3;2,8,11;,
    3;17,53,55;,
    3;26,77,79;,
    3;38,37,89;,
    3;46,45,103;,
    3;64,102,84;,
    3;5,4,18;,
    3;6,29,31;,
    3;13,12,34;,
    3;0,16,42;,
    3;19,50,61;,
    3;56,54,65;,
    3;24,59,73;,
    3;74,72,85;,
    3;32,82,90;,
    3;94,92,86;,
    3;43,41,95;,
    3;62,107,99;;
   }

   VertexDuplicationIndices {
    108;
    104;
    0,
    0,
    0,
    0,
    0,
    5,
    5,
    5,
    5,
    9,
    9,
    9,
    9,
    13,
    13,
    13,
    13,
    17,
    17,
    17,
    17,
    17,
    22,
    22,
    22,
    22,
    26,
    26,
    26,
    26,
    30,
    30,
    30,
    30,
    34,
    34,
    34,
    34,
    38,
    38,
    38,
    38,
    42,
    42,
    42,
    42,
    46,
    46,
    46,
    46,
    46,
    51,
    51,
    51,
    51,
    55,
    55,
    55,
    55,
    55,
    60,
    60,
    60,
    60,
    64,
    64,
    64,
    64,
    68,
    68,
    68,
    68,
    68,
    73,
    73,
    73,
    73,
    73,
    78,
    78,
    78,
    78,
    78,
    83,
    83,
    83,
    83,
    83,
    88,
    88,
    88,
    88,
    88,
    93,
    93,
    93,
    93,
    93,
    98,
    98,
    98,
    98,
    98,
    103,
    103,
    103,
    103,
    103;
   }

   MeshMaterialList {
    1;
    48;
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0;

    Material {
     0.678431;0.882353;0.333333;1.000000;;
     51.200001;
     0.000000;0.000000;0.000000;;
     0.000000;0.000000;0.000000;;
    }
   }
  }
 }
}