#include "stdafx.h"
#include "Shop.h"
#include "Camera.h"

Shop::Shop(GameData& _gameData)
{
	for (int i = 0; i < SHOP_BUTTONS_NUMBER; i++)
	{
		switch ((ShopButtonType)i)
		{
		case ShopButtonType::POTION:
			buttonVect.push_back(new Button("Assets/Images/Shop/Potion.png", { 200.f,(float)WORKING_HEIGHT / 2 + 20.f}, 2));
			break;
		case ShopButtonType::SPEED_BUFF:
			buttonVect.push_back(new Button("Assets/Images/Shop/BuffSpeed.png", { 450.f, (float)WORKING_HEIGHT / 2 }, 2));
			break;
		case ShopButtonType::STRENGTH_BUFF:
			buttonVect.push_back(new Button("Assets/Images/Shop/BuffAttack.png", { (float)(WORKING_WIDTH - 550.f), (float)WORKING_HEIGHT / 2 }, 2));
			break;
		case ShopButtonType::MAGIC_BUFF:
			buttonVect.push_back(new Button("Assets/Images/Shop/BuffMagic.png", { (float)(WORKING_WIDTH - 300.f), (float)WORKING_HEIGHT / 2 }, 2));
			break;
		case ShopButtonType::QUIT:
			buttonVect.push_back(new Button("Assets/Images/Shop/Quit.png", { (float)WORKING_WIDTH - WORKING_WIDTH / 10, (float)WORKING_HEIGHT - WORKING_HEIGHT / 10 }, 7));
			break;
		default:
			break;
		}
	}
	
	//////////////////Text for the description of each item////////////////////////
	//load the font 
	shopFont.loadFromFile("Assets/Fonts/PopUp.ttf");
	//load sprite for Background
	spriteBackground = Utils::LoadSprite("Assets/Images/Shop/BackgroundShop.png", TOP_LEFT);
	spriteForeground = Utils::LoadSprite("Assets/Images/Shop/ForegroundShop.png", TOP_LEFT);
	
	//Load the Nutman
	spriteNutman = Utils::LoadSprite("Assets/Images/Shop/Nutman.png", TOP_LEFT);
	animNutman = new Animation;
	sf::IntRect rect = sf::IntRect(0, 0, 5103 / 9, 703);
	spriteNutman.setTextureRect(rect);
	animNutman->CreateLeftToRight(spriteNutman, 9, 1, { 5103,703 });
	spriteNutman.setPosition(WORKING_WIDTH / 2 - spriteNutman.getGlobalBounds().width /2 - 100.f, 12.f);
	
	//Load the Closet
	spriteCloset = Utils::LoadSprite("Assets/Images/Shop/Closet.png", TOP_LEFT);
	animCloset = new Animation;
	rect = sf::IntRect(0, 0, 11913 / 11, 333);
	spriteCloset.setTextureRect(rect);
	animCloset->CreateLeftToRight(spriteCloset, 11, 1, { 11913,333 });
	spriteCloset.setPosition(837.f, 747.f);
	
	//Load the Sign
	spriteSign = Utils::LoadSprite("Assets/Images/Shop/Sign.png", TOP_LEFT);
	animSign = new Animation;
	rect = sf::IntRect(0, 0, 9804 / 12, 1066/2);
	spriteSign.setTextureRect(rect);
	animSign->CreateLeftToRight(spriteSign, 12, 2, { 9804,1066 });
	spriteSign.setPosition(1100.f, -50.f);
	
	//Load the Cauldron
	spriteCauldron = Utils::LoadSprite("Assets/Images/Shop/Cauldron.png", TOP_LEFT);
	animCauldron = new Animation;
	rect = sf::IntRect(0, 0, 3816 / 8, 551);
	spriteCauldron.setTextureRect(rect);
	animCauldron->CreateLeftToRight(spriteCauldron, 8, 1, { 3816,551 });
	spriteCauldron.setPosition(29.f, 173.f);
	
	//texts to display descriptions of each item
	for (int i = 0; i < SHOP_BUTTONS_NUMBER; i++)
	{
		sf::Text temp;
		temp.setFont(shopFont);
		temp.setCharacterSize(70);
		temp.setFillColor(sf::Color::White);
		temp.setPosition(WORKING_WIDTH / 21, (WORKING_HEIGHT / 13) * 10);
		descriptionItems.push_back(temp);
		//set all items available
		itemIsAvailable.push_back(true);
	}
	descriptionItems[0].setString("Heal 1/2 of your max HP");
	descriptionItems[1].setString("Boost your speed \n for 30 seconds");
	descriptionItems[2].setString("Boost your strenght \n for 30 seconds");
	descriptionItems[3].setString("Boost your magic \n for 30 seconds");
	descriptionItems[4].setString("Quit the shop");
	currentDescription = descriptionItems[0];

	//text to display the gold of the player
	goldText.setFont(shopFont);
	goldText.setCharacterSize(70);
	goldText.setFillColor(sf::Color::White);
	goldText.setPosition((WORKING_WIDTH / 20)*2, WORKING_HEIGHT / 11);
	spriteGold = Utils::LoadSprite("Assets/Images/Shop/Gold.png", TOP_LEFT);
	spriteGold.setPosition(goldText.getPosition().x - spriteGold.getGlobalBounds().width, WORKING_HEIGHT / 8 - spriteGold.getGlobalBounds().height / 2);
	
	//text to display the cost of the items
	spritePotionsCost = spriteGold;
	spritePotionsCost.setPosition((WORKING_WIDTH / 21) * 6.5f, (WORKING_HEIGHT / 13) * 11.5f);
	spritePotionsCost.setScale(0.5f, 0.5f);
	potionPriceText.setFont(shopFont);
	potionPriceText.setCharacterSize(70);
	potionPriceText.setFillColor(sf::Color::White);
	potionPriceText.setPosition(spritePotionsCost.getPosition().x + spritePotionsCost.getGlobalBounds().width, spritePotionsCost.getPosition().y);
	/////////////////////////////////////////////////////////////////////////////
}


