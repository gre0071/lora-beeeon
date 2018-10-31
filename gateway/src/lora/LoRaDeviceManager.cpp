#include <vector>

#include <Poco/Clock.h>
#include <Poco/Message.h>
#include <Poco/NumberFormatter.h>
#include <Poco/NumberParser.h>
#include <Poco/RegularExpression.h>
#include <Poco/StringTokenizer.h>
#include <Poco/Thread.h>

#include "lora/LoRaDevice.h"
#include "lora/LoRaLogger.c"
#include "lora/LoRaPacketTools.h"

BEEEON_OBJECT_BEGIN(BeeeOn, LoRaDeviceManager)
BEEEON_OBJECT_CASTABLE(CommandHandler)
BEEEON_OBJECT_CASTABLE(StoppableRunnable)
BEEEON_OBJECT_CASTABLE(HotplugListener)
BEEEON_OBJECT_CASTABLE(DeviceStatusHandler)
BEEEON_OBJECT_PROPERTY("deviceCache", &LoRaDeviceManager::setDeviceCache)
BEEEON_OBJECT_END(BeeeOn, LoRaDeviceManager)

using namespace BeeeOn;
using namespace Poco;
using namespace std;











void LoRaDeviceManager::onAdd(const LoRaPacketTools &p)
{
        dl_insert_device(e.DevAddr);

	m_controller.probe(dev);
	initDongle();
	syncSlots();
}

void LoRaDeviceManager::onRemove(const LoRaDevice &e)
{
	const auto dev = hotplugMatch(e);
	if (dev.empty())
		return;

	m_controller.release(dev);
}


void JablotronDeviceManager::newDevice(
		const DeviceID &id,
		const string &name,
		const list<ModuleType> &types,
		const Timespan &refreshTime)
{
	NewDeviceCommand::Ptr cmd = new NewDeviceCommand(
		id, "Jablotron", name, types, refreshTime);
	dispatch(cmd);
}