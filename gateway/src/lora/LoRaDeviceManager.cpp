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
BEEEON_OBJECT_END(BeeeOn, LoRaDeviceManager)

using namespace BeeeOn;
using namespace Poco;
using namespace std;

void LoRaDeviceManager::setAbpAuthMethod(bool m)
{
	m_abpAuthMethod = m;
}

void LoRaDeviceManager::setMaxProbeAttempts(int count)
{
	m_controller.setMaxProbeAttempts(count);
}

void LoRaDeviceManager::run()
{
	UnsafePtr<Thread>(Thread::current())->setName("reporting");

	StopControl::Run run(m_stopControl);

	while (run) {
		try {
			const auto report = m_controller.pollReport(-1);
			if (!report)
				continue;

			if (logger().debug()) {
				logger().debug(
					"shipping report " + report.toString(),
					__FILE__, __LINE__);
			}

			const auto id = buildID(report.address);

			if (!deviceCache()->paired(id)) {
				if (logger().debug()) {
					logger().debug(
						"skipping report from unpaired device " + id.toString(),
						__FILE__, __LINE__);
				}

				continue;
			}

			shipReport(report);
		}
		BEEEON_CATCH_CHAIN(logger())
	}
}

void LoRaDeviceManager::stop()
{
	answerQueue().dispose();
	DeviceManager::stop();
	m_controller.dispose();
}

// TODO initDongle

// Set your DevAddr, NwkSKey, AppSKey
const char *devAddr = "00000000";
const char *nwkSKey = "00000000000000000000000000000000";
const char *appSKey = "00000000000000000000000000000000";

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