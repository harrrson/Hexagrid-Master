#include "include.hh"

// namespace asio = boost::asio;
using json = nlohmann::json;
namespace dpp = discordpp;
#include "extensions.h"

std::string getToken();

std::istream &safeGetline(std::istream &is, std::string &t);

void filter(std::string &target, const std::string &pattern);

int main() {
    dpp::log::filter = dpp::log::info;
    dpp::log::out = &std::cerr;

    std::cout << "Starting bot...\n\n";

    std::string token = getToken();
    if (token.empty()) {
        std::cerr << "CRITICAL: "
                  << "There is no valid way for Echo to obtain a token! Use "
                     "one of the following ways:"
                  << std::endl
                  << "(1) Fill the BOT_TOKEN environment variable with the "
                     "token (e.g. 'Bot 123456abcdef')."
                  << std::endl
                  << "(2) Copy the example `token.eg.dat` as `token.dat` and "
                     "write your own token to it.\n";
        exit(1);
    }

    // Create Bot object
    auto bot = std::make_shared<DppBot>();

    // Set prefix
    bot->prefix = "d!dc ";

    // Don't complain about unhandled events
    bot->debugUnhandled = false;

    // Declare the intent to receive guild messages
    bot->intents = dpp::intents::GUILD_MESSAGES;

    // Map of all commands, used in help command
    std::shared_ptr<commandMap> cmdMap = std::make_shared<commandMap>();

    bot->handlers.insert({"READY",
                          [cmdMap](json data) {
                              for (auto &cmd : *cmdMap) {
                                  cmd.second->createSlashCommands();
                              }
                          }});

    {
#define HELP_COMMAND_NAME "help"
        std::unique_ptr<HelpCommand> helpCommand(
            new HelpCommand(HELP_COMMAND_NAME, bot, cmdMap));
        cmdMap->insert(commandPair(HELP_COMMAND_NAME, std::move(helpCommand)));

#define ROLL_COMMAND_NAME "roll"
        std::unique_ptr<DiceRoller> rollCommand(
            new DiceRoller(ROLL_COMMAND_NAME, bot));
        cmdMap->insert(commandPair(ROLL_COMMAND_NAME, std::move(rollCommand)));
    }

    // Create Asio context, this handles async stuff.
    auto aioc = std::make_shared<asio::io_context>();

    // Set the bot up
    bot->initBot(9, token, aioc);

    // Run the bot!
    bot->run();

    return 0;
}

std::string getToken() {
    std::string token;

    /*
                    First attempt to read the token from the BOT_TOKEN
       environment variable.
    */
    char const *env = std::getenv("BOT_TOKEN");
    if (env != nullptr) {
        token = std::string(env);
    } else {
        /*/
         * Read token from token file.
         * Tokens are required to communicate with Discord, and hardcoding
        tokens is a bad idea.
         * If your bot is open source, make sure it's ignore by git in your
        .gitignore file.
        /*/
        std::ifstream tokenFile("token.dat");
        if (!tokenFile) {
            return "";
        }
        safeGetline(tokenFile, token);
        tokenFile.close();
    }
    return token;
}

/*/
 * Source: https://stackoverflow.com/a/6089413/1526048
/*/
std::istream &safeGetline(std::istream &is, std::string &t) {
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf *sb = is.rdbuf();

    for (;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if (sb->sgetc() == '\n') {
                sb->sbumpc();
            }
            return is;
        case std::streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if (t.empty()) {
                is.setstate(std::ios::eofbit);
            }
            return is;
        default:
            t += (char)c;
        }
    }
}
