


#if _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#endif

#pragma comment(lib,"winmm.lib")
#include <windows.h>

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#include <tchar.h>
#include <d3dx9.h>
#include <d3dx9.h>

#if _DEBUG
#define new ::new(_NORMAL_BLOCK,__FILE__,__LINE__)
#endif

//参考　プログラム
//  HP    : http://marupeke296.com

//最低限の頂点情報
//座標と各ボーンの重みとボーン行列番号
//の3つがあればスキンメッシュはできる
struct Vertex
{
	D3DXVECTOR3 coord;
	D3DXVECTOR3 weight;
	unsigned char matrixIndex[4];
};

//ボーン構造体
struct Bone
{
	int id;					//ボーンid
	Bone* firstChild;		//第一子ボーン
	Bone* sibling;			//次の兄弟ボーン
	D3DXMATRIX offsetMat;	//ボーンオフセット行列
	D3DXMATRIX initMat;		//初期姿勢行列
	D3DXMATRIX boneMat;		//ボーン姿勢行列
	D3DXMATRIX *combMatAry;	//合成姿勢行列配列へのポインタ

	Bone() :id(), firstChild(), sibling(), combMatAry()
	{
		D3DXMatrixIdentity(&initMat);
		D3DXMatrixIdentity(&offsetMat);
		D3DXMatrixIdentity(&boneMat);
	}

};


int SkinMeshAppMain(LPDIRECT3DDEVICE9 g_pD3DDev);

//ウィンドウプロシージャ構造体
struct WP
{
	static LRESULT CALLBACK WndProc
	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{//特別な処理を必要とするメッセージは　とりあえず終了処理だけ書いておく
		switch (msg)
		{
		case WM_DESTROY: //右上のバツが押された、AltF4が押された時などに受信するメッセージ
			PostQuitMessage(0);//メッセージキューにWM_QUITメッセージを送る
			break;
		}
		//特別な処理を要さないメッセージはデフォルトのウィンドウプロシージャが処理する
		return DefWindowProc(hWnd, msg, wParam, lParam);//移動とかサイズ変更とか
	}
};

_TCHAR gName[100] = _T("完全ホワイトボックスなスキンメッシュアニメーションテストプログラム");


// 最低限のシェーダ
//  重み係数と頂点を動かすためのワールド変換行列の配列を渡します
const char *vertexShaderStr =
"float4x4 view : register(c0);"
"float4x4 proj : register(c4);"
"float4x4 world[12] : register(c8);"
"  "
"struct VS_IN {"
"    float3 pos : POSITION;"
"    float3 blend : BLENDWEIGHT;"
"    int4 idx : BLENDINDICES;"
"};"
"struct VS_OUT {"
"    float4 pos : POSITION;"
"};"
"VS_OUT main( VS_IN In ) {"
"    VS_OUT Out = (VS_OUT)0;"
"    float w[3] = (float[3])In.blend;"
"    float4x4 comb = (float4x4)0;"
"    for ( int i = 0; i < 3; i++ )"
"        comb += world[In.idx[i]] * w[i];"
"    comb += world[In.idx[3]] * (1.0f - w[0] - w[1] - w[2]);"
"    "
"    Out.pos = mul( float4(In.pos, 1.0f), comb );"
"    Out.pos = mul( Out.pos, view );"
"    Out.pos = mul( Out.pos, proj );"
"    return Out;"
"}";

// ピクセルシェーダは至って適当
// 好きなように点を穿って下さい。
const char *pixelShaderStr =
"struct VS_OUT {"
"    float4 pos : POSITION;"
"};"
"float4 main( VS_OUT In ) : COLOR {"
"    return float4(1.0f, 1.0f, 1.0f, 1.0f);"
"}"
"";


