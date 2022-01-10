//
// Created by piotr on 01.01.2022.
//

#include "HelpCommand.h"

HelpCommand::HelpCommand(std::string name, const std::shared_ptr<DppBot> &bot,
                         const std::shared_ptr<commandMap> &commands)
    : CommandBase(std::move(name), bot) {
    mCommands = commands;
}

void HelpCommand::handler(dpp::Message &msg) {

    if (msg.content->length() == mBot->prefix.length() + mName.length()) {
        dpp::Embed embed{};
        generateMainHelp(embed);
        dpp::EmbedFooter footer{};
        footer.text = "Use " + mBot->prefix + "help <command> for more info";
        embed.footer = footer;
        mBot->createMessage()
            ->channel_id(*msg.channel_id)
            ->embeds({embed})
            ->run();
    } else {
        std::string hCommand = *msg.content;
        hCommand.erase(0, mBot->prefix.length() + mName.length());
        if (hCommand[0] != ' ') {
            return;
        }
        hCommand.erase(0, 1);
        if (mCommands->find(hCommand) == mCommands->end()) {
            mBot->createMessage()
                ->channel_id(*msg.channel_id)
                ->content("Command \"" + hCommand + "\" not found")
                ->run();
        } else {
            dpp::Embed embed{};
            generateCommandHelp(embed, hCommand);
            mBot->createMessage()
                ->channel_id(*msg.channel_id)
                ->embeds({embed})
                ->run();
        }
    }
}

void HelpCommand::slashHandler(dpp::Interaction &msg) {
    if (!msg.data->options.is_present()) {
        dpp::Embed embed{};
        generateMainHelp(embed);
        dpp::EmbedFooter footer{};
        footer.text = "Use " + mBot->prefix + "help <command> for more info";
        embed.footer = footer;
        mBot->createResponse()
            ->interaction_id(*msg.id)
            ->interaction_token(*msg.token)
            ->interaction_type(
                dpp::InteractionCallbackType::CHANNEL_MESSAGE_WITH_SOURCE)
            ->data({{"embeds", {embed}}})
            ->run();
    } else {
        std::string hCommand = msg.data->options->at(0).value->get<std::string>();
        if (mCommands->find(hCommand) == mCommands->end()) {
            mBot->createResponse()
                ->interaction_id(*msg.id)
                ->interaction_token(*msg.token)
                ->interaction_type(
                    dpp::InteractionCallbackType::CHANNEL_MESSAGE_WITH_SOURCE)
                ->data({{"content", "Command \"" + hCommand + "\" not found"}})
                ->run();
        } else {
            dpp::Embed embed{};
            generateCommandHelp(embed, hCommand);
            mBot->createResponse()
                ->interaction_id(*msg.id)
                ->interaction_token(*msg.token)
                ->interaction_type(
                    dpp::InteractionCallbackType::CHANNEL_MESSAGE_WITH_SOURCE)
                ->data({{"embeds", {embed}}})
                ->run();
        }
    }
}
void HelpCommand::createSlashCommands(const dpp::User &user) {
    std::vector<dpp::ApplicationCommandOption> options;
    dpp::ApplicationCommandOption option{};
    option.name = std::string("command");
    option.type = dpp::ApplicationCommandOptionType::STRING;
    option.description = std::string("Command which help page you want to see");
    options.push_back(option);

    mBot->createGlobalApplicationCommand()
        ->application_id(*user.id)
        ->name("help")
        ->description("Help for available commands")
        ->options(options)
        ->default_permission(true)
        ->command_type(dpp::ApplicationCommandType::CHAT_INPUT)
        ->onRead(
            [this](bool error, json res) { this->registerSlashHandler(res); })
        ->run();
}

void HelpCommand::registerSlashHandler(json res) {
    dpp::Snowflake id = res["body"]["id"].get<dpp::Snowflake>();
    mBot->interactionHandlers.insert(
        {id, [this](dpp::Interaction msg) { this->slashHandler(msg); }});
}

std::string HelpCommand::helpShort() { return "Shows bot's help page"; }

void HelpCommand::helpMain(dpp::Embed &embed) {
    std::vector<dpp::EmbedField> fields{};
    dpp::EmbedField field{};
    field.name = "\u2b8a (" + mBot->prefix + mName + " || /" + mName +
                 ") <command>";
    field.value = std::string("Shows help page for given command");
    fields.push_back(field);

    embed.fields = fields;
}

void HelpCommand::generateMainHelp(dpp::Embed &embed) {
    embed.color = 0xFF0000;
    embed.title = std::string(":question: Available commands:");
    std::vector<dpp::EmbedField> fields{};
    dpp::EmbedField field{};
    field.display_inline = false;
    for (auto &command : *mCommands) {
        field.name =
            "\u2b8a " + mBot->prefix + command.first + "  |  /" + command.first;
        field.value = command.second->helpShort();
        fields.push_back(field);
    }
    embed.fields = fields;
}
void HelpCommand::generateCommandHelp(dpp::Embed &embed,
                                      const std::string &commandName) {

    embed.title = ":question: Help for command \"" + commandName + "\"";
    embed.description = mCommands->at(commandName)->helpShort();
    embed.color = 0xff0000;
    mCommands->at(commandName)->helpMain(embed);
}
