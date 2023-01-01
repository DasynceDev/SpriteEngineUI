
#include "UIHirachy.h"

//Constructor

s2d::UIHirachy::UIHirachy()
{
	this->m_deleteSprite = nullptr;
	this->m_childSelectedToParent = nullptr;
	this->isHovered = false;
	this->m_clickedValidSprite = false;
	this->m_isAnySpriteRightClicked = false;
	this->m_isPopUpOpen = false;
	this->m_menuName = "menu";
	this->m_width = 250.0f;
	this->m_setAllKeepOnHirachyBoolsToFalse = false;

	this->m_spriteSelectedColor = ImVec4(139.0f / 255.0f,
										180.0f / 255.0f,
										234.0f / 255.0f,
										1.0f);
}
									

//Public functions

void s2d::UIHirachy::createHirachyWindow()
{
	//Pushing transperany
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.1f);

	this->m_clickedValidSprite = false;
	if (ImGui::Begin("Hirachy", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
	{
		this->displayHirachyComponents();
		this->isHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) || this->m_isPopUpOpen;
		ImGui::End();
	}

	ImGui::PopStyleVar();

	if (this->m_childSelectedToParent != nullptr)
	{
		ImVec2 cursor = ImGui::GetCursorPos();
		ImVec2 pos = ImVec2(sf::Mouse::getPosition().x - 100, sf::Mouse::getPosition().y + 10);

		if (ImGui::Begin("##Drag", NULL, ImGuiWindowFlags_NoTitleBar))
		{
			ImGui::SetWindowPos(pos);
			ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);

			ImGui::Text(this->m_childSelectedToParent->name.c_str());
			ImGui::End();
		}
	}
}

//private functions

void s2d::UIHirachy::displayHirachyComponents()
{
	//Setting HY window size
	ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetWindowSize(ImVec2(this->m_width, 1080.0f));

	//Creating additonal windows
	this->createMenuPopup();

	//Adding sprites to hirachy
	this->addSpritesToHirachy();
}

void s2d::UIHirachy::createMenuPopup()
{
	//Checking inputs to open the pop up
	if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered() && !this->m_isPopUpOpen)
	{
		ImGui::OpenPopup(this->m_menuName);
	}

	ImGui::SetNextWindowSize(ImVec2(150, 200));

	//Rendering the popup
	renderMenuPopup();

	//The pop up needs to shut down when we right click again -> alr done
	if (ImGui::IsMouseClicked(1) && this->m_isPopUpOpen)
	{
		ImGui::CloseCurrentPopup();
	}
	this->m_isPopUpOpen = ImGui::IsPopupOpen(this->m_menuName);
}

void s2d::UIHirachy::renderMenuPopup()
{
	//Pop up, only the window this aint the menu
	if (ImGui::BeginPopup(this->m_menuName))
	{
		this->createButton();
		this->deleteButton();

		ImGui::EndPopup();
	}
}

void s2d::UIHirachy::deleteChildsRecursivly(s2d::Sprite* toDelete)
{
	for (s2d::Sprite* child : toDelete->childs)
	{
		//Erasing it from the vector - deletet sprites because if we delete 1 sprite our list already pops back so we need to move 1 to
		s2d::Sprite::activeSprites.erase(s2d::Sprite::activeSprites.begin() + child->getVectorPosition() - 1);

		for (s2d::Sprite* spr : s2d::Sprite::activeSprites)
		{
			//Decrement the vector position so we dont have to rememer the deletet sprites
			if (spr->getVectorPosition() > child->getVectorPosition())
				spr->setVectorPosition(spr->getVectorPosition() - 1);
		}

		if (child->childs.size() > 0)
		{
			this->deleteChildsRecursivly(child);
		}
		delete child;
	}

	//Clearing the childs since the whole parent gets deletet
	toDelete->childs.clear();
}

void s2d::UIHirachy::createButton()
{
	ImGui::SetNextWindowSize(ImVec2(100, 40));

	//Right click menu in the HY 
	if (ImGui::BeginMenu("Create"))
	{
		//Creating the Menu item "sprite" to create sprites
		if (ImGui::MenuItem("Sprite"))
		{
			s2d::SpriteData::highestIndex++;

			//Adding a new sprite to the hirachy + game scene	

			int vectorPos = s2d::Sprite::activeSprites.size() + 1;
			std::string temp = "Sprite " + std::to_string(vectorPos) + " id: " + std::to_string(s2d::SpriteData::highestIndex);

			s2d::Sprite* sprite = new s2d::Sprite(temp, s2d::Vector2(0, 0), s2d::SpriteData::defaultSpritePath);
			sprite->setId(s2d::SpriteData::highestIndex);

			sprite->addSpriteToScene();

		}
		ImGui::EndMenu();
	}
	ImGui::SetCursorPos(ImVec2(0, 35));

}

void s2d::UIHirachy::deleteButton()
{
	if (ImGui::Button("Delete") && this->m_deleteSprite != nullptr)
	{
		//Erasing sprites from the list ( when deleting a child)
		s2d::Sprite* parent = s2d::Sprite::getSpriteById(this->m_deleteSprite->getParentId());
		if (parent != nullptr)
		{
			for (s2d::Sprite* child : parent->childs)
			{
				if (child->getId() == this->m_deleteSprite->getId())
				{
					//Erasing the child when deleting a child in a parent
					parent->childs.erase(parent->childs.begin() + child->getChildListPosition() - 1);
				}
			}
		}

		//Deleting the sprite(s) & ch�lds
		//Deleting childs from HY scene + freeing them
		this->deleteChildsRecursivly(this->m_deleteSprite);

		//deleting the deletet sprite + freeing it
		s2d::Sprite::activeSprites.erase((s2d::Sprite::activeSprites.begin() + this->m_deleteSprite->getVectorPosition() - 1));

		for (s2d::Sprite* greater : s2d::Sprite::activeSprites)
		{
			if (greater->getVectorPosition() > this->m_deleteSprite->getVectorPosition())
			{
				greater->setVectorPosition(greater->getVectorPosition() - 1);
			}
		}

		delete this->m_deleteSprite;
		this->m_deleteSprite = nullptr;
	}
}

