/*
** EPITECH PROJECT, 2018
** bomberman
** File description:
**
*/

#include "../../include/States/SettingsState.hpp"
#include <cmath>
#include "../../include/States/MenuState.hpp"
#include "../../include/Singletons/IrrManager.hpp"
#include "../../include/Singletons/StateMachine.hpp"
#include "../../include/Singletons/EventReceiver.hpp"
#include "../../include/Singletons/AssetsPool.hpp"
#include "../../include/States/GameState.hpp"
#include "../../include/States/AIChooseState.hpp"

const std::map<MenuActions, MenuState::BouttonsDesc>
	MenuState::_descs{
	{LAUNCH,    {
		            {50, 50,  750, 100},
		            "launch",
		            [](MenuState *self) {
		            	auto &sm = StateMachine::getInstance();
		            	auto &res = self->getSharedResources();
		            	sm.push(new AIChooseState(res), false);
//			        sm.push(new GameState(res), false);
		            }
	            }},
	{LOAD,      {
		            {50, 150, 750, 200},
		            "load",
		            [](MenuState *self) {
			            StateMachine::getInstance().pop();
		            }
	            }},
	{SETTINGS,  {
		            {50, 250, 750, 300},
		            "settings",
		            [](MenuState *self) {
			            StateMachine::getInstance().push(new SettingsState(self->_share), false);
		            }
	            }},
	{EXIT_GAME, {
		            {50, 350, 750, 400},
		            "exit",
		            [](MenuState *self) {
			            StateMachine::getInstance().popAll();
		            }
	            }},
};

MenuState::MenuState(AStateShare &_share) : AState(_share),
_camRotate(2.3, static_cast<irr::f32>(3.14159265 / 3.0), 700, {450, 0, 100})
{
}

MenuState::~MenuState()
{
}

void MenuState::load()
{
	loadBouttons();
	_sound = AssetsPool::getInstance().loadSound(AssetsPool::MENU, true);
	AState::load();
}

void MenuState::unload()
{
	unloadBouttons();
//	if (_sound)
//		AssetsPool::getInstance().unloadSound(AssetsPool::MENU, _sound);
//	_sound = nullptr;
	AState::unload();
}

void MenuState::draw()
{
	auto &im = IrrManager::getInstance();
	im.getSmgr()->drawAll();
	im.getGuienv()->drawAll();
}

irr::gui::IGUIButton *MenuState::getBoutton(MenuActions id) const
{
	if (id < LAUNCH || id > LAUNCH + BOUTON_NUMBER)
		return nullptr;
	return (_bouttons.at(id - LAUNCH));
}

void MenuState::unloadBouttons()
{
	auto &er = EventReceiver::getInstance();
	er.unregisterEvent(1, irr::EEVENT_TYPE::EET_GUI_EVENT);
	for (auto &n : _bouttons)
		n->remove();
	_bouttons.clear();
}

void MenuState::loadBouttons()
{
	auto gui = IrrManager::getInstance().getGuienv();
	auto &er = EventReceiver::getInstance();
	auto &ap = AssetsPool::getInstance();

	for (auto &n : _descs) {
		auto b = gui->addButton(n.second.pos, nullptr, n.first);
		auto name = n.second.name;
		b->setImage(ap.loadTexture("bouttons/" + name + ".png"));
		b->setPressedImage(ap.loadTexture("bouttons/" + name + "_hover.png"));
		_bouttons.push_back(b);
	}

	er.registerEvent(1, irr::EEVENT_TYPE::EET_GUI_EVENT,
	                 [this](const irr::SEvent &ev) {
		                 auto id = static_cast<MenuActions>(ev.GUIEvent.Caller->getID());
		                 if (MenuState::_descs.count(id) > 0)
			                 this->applyEventBoutton(ev, id);
		                 return true;
	                 });
}

void MenuState::applyEventBoutton(const irr::SEvent &ev, MenuActions id)
{
	auto b = getBoutton(id);
	auto hover_name = "bouttons/" + _descs.at(id).name + "_hover.png";
	auto name = "bouttons/" + _descs.at(id).name + ".png";
	auto &ap = AssetsPool::getInstance();

	switch (ev.GUIEvent.EventType) {
		case irr::gui::EGET_BUTTON_CLICKED:
			MenuState::_descs.at(id).fct(this);
			break;
		case irr::gui::EGET_ELEMENT_HOVERED:
			b->setImage(ap.loadTexture(hover_name));
			break;
		case irr::gui::EGET_ELEMENT_LEFT:
			b->setImage(ap.loadTexture(name));
			break;
		default:
			break;
	}
}

void MenuState::update()
{
	auto step  = static_cast<irr::f32>((2.0 * M_PI) / 1000.0);
	irr::f32 min = 2.7;
	irr::f32 max = 4.4;
	static irr::f32 inc = 0;
	auto &cam = dynamic_cast<irr::scene::ICameraSceneNode &>(_share.getSharedNode("cam"));

	cam.setTarget({450, 0, 100});
	inc += step;
	_camRotate.setInc(static_cast<irr::f32>((sinf(inc) - -1.0) * (max - min) / (1.0 - -1.0) + min));
	cam.setPosition(_camRotate.calc());
}