bool Tri(PlayerSort _a, PlayerSort _b)
{
	return _a.gold < _b.gold;
}
void Shop::Reload(GameData& _gameData)
{
	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		PlayerSort temp;
		temp.id = i;
		temp.gold = _gameData.tabPlayers[i]->GetGold();
		turnOrder.push_back(temp);
	}

	//sort turn order by gold the first is the one who have the less gold
	//if the player have the same gold as the other player randomize the order

	std::sort(turnOrder.begin(), turnOrder.end(), Tri);

	for (int i = 0; i < buttonVect.size(); i++)
	{
		buttonVect[i]->isPressedFinished = false;
	}
	buttonSelected = 0;
	lastButtonSelected = 0;
	playerTurn = 0;
	timerAnimNutman = 3.f;
	timerAnimCloset = 5.f;
	for (int i = 0; i < SHOP_BUTTONS_NUMBER - 1; i++)
	{
		itemIsAvailable[i] = true;
	}
}

void Shop::Update(GameData& _gameData)
{
	if (_gameData.reloadShop)
	{
		Reload(_gameData);
		_gameData.reloadShop = false;
	}

	if (timerJoystickPressed > 0)
	{
		timerJoystickPressed -= _gameData.dt;
	}
	goldText.setString(std::to_string(_gameData.tabPlayers[playerTurn]->GetGold()));
	JoysticksButtonsMovements(_gameData);
	UpdateButtonsSelection(_gameData);
	UpdateButtonPressedEffect(_gameData);
	UpdateAnims(_gameData.dt);
	
	currentDescription = descriptionItems[buttonSelected];	
	if (buttonSelected == (int)ShopButtonType::POTION)
	{
		spritePotionsCost.setColor(sf::Color{ 255,255,255, 255 });
		potionPriceText.setString("10");
	}
	else if (buttonSelected != (int)ShopButtonType::QUIT)
	{
		spritePotionsCost.setColor(sf::Color{ 255,255,255, 255 });
		potionPriceText.setString("20");
	}
	else
	{
		spritePotionsCost.setColor(sf::Color{ 255,255,255, 0 });
		potionPriceText.setString("");
	}

	if (wantToQuit && timerJoystickPressed <= 0)
	{
		if (playerTurn + 1 < _gameData.tabPlayers.size())
		{
			playerTurn++;
			wantToQuit = false;
		}
		else
		{
			_gameData.ChangeLevelState(MAP);
			_gameData.ChangeState(GAME);
			wantToQuit = false;
		}
	}
	else if (wantToQuit && timerJoystickPressed > 0)
	{
		timerJoystickPressed -= _gameData.dt;
	}
}

