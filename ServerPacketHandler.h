#pragma once

using PacketHandlerFunc = function<bool(Session*, BYTE*, int)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

class ServerPacketHandler
{
};

