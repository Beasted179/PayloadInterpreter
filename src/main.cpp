#include "events.h"
#include "hooks.h"

#include "PayloadHandlerManager.h"
#include "PayloadInterpreter.h"

#include "CPR.h"

void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kDataLoaded:
		//logger::info("Data loaded.");
		REL::Relocation<uintptr_t> npcPtr{ RELOCATION_ID(261399,207890) }; //165e3b0
		REL::Relocation<uintptr_t> pcPtr{ RELOCATION_ID(261918,208044) }; //1663f78
		Hooks::install();

		auto payloadHandlerManager = PayloadHandlerManager::GetSingleton();

		payloadHandlerManager->RegisterPayloadHandler("PIE", PayloadInterpreter::GetSingleton());
		payloadHandlerManager->RegisterPayloadHandler("CPR", CPR::PayloadInterpreter::GetSingleton());

		// Load payload handlers from mods registered for messages from Payload Interpreter
		payloadHandlerManager->CollectExternPayloadHandlers();

		break;
	}
}


void onSKSEInit()
{
	PayloadInterpreter::GetSingleton()->LoadPreDefinedPayloads();
}

namespace
{
	void InitializeLog()
	{
		auto path = logger::log_directory();
		if (!path) {
			util::report_and_fail("Failed to find standard logging directory"sv);
		}

		*path /= std::format("{}.log", Plugin::NAME);
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

		const auto level = spdlog::level::info;

		auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
		log->set_level(level);
		log->flush_on(level);

		spdlog::set_default_logger(std::move(log));
		spdlog::set_pattern("%s(%#): [%^%l%$] %v"s);
	}
}

std::string wstring2string(const std::wstring& wstr, UINT CodePage)

{
	std::string ret;

	int len = WideCharToMultiByte(CodePage, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);

	ret.resize((size_t)len, 0);

	WideCharToMultiByte(CodePage, 0, wstr.c_str(), (int)wstr.size(), &ret[0], len, NULL, NULL);

	return ret;
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	InitializeLog();
	logger::info("{} v{} (built for Skyrim 1.7 by ChiRhoXP for Enderal Co-op)"sv, Plugin::NAME, Plugin::VERSION.string());

	SKSE::Init(a_skse);

	auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener("SKSE", MessageHandler)) {
		return false;
	}

	onSKSEInit();

	return true;
}