void Shop::UpdateButtonPressedEffect(GameData& _gameData)
{
	if (_gameData.friendShip)
	{
		if (buttonVect[(int)ShopButtonType::POTION]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetGold() >= 10 && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetHasPotion() == false)
		{
			_gameData.tabPlayers[turnOrder[playerTurn].id]->AddGold(-10);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasPotion(true);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasSpeedBuff(false);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasStrengthBuff(false);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasMagicBuff(false);
			buttonVect[(int)ShopButtonType::POTION]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("MERCHANT_BUY" + rand() % 4 + 1);
		}
		else if (buttonVect[(int)ShopButtonType::POTION]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetGold() < 10)
		{
			buttonVect[(int)ShopButtonType::POTION]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR_HINHIN");
		}
		else if (buttonVect[(int)ShopButtonType::POTION]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetHasPotion() == true)
		{
			buttonVect[(int)ShopButtonType::POTION]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR_HINHIN");
		}
		else if (buttonVect[(int)ShopButtonType::SPEED_BUFF]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetGold() >= 20 && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetHasSpeedBuff() == false)
		{
			_gameData.tabPlayers[turnOrder[playerTurn].id]->AddGold(-20);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasSpeedBuff(true);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasPotion(false);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasStrengthBuff(false);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasMagicBuff(false);
			buttonVect[(int)ShopButtonType::SPEED_BUFF]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("MERCHANT_BUY" + rand() % 4 + 1);
		}
		else if (buttonVect[(int)ShopButtonType::SPEED_BUFF]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetGold() < 20)
		{
			buttonVect[(int)ShopButtonType::SPEED_BUFF]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR_HINHIN");
		}
		else if (buttonVect[(int)ShopButtonType::SPEED_BUFF]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetHasSpeedBuff() == true)
		{
			buttonVect[(int)ShopButtonType::SPEED_BUFF]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR_HINHIN");
		}		
		else if (buttonVect[(int)ShopButtonType::STRENGTH_BUFF]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetGold() >= 20 && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetHasStrengthBuff() == false)
		{
			_gameData.tabPlayers[turnOrder[playerTurn].id]->AddGold(-20);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasStrengthBuff(true);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasPotion(false);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasSpeedBuff(false);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasMagicBuff(false);
			buttonVect[(int)ShopButtonType::STRENGTH_BUFF]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("MERCHANT_BUY" + rand() % 4 + 1);
		}
		else if (buttonVect[(int)ShopButtonType::STRENGTH_BUFF]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetGold() < 20 )
		{
			buttonVect[(int)ShopButtonType::STRENGTH_BUFF]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR_HINHIN");
		}
		else if (buttonVect[(int)ShopButtonType::STRENGTH_BUFF]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetHasStrengthBuff() == true)
		{
			buttonVect[(int)ShopButtonType::STRENGTH_BUFF]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR_HINHIN");
		}
		else if (buttonVect[(int)ShopButtonType::MAGIC_BUFF]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetGold() >= 20 && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetHasMagicBuff() == false)
		{
			_gameData.tabPlayers[turnOrder[playerTurn].id]->AddGold(-20);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasMagicBuff(true);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasPotion(false);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasSpeedBuff(false);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasStrengthBuff(false);
			buttonVect[(int)ShopButtonType::MAGIC_BUFF]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("MERCHANT_BUY" + rand() % 4 + 1);
		}
		else if (buttonVect[(int)ShopButtonType::MAGIC_BUFF]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetGold() < 20)
		{
			buttonVect[(int)ShopButtonType::MAGIC_BUFF]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR_HINHIN");
		}
		else if (buttonVect[(int)ShopButtonType::MAGIC_BUFF]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetHasMagicBuff() == true)
		{
			buttonVect[(int)ShopButtonType::MAGIC_BUFF]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR_HINHIN");
		}
		else if (buttonVect[(int)ShopButtonType::QUIT]->isPressedFinished)
		{
			buttonVect[(int)ShopButtonType::QUIT]->SetIsPressed(false, _gameData);
			buttonVect[(int)ShopButtonType::QUIT]->isPressedFinished = false;
			wantToQuit = true;
			timerJoystickPressed = 0.4f;
		}
	}
	else
	{
		if (buttonVect[(int)ShopButtonType::POTION]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetGold() >= 10 && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetHasPotion() == false && itemIsAvailable[(int)ShopButtonType::POTION])
		{
			_gameData.tabPlayers[turnOrder[playerTurn].id]->AddGold(-10);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasPotion(true);
			itemIsAvailable[(int)ShopButtonType::POTION] = false;
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasSpeedBuff(false);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasStrengthBuff(false);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasMagicBuff(false);
			buttonVect[(int)ShopButtonType::POTION]->isPressedFinished = false;
			//buttonVect[(int)ShopButtonType::POTION]->SetTexture("Assets/Images/Shop/PotionSoldOut.png", { 200.f,(float)WORKING_HEIGHT / 2 + 20.f}, 2);
		}
		else if (buttonVect[(int)ShopButtonType::POTION]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetGold() < 10)
		{
			buttonVect[(int)ShopButtonType::POTION]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR_HINHIN");
		}
		else if (buttonVect[(int)ShopButtonType::POTION]->isPressedFinished && !itemIsAvailable[(int)ShopButtonType::POTION])
		{
			buttonVect[(int)ShopButtonType::POTION]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("MERCHANT_NOPOTION1");
		}
		else if (buttonVect[(int)ShopButtonType::POTION]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetHasPotion() == true)
		{
			buttonVect[(int)ShopButtonType::POTION]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR_HINHIN");
		}		
		else if (buttonVect[(int)ShopButtonType::SPEED_BUFF]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetGold() >= 20 && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetHasSpeedBuff() == false && itemIsAvailable[(int)ShopButtonType::SPEED_BUFF])
		{
			_gameData.tabPlayers[turnOrder[playerTurn].id]->AddGold(-20);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasSpeedBuff(true);
			itemIsAvailable[(int)ShopButtonType::SPEED_BUFF] = false;
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasPotion(false);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasStrengthBuff(false);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasMagicBuff(false);
			buttonVect[(int)ShopButtonType::SPEED_BUFF]->isPressedFinished = false;
			buttonVect[(int)ShopButtonType::SPEED_BUFF]->SetTexture("Assets/Images/Shop/BuffSpeedSoldOut.png", { 450.f, (float)WORKING_HEIGHT / 2 }, 2);
		}
		else if (buttonVect[(int)ShopButtonType::SPEED_BUFF]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetGold() < 20)
		{
			buttonVect[(int)ShopButtonType::SPEED_BUFF]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR_HINHIN");
		}
		else if (buttonVect[(int)ShopButtonType::SPEED_BUFF]->isPressedFinished && !itemIsAvailable[(int)ShopButtonType::SPEED_BUFF])
		{
			buttonVect[(int)ShopButtonType::SPEED_BUFF]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("MERCHANT_NOPOTION1");
		}
		else if (buttonVect[(int)ShopButtonType::SPEED_BUFF]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetHasSpeedBuff() == true)
		{
			buttonVect[(int)ShopButtonType::SPEED_BUFF]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR_HINHIN");
		}
		else if (buttonVect[(int)ShopButtonType::STRENGTH_BUFF]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetGold() >= 20 && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetHasStrengthBuff() == false && itemIsAvailable[(int)ShopButtonType::STRENGTH_BUFF])
		{
			_gameData.tabPlayers[turnOrder[playerTurn].id]->AddGold(-20);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasStrengthBuff(true);
			itemIsAvailable[(int)ShopButtonType::STRENGTH_BUFF] = false;
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasPotion(false);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasSpeedBuff(false);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasMagicBuff(false);
			buttonVect[(int)ShopButtonType::STRENGTH_BUFF]->isPressedFinished = false;
			buttonVect[(int)ShopButtonType::STRENGTH_BUFF]->SetTexture("Assets/Images/Shop/BuffAttackSoldOut.png", { (float)(WORKING_WIDTH - 550.f), (float)WORKING_HEIGHT / 2 }, 2);
		}
		else if (buttonVect[(int)ShopButtonType::STRENGTH_BUFF]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetGold() < 20)
		{
			buttonVect[(int)ShopButtonType::STRENGTH_BUFF]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR_HINHIN");
		}
		else if (buttonVect[(int)ShopButtonType::STRENGTH_BUFF]->isPressedFinished && !itemIsAvailable[(int)ShopButtonType::STRENGTH_BUFF])
		{
			buttonVect[(int)ShopButtonType::STRENGTH_BUFF]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("MERCHANT_NOPOTION1");
		}
		else if (buttonVect[(int)ShopButtonType::STRENGTH_BUFF]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetHasStrengthBuff() == true)
		{
			buttonVect[(int)ShopButtonType::STRENGTH_BUFF]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR_HINHIN");
		}
		else if (buttonVect[(int)ShopButtonType::MAGIC_BUFF]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetGold() >= 20 && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetHasMagicBuff() == false && itemIsAvailable[(int)ShopButtonType::MAGIC_BUFF])
		{
			_gameData.tabPlayers[turnOrder[playerTurn].id]->AddGold(-20);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasMagicBuff(true);
			itemIsAvailable[(int)ShopButtonType::MAGIC_BUFF] = false;
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasPotion(false);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasSpeedBuff(false);
			_gameData.tabPlayers[turnOrder[playerTurn].id]->SetHasStrengthBuff(false);
			buttonVect[(int)ShopButtonType::MAGIC_BUFF]->isPressedFinished = false;
			buttonVect[(int)ShopButtonType::MAGIC_BUFF]->SetTexture("Assets/Images/Shop/BuffMagicSoldOut.png", { (float)(WORKING_WIDTH - 300.f), (float)WORKING_HEIGHT / 2 }, 2);		
		}
		else if (buttonVect[(int)ShopButtonType::MAGIC_BUFF]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetGold() < 20)
		{
			buttonVect[(int)ShopButtonType::MAGIC_BUFF]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR_HINHIN");
		}
		else if (buttonVect[(int)ShopButtonType::MAGIC_BUFF]->isPressedFinished && !itemIsAvailable[(int)ShopButtonType::MAGIC_BUFF])
		{
			buttonVect[(int)ShopButtonType::MAGIC_BUFF]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("MERCHANT_NOPOTION1");
		}
		else if (buttonVect[(int)ShopButtonType::MAGIC_BUFF]->isPressedFinished && _gameData.tabPlayers[turnOrder[playerTurn].id]->GetHasMagicBuff() == true)
		{
			buttonVect[(int)ShopButtonType::MAGIC_BUFF]->isPressedFinished = false;
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR_HINHIN");
		}
		else if (buttonVect[(int)ShopButtonType::QUIT]->isPressedFinished)
		{
			buttonVect[(int)ShopButtonType::QUIT]->SetIsPressed(false, _gameData);
			buttonVect[(int)ShopButtonType::QUIT]->isPressedFinished = false;
			wantToQuit = true;
			timerJoystickPressed = 0.4f;
		}
	}

}

