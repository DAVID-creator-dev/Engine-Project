#include "Core/UI/Windows/ContentBrowserUI.h"
#include "Core/Engine.h"

#include "GameBehaviour/ScriptsManager.h"
#include "GameBehaviour/GameStateManager.h"

#include "Resources/ResourceManager.h"

#include <imgui.h>
#include <imgui_internal.h>

#ifdef CreateFile
#undef CreateFile
#endif

#ifdef DeleteFile
#undef DeleteFile
#endif

#include "Core/UI/Windows/HierarchyUI.h"
#include "Utils/FileUtils.h"

ContentBrowserUI::ContentBrowserUI(HierarchyUI* _hierarchy) : hierarchy(_hierarchy)  
{
	SetRootPath("Assets");

	scriptManager = Engine::GetInstance()->GetScriptsManager(); 
	resourceManager = Engine::GetInstance()->GetResourceManager();
	stateManager = Engine::GetInstance()->GetGameStateManager(); 
}

void ContentBrowserUI::SetRootPath(const std::string& rootPath)
{
	currentPath = rootPath;
	while (!pathStack.empty()) pathStack.pop();
}

void ContentBrowserUI::Render()
{
	ImGui::Begin("Content Browser", nullptr, ImGuiWindowFlags_NoCollapse);

	if (ImGui::Button("Back") && !pathStack.empty()) {
		currentPath = pathStack.top();
		pathStack.pop();
	}

	ContextContentBrowser();

	DisplayResources(currentPath);

	ImportResources(currentPath);

	ImGui::End();
}

void ContentBrowserUI::DisplayResources(const std::string& path)
{
	float windowWidth = ImGui::GetWindowSize().x;
	int columnCount = static_cast<int>(windowWidth / cellSize);
	ImGui::Columns(columnCount, nullptr, false);

    std::vector<std::string> directories = FileUtils::ListSubdirectories(path);
	DisplayDirectories(directories, path);

	std::vector<std::string> files = FileUtils::ListFiles(path, true);
	DisplayFiles(files);
}

void ContentBrowserUI::ImportResources(std::string currentPath)
{
	for (const auto& src : m_DroppedFiles)
	{
		std::filesystem::path sourcePath(src);
		std::string filename = sourcePath.filename().string();
		std::string destination = currentPath + "/" + filename;

		try
		{
			std::filesystem::copy(sourcePath, destination, std::filesystem::copy_options::overwrite_existing);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error copy: " << e.what() << std::endl;
		}

		if (filename.ends_with(".png") || filename.ends_with(".jpg"))
		{
			resourceManager->CreateResource<ITexture>(destination.c_str());
		}
		else if (filename.ends_with(".obj"))
		{
			resourceManager->CreateResource<IModel>(destination.c_str());
		}
	}

	m_DroppedFiles.clear();
}

void ContentBrowserUI::DropFile(const std::string& path)
{
	m_DroppedFiles.push_back(path);
}

void ContentBrowserUI::DropAsset()
{
	ImVec2 winPos = ImGui::GetWindowPos();
	ImVec2 winSize = ImGui::GetWindowSize();

	float x = winPos.x + winSize.x;
	float y = winPos.y + winSize.y;
	ImVec2 size(x, y); 

	ImRect dropZone(winPos, size);

	if (ImGui::BeginDragDropTargetCustom(dropZone, ImGui::GetID("CustomDropZone")))
	{
		std::string payloadNames[] = { "obj", "dae" };

		for (const std::string& payloadName : payloadNames) 
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payloadName.c_str()))
			{
				const char* droppedPath = static_cast<const char*>(payload->Data);
				std::string pathStr = droppedPath;
				if (pathStr.ends_with(".obj"))
				{
					IModel* model = resourceManager->Get<IModel>(droppedPath);
					stateManager->GetCurrentScene()->CreateGameObject("New GameObject", model, resourceManager->Get<IMaterial>("default.mat"), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(1.0f, 1.0f, 1.0f));
				}
			}
		}

		ImGui::EndDragDropTarget();
	}
}

