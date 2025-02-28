#pragma once

using PacketHandlerFunc = function<bool(Session*, BYTE*, int)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(Session* session, BYTE* buffer, int len);

class ServerPacketHandler
{
public:
	static void Init()
	{
		for (int i = 0; i < UINT16_MAX;i++)
			GPacketHandler[i] = Handle_INVALID;
	}

	static bool HandlePacket(Session* session, BYTE* buffer, int len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
};

