#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() 
{ 
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	////ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	//3Dモデルの生成
	model_ = Model::Create();

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			// X,Y,Z方向のスケーリングを設定
			//i,j共に偶数の場合大きさを0にする
			if (i == 0 || j == 0||i % 2 == 0 && j % 2 == 0) {
				worldTransform_[i][j].scale_ = {0.0f, 0.0f, 0.0f};
			} else {
				worldTransform_[i][j].scale_ = {1.0f, 1.0f, 1.0f};
			}
			

			// X, Y, Z軸周りの回転角を設定 XMConvertToRadians()
			worldTransform_[i][j].rotation_ = {0.0f, 0.0f, 0.0f};

			// X, Y, Z軸周りの平行移動を設定
			worldTransform_[i][j].translation_ = {-20.0f, 20.f, 10.0f};

			// 座標を4ずつずらす
			worldTransform_[i][j].translation_.x += i * 4;
			worldTransform_[i][j].translation_.y -= j * 4;
		}
	}

	//ワールドトランスフォームの初期化
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			worldTransform_[i][j].Initialize();
		}
	}
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() { 

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			model_->Draw(worldTransform_[i][j], viewProjection_, textureHandle_);
		}
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
