#pragma once

#ifndef SPAN_HPP
#define SPAN_HPP

#include "LineColumn.hpp"

struct Span
{
    LineColumn start;
    LineColumn end;
};

#endif