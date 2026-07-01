#pragma once

namespace RE
{
	struct MenuModeChangeEvent
	{
		BSFixedString menuName;
		bool enteringMenuMode;
	};
	static_assert(sizeof(MenuModeChangeEvent) == 0x10);

	struct InitLoadEvent
	{
		enum Stage
		{
			Unk1 = 1,
			Unk2,
			Unk3,
			Unk4,
			Unk5
		};
		uint32_t stage;
		uint32_t stageMax;
		uint64_t unk;

		static void RegisterSink(BSTEventSink<InitLoadEvent>* a_sink) {
			using func_t = decltype(&RegisterSink);
		// Address Library ID dla InitLoadEvent::RegisterSink.
		// Zweryfikowane na bazie offsetów:
		// - 1.16.242: VA 0x142B6B7A0 => ID 145931
		// - 1.16.244: ID 145931 => VA 0x142B671C0
		REL::Relocation<func_t> func(REL::ID(145931));
			return func(a_sink);
		}
	};
}
