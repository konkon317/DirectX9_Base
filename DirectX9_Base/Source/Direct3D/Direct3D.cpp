#include "direct3d.h"
#include "texture.h"
#include "sprite.h"
#include "mesh.h"

//コンストラクタ
Direct3D::Direct3D()
{
	pD3D9 = NULL;
	pDevice3D = NULL;
	isDeviceCreated = false;
	DrawFunc = NULL;
}

void Direct3D::ReleaseDevice()
{
	if (isDeviceCreated)
	{
		pDevice3D->Release();
		pD3D9->Release();

		pDevice3D = NULL;
		pD3D9 = NULL;

		isDeviceCreated = false;
	}
}

//デストラクタ
Direct3D::~Direct3D()
{
	ReleaseDevice();	
}

bool Direct3D::TryCreate(HWND hWnd)
{
	if (isDeviceCreated == false)//作られていない場合のみ
	{
		isDeviceCreated = Create(hWnd);
	}

	return isDeviceCreated;
}

bool Direct3D::Create(HWND hWnd)
{
	RECT rec;
	GetClientRect(hWnd,&rec);
	int Width=rec.right-rec.left;
	int Height=rec.bottom-rec.top;

	//Direct3D9オブジェクトの作成
	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	//ディスプレイ情報取得
	D3DDISPLAYMODE Display;
	pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);

	//スワップチェイン設定
	//スワップチェイン　バックバッファをフロントバッファに書きかえる
	//DISCARDはスワップチェインの方法をDIrectXに任せるという設定
	D3DPRESENT_PARAMETERS D3DParam;
	D3DParam.BackBufferWidth = Width;					//バックバッファの幅
	D3DParam.BackBufferHeight = Height;					//バックバッファの高さ
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
	D3DParam.FullScreen_RefreshRateInHz = 0;			//スクリーンのリフレッシュレート　ウィンドウモードだと必ず0
	D3DParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//アダプタリフレッシュレートとpresent処理を実行するレートの関係


	//幾つかの設定でデバイス作成を試みる
	//HALモードで3Dデバイス作成
	if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DParam.hDeviceWindow,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
	{
		if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DParam.hDeviceWindow,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
		{
			//Refモードで3Dデバイス作成
			if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, D3DParam.hDeviceWindow,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
			{
				if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, D3DParam.hDeviceWindow,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
				{
					// 3Dデバイス作成失敗
					pD3D9->Release();
					return false;
				}
			}
		}
	}
	return true;//どれかで作成成功すればtrueが返る

	/*HRESULT CreateDevice
		(UINT Adapter,			ディスプレイ アダプタを示す序数
		D3DDEVTYPE DeviceType,	目的のデバイス タイプ
		HWND hFocusWindow,		デバイスでフォーカスを設定するウィンドウのハンドル
		DWORD BehaviorFlags,	デバイス作成のオプション
		D3DPRESENT_PARAMETERS *pPresentationParameters,
		IDirect3DDevice9** ppReturnedDeviceInterface
		);*/




}

//描画の反映時に実行
void Direct3D::Render()
{
	if (Singleton<Direct3D>::IsInstatnceCreated())
	{
		Direct3D & d3d = Direct3D::GetInstance();

		if (d3d.IsDeviceCreated())
		{
			if (SUCCEEDED(d3d.pDevice3D->BeginScene()))
			{
				DWORD ClearColor =/* (pathed) ? 0x11111111 :*/ 0xFFFF0000;//背景クリア色

				//背景クリア
				d3d.pDevice3D->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, ClearColor, 1.0f, 0);

				//ここに関数ポインタで描画処理を行う様にする
				d3d.TryCallDrawFunc();

				d3d.pDevice3D->EndScene();

				d3d.pDevice3D->Present(NULL, NULL, NULL, NULL);
			}
		}
		else
		{
			MessageBox(NULL, "デバイスが作成されていないので実行できません", TEXT("Direct3D Render"), MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, "インスタンスが作成されていないので実行できません", TEXT("Direct3D Render"), MB_OK);
	}
}

