/*
** EPITECH PROJECT, 2018
** bomberman
** File description:
** UpBombBonus.hpp
*/

#ifndef BOMBERMAN_UPBOMBBONUS_HPP
	#define BOMBERMAN_UPBOMBBONUS_HPP

	#include "../Entities/ABonusEntity.hpp"

class UpBombBonus : public ABonusEntity {
public:
	UpBombBonus();

	void playerChanging(PlayerEntity *entity);

};

#endif /* !BOMBERMAN_UPBOMBBONUS_HPP */
