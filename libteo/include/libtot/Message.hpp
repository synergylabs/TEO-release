//
// Created by han on 2/17/21.
//

#ifndef LIBTOT_MESSAGE_HPP
#define LIBTOT_MESSAGE_HPP


#include "DataStore/SieveDataBlock_generated.h"

#include "Message/AcquirePreAuthTokenRequest_generated.h"
#include "Message/AcquirePreAuthTokenResponse_generated.h"
#include "Message/ClaimDeviceDiscovery_generated.h"
#include "Message/ClaimDeviceDiscoveryResponse_generated.h"
#include "Message/ClaimDeviceRequest_generated.h"
#include "Message/ClaimDeviceResponse_generated.h"
#include "Message/DataAccessFetch_generated.h"
#include "Message/DataAccessResponse_generated.h"
#include "Message/DataReencryptionPreRequest_generated.h"
#include "Message/DataReencryptionPreResponse_generated.h"
#include "Message/DataReencryptionRequest_generated.h"
#include "Message/DataReencryptionResponse_generated.h"
#include "Message/DataStoreDownloadRequest_generated.h"
#include "Message/DataStoreDownloadResponse1_generated.h"
#include "Message/DataStoreDownloadResponse2_generated.h"
#include "Message/DataStoreSieveCredRequest_generated.h"
#include "Message/DataStoreSieveCredResponse_generated.h"
#include "Message/DataStoreUpload_generated.h"
#include "Message/DataStoreUploadNotification_generated.h"
#include "Message/InitializationAdminReply_generated.h"
#include "Message/InitializationDeviceInfo_generated.h"
#include "Message/InitializationRequest_generated.h"
#include "Message/MessageType_generated.h"
#include "Message/UtilFetchIpRequest_generated.h"
#include "Message/UtilFetchIpResponse_generated.h"
#include "Message/UtilFetchStorePubkey_generated.h"
#include "Message/UtilHeartbeat_generated.h"
#include "Message/UtilRealTimeAccessRequest_generated.h"
#include "Message/UtilRealTimeAccessResponse_generated.h"
#include "Message/UtilRegisterIp_generated.h"
#include "Message/UtilReleaseDevice_generated.h"
#include "Message/UtilRemoveRealTimeAccess_generated.h"
#endif //LIBTOT_MESSAGE_HPP
