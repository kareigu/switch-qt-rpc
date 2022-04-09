#ifndef DISCORDRPC_H
#define DISCORDRPC_H

#include <discord.h>

namespace DC_RPC {
  void runDiscordCallbacks(discord::Core* core, bool* interrupted);
}


#endif // DISCORDRPC_H