//ブレンドモードの変更
void Direct3D::SetRenderState(RENDERSTATE RenderState)
{
	if (Singleton<Direct3D>::IsInstatnceCreated())
	{
		Direct3D & d3d = Direct3D::GetInstance();

		if (d3d.IsDeviceCreated())
		{
			switch (RenderState)
			{
			case RENDER_DEFAULT:
			{
				d3d.pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, false);
				d3d.pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, false);	//αブレンドの無効化
			}
			break;

			case RENDER_ALPHATEST:
			{
				//αテストによる透明領域の切り抜き
				d3d.pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			//αテストの有効化
				d3d.pDevice3D->SetRenderState(D3DRS_ALPHAREF, 0x80);					// アルファ参照値
				d3d.pDevice3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	//αテスト合格基準

				d3d.pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);			//αブレンドの無効化
			}
			break;

			case RENDER_HALFADD:
			{
				//半加算
				d3d.pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	//αテストの無効化
				d3d.pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//αブレンディングの有効化
				d3d.pDevice3D->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//Zバッファを行わない

				//pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);			//zテストを行わない
				//pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		//ライティングを行わない

				d3d.pDevice3D->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		//ブレンディングオプション加算
				d3d.pDevice3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	//SRCの設定
				d3d.pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		//DESTの設定

			}
			break;

			case RENDER_ADD:
			{
				//全加算合成
				d3d.pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//αブレンディングの有効化
				d3d.pDevice3D->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);	//ブレンディングオプション加算
				d3d.pDevice3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);	//SRCの設定
				d3d.pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);	//DESTの設定
				d3d.pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	//αテストの無効化

			}
			break;

			case RENDER_MESH:
			{
				d3d.pDevice3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
				d3d.pDevice3D->SetRenderState(D3DRS_LIGHTING, TRUE);
				d3d.pDevice3D->SetRenderState(D3DRS_ZENABLE, TRUE);


				d3d.SetupRrojectionMatrix();

				

				D3DLIGHT9 light;
				ZeroMemory(&light, sizeof(D3DLIGHT9));
				light.Type = D3DLIGHT_DIRECTIONAL;
				light.Diffuse.r = 1.0f;
				light.Diffuse.g = 0.0f;
				light.Diffuse.b = 0.0f;
				light.Direction = D3DXVECTOR3(-0.5f, -1.0f, 0.5f);
				light.Range = 1000.0f;

				d3d.pDevice3D->SetLight(0, &light);

				ZeroMemory(&light, sizeof(D3DLIGHT9));
				light.Type = D3DLIGHT_DIRECTIONAL;
				light.Diffuse.r = 0.0f;
				light.Diffuse.g = 1.0f;
				light.Diffuse.b = 0.0f;
				light.Direction = D3DXVECTOR3(0.5f, -1.0f, 0.5f);
				light.Range = 1000.0f;

				d3d.pDevice3D->SetLight(1, &light);

				d3d.pDevice3D->LightEnable(0, TRUE);
				d3d.pDevice3D->LightEnable(1, TRUE);

				d3d.pDevice3D->SetRenderState(D3DRS_AMBIENT, 0x00444444);
			}
			break;
			

			}
		}
		else
		{
			MessageBox(NULL, "デバイスが作成されていないので実行できません", TEXT("Direct3D SetRenderState"), MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, "インスタンスが作成されていないので実行できません", TEXT("Direct3D SetRenderState"), MB_OK);
	}
}

bool Direct3D::LoadTexture(Texture& texture,TCHAR* FileName)
{
	//画像読み込み
	//DirectXやWindowsAPIの関数はHRESULTを結果に返す関数が多い
	//FAILEDマクロで失敗したかの判断
	//SUCEEDEDマクロで関数が成功したかの判断
	if (IsDeviceCreated())
	{
		if (FAILED(D3DXCreateTextureFromFile(pDevice3D, FileName, &texture.pTexture)))
		{
			//読み込み失敗（ファイルが無い可能性が高い）
			return false;
		}

		//読み込み成功
		return true;
	}
	else
	{
		return false;
	}
}

//スプライトの表示
void Direct3D::DrawSprite(Sprite& sprite, Texture& texture, bool isTurn)
{
	Direct3D& d3d = Direct3D::GetInstance();

	//頂点の位置
	SpriteVertex vtx[4]
	{
		//{x,y,z, 2D変換済みフラグ , u,v}
		//uv座標はテクスチャのどの角を表示するか (0,0)左上	(1,0)右上	(0,1)左下	(1,1)右下	(0.5,0.5 )テクスチャの中心
		//その角にテクスチャの何パーセントの位置が来るかを指定

		//
		//右上
		{ (float)sprite.width / 2, -(float)sprite.height / 2, 0.0f, 1.0f,
			(isTurn ? static_cast<float>(texture.numU) / texture.divU : static_cast<float>(texture.numU + 1) / texture.divU), static_cast<float>(texture.numV) / texture.divV
		},
		//右下
		{ (float)sprite.width / 2, (float)sprite.height / 2, 0.0f, 1.0f,
		(isTurn ? static_cast<float>(texture.numU) / texture.divU : static_cast<float>(texture.numU + 1) / texture.divU), static_cast<float>(texture.numV + 1) / texture.divV
		},
		//左上
		{ -(float)sprite.width / 2, -(float)sprite.height / 2, 0.0f, 1.0f,
		(isTurn ? static_cast<float>(texture.numU + 1) / texture.divU : static_cast<float>(texture.numU) / texture.divU), static_cast<float>(texture.numV) / texture.divV
		},
		//左下
		{ -(float)sprite.width / 2, (float)sprite.height / 2, 0.0f, 1.0f,
		(isTurn ? static_cast<float>(texture.numU + 1) / texture.divU : static_cast<float>(texture.numU) / texture.divU), static_cast<float>(texture.numV + 1) / texture.divV
		},

	};

	for (int i = 0; i < 4; i++)
	{
		//回転
		float x = vtx[i].x*cosf(sprite.rotate) - vtx[i].y*sinf(sprite.rotate);
		float y = vtx[i].x*sinf(sprite.rotate) + vtx[i].y*cosf(sprite.rotate);

		//平行移動
		vtx[i].x = x + sprite.pos.x;
		vtx[i].y = y + sprite.pos.y;

	}

	//テクスチャのセット
	d3d.pDevice3D->SetTexture(0, texture.pTexture);
	//頂点構造体宣言をセット
	d3d.pDevice3D->SetFVF(Sprite::SPRITE_FVF);
	//スプライト描画
	//TRIANGLESTRIPで頂点を描画する 2はprimitiveCountポリゴン数
	if (SUCCEEDED(d3d.pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx, sizeof(SpriteVertex))))
	{
		int a = 0;
	}
	else

	{
		int a = 0;
	}
}