void s2d::UIHirachy::addSpritesToHirachy()
{
	for (s2d::Sprite* sprite : s2d::Sprite::activeSprites)
	{
		if (sprite->parent != nullptr)
		{
			continue;
		}
		this->displayChildsRecursivly(sprite);

		//We clicked no item, we should display a default inspector view because we set the selected sprit to nullptr
		if (ImGui::IsMouseClicked(0) && !ImGui::IsAnyItemHovered() && ImGui::IsWindowHovered())
		{
			this->selectedSprite = nullptr;
		}

		ImGui::Spacing();
	}

	//Setting drag and drop sprite to nullptr (outide the for, fixed bug)
	if (ImGui::IsMouseReleased(0))
	{
		this->m_childSelectedToParent = nullptr;
	}

	ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
}

#pragma region  display childs

void s2d::UIHirachy::displayChildsRecursivly(s2d::Sprite* sprite)
{
	//Setting sprite name
	std::string s_empty = " ";

	const char* name = sprite->name.c_str();
	bool popStyle = false;

	if (sprite->childs.size() > 0)
	{
		this->displayTreeNode(sprite, popStyle);
	}
	else
	{
		this->displayMenuItem(sprite, popStyle);
	}
}


void s2d::UIHirachy::displayTreeNode(s2d::Sprite* sprite, bool& popStyle)
{
	const char* name = sprite->name.c_str();

	if (s2d::UIHirachy::selectedSprite != nullptr && s2d::UIHirachy::selectedSprite->getId() == sprite->getId())
	{
		popStyle = true;
		ImGui::PushStyleColor(ImGuiCol_Text, this->m_spriteSelectedColor);
	}

	// Iterating recursivly over the sprites, if we select 1 which gets not displayed in the 
	// Hirachy this will open the trees for it
	if (s2d::UIHirachy::selectedSprite != nullptr)
	{
		if (sprite->containsChild(s2d::UIHirachy::selectedSprite))
		{
			ImGui::SetNextItemOpen(true);
		}

	}

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 10);
	// Displaying the name
	if (ImGui::TreeNode(name))
	{
		if (popStyle)
		{
			popStyle = false;
			ImGui::PopStyleColor();
		}
		//Tree needs to be collapsed to delete the Sprite ( parent ) 
		if (ImGui::IsItemClicked(0))
		{
			std::cout << "h ";
			m_clickedValidSprite = true;
			s2d::UIHirachy::selectedSprite = sprite;
		}
		//Setting sprit which will be deletet when we right click and dlcik button delete
		if (ImGui::IsItemClicked(1))
		{
			m_deleteSprite = sprite;
		}

		this->childSystem(sprite, false);

		for (s2d::Sprite* child : sprite->childs)
		{
			displayChildsRecursivly(child);
		}
		ImGui::TreePop();
	}
	if (popStyle)
	{
		popStyle = false;
		ImGui::PopStyleColor();
	}
}

void s2d::UIHirachy::displayMenuItem(s2d::Sprite* sprite, bool popStyle)
{
	const char* name = sprite->name.c_str();

	if (sprite->parent != nullptr)
	{
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 40, ImGui::GetCursorPosY()));
	}

	if (s2d::UIHirachy::selectedSprite != nullptr && s2d::UIHirachy::selectedSprite->getId() == sprite->getId())
	{
		popStyle = true;
		ImGui::PushStyleColor(ImGuiCol_Text, this->m_spriteSelectedColor);
	}

	// Displaying the sprite name
	bool clicked = ImGui::MenuItem(name);
	if (popStyle)
	{
		ImGui::PopStyleColor();
	}

	//Setting sprit which will be deletet when we right click and dlcik button delete
	if (ImGui::IsItemClicked(1))
	{
		m_deleteSprite = sprite;
	}

	this->childSystem(sprite, false);

	if (clicked)
	{
		//Setting  the sprite to selected when we click it in the HY
		m_clickedValidSprite = true;
		s2d::UIHirachy::selectedSprite = sprite;
	}
}

#pragma endregion


void s2d::UIHirachy::childSystem(s2d::Sprite* sprite, bool isHEader)
{
	//Setting child sprite (drag and drop)
	if (this->m_childSelectedToParent == nullptr)
	{
		if (ImGui::IsItemClicked())
		{
			this->m_childSelectedToParent = sprite;
		}
	}

	//When holding a sprite and dragging it to a parent this will be set ehre
	if (ImGui::IsMouseReleased(0) && this->m_childSelectedToParent != nullptr && this->m_childSelectedToParent->name != sprite->name && ImGui::IsItemHovered())
	{
		if (this->m_childSelectedToParent->containsChild(sprite))
		{
			return;
		}

		for (s2d::Sprite* parent : s2d::Sprite::activeSprites)
		{
			for (s2d::Sprite* child : parent->childs)
			{
				if (child->getId() == this->m_childSelectedToParent->getId())
				{
					//When dragging a new sprite as child we need to remove the child from the current sprite 
					child->resetChildData();
				}
			}
		}
		this->m_childSelectedToParent->setParent(sprite);
	}
}


s2d::Sprite* s2d::UIHirachy::selectedSprite = nullptr;

