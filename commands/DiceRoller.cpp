//
// Created by piotr on 30.12.2021.
//
#include "DiceRoller.h"
std::string DiceRoller::helpShort() {
    return "Dice roller with automatic modifier calculation.";
}
void DiceRoller::helpMain(dpp::Embed &embed) {
    std::vector<dpp::EmbedField> fields{};
    fields.reserve(5);
    dpp::EmbedField field{};
    // Every field is in separate line, so we don't need to change
    // display_inline
    field.display_inline = false;

    field.name = std::string("\u2b8a Dice command format");
    field.value =
        "**<number of dices>**d**<dice size><dice modifier>**e**<threshold>**"
        "\n**number of dices**: 1-" +
        std::to_string(maxRolls) +
        " (optional)"
        "\n**dice size**: 2-" +
        std::to_string(maxDiceSize) +
        "\n**dice modifier**: single add/substract/multiply/divide operator "
        "with "
        "value (optional)"
        "\n**threshold**: values below this do not appear in the result list "
        "(optional)";
    fields.push_back(field);

    field.name = "\u2b8a (" + mBot->prefix + mName + " || /" + mName +
                 ") <repetitions> <dice command> !<comment>";
    field.value =
        "Standard dice roller command.\nExample: " + mBot->prefix + mName +
        " 2d6+1 !Sword attack\nWhen used without dice command and comment, "
        "rolls d10 dice";
    fields.push_back(field);

    field.name = "\u2b8a (" + mBot->prefix + mName + " || /" + mName +
                 ") duel <dice command> <dice command> <1st player name> 2nd "
                 "player name> !<comment>";
    field.value = "Rolls duels between two players.\nExample: " + mBot->prefix +
                  mName + " duel 2d6+1 2d6+1 Kruug Joseph !brawl round 1";
    fields.push_back(field);

    field.name = "\u2b8a (" + mBot->prefix + mName + " || /" + mName +
                 ") fate !<comment>";
    field.value = std::string("Rolls fate coin.");
    fields.push_back(field);

    field.name = "\u2b8a (" + mBot->prefix + mName + " || /" + mName +
                 ") <colour> !<comment>";
    field.value = "Rolls coloured dice. You can see available colours with "
                  "results under " +
                  mBot->prefix + mName + " colours or /" + mName + " colours";
    fields.push_back(field);

    embed.fields = fields;
}
void DiceRoller::handler(dpp::Message &msg) {
    std::string content = *msg.content;

    content.erase(0, mBot->prefix.size() + mName.size());
    std::cout << "content: \"" << content << "\"\n";
    if(content.length()>1){
        content.erase(0, 1);
        RollParams params{};
        std::string result = parseDiceString(content, params);
        std::stringstream ss;
        ss << "Parsing result:\t\t" << result << "\n"
           << "Dice rolls:\t\t\t" << params.rolls << "\n"
           << "Dice size:\t\t\t" << params.diceSize << "\n"
           << "Modifier:\t\t\t" << params.modifier << "\n"
           << "Modifier operator:\t" << params.modifierOperator << "\n"
           << "Threshold available:\t" << params.thresholdPresent << "\n"
           << "Threshold:\t\t\t" << params.thresholdValue << "\n";
        mBot->createMessage()
            ->channel_id(*msg.channel_id)
            ->content(ss.str())
            ->run();
    }
}

void DiceRoller::slashHandler(dpp::Interaction &msg) {}

void DiceRoller::registerSlashHandler(json res) {}

void DiceRoller::createSlashCommands(const dpp::User &user) {}

std::vector<int> DiceRoller::generateRolls(int diceCount, int diceSize) {
    std::uniform_int_distribution<int> dist(1, diceSize);
    std::vector<int> rolls{};
    rolls.reserve(diceCount);
    for (int i = 0; i < diceCount; i++) {
        rolls.push_back(dist(generator));
    }
    return rolls;
}

