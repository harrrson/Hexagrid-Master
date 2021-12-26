//
// Created by piotr on 22.12.2021.
//

#ifndef HEXAGRID_MASTER_EXTENSIONSBASE_H
#define HEXAGRID_MASTER_EXTENSIONSBASE_H

#include "include.hh"

class ExtensionsBase {
  public:
    ExtensionsBase(DppBot* bot, std::string name);
    virtual ~ExtensionsBase();

  private:
    virtual void handler(nlohmann::json& json)=0;
    std::string mName{};
    std::weak_ptr<DppBot> mBot;
};

#endif // HEXAGRID_MASTER_EXTENSIONSBASE_H
