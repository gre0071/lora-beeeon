#include <Poco/Exception.h>

#include "bluetooth/BLESmartDevice.h"
#include "model/DevicePrefix.h"

using namespace BeeeOn;
using namespace Poco;

LoRaDevice::LoRaDevice(const MACAddress& address, const Timespan& timeout):
	m_deviceId(DevicePrefix::PREFIX_BLE_SMART, address),
	m_address(address),
	m_timeout(timeout)
{
}

LoRaDevice::~LoRaDevice()
{
}

DeviceID LoRaDevice::deviceID() const
{
	return m_deviceId;
}

void LoRaDevice::pair(
		HciInterface::Ptr,
		Poco::SharedPtr<HciInterface::WatchCallback>)
{
}

void LoRaDevice::requestModifyState(
		const ModuleID&,
		const double,
		const HciInterface::Ptr)
{
	throw NotImplementedException(__func__ );
}

SensorData LoRaDevice::requestState(const HciInterface::Ptr)
{
	throw NotImplementedException(__func__ );
}

SensorData LoRaDevice::parseAdvertisingData(
		const std::vector<unsigned char>&) const
{
	throw NotImplementedException(__func__ );
}
