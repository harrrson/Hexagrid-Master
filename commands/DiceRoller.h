//
// Created by piotr on 25.12.2021.
//

#ifndef HEXAGRID_MASTER_DICEROLLER_H
#define HEXAGRID_MASTER_DICEROLLER_H

#include <utility>

#include "CommandBase.h"

struct RollParams{
    int rolls;
    int diceSize;
    float modifier;
    char modifierOperator;
    bool thresholdPresent;
    float thresholdValue;
};
const std::string WHITESPACE = " \n\r\t\f\v";
const std::string allowed_chars = "deDE1234567890+-*/";
const int maxRolls = 20;
const int maxDiceSize=INT_MAX;

const std::unordered_map<std::string,std::vector<std::string>> diceColours{
    {"green",{"Small Fail", "Big Success", "Big Success", "Big Success"}},
    {"lime",{"Fail", "Success", "Big Success", "Big Success"}},
    {"yellow",{"Fail", "Success", "Success", "Big Success"}},
    {"white",{"Big Fail", "Fail", "Success", "Big Success"}},
    {"orange",{"Big Fail", "Fail", "Fail", "Success"}},
    {"red",{"Big Fail", "Big Fail", "Fail", "Success"}},
    {"black",{"Big Fail", "Big Fail", "Big Fail", "Small Success"}}
};

class DiceRoller : public CommandBase
{
  public:
    using CommandBase::CommandBase;

    void handler(dpp::Message &msg) override;
    void slashHandler(dpp::Interaction &msg) override;

    std::string helpShort() override;

    void helpMain(dpp::Embed &embed) override;
    void createSlashCommands(const dpp::User &user) override;
    void registerSlashHandler(json res);

  private:
    std::vector<int> generateRolls(int diceCount, int diceSize);

    std::string parseDiceString(std::string &diceString, RollParams &params);

    std::random_device generator;

};


#endif // HEXAGRID_MASTER_DICEROLLER_H