void ContentBrowserUI::DisplayDirectories(const std::vector<std::string>& directories, const std::string& currentPath)
{
	for (const auto& dir : directories)
	{
		ImGui::BeginGroup();
		ImGui::PushID(dir.c_str());
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

		bool clicked = false; 
		ITexture* tex = resourceManager->Get<ITexture>("folder.png");
		clicked = BouttonResource(FileUtils::FileName(dir), tex);

		if (clicked) 
		{
			pathStack.push(currentPath);
			this->currentPath = currentPath + "/" + dir;
		}

		ImGui::PopStyleColor();
		ImGui::PopID();

		ImGui::EndGroup();
		ImGui::NextColumn();
	}
}

bool ContentBrowserUI::BouttonResource(const std::string& file, ITexture* tex, bool enableDrag) const
{
	const float buttonSize = thumbnailSize;
	const float imageScale = 0.8f;
	const float imageSize = buttonSize * imageScale;

	bool clicked = ImGui::Button("", ImVec2(buttonSize, buttonSize));

	ImVec2 min = ImGui::GetItemRectMin();
	ImVec2 size = ImGui::GetItemRectSize();

	ImVec2 imagePos(
		min.x + (size.x - imageSize) * 0.5f,
		min.y + (size.y - imageSize) * 0.5f
	);

	if (tex)
	{
		if (tex && tex->GetTextureID() != 0)
		{
			ImGui::GetWindowDrawList()->AddImage(
				(ImTextureID)(intptr_t)tex->GetTextureID(),
				imagePos,
				ImVec2(imagePos.x + imageSize, imagePos.y + imageSize)
			);
		}
	}

	if (enableDrag)
		DragAndDropResource(file);

	float textWidth = ImGui::CalcTextSize(file.c_str()).x;
	float textX = ImGui::GetCursorPosX() + (thumbnailSize - textWidth) * 0.5f;
	ImGui::SetCursorPosX(textX);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
	ImGui::TextWrapped("%s", file.c_str());

	return clicked;
}

void ContentBrowserUI::DragAndDropResource(const std::string& file) const
{
	std::string ext = FileUtils::GetExtension(file, false);
	const char* dragType = ext.c_str();

	void* payloadData = (void*)file.c_str();
	int payloadSize = (int)strlen(file.c_str()) + 1;

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
	{
		ImGui::SetDragDropPayload(dragType, payloadData, payloadSize);
		ImGui::Text("Dragging: %s", file.c_str());
		ImGui::EndDragDropSource();
	}
}

ITexture* ContentBrowserUI::GetThumbnailTexture(const std::string& file) const
{
	std::string ext = FileUtils::GetExtension(file, false);

	if (ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "tga" || ext == "bmp")
		return resourceManager->Get<ITexture>(file.c_str());

	static const std::unordered_map<std::string, std::string> iconMap = {
		{ "obj",  "obj.png"  }, { "fbx",  "obj.png"  }, { "dae",  "obj.png"  },	//Object
		{ "cpp",  "cpp.png"  }, { "h",    "h.png"    }, //Script
	};

	auto it = iconMap.find(ext);
	if (it != iconMap.end())
		return resourceManager->Get<ITexture>(it->second.c_str());
	
	return nullptr; 
}

void ContentBrowserUI::HandleLeftClickResource(const std::string& file) const
{
	std::string extension = FileUtils::GetExtension(file, false);

	if (extension == "scene")
	{
		hierarchy->ClearCurrentObject();
		ISceneGraph* graph = resourceManager->Get<SceneGraph>(file.c_str());
		stateManager->SetCurrentScene(graph);
	}
	else if (extension == "h")
	{
		scriptManager->OpenScriptInVisualStudio("../EchoEngine.sln", "Assets/Scripts/" + file);
	}
	else if (extension == "cpp")
	{
		scriptManager->OpenScriptInVisualStudio("../EchoEngine.sln", "Assets/Scripts/" + file);
	}
}

void ContentBrowserUI::HandleRightClickResource(const std::string& file) const
{
	std::string extension = FileUtils::GetExtension(file, false);
	std::string filename = FileUtils::FileName(file);

	if (ImGui::BeginPopupContextItem(("ContextMenu##" + filename).c_str()))
	{
		if (extension == "scene")
		{
			if (ImGui::MenuItem("Delete"))
			{
				ISceneGraph* graph = resourceManager->Get<SceneGraph>(filename.c_str());
				if (stateManager->GetCurrentScene() != graph)
				{
					FileUtils::DeleteFile(graph->GetPath());
					resourceManager->Delete<SceneGraph>(filename.c_str());
				}
				else
				{
					std::cerr << "[Error] Cannot delete scene in use: " << filename << std::endl;
				}
			}
		}
		else if (extension == "cpp" || extension == "h")
		{
			if (ImGui::MenuItem("Delete"))
			{
				DeleteScript(scriptManager->GetScriptNameFromFile(file));
			}
		}
		ImGui::EndPopup();
	}
}

