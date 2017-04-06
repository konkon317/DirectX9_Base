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

	g_pD3DDEV->Release();
	g_pD3D->Release();



	return 0;
}


//スキンメッシュ
int SkinMeshAppMain(LPDIRECT3DDEVICE9 g_pD3DDev)
{
	//ポリゴンの頂点定義
	//頂点数は15個
	Vertex vtx[15]=
	{
		{ D3DXVECTOR3(-0.5000f,-2.2887f,0.0f),D3DXVECTOR3(1.00f,0.00f,0.00f),{ 2,0,0,0 } },//0
		{ D3DXVECTOR3(-0.5000f,-1.2887f,0.0f),D3DXVECTOR3(0.50f,0.50f,0.00f),{ 1,2,0,0 } },//1
		{ D3DXVECTOR3(-0.5000f,-0.2887f,0.0f),D3DXVECTOR3(1.00f,0.00f,0.00f),{ 0,0,0,0 } },//2
		{ D3DXVECTOR3(-1.3660f, 0.2113f,0.0f),D3DXVECTOR3(0.50f,0.50f,0.00f),{ 3,4,0,0 } },//3
		{ D3DXVECTOR3(-2.2321f, 0.7113f,0.0f),D3DXVECTOR3(1.00f,0.00f,0.00f),{ 4,0,0,0 } },//4
		{ D3DXVECTOR3(-1.7321f, 1.5774f,0.0f),D3DXVECTOR3(1.00f,0.00f,0.00f),{ 4,0,0,0 } },//5
		{ D3DXVECTOR3(-0.8660f, 1.0774f,0.0f),D3DXVECTOR3(0.50f,0.50f,0.00f),{ 3,4,0,0 } },//6
		{ D3DXVECTOR3( 0.0000f, 0.5774f,0.0f),D3DXVECTOR3(1.00f,0.00f,0.00f),{ 0,0,0,0 } },//7
		{ D3DXVECTOR3( 0.8660f, 1.0774f,0.0f),D3DXVECTOR3(0.50f,0.50f,0.00f),{ 5,6,0,0 } },//8
		{ D3DXVECTOR3( 1.7321f, 1.5774f,0.0f),D3DXVECTOR3(1.00f,0.00f,0.00f),{ 6,0,0,0 } },//9
		{ D3DXVECTOR3( 2.2321f, 0.7113f,0.0f),D3DXVECTOR3(1.00f,0.00f,0.00f),{ 6,0,0,0 } },//10
		{ D3DXVECTOR3( 1.3660f, 0.2113f,0.0f),D3DXVECTOR3(0.50f,0.50f,0.00f),{ 5,6,0,0 } },//11
		{ D3DXVECTOR3( 0.5000f,-2.2887f,0.0f),D3DXVECTOR3(1.00f,0.00f,0.00f),{ 0,0,0,0 } },//12
		{ D3DXVECTOR3( 0.5000f,-1.2887f,0.0f),D3DXVECTOR3(0.50f,0.50f,0.00f),{ 1,2,0,0 } },//13
		{ D3DXVECTOR3( 0.5000f,-2.2887f,0.0f),D3DXVECTOR3(1.00f,0.00f,0.00f),{ 2,0,0,0 } } //14
	};

	//頂点インデックス
	//ポリゴンは13枚で　三角形なので　13*3 39個
	WORD idx[39] =
	{
		0,1,14,
		1,13,14,
		1,2,13,
		2,12,13,
		2,7,12,
		2,6,7,
		2,3,6,
		3,5,6,
		3,4,5,
		7,8,12,
		8,11,12,
		8,9,11,
		9,10,11,
	};

	//インデックスをたどって三角形リストを作成
	Vertex v[39];
	for (int i = 0; i < 39; i++)
	{
		v[i] = vtx[idx[i]];
	}

	//頂点宣言　FVF作成 
	//頂点構造体の構成情報　
	D3DVERTEXELEMENT9 declAry[]=
	{
		{0,0,D3DDECLTYPE_FLOAT3 ,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},
		{0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_BLENDWEIGHT,0},
		{0,24,D3DDECLTYPE_UBYTE4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_BLENDINDICES,0},
		D3DDECL_END()	
	};
	IDirect3DVertexDeclaration9 *decl = 0;
	g_pD3DDev->CreateVertexDeclaration(declAry, &decl);


	return 0;

}