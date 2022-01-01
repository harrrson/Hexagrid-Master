//
// Created by piotr on 25.12.2021.
//

#ifndef HEXAGRID_MASTER_DICEROLLER_H
#define HEXAGRID_MASTER_DICEROLLER_H

#include <utility>

#include "CommandBase.h"

class DiceRoller : public CommandBase
{
  public:
    using CommandBase::CommandBase;

    void handler(dpp::Message &msg) override;
    void slashHandler(dpp::Interaction &msg) override;

    std::string helpShort() override;

    void helpMain(dpp::Embed &embed) override;
    void createSlashCommands() override;

  private:
    std::vector<int> generateRolls(int diceCount, int diceSides);

    std::random_device generator;
};


#endif // HEXAGRID_MASTER_DICEROLLER_H
