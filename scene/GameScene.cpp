﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() 
{ 
	delete model_;
	delete sprite_;
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

	//スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	// X,Y,Z方向のスケーリングを設定
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	// X, Y, Z軸周りの回転角を設定 XMConvertToRadians()
	worldTransform_.rotation_ = {0.f,0.f , 0.0f};

	// X, Y, Z軸周りの平行移動を設定
	worldTransform_.translation_ = {0.0f, 0.f, 0.0f};

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {
	//スプライトの今の座標を取得
	XMFLOAT2 position = sprite_->GetPosition();
	//座標を{2.1}移動
	position.x += 2.0f;
	position.y += 1.0f;
	//移動した座標をスプライトに反映
	sprite_->SetPosition(position);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

//#pragma region 3Dオブジェクト描画
//	 3Dオブジェクト描画前処理
//	Model::PreDraw(commandList);
//
//	 <summary>
//	 ここに3Dオブジェクトの描画処理を追加できる
//	 </summary>
//	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
//
//	 3Dオブジェクト描画後処理
//	Model::PostDraw();
//#pragma endregion

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