void Direct3D::SetDrawFunc(FuncPointer pointer)
{
	DrawFunc = pointer;
}

void Direct3D::TryCallDrawFunc()
{
	if (DrawFunc != NULL)
	{	
		
		DrawFunc();
		
	
	}
	else
	{
		int a = 0;
	}
}

void Direct3D::SetViewMatrix(D3DXMATRIXA16& mat)
{
	pDevice3D->SetTransform(D3DTS_VIEW, &mat);
}

void Direct3D::LoadMesh(Mesh& mesh,TCHAR* path)
{
	////LPSTR からLPCWSTRに変換
	////https://msdn.microsoft.com/ja-jp/library/ms235631(VS.80).aspx
	//LPSTR temp = path;
	//size_t origsize = strlen(temp)+1;
	//size_t convertedChars = 0;
	//wchar_t wcstring[1024];
	//ZeroMemory(wcstring, sizeof(wcstring));
	//mbstowcs_s(&convertedChars, wcstring, origsize, temp, _TRUNCATE);
	//
	////	wcscat_s(wcstring, L"(wchar_t *)");
	//LPCWSTR path2 = wcstring;


	//Xファイルのパスを取得
	CHAR dir[_MAX_DIR];
	_splitpath_s(path, NULL, 0, dir, _MAX_DIR, NULL, 0, NULL, 0);

	LPD3DXBUFFER pBufferMaterial;

	if (D3DXLoadMeshFromX(path, D3DXMESH_SYSTEMMEM, pDevice3D, NULL, &pBufferMaterial, NULL, &mesh.numMaterials, &mesh.pMesh) != D3D_OK)
	{
		return;
	}

	//マテリアルの準備
	if (mesh.numMaterials > 0)
	{
		mesh.pMaterials = new D3DMATERIAL9[mesh.numMaterials];
		mesh.ppTextures = new LPDIRECT3DTEXTURE9[mesh.numMaterials];

		D3DXMATERIAL * d3dxMaterials = (D3DXMATERIAL*)pBufferMaterial->GetBufferPointer();

		for (int i = 0; i < mesh.numMaterials; i++)
		{
			//夫々のマテリアルをバッファからコピーする
			mesh.pMaterials[i] = d3dxMaterials[i].MatD3D;
			mesh.pMaterials[i].Ambient = mesh.pMaterials[i].Diffuse;

			mesh.ppTextures[i] = NULL;

			//テクスチャのファイル名を取り出してロード
			if (d3dxMaterials[i].pTextureFilename != NULL)
			{			

				//テクスチャファイルパスを作成する
				CHAR texturefile[1024];
				ZeroMemory(texturefile, sizeof(texturefile));
				lstrcat(texturefile, dir);
				lstrcat(texturefile, d3dxMaterials[i].pTextureFilename);

				if (D3DXCreateTextureFromFile(pDevice3D,texturefile, &mesh.ppTextures[i])!=D3D_OK)
				{
					mesh.ppTextures[i] = NULL;
				}

			}

		}
	}

	pBufferMaterial->Release();

}

void Direct3D::DrawMesh(Mesh& mesh, D3DXMATRIXA16& worldMat)
{
	if (mesh.pMesh != NULL)
	{
		pDevice3D->SetTransform(D3DTS_WORLD, &worldMat);

		//頂点シェーダ
		pDevice3D->SetVertexShader(NULL);

		//頂点フォーマット
		pDevice3D->SetFVF(mesh.pMesh->GetFVF());

		if (mesh.numMaterials > 0)
		{
			for (unsigned int i = 0; i < mesh.numMaterials; i++)
			{

				pDevice3D->SetMaterial(&mesh.pMaterials[i]);
				pDevice3D->SetTexture(0, mesh.ppTextures[i]);
				mesh.pMesh->DrawSubset(i);
			}
		}
		else
		{
			//マテリアルが無かった場合は（そんなケースまずないが）
			//適当に作ったマテリアルで表示
			D3DMATERIAL9 mtrl;
			ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
			mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
			mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
			mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
			mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
			pDevice3D->SetMaterial(&mtrl);

			mesh.pMesh->DrawSubset(0);
		}
	}
}

void Direct3D::SetupRrojectionMatrix()
{
	D3DXMATRIXA16 matProj;

	D3DXMatrixPerspectiveFovLH(&matProj, 3.0f / 4.0f, 1.0f, 1.0f, 100.0f);
	pDevice3D->SetTransform(D3DTS_PROJECTION, &matProj);
}
