/*
** EPITECH PROJECT, 2018
** bomberman
** File description:
** LoadState.cpp
*/

#include <glob.h>
#include "../../include/States/LoadState.hpp"
#include "../../include/Singletons/StateMachine.hpp"
#include "../../include/Singletons/IrrManager.hpp"
#include "../../include/Singletons/EventReceiver.hpp"
#include "../../include/Singletons/AssetsPool.hpp"

const std::map<LoadState::Actions , LoadState::ButtonsDesc>
	LoadState::_descs {
	{LoadState::SAVE1,    {
		{610, 250, 1300, 300},
		"save",
		[](LoadState *self) {
			self->externalEventsClean();
			StateMachine::getInstance().pop();
			return false;
		}
	}},
	{LoadState::SAVE2,    {
		{610, 350, 1300, 400},
		"save",
		[](LoadState *self) {
			self->externalEventsClean();
			StateMachine::getInstance().pop();
			return false;
		}
	}},
	{LoadState::SAVE3,    {
		{610, 450, 1300, 500},
		"save",
		[](LoadState *self) {
			self->externalEventsClean();
			StateMachine::getInstance().pop();
			return false;
		}
	}},
	{LoadState::SAVE4,    {
		{610, 550, 1300, 600},
		"save",
		[](LoadState *self) {
			self->externalEventsClean();
			StateMachine::getInstance().pop();
			return false;
		}
	}},
	{LoadState::CANCEL,  {
		{1570, 850,  1870, 900},
		"cancel",
		[](LoadState *self) {
			self->externalEventsClean();
			StateMachine::getInstance().pop();
			return false;
		}
	}},
	{LoadState::PREV,    {
		{835, 850,  935, 900},
		"save",
		[](LoadState *self) {
			self->_idx -= 1;
			self->setSaveButtons();
			return true;
		}
	}},
	{LoadState::NEXT,    {
		{985, 850,  1085, 900},
		"save",
		[](LoadState *self) {
			self->_idx += 1;
			self->setSaveButtons();
			return true;
		}
	}}
};

LoadState::LoadState(AStateShare &_share) : AState(_share),
AMenuSound(), _idx(0)
{
}

LoadState::~LoadState()
{
	eventsClean();
}

void LoadState::loadButtons()
{
	auto gui = IrrManager::getInstance().getGuienv();
	auto &er = EventReceiver::getInstance();
	auto &ap = AssetsPool::getInstance();

	for (auto &n : _descs) {
		auto b = gui->addButton(n.second.pos, nullptr, n.first);
		auto name = n.second.name;
		b->setImage(ap.loadTexture("buttons/" + name + ".png"));
		b->setPressedImage(ap.loadTexture("buttons/" + name + "_hover.png"));
		b->setOverrideFont(_share.getFont());
		_buttons.push_back(b);
	}
	glob_t glob_result;

	glob(".save/*", GLOB_TILDE, NULL, &glob_result);
	for (unsigned int i = 0; i < glob_result.gl_pathc; ++i)
		_saves.emplace_back(glob_result.gl_pathv[i]);
	_idx = 0;
	setSaveButtons();
}

void LoadState::unloadButtons()
{
	auto &er = EventReceiver::getInstance();
	er.unregisterEvent(1, irr::EEVENT_TYPE::EET_GUI_EVENT);
	for (auto &n : _buttons)
		n->remove();
	_buttons.clear();
}

void LoadState::load()
{
	eventsSetup();
	loadButtons();
	AState::load();
}

void LoadState::unload()
{
	unloadButtons();
	AState::unload();
}

void LoadState::update()
{
	_share.getFunc("rotateMenu")();
	AState::update();

	if (getSharedResources().isKeyDown(irr::KEY_ESCAPE))
		StateMachine::getInstance().pop();
}

void LoadState::draw()
{
	auto &im = IrrManager::getInstance();
	im.getSmgr()->drawAll();
	im.getGuienv()->drawAll();
}

bool LoadState::applyEventButton(const irr::SEvent &ev, LoadState::Actions id)
{
	auto b = getButton(id);
	auto hover_name = "buttons/" + _descs.at(id).name + "_hover.png";
	auto name = "buttons/" + _descs.at(id).name + ".png";
	auto &ap = AssetsPool::getInstance();

	switch (ev.GUIEvent.EventType) {
		case irr::gui::EGET_BUTTON_CLICKED:
			playSelect();
			return LoadState::_descs.at(id).fct(this);
		case irr::gui::EGET_ELEMENT_HOVERED:
			playCursor();
			b->setImage(ap.loadTexture(hover_name));
			break;
		case irr::gui::EGET_ELEMENT_LEFT:
			b->setImage(ap.loadTexture(name));
			break;
		default:
			break;
	}
	return true;
}

irr::gui::IGUIButton *LoadState::getButton(LoadState::Actions id) const
{
	if (id < SAVE1 || id > SAVE1 + LOAD_BUTTON_NUMBER)
		return nullptr;
	return (_buttons.at(id - SAVE1));
}

void LoadState::setSaveButtons()
{
	size_t i = _idx * 4;
	std::string empty = "- Empty Slot -";

	for (; i < _saves.size() && (i == (_idx * 4) || i%4); ++i) {
		std::string temp(_saves[i].substr(_saves[i].rfind('/') + 1));
		_buttons[i%4]->setText(std::wstring(temp.begin(),
			temp.end()).c_str());
		_buttons[i%4]->setEnabled(true);
	}
	for (; i == _idx * 4 || i%4; ++i) {
		_buttons[i%4]->setEnabled(false);
		_buttons[i%4]->setText(std::wstring(empty.begin(),
			empty.end()).c_str());
	}
	_buttons[PREV - SAVE1]->setEnabled(_idx > 0);
	_buttons[NEXT - SAVE1]->setEnabled((_idx + 1) * 4 < _saves.size());
}

void LoadState::eventsSetup()
{
	_eventsActivate = true;
	auto &er = EventReceiver::getInstance();
	er.registerEvent(2, irr::EEVENT_TYPE::EET_GUI_EVENT,
		[this](const irr::SEvent &ev) {
			if (!this->isLoaded() || !this->isEnable())
				return true;
		auto id = static_cast<Actions >(ev.GUIEvent.Caller->getID());
			if (LoadState::_descs.count(id) > 0)
				return this->applyEventButton(ev, id);
			return true;
		});
}

void LoadState::eventsClean()
{
	if (!_eventsActivate)
		return;
	auto &er = EventReceiver::getInstance();
	er.unregisterEvent(2, irr::EEVENT_TYPE::EET_GUI_EVENT);
	_eventsActivate = false;
}

void LoadState::externalEventsClean()
{
	if (!_eventsActivate)
		return;
	_eventsActivate = false;
}


const std::string LoadState::getName() const
{
	return "load";
}
