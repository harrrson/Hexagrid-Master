//
// Created by piotr on 01.01.2022.
//

#ifndef HEXAGRID_MASTER_HELPCOMMAND_H
#define HEXAGRID_MASTER_HELPCOMMAND_H

#include "CommandBase.h"
// Pair of command name and command class
using commandPair = std::pair<std::string, std::unique_ptr<CommandBase>>;
using commandMap =
    std::unordered_map<std::string, std::unique_ptr<CommandBase>>;

class HelpCommand : public CommandBase {

  public:
    HelpCommand(std::string name, const std::shared_ptr<DppBot> &bot,
                const std::shared_ptr<commandMap> &commands);
    void handler(dpp::Message &msg) override;
    void slashHandler(dpp::Interaction &msg) override;
    std::string helpShort() override;
    void helpMain(dpp::Embed &embed) override;
    void generateMainHelp(dpp::Embed &embed);
    void generateCommandHelp(dpp::Embed &embed, const std::string &commandName);

  private:
  protected:
    void createSlashCommands() override;

  private:
    std::shared_ptr<commandMap> mCommands;
};

#endif // HEXAGRID_MASTER_HELPCOMMAND_H
