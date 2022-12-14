#pragma once

#include <bln_log/log.hpp>

#define rtx_log(args...) bln_log_rel(args)
#define rtx_dbg(args...) bln_log_dbg(args)
