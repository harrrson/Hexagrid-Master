//
// Created by piotr on 22.12.2021.
//

#include "ExtensionsBase.h"

ExtensionsBase::ExtensionsBase(DppBot *bot, std::string name) : mName(name)
{
    mName=name;
    bot->respond(name,handler);
}