void ContentBrowserUI::DisplayFiles(const std::vector<std::string>& files) const
{
	for (const auto& file : files)
	{
		ImGui::BeginGroup();

		ContentResources(file);

		ImGui::EndGroup();
		ImGui::NextColumn();
	}
}

void ContentBrowserUI::ContentResources(const std::string& file) const
{
	ImGui::BeginGroup();

	ITexture* displayTex = GetThumbnailTexture(file);

	ImGui::PushID(file.c_str());

	bool clicked = false;

	if (displayTex && displayTex->GetTextureID() != 0)
		clicked = BouttonResource(file, displayTex, true);
	else
		clicked = BouttonResource(file, nullptr, true);

	ImGui::PopID();
	
	if (clicked)
		HandleLeftClickResource(file);
	HandleRightClickResource(file);

	ImGui::EndGroup();
}

void ContentBrowserUI::ContextContentBrowser()
{
	if (ImGui::BeginPopupContextWindow("RightClickBrowser"))
	{
		if (ImGui::Button("Create new Scene"))
		{
			OpenFileCreationPopup(FileCreationType::Scene);
		}

		if (ImGui::Button("New Script"))
		{
			OpenFileCreationPopup(FileCreationType::Script);
		}

		ImGui::EndPopup();
	}

	if (openFilePopup)
	{
		ImGui::OpenPopup("Create New File");
		openFilePopup = false;
	}

	if (ImGui::BeginPopupModal("Create New File", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText("Name", newFileName, IM_ARRAYSIZE(newFileName));

		if (ImGui::Button("Create"))
		{
			std::string name = newFileName;

			if (currentCreationType == FileCreationType::Scene)
			{
				std::string outPath;
				FileUtils::CreateFile(currentPath, name, ".scene", outPath);
				resourceManager->CreateResource<ISceneGraph>(outPath.c_str()); 
			}
			else if (currentCreationType == FileCreationType::Script)
			{
				scriptManager->scriptNames.push_back(name);
				scriptManager->UpdateGameRegistryFile(scriptManager->scriptNames);
				scriptManager->AddScriptToVcxproj(name);

				std::ifstream headerTemplateFile("Templates/MonoBehaviourTemplateH.txt");
				std::ifstream sourceTemplateFile("Templates/MonoBehaviourTemplateCPP.txt");
				std::string headerTemplate((std::istreambuf_iterator<char>(headerTemplateFile)), {});
				std::string sourceTemplate((std::istreambuf_iterator<char>(sourceTemplateFile)), {});

				size_t pos;
				while ((pos = headerTemplate.find("{{ClassName}}")) != std::string::npos)
					headerTemplate.replace(pos, 13, name);
				while ((pos = sourceTemplate.find("{{ClassName}}")) != std::string::npos)
					sourceTemplate.replace(pos, 13, name);

				std::filesystem::create_directories("Assets/Scripts/");
				std::ofstream headerFile("Assets/Scripts/" + name + ".h");
				std::ofstream sourceFile("Assets/Scripts/" + name + ".cpp");

				headerFile << headerTemplate;
				sourceFile << sourceTemplate;
			}

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void ContentBrowserUI::OpenFileCreationPopup(FileCreationType type)
{
	currentCreationType = type;
	strcpy_s(newFileName, "NewFile"); 
	openFilePopup = true;
}

void ContentBrowserUI::DeleteScript(const std::string& className) const
{
	std::filesystem::remove("Assets/Scripts/" + className + ".cpp");
	std::filesystem::remove("Assets/Scripts/" + className + ".h");

	scriptManager->scriptNames.erase(std::remove(scriptManager->scriptNames.begin(), scriptManager->scriptNames.end(), className), scriptManager->scriptNames.end());
	scriptManager->UpdateGameRegistryFile(scriptManager->scriptNames);
	scriptManager->RemoveScriptFromVcxproj(className);

	scriptManager->ReloadGame();
}

