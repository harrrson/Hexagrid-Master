//
// Created by piotr on 25.12.2021.
//

#ifndef HEXAGRID_MASTER_DICEROLLER_H
#define HEXAGRID_MASTER_DICEROLLER_H

#include "ExtensionsBase.h"

class DiceRoller : public ExtensionsBase
{
    virtual void handler(nlohmann::json &json) override;
};

#endif // HEXAGRID_MASTER_DICEROLLER_H