void Shop::UpdateButtonsSelection(GameData& _gameData)
{
	if (buttonVect.size() > 1)
	{
		for (unsigned char i = 0; i < buttonVect.size(); i++)
		{
			if (buttonSelected == i)
			{
				buttonVect[i]->SetIsSelected(true, _gameData);
			}
			else
			{
				buttonVect[i]->SetIsSelected(false, _gameData);
			}
		}
	}
}

void Shop::UpdateAnims(const float& _dt)
{
	UpdateAllButtonsAnim(_dt);
	
	//Update Nutman Anim	
	bool temp = true;
	animNutman->UpdateLeftToRight(spriteNutman, _dt, 10, 0, temp, false);
	if (timerAnimNutman <= 0.f)
	{
		rand(); rand(); rand();
		timerAnimNutman = (float)(rand() % 10 + rand() % 5);
		animNutman->currentFrame = 0;
	}
	else
	{
		timerAnimNutman -= _dt;
	}
	
	//Update Closet Anim
	if (timerAnimCloset <= 0.f)
	{
		rand(); rand(); rand();
		timerAnimCloset = (float)(rand() % 10 + 5);
		animClosetIsPlayingToRight = true;
	}
	else
	{
		timerAnimCloset -= _dt;
	}
	
	if (animClosetIsPlayingToRight)
	{
		animCloset->UpdateLeftToRight(spriteCloset, _dt, 10, 0, temp, false);
	}
	
	if (animCloset->currentFrame == animCloset->maxFrame && animClosetIsPlayingToRight)
	{
		if (timerAnimWaitingCloset <= 0.f)
		{
			timerAnimWaitingCloset = 1.f;
			animClosetIsPlayingToRight = false;
			animClosetIsPlayingToLeft = true;
			animCloset->currentFrame = 0;
		}
		else
		{
			timerAnimWaitingCloset -= _dt;
		}		
	}
	
	if (animClosetIsPlayingToLeft)
	{
		animCloset->UpdateRightToLeft(spriteCloset, _dt, 10, 0, temp, false);
	}
	
	if (animCloset->currentFrame == animCloset->maxFrame && animClosetIsPlayingToLeft)
	{
		animClosetIsPlayingToLeft = false;
		animCloset->currentFrame = 0;
	}

	//Update Sign Anim
	animSign->UpdateLeftToRight(spriteSign, _dt, 12, ligne, temp, true);
	if (animSign->currentFrame == animSign->maxFrame && !ligne)
	{
		animSign->currentFrame = 0;
		ligne++;
	}
	else if (animSign->currentFrame == animSign->maxFrame && ligne)
	{
		animSign->currentFrame = 0;
		ligne--;
	}

	//Update Cauldron Anim
	//if (animCauldron->currentFrame == animCauldron->maxFrame)
	//{
	//	animCauldron->currentFrame = 0;
	//	animCauldron->UpdateLeftToRight(spriteCauldron, _dt, 10, 0, temp, false);
	//	animCauldronIsPlaying = false;
	//}
	
	//if (animCauldronIsPlaying)
	//{
		animCauldron->UpdateLeftToRight(spriteCauldron, _dt, 8, 0, temp, true);
	//}
	
	if (timerAnimCauldron <= 0.f)
	{
		timerAnimCauldron = 1.f;
		animCauldronIsPlaying = true;
	}
	else if (!animCauldronIsPlaying)
	{
		timerAnimCauldron -= _dt;
	}
}
void Shop::UpdateAllButtonsAnim(const float& _dt)
{
	for (unsigned char i = 0; i < buttonVect.size(); i++)
	{
		buttonVect[i]->UpdateAnimButton(_dt);
	}
}

