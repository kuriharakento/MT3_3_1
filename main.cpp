#include <imgui.h>
#include <Novice.h>
#include "MyFunctions.h"

const char kWindowTitle[] = "LC1A_13_クリハラ_ケント_タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 translates[3] =
	{
		{0.2f,1.0f,0.0f},
		{0.4f,0.0f,0.0f},
		{0.3f,0.0f,0.0f},
	};

	Vector3 rotates[3] =
	{
		{0.0f,0.0f,-6.8f},
		{0.0f,0.0f,-1.4f},
		{0.0f,0.0f,0.0f},
	};

	Vector3 scales[3] =
	{
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
	};

	Sphere sphere[3] =
	{
		{{0,0,0},0.1f},
		{{0,0,0},0.1f},
		{{0,0,0},0.1f},
	};

	uint32_t color[3] =
	{
		RED,
		GREEN,
		BLUE,
	};

	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	float cameraSpeed = 0.01f;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		if (keys[DIK_W])
		{
			cameraTranslate.z += cameraSpeed;
		}
		if (keys[DIK_S])
		{
			cameraTranslate.z -= cameraSpeed;
		}
		if (keys[DIK_A])
		{
			cameraTranslate.x -= cameraSpeed;
		}
		if (keys[DIK_D])
		{
			cameraTranslate.x += cameraSpeed;
		}
		if (keys[DIK_Q])
		{
			cameraTranslate.y += cameraSpeed;
		}
		if (keys[DIK_E])
		{
			cameraTranslate.y -= cameraSpeed;
		}

		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, (float)kWindowWidth / (float)kWindowHeight, 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, (float)kWindowWidth, (float)kWindowHeight, 0.0f, 1.0f);


		Matrix4x4 worldMatrix[3], localMatrix[3];
		Vector3 linePoint[3];

		for (int i = 0; i < 3; ++i)
		{
			localMatrix[i] = MakeAffineMatrix(scales[i], rotates[i], translates[i]);
		}

		worldMatrix[0] = localMatrix[0];
		worldMatrix[1] = Multiply(localMatrix[1], localMatrix[0]);
		worldMatrix[2] = Multiply(Multiply(localMatrix[2], localMatrix[1]), localMatrix[0]);

		for (int i = 0; i < 3; ++i)
		{
			sphere[i].center = GetWorldPosition(worldMatrix[i]);

			linePoint[i] = Transform(Transform(sphere[i].center, viewProjectionMatrix), viewportMatrix);
		}

		ImGui::Begin("window");
		ImGui::Text("camera");
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::Text("setting");

		ImGui::DragFloat3("translates[0]", &translates[0].x, 0.01f);
		ImGui::DragFloat3("rotates[0]", &rotates[0].x, 0.01f);
		ImGui::DragFloat3("scales[0]", &scales[0].x, 0.01f);

		ImGui::DragFloat3("translates[1]", &translates[1].x, 0.01f);
		ImGui::DragFloat3("rotates[1]", &rotates[1].x, 0.01f);
		ImGui::DragFloat3("scales[1]", &scales[1].x, 0.01f);

		ImGui::DragFloat3("translates[2]", &translates[2].x, 0.01f);
		ImGui::DragFloat3("rotates[2]", &rotates[2].x, 0.01f);
		ImGui::DragFloat3("scales[2]", &scales[2].x, 0.01f);

		ImGui::End();


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);
		for (int i = 0; i < 3; ++i)
		{
			DrawSphere(sphere[i], viewProjectionMatrix, viewportMatrix, color[i]);
		}

		for (int i = 0; i < 2; ++i)
		{
			Novice::DrawLine((int)linePoint[i].x, (int)linePoint[i].y, (int)linePoint[i + 1].x, (int)linePoint[i + 1].y, WHITE);
		}

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