std::string DiceRoller::parseDiceString(std::string &diceString,
                                        RollParams &params) {
#define DICE_SIZE_FLAG 0
#define MODIFIER_FLAG 1
#define THRESHOLD_FLAG 2
#define COMMAND_NOT_VALID "Command is not valid: "
#define NUMBER_OUT_OF_RANGE "Number is out of available range: "
    std::bitset<3> flags{};
    std::string s{};
    for (auto it = diceString.begin(); it != diceString.end(); ++it) {
        switch (*it) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            s.append(1, *it);
            break;
        case 'd':
        case 'D':
            if (flags[DICE_SIZE_FLAG] || flags[MODIFIER_FLAG] ||
                flags[THRESHOLD_FLAG]) {
                return COMMAND_NOT_VALID + diceString;
            }
            flags[DICE_SIZE_FLAG] = true;
            if (s.size() == 0) {
                params.rolls = 1;
            } else {
                try {
                    params.rolls = std::stoi(s);
                } catch (const std::out_of_range &) {
                    return NUMBER_OUT_OF_RANGE + s + " (1-" +
                           std::to_string(maxRolls) + ")";
                }
            }
            if (params.rolls < 1 || params.rolls > maxRolls) {
                return NUMBER_OUT_OF_RANGE + s + " (1-" +
                       std::to_string(maxRolls) + ")";
            }
            s.clear();
            break;
        case '+':
        case '-':
        case '*':
        case '/':
            if (!flags[DICE_SIZE_FLAG] || flags[MODIFIER_FLAG] ||
                flags[THRESHOLD_FLAG] || s.empty()) {
                return COMMAND_NOT_VALID + diceString;
            }
            flags[MODIFIER_FLAG] = true;
            try {
                params.diceSize = std::stoi(s);
            } catch (const std::out_of_range &) {
                return NUMBER_OUT_OF_RANGE + s + " (1-" +
                       std::to_string(maxDiceSize) + ")";
            }
            if (params.diceSize < 1) {
                return NUMBER_OUT_OF_RANGE + s + " (1-" +
                       std::to_string(maxDiceSize) + ")";
            }
            s.clear();
            params.modifierOperator = *it;
            break;
        case 'e':
        case 'E':
            if (!flags[DICE_SIZE_FLAG] || flags[THRESHOLD_FLAG] || s.empty()) {
                return COMMAND_NOT_VALID + diceString;
            }
            flags[THRESHOLD_FLAG] = true;
            try {
                if(flags[MODIFIER_FLAG]) {
                    params.modifier = std::stoi(s);
                } else {
                    params.diceSize = std::stoi(s);
                }
            } catch (const std::out_of_range &) {
                return NUMBER_OUT_OF_RANGE + s + " (1-" +
                       std::to_string(maxDiceSize) + ")";
            }
            s.clear();
            break;
        default:
            return COMMAND_NOT_VALID + diceString;
            break;
        }
    }
    if (s.size() > 0) {
        try {
            if (flags[THRESHOLD_FLAG]) {
                params.thresholdValue = std::stoi(s);
                params.thresholdPresent = true;
            } else if (flags[MODIFIER_FLAG]) {
                params.modifier = std::stoi(s);
            } else {
                params.diceSize = std::stoi(s);
                if(params.diceSize < 1) {
                    return NUMBER_OUT_OF_RANGE + s + " (1-" +
                           std::to_string(maxDiceSize) + ")";
                }
            }
        }catch(const std::out_of_range &) {
            return NUMBER_OUT_OF_RANGE + s + " (1-" +
                   std::to_string(maxDiceSize) + ")";
        }
    }
    if (!flags[DICE_SIZE_FLAG] || (flags[DICE_SIZE_FLAG] && params.diceSize<1)) {
        return COMMAND_NOT_VALID + diceString;
    }

    std::cout<<'"'<<s<<"\""<<s.size()<<"\n";

    return "";
}