//メイン
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//アプリけションの初期化

	HWND hWnd;

	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(WNDCLASSEX));						// 変数のメンバを全て0で初期化
	wcex.cbSize			= sizeof(WNDCLASSEX);					// cbSize : 構造体のサイズ
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;	// style : ウィンドウスタイル
	wcex.lpfnWndProc	= (WNDPROC)WP::WndProc;					// ipfnWndProc : ウィンドウプロシージャのアドレス　関数ポインタ
	wcex.cbClsExtra		= 0;									// cbClsExtra : 0固定
	wcex.cbWndExtra		= 0;									// cbWndExtra : 0固定
	wcex.hInstance		= hInstance;							// hInstance : WinMainのインスタンスハンドル
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;

	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	/*GetStockObject(BLACK_BRUSH);*/							// hbrBackground : ウィンドウクライアント領域の背景色
	
	wcex.lpszMenuName	= NULL;									// lpszMeueName : メニュー
	wcex.lpszClassName	= (_TCHAR*)gName;						// lpszClassName : ウィンドウクラスの名前
	wcex.hIconSm		= NULL;


	if (!(RegisterClassEx(&wcex)))
	{
		return 0;
	}

	RECT rect;
	const int width = 800; 
	const int height = 600;
	SetRect(&rect, 0, 0, width, height);

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	if(!(hWnd =CreateWindow(gName,gName,WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,0,rect.right-rect.left,rect.bottom-rect.top,NULL,NULL,hInstance,NULL)))
	{
		return 0;
	}

	//Direct3D初期化
	LPDIRECT3D9 g_pD3D;
	LPDIRECT3DDEVICE9 g_pD3DDEV;

	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == false) { return 0; }

	//ディスプレイ情報取得
	D3DDISPLAYMODE Display;
	g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);


	D3DPRESENT_PARAMETERS D3DParam;
	D3DParam.BackBufferWidth = width;					//バックバッファの幅
	D3DParam.BackBufferHeight = height;					//バックバッファの高さ
	D3DParam.BackBufferFormat = Display.Format;			//バックバッファのフォーマット
	D3DParam.BackBufferCount = 1;						//バックバッファ数
	D3DParam.MultiSampleType = D3DMULTISAMPLE_NONE;		//マルチサンプリングの設定 なし
	D3DParam.MultiSampleQuality = 0;					//マルチサンプリングのクオリティ
	D3DParam.SwapEffect = D3DSWAPEFFECT_DISCARD;		//スワップチェインの方法　DirectXまかせ
	D3DParam.hDeviceWindow = hWnd;						//対象のウィンドウのハンドル
	D3DParam.Windowed = TRUE;							//ウィンドウ
	D3DParam.EnableAutoDepthStencil = TRUE;				//震度ステンシルバッファの作成
	D3DParam.AutoDepthStencilFormat = D3DFMT_D24S8;		//震度ステンシルのフォーマット
	D3DParam.Flags = 0;									//
	D3DParam.FullScreen_RefreshRateInHz = /*0*/D3DPRESENT_RATE_DEFAULT;			//スクリーンのリフレッシュレート　ウィンドウモードだと必ず0
	D3DParam.PresentationInterval =/* D3DPRESENT_INTERVAL_DEFAULT*/ D3DPRESENT_INTERVAL_ONE;	//アダプタリフレッシュレートとpresent処理を実行するレートの関係

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &D3DParam, &g_pD3DDEV)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DParam, &g_pD3DDEV)))
		{
			g_pD3D->Release();
			return 0;
		}
	}
	//ウィンドウ表示
	ShowWindow(hWnd, SW_SHOW);


	SkinMeshAppMain(g_pD3DDEV);

	g_pD3DDEV->Release();
	g_pD3D->Release();



	return 0;
}


