//
// Created by piotr on 30.12.2021.
//
#include "DiceRoller.h"
std::string DiceRoller::helpShort() { return "Dice roller helpShort test"; }
void DiceRoller::helpMain(dpp::Embed &embed) {std::cout<<"test helpMain";}
void DiceRoller::handler(dpp::Message &msg)
{
    std::cout<<"test handler roll"<<std::endl;
}

void DiceRoller::slashHandler(dpp::Interaction &msg) {

}

std::vector<int> DiceRoller::generateRolls(int diceCount, int diceSides) {
    std::uniform_int_distribution<int> dist(1, diceSides);
    std::vector<int> rolls;
    for(int i = 0; i < diceCount; i++)
    {
        rolls.push_back(dist(this->generator));
    }
    return rolls;
}
void DiceRoller::createSlashCommands() {

}
