//
// Created by piotr on 30.12.2021.
//

#ifndef HEXAGRID_MASTER_COMMANDBASE_H
#define HEXAGRID_MASTER_COMMANDBASE_H
#include <utility>
#include <discordpp/plugin-native.hh>

namespace dpp = discordpp;
class CommandBase {
  public:
    explicit CommandBase(std::string name, const std::shared_ptr<DppBot> &bot)
        : mName(std::move(name)) {
        mBot = bot;
        bot->respond(mName, [this](dpp::Message msg) { this->handler(msg); });
    }

    virtual void handler(dpp::Message &msg) = 0;
    virtual void slashHandler(dpp::Interaction &msg) = 0;

    virtual std::string helpShort() = 0;

    virtual void helpMain(dpp::Embed &embed) = 0;
    virtual void createSlashCommands(const dpp::User &user) {
        std::cout << "Called base function" << std::endl;
    };

  protected:
    std::string mName{};
    std::shared_ptr<DppBot> mBot;
};

inline void CommandBase::handler(dpp::Message &msg) {}
inline void CommandBase::slashHandler(dpp::Interaction &msg) {}
inline void CommandBase::helpMain(dpp::Embed &embed) {}
inline std::string CommandBase::helpShort() { return ""; }

#endif // HEXAGRID_MASTER_COMMANDBASE_H
