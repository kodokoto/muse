#pragma once

#ifndef TYPES_HPP
#define TYPES_HPP

#include <map>


enum class TypeDef
{
    Tfloat64,
    Tint64,
    Tstring,
    Tboolean,
    Tvoid,
    Tuser,
};

std::map<std::string, TypeDef> type_env;


#endif