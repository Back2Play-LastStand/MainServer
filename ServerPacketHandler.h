#pragma once

using PacketHandlerFunc = function<bool(Session*, BYTE*, int)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

class ServerPacketHandler
{
	static bool HandlePacket(Session* session, BYTE* buffer, int len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
};