void Shop::Display(GameData& _gameData)
{
	_gameData.window.draw(spriteBackground);
	_gameData.window.draw(spriteCauldron);
	_gameData.window.draw(spriteForeground);
	_gameData.window.draw(spriteNutman);
	_gameData.window.draw(spriteCloset);
	_gameData.window.draw(spriteSign);
	_gameData.window.setView(_gameData.camera->GetView());
	DisplayButtons(_gameData.window);
	_gameData.window.draw(currentDescription);
	_gameData.window.draw(goldText);
	_gameData.window.draw(spriteGold);
	_gameData.window.draw(spritePotionsCost);
	_gameData.window.draw(potionPriceText);
}

void Shop::Display3D(GameData& _gameData)
{
	Utils::BlitSprite(spriteBackground, sf::Vector2f{ spriteBackground.getPosition().x - (TROAD_GAP * (-1 * spriteBackground.getPosition().y + WORKING_HEIGHT) / 100.f),spriteBackground.getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(spriteBackground, sf::Vector2f{ spriteBackground.getPosition().x + (TROAD_GAP * (-1 * spriteBackground.getPosition().y + WORKING_HEIGHT) / 100.f),spriteBackground.getPosition().y }, 0, _gameData.renderTextureBlue);

	Utils::BlitSprite(spriteCauldron, sf::Vector2f{ spriteCauldron.getPosition().x - (TROAD_GAP * (-1 * spriteCauldron.getPosition().y + WORKING_HEIGHT) / 100.f),spriteCauldron.getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(spriteCauldron, sf::Vector2f{ spriteCauldron.getPosition().x + (TROAD_GAP * (-1 * spriteCauldron.getPosition().y + WORKING_HEIGHT) / 100.f),spriteCauldron.getPosition().y }, 0, _gameData.renderTextureBlue);

	Utils::BlitSprite(spriteForeground, sf::Vector2f{ spriteForeground.getPosition().x - (TROAD_GAP * (-1 * spriteForeground.getPosition().y + WORKING_HEIGHT) / 100.f),spriteForeground.getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(spriteForeground, sf::Vector2f{ spriteForeground.getPosition().x + (TROAD_GAP * (-1 * spriteForeground.getPosition().y + WORKING_HEIGHT) / 100.f),spriteForeground.getPosition().y }, 0, _gameData.renderTextureBlue);

	Utils::BlitSprite(spriteNutman, sf::Vector2f{ spriteNutman.getPosition().x + (TROAD_GAP * (-1 * spriteNutman.getPosition().y + WORKING_HEIGHT) / 100.f),spriteNutman.getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(spriteNutman, sf::Vector2f{ spriteNutman.getPosition().x - (TROAD_GAP * (-1 * spriteNutman.getPosition().y + WORKING_HEIGHT) / 100.f),spriteNutman.getPosition().y }, 0, _gameData.renderTextureBlue);

	Utils::BlitSprite(spriteCloset, sf::Vector2f{ spriteCloset.getPosition().x - (TROAD_GAP * (-1 * spriteCloset.getPosition().y + WORKING_HEIGHT) / 100.f),spriteCloset.getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(spriteCloset, sf::Vector2f{ spriteCloset.getPosition().x + (TROAD_GAP * (-1 * spriteCloset.getPosition().y + WORKING_HEIGHT) / 100.f),spriteCloset.getPosition().y }, 0, _gameData.renderTextureBlue);

	Utils::BlitSprite(spriteSign, sf::Vector2f{ spriteSign.getPosition().x - (TROAD_GAP * (-1 * spriteSign.getPosition().y + WORKING_HEIGHT) / 100.f),spriteSign.getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(spriteSign, sf::Vector2f{ spriteSign.getPosition().x + (TROAD_GAP * (-1 * spriteSign.getPosition().y + WORKING_HEIGHT) / 100.f),spriteSign.getPosition().y }, 0, _gameData.renderTextureBlue);

	_gameData.renderTextureRed.setView(_gameData.camera->GetView());
	_gameData.renderTextureBlue.setView(_gameData.camera->GetView());

	DisplayButtons(_gameData.renderTextureRed, _gameData.renderTextureBlue);

	sf::Vector2f posDescription = currentDescription.getPosition();
	sf::Vector2f posGold = goldText.getPosition();

	currentDescription.setPosition(posDescription.x + (TROAD_GAP * (-1 * posDescription.y + WORKING_HEIGHT) / 100.f), posDescription.y);
	_gameData.renderTextureRed.draw(currentDescription);

	currentDescription.setPosition(posDescription.x - (TROAD_GAP * (-1 * posDescription.y + WORKING_HEIGHT) / 100.f), posDescription.y);
	_gameData.renderTextureBlue.draw(currentDescription);

	goldText.setPosition(posGold.x + (TROAD_GAP * (-1 * posGold.y + WORKING_HEIGHT) / 100.f), posGold.y);
	_gameData.renderTextureRed.draw(goldText);

	goldText.setPosition(posGold.x - (TROAD_GAP * (-1 * posGold.y + WORKING_HEIGHT) / 100.f), posGold.y);
	_gameData.renderTextureBlue.draw(goldText);

	currentDescription.setPosition(posDescription);
	goldText.setPosition(posGold);

	Utils::BlitSprite(spriteGold, sf::Vector2f{ spriteGold.getPosition().x - (TROAD_GAP * (-1 * spriteGold.getPosition().y + WORKING_HEIGHT) / 100.f),spriteGold.getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(spriteGold, sf::Vector2f{ spriteGold.getPosition().x + (TROAD_GAP * (-1 * spriteGold.getPosition().y + WORKING_HEIGHT) / 100.f),spriteGold.getPosition().y }, 0, _gameData.renderTextureBlue);
}

void Shop::DisplayButtons(sf::RenderWindow& _window)
{
	for (unsigned char i = 0; i < SHOP_BUTTONS_NUMBER; i++)
	{
		_window.draw(buttonVect[i]->sprite);
	}
}

void Shop::DisplayButtons(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	for (unsigned char i = 0; i < SHOP_BUTTONS_NUMBER; i++)
	{
		Utils::BlitSprite(buttonVect[i]->sprite, sf::Vector2f{ buttonVect[i]->sprite.getPosition().x + (TROAD_GAP * (-1 * buttonVect[i]->sprite.getPosition().y + WORKING_HEIGHT) / 100.f),buttonVect[i]->sprite.getPosition().y }, 0, _red);
		Utils::BlitSprite(buttonVect[i]->sprite, sf::Vector2f{ buttonVect[i]->sprite.getPosition().x - (TROAD_GAP * (-1 * buttonVect[i]->sprite.getPosition().y + WORKING_HEIGHT) / 100.f),buttonVect[i]->sprite.getPosition().y }, 0, _blue);
	}
}

void Shop::KeyPressed(GameData& _gameData)
{

}

void Shop::SetButtonSelectedPressed(GameData& _gameData)
{
	if (buttonVect[(int)ShopButtonType::QUIT]->GetIsSelected())
	{
		buttonVect[(int)ShopButtonType::QUIT]->SetIsPressed(true, _gameData);
	}
	else
	{
		buttonVect[buttonSelected]->isPressedFinished = true;
	}
	
}

void Shop::JoystickPressed(GameData& _gameData)
{
	if (_gameData.event.type == sf::Event::JoystickButtonPressed)
	{
		JoystickHandler(_gameData);
	}
}

void Shop::JoystickHandler(GameData& _gameData)
{
	if (turnOrder.size() > 0 && _gameData.timerButton <= 0)
	{
		if (sf::Joystick::getIdentification(turnOrder[playerTurn].id).name == "Wireless Controller")
		{
			if (sf::Joystick::isButtonPressed(turnOrder[playerTurn].id, (unsigned int)ControllerButtonPS4::CROSS))
			{
				SetButtonSelectedPressed(_gameData);
			}
			if (sf::Joystick::isButtonPressed(turnOrder[playerTurn].id, (unsigned int)ControllerButtonPS4::CIRCLE))
			{
				_gameData.ChangeLevelState(MAP);
				_gameData.ChangeState(GAME);
			}
		}
		else
		{
			if (sf::Joystick::isButtonPressed(turnOrder[playerTurn].id, (unsigned int)ControllerButtonXBOX::A))
			{
				SetButtonSelectedPressed(_gameData);
			}
			if (sf::Joystick::isButtonPressed(turnOrder[playerTurn].id, (unsigned int)ControllerButtonXBOX::B))
			{
				_gameData.ChangeLevelState(MAP);
				_gameData.ChangeState(GAME);
			}
		}
	}
}

void Shop::JoysticksButtonsMovements(GameData& _gameData)
{
	if (!buttonVect.empty() && turnOrder.size() > 0)
	{
		float xPos1 = sf::Joystick::getAxisPosition(turnOrder[playerTurn].id, sf::Joystick::Axis::LEFT_JOYSTICK_X);
		float yPos1 = sf::Joystick::getAxisPosition(turnOrder[playerTurn].id, sf::Joystick::Axis::LEFT_JOYSTICK_Y);
		
		joystickTimer += _gameData.dt;

		if (joystickTimer >= 0.2f)
		{
			joystickTimer = 0;

			if (xPos1 > 15 && buttonSelected != (int)ShopButtonType::QUIT)
			{
				if (buttonSelected != (buttonVect.size() - 1))
				{
					++buttonSelected;
					if (buttonSelected == (int)ShopButtonType::QUIT)
					{
						buttonSelected = (int)ShopButtonType::POTION;
					}
				}
				else
				{
					buttonSelected = 0;
					
				}
				lastButtonSelected = buttonSelected;
				buttonVect[buttonSelected]->SetIsSelected(true, _gameData);
			}
			if (xPos1 < -15 && buttonSelected != (int)ShopButtonType::QUIT)
			{
				if (buttonSelected != 0)
				{
					--buttonSelected;
					if (buttonSelected == (int)ShopButtonType::QUIT)
					{
						buttonSelected = (int)ShopButtonType::MAGIC_BUFF;
					}
				}
				else
				{
					buttonSelected = (int)(buttonVect.size() - 2);
				}
				lastButtonSelected = buttonSelected;
				buttonVect[buttonSelected]->SetIsSelected(true, _gameData);

			}
			if (yPos1 < -15)//Joystick vers le haut
			{
				buttonSelected = lastButtonSelected;
				buttonVect[buttonSelected]->SetIsSelected(true, _gameData);
			}
			if (yPos1 > 70)//Joystick vers le bas
			{
				buttonSelected = (int)ShopButtonType::QUIT;
				buttonVect[buttonSelected]->SetIsSelected(true, _gameData);
			}
		}
	}
}
