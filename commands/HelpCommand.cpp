//
// Created by piotr on 01.01.2022.
//

#include "HelpCommand.h"

HelpCommand::HelpCommand(std::string name,
const std::shared_ptr<DppBot> &bot,
const std::shared_ptr<commandMap> &commands)
: CommandBase(std::move(name), bot) {mCommands=commands;}

void HelpCommand::handler(dpp::Message &msg) {

    if (msg.content->length() == mBot->prefix.length()+mName.length()){
        dpp::Embed embed;
        generateMainHelp(embed);
        mBot->createMessage()
            ->channel_id(*msg.channel_id)
            ->embeds({embed})
            ->run();
    }else {
        std::string hCommand = *msg.content;
        hCommand.erase(0, mBot->prefix.length()+mName.length());
        if(hCommand[0]!=' '){
            return;
        }
        hCommand.erase(0,1);
        if(mCommands->find(hCommand)==mCommands->end()){
            std::ostringstream content;
            content<<"Command \""<<hCommand<<"\" not found";
            mBot->createMessage()
                ->channel_id(*msg.channel_id)
                ->content(content.str())
                ->run();
        }
        else{
            dpp::Embed embed;
            generateCommandHelp(embed, hCommand);
            mBot->createMessage()
                ->channel_id(*msg.channel_id)
                ->embeds({embed})
                ->run();
        }
    }
}

void HelpCommand::slashHandler(dpp::Interaction &msg) {

}
void HelpCommand::createSlashCommands() {

}



std::string HelpCommand::helpShort() { return "Shows bot's help page"; }
void HelpCommand::helpMain(dpp::Embed &embed) {
    dpp::EmbedField field;
    field.name = "help <command>";
    field.value = "Shows help page for given command";
    embed.fields->push_back(field);
}

void HelpCommand::generateMainHelp(dpp::Embed &embed) {
    embed.color = 0xFF0000;
    embed.title = std::string("Help page");
    //Every field is nearly identical, so we can reuse class
    dpp::EmbedField field;
    field.display_inline=false;
    for(auto &command:*mCommands){
        field.name=command.first;
        field.value=command.second->helpShort();
        embed.fields->push_back(field);
    }
}
void HelpCommand::generateCommandHelp(dpp::Embed &embed, const std::string &commandName) {

        embed.title="Help for command "+commandName;
        embed.description = mCommands->at(commandName)->helpShort();
        embed.color = 0xff0000;
        mCommands->at(commandName)->helpMain(embed);
}
