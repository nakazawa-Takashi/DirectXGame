#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete debugCamera_;
}

Vector3 pos = {0, 0, 0};
Vector3 posend = {100, 100, 100};
Vector4 color = {0x000000f};


void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	/// ファイル名を指定してテクスチャを読み込む	
	textureHandle_ = TextureManager::Load("./Resources./sonic.png");
	//スプライトの作成
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	//3Dモデルの作成
	model_ = Model::Create();
	//ワールドトランスフォームの初期化
	worldtransform_.Initialize();
	//ビュープロジェクションの初期化 
	viewprojection_.Initialize();
	//サウンドデータの読み込み
	soundDataHandle_ = audio_->LoadWave("./Resources./fanfare.wav");
	//音声再生
	audio_->PlayWave(soundDataHandle_);
	//音声再生
	voiceHandole_ = audio_->PlayWave(soundDataHandle_, true);
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewprojection_);
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	
}

void GameScene::Update() {
	Vector2 position = sprite_->GetPosition();
//座標を｛２，１｝移動
	position.x += 2.0f;
	position.y += 1.0f;
	//移動した座標をスプライトに反映
	sprite_->SetPosition(position);
	//スペースキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE)) {
	//音声停止
		audio_->StopWave(voiceHandole_);
	}

	//デバッグテキストの表示
	ImGui::Text("Nakazawa Takashi %d.%d.%d", 2050, 12, 31);

	ImGui::Begin("Debug1");
	ImGui::Text("Nakazawa Takashi %d.%d.%d", 2050, 12, 31);
	ImGui::End();

	ImGui::Begin("Debug1");
	//float3入力ボックス
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	//float3スライダー
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::End();
	//デモウィンドウの表示を有効化
	ImGui::ShowDemoWindow();
	//デバッグカメラの更新
	debugCamera_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	//ラインを描画する
	PrimitiveDrawer::GetInstance()->DrawLine3d(pos, posend, color);

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
	model_->Draw(worldtransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

	

#pragma endregion
}