//スキンメッシュ
int SkinMeshAppMain(LPDIRECT3DDEVICE9 g_pD3DDev)
{
	//ポリゴンの頂点定義=======================================================================
	//頂点数は17個
	Vertex vtx[17] =
	{
		{ D3DXVECTOR3( 5, 0, 1)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 2,0,0,0 } },//0
		{ D3DXVECTOR3( 4, 0, 1)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 2,0,0,0 } },//1
		{ D3DXVECTOR3( 3, 0, 1)	,D3DXVECTOR3(0.50f,0.50f,0.00f),{ 2,1,0,0 } },//2
		{ D3DXVECTOR3( 2, 0, 1)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 1,0,0,0 } },//3
		{ D3DXVECTOR3( 1, 0, 1)	,D3DXVECTOR3(0.50f,0.50f,0.00f),{ 1,0,0,0 } },//4
		{ D3DXVECTOR3( 1, 0, 0)	,D3DXVECTOR3(0.50f,0.50f,0.00f),{ 1,0,0,0 } },//5
		{ D3DXVECTOR3( 2, 0, 0)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 1,0,0,0 } },//6
		{ D3DXVECTOR3( 3, 0, 0)	,D3DXVECTOR3(0.50f,0.50f,0.00f),{ 2,1,0,0 } },//7
		{ D3DXVECTOR3( 4, 0, 0)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 2,0,0,0 } },//8
		{ D3DXVECTOR3( 5, 0, 0)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 2,0,0,0 } },//9
		{ D3DXVECTOR3( 5, 0,-1)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 2,0,0,0 } },//10
		{ D3DXVECTOR3( 4, 0,-1)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 2,0,0,0 } },//11
		{ D3DXVECTOR3( 3, 0,-1)	,D3DXVECTOR3(0.50f,0.50f,0.00f),{ 2,1,0,0 } },//12
		{ D3DXVECTOR3( 2, 0,-1)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 1,0,0,0 } },//13
		{ D3DXVECTOR3( 1, 0,-1)	,D3DXVECTOR3(0.50f,0.50f,0.00f),{ 1,0,0,0 } },//14
		{ D3DXVECTOR3(-1, 0, 0)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 0,0,0,0 } },//15
		{ D3DXVECTOR3( 0, 0, 0)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 0,0,0,0 } },//16

	};

	//頂点インデックス
	//ポリゴンは20枚で　三角形なので　20*3 60個
	WORD idx[3*20] =
	{
		 0, 1, 8,
		 1, 2, 8,
		 2, 3, 6,
		 3, 4, 6,

		 4, 5, 6,
		 2, 6, 7,
		 2, 7, 8,
		 0, 8, 9,

		 9, 8,10,
		 8, 7,12,
		 7, 6,12,
		 6, 5,14,

		14,13, 6,
		 6,13,12,
		12,11, 8,
		 8,11,10,

		 4,16, 5,
		 5,16,14,
		 4,15,16,
		16,15,14
	};

	//インデックスをたどって三角形リストを作成
	Vertex v[3*20];
	for (int i = 0; i < 60; i++)
	{
		v[i] = vtx[idx[i]];
	}

	//頂点宣言　FVF作成========================================================================
	//頂点構造体の構成情報 何バイト目からどんな情報？　
	//Declaration :　宣言
	D3DVERTEXELEMENT9 declAry[] =
	{
		{ 0,0,D3DDECLTYPE_FLOAT3 ,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0 },
		{ 0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_BLENDWEIGHT,0 },
		{ 0,24,D3DDECLTYPE_UBYTE4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_BLENDINDICES,0 },
		D3DDECL_END()
	};
	IDirect3DVertexDeclaration9 *decl = 0;
	g_pD3DDev->CreateVertexDeclaration(declAry, &decl);


	//ボーン情報の作成========================================================================
	//必要なのは　・ボーンオフセット行列　・ボーン行列

	Bone * pBones = new Bone[3];

	//ボーンの親子関係の構築
	//0 --- 1 - 2

	pBones[0].firstChild = &pBones[1];
	pBones[1].firstChild = &pBones[2];


	//初期姿勢の計算==================================================================================
	//ローカル姿勢を設定し
	//最終的に自分の親からの相対姿勢に修正
	D3DXMatrixRotationY(&pBones[0].initMat, D3DXToRadian(0.0f));
	D3DXMatrixRotationY(&pBones[1].initMat, D3DXToRadian(0.0f));
	D3DXMatrixRotationY(&pBones[2].initMat, D3DXToRadian(0.0f));
	
	//それぞれのボーンの x y座標を入力
	pBones[0].initMat._41 = 0.0000f; 	pBones[0].initMat._42 = 0.0000f; pBones[0].initMat._43 = 0.0000f;
	pBones[1].initMat._41 = 2.0000f;	pBones[1].initMat._42 = 0.0000f; pBones[1].initMat._42 = 0.0000f;
	pBones[2].initMat._41 = 4.0000f;	pBones[2].initMat._42 = 0.0000f; pBones[1].initMat._42 = 0.0000f;

	//ボーン　オフセット行列の計算
	//オフセット行列は各ボーンの「ローカル姿勢」の逆行列
	D3DXMATRIX *pCombMat = new D3DXMATRIX[7];
	for (int i = 0; i < 3; i++)
	{
		pBones[i].id = i;
		pBones[i].combMatAry = pCombMat;

		//親空間ベースに修正する前にオフセット行列を求めておく
		//ボーンと頂点の座標差分を求めるために必要

		//ボーンオフセット行列 * 初期姿勢の頂点座標 =頂点の　ボーンから見た相対座標
		D3DXMatrixInverse(&pBones[i].offsetMat, 0, &pBones[i].initMat);
	}

	//初期姿勢を親の姿勢~相対姿勢に直す
	//先ず子の末端まで降りて自分のローカル空間で初期姿勢＊親のボーンオフセット行列で相対姿勢を求める	

	//初期姿勢を親空間ベースに変換する関数の定義
	//親子関係をたどるため再帰関数が必要
	struct CalcRelativeMat
	{
		static void run(Bone* pMe, D3DXMATRIX *pParentOffsetMat = NULL)
		{
			if (pMe->firstChild)
			{
				run(pMe->firstChild, &pMe->offsetMat);
			}
			if (pMe->sibling)
			{
				run(pMe->sibling, pParentOffsetMat);
			}

			if (pParentOffsetMat)
			{
				pMe->initMat *= *pParentOffsetMat;
			}
		}
	};
	//初期姿勢を親空間ベースに変換する関数の実行
	CalcRelativeMat::run(&pBones[0]);


	/////////////////////////////////////////
	// シェーダのコンパイルとシェーダ作成
	//////
	ID3DXBuffer *shader, *error;
	IDirect3DVertexShader9 *vertexShader;
	IDirect3DPixelShader9 *pixelShader;
	HRESULT res = D3DXCompileShader(vertexShaderStr, (UINT)strlen(vertexShaderStr), 0, 0, "main", "vs_3_0", D3DXSHADER_PACKMATRIX_ROWMAJOR, &shader, &error, 0);
	if (FAILED(res)) {
		OutputDebugStringA((const char*)error->GetBufferPointer());
		return 0;
	};
	g_pD3DDev->CreateVertexShader((const DWORD*)shader->GetBufferPointer(), &vertexShader);
	shader->Release();
	res = D3DXCompileShader(pixelShaderStr, (UINT)strlen(pixelShaderStr), 0, 0, "main", "ps_3_0", D3DXSHADER_PACKMATRIX_ROWMAJOR, &shader, &error, 0);
	if (FAILED(res)) {
		OutputDebugStringA((const char*)error->GetBufferPointer());
		return 0;
	};
	g_pD3DDev->CreatePixelShader((const DWORD*)shader->GetBufferPointer(), &pixelShader);
	shader->Release();


	//===========================================================
	// 各種行列初期化
	D3DXMATRIX view, proj;
	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0.0f, -20.0f,-0.1f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(30), 64.0f / 48, 1.0f, 10000.0f);




	//========================================================

	g_pD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pD3DDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_pD3DDev->SetRenderState(D3DRS_ZENABLE, TRUE);


	//d3d.SetupRrojectionMatrix();

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 0.0f;
	light.Diffuse.b = 0.0f;
	light.Direction = D3DXVECTOR3(-0.5f, -1.0f, 0.5f);
	light.Range = 1000.0f;

	g_pD3DDev->SetLight(0, &light);

	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 0.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 0.0f;
	light.Direction = D3DXVECTOR3(0.5f, -1.0f, 0.5f);
	light.Range = 1000.0f;

	g_pD3DDev->SetLight(1, &light);

	g_pD3DDev->LightEnable(0, TRUE);
	g_pD3DDev->LightEnable(1, TRUE);

	g_pD3DDev->SetRenderState(D3DRS_AMBIENT, 0x00444444);

	//描画ループ==========================================
	//毎フレームの姿勢制御
	MSG msg;

	float val = 0.0f;
	float a[7] = { 0 };


	do
	{
		val += 0.03f;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}

		g_pD3DDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(40, 40, 80), 1.0f, 0);
		g_pD3DDev->BeginScene();

		//ボーンの姿勢を更新
		//最終的には
		//[ボーンオフセット行列] * [ワールド空間でのボーン姿勢]
		//を計算する

		//各ボーンの初期姿勢からの差分姿勢(親空間ベース)を更新
		D3DXMATRIX defBone[3];
		D3DXMatrixIdentity(&defBone[0]);

		//ボーンの回転を適当に動かす
		for (int i = 1; i < 3; i++)
		{
			D3DXMATRIX transLation;
			D3DXMATRIX rotate;
			D3DXMATRIX scale;
			D3DXMatrixIdentity(&scale);
			D3DXMatrixIdentity(&transLation);
			D3DXMatrixIdentity(&rotate);
			D3DXMatrixIdentity(&defBone[i]);

			//
			//行列のかける順番はこれで確定っぽい
			//

			D3DXMatrixScaling(&scale, 1.0f, 1.0f, 1.0f);
			D3DXMatrixTranslation(&transLation,2.0f*(i-1), 0.0f, 0.0f );
			D3DXMatrixRotationY(&rotate, D3DXToRadian(sinf(val*0.1f))*70.0f);
			D3DXMatrixMultiply(&defBone[i], &scale, &rotate);
			D3DXMatrixMultiply(&defBone[i], &defBone[i], &transLation);

		}


		//各ボーン行列の親空間ベースでの姿勢を更新
		//基本姿勢* 初期姿勢(ともに親空間ベース)
		for (int i = 0; i < 3; i++)
		{
			pBones[i].boneMat = defBone[i] * pBones[i].initMat;
		}


		//親空間ベースにアqる各ボーン行列をローカル空間ベースの姿勢に変換
		//ここは親子関係にしたがって行列をかける必要がある
		//かける順番は子*親
		D3DXMATRIX global;
		D3DXMATRIX tmp;
		D3DXMatrixIdentity(&global);		
		D3DXMatrixIdentity(&tmp);
		D3DXMatrixRotationY(&global,D3DXToRadian(val*1.5f)*0.0f);
		D3DXMatrixTranslation(&tmp, -5.5f-(sinf(val*0.0f)), 0, 0);

		D3DXMatrixMultiply(&global, &global, &tmp);


		struct UpdateBone
		{
			static void run(Bone* pMe, D3DXMATRIX*pParentWorldMat)
			{
				pMe->boneMat *= *pParentWorldMat;
				pMe->combMatAry[pMe->id] = pMe->offsetMat * pMe->boneMat;

				if (pMe->firstChild)
				{
					run(pMe->firstChild, &pMe->boneMat);
				}
				if (pMe->sibling)
				{
					run(pMe->sibling, pParentWorldMat);
				}
			}
		};

		UpdateBone::run(pBones, &global);


		// シェーダ設定
		// 変数を書き込むレジスタ位置はシェーダに書いてありますよ。
		g_pD3DDev->SetVertexShader(vertexShader);
		g_pD3DDev->SetPixelShader(pixelShader);
		g_pD3DDev->SetVertexShaderConstantF(0, (const float*)&view, 4);
		g_pD3DDev->SetVertexShaderConstantF(4, (const float*)&proj, 4);
		g_pD3DDev->SetVertexShaderConstantF(8, (const float*)pCombMat, 4 * 3);

		g_pD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		g_pD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		g_pD3DDev->SetVertexDeclaration(decl);
		g_pD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 20, &v, sizeof(Vertex));



		g_pD3DDev->EndScene();
		g_pD3DDev->Present(NULL, NULL, NULL, NULL);

	} while (msg.message != WM_QUIT);

	/*for (int i = 0; i < ; i++)
		boneObj[i]->Release();*/
	vertexShader->Release();
	pixelShader->Release();
	decl->Release();


	delete[]pCombMat;
	delete[]pBones;


	return 0;

}