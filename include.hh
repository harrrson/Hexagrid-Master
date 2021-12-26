//
// Created by Aidan on 10/2/2020.
//

#pragma once

#include <discordpp/macros.hh>

#define MY_BOT_DEF BOT_DEF(ALL_DISCORDPP_PLUGINS)

#ifndef DPPBOT_EXTERN

#include <fstream>
#include <iostream>
#include <regex>
#include <optional>

#include <asio.hpp>

// Put more non-plugin `include`s here

#endif

#include <discordpp/bot.hh>
#include <discordpp/rest-simpleweb.hh>
#include <discordpp/websocket-simpleweb.hh>
#include <discordpp/plugin-interactionhandler.hh>
#include <discordpp/plugin-objects.hh>
#include <discordpp/plugin-endpoints.hh>
#include <discordpp/plugin-overload.hh>
#include <discordpp/plugin-ratelimit.hh>
#include <discordpp/plugin-responder.hh>
#include <discordpp/plugin-commandhandler.hh>

// Put more D++ plugin `include`s here

#ifndef DPPBOT_EXTERN

extern template class MY_BOT_DEF;
using DppBot = MY_BOT_DEF;

#undef DPPBOT_EXTERN

#endif
