/*
** EPITECH PROJECT, 2018
** bomberman
** File description:
**
*/

#ifndef BOMBERMAN_GAMEMANAGER_HPP
#define BOMBERMAN_GAMEMANAGER_HPP

#include <irrlicht.h>
#include <memory>

class GameManager {
public:
	virtual ~GameManager();

	irr::IrrlichtDevice *getDevice() const;
	irr::video::IVideoDriver *getDriver() const;
	irr::scene::ISceneManager *getSmgr() const;
	irr::gui::IGUIEnvironment *getGuienv() const;
	static GameManager &getInstance();
private:
	GameManager();
	irr::IrrlichtDevice *_device;
	irr::video::IVideoDriver *_driver;
	irr::scene::ISceneManager *_smgr;
	irr::gui::IGUIEnvironment *_guienv;
	static GameManager _instance;
	irr::u32 _width;
	irr::u32 _height;
};


#endif //BOMBERMAN_GAMEMANAGER_HPP
