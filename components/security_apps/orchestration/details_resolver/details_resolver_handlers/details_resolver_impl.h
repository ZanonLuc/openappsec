// Copyright (C) 2022 Check Point Software Technologies Ltd. All rights reserved.

// Licensed under the Apache License, Version 2.0 (the "License");
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __DETAILS_RESOLVER_HANDLER_CC__
#error details_resolver_handlers/details_resolver_impl.h should not be included directly.
#endif // __DETAILS_RESOLVER_HANDLER_CC__

// Retrieve artifacts by incorporating nano service names into additional metadata:
// To include a required nano service in the additional metadata sent to the manifest generator,
// add a handler in this file. The key to use is 'requiredNanoServices', and its value should be
// a string representing an array of nano service prefix names, separated by semicolons.
// For example: "httpTransactionHandler_linux;iotSnmp_gaia;"
//
// Handler example for reading the content of a configuration file:
// FILE_CONTENT_HANDLER("requiredNanoServices", "/tmp/nano_services_list", getRequiredNanoServices)

// use SHELL_CMD_HANDLER(key as string, shell command as string, ptr to Maybe<string> handler(const string&))
// to return a string value for an attribute key based on a logic executed in a handler that receives
// shell command execution output as its input
#ifdef SHELL_CMD_HANDLER
#if defined(gaia) || defined(smb)
SHELL_CMD_HANDLER("cpProductIntegrationMgmtObjectType", "cpprod_util CPPROD_IsMgmtMachine", getMgmtObjType)
SHELL_CMD_HANDLER("hasSDWan", "[ -f $FWDIR/bin/sdwan_steering ] && echo '1' || echo '0'", checkHasSDWan)
SHELL_CMD_HANDLER("canUpdateSDWanData", "cpsdwan get_data | jq -r .can_update_sdwan_data", checkCanUpdateSDWanData)
SHELL_CMD_HANDLER(
    "isSdwanRunning",
    "[ -v $(pidof cp-nano-sdwan) ] && echo 'false' || echo 'true'",
    checkIfSdwanRunning)
#endif //gaia || smb

#if defined(gaia)
SHELL_CMD_HANDLER("hasSupportedBlade", "enabled_blades", checkHasSupportedBlade)
SHELL_CMD_HANDLER("hasSamlPortal", "mpclient status saml-vpn", checkSamlPortal)
#endif //gaia

#if defined(smb)
SHELL_CMD_HANDLER(
    "cpProductIntegrationMgmtParentObjectName",
    "cpsdwan get_data | jq -r .cluster_name",
    getMgmtParentObjName
)
SHELL_CMD_HANDLER(
    "cpProductIntegrationMgmtParentObjectUid",
    "cpsdwan get_data | jq -r .cluster_uuid",
    getMgmtParentObjUid
)
SHELL_CMD_HANDLER(
    "cpProductIntegrationMgmtObjectName",
    "cpprod_util FwIsLocalMgmt",
    getSmbObjectName
)
#endif//smb

SHELL_CMD_OUTPUT("kernel_version", "uname -r")
SHELL_CMD_OUTPUT("helloWorld", "cat /tmp/agentHelloWorld 2>/dev/null")
#endif // SHELL_CMD_OUTPUT


// use FILE_CONTENT_HANDLER(key as string, path to file as string, ptr to Maybe<string> handler(ifstream&))
// to return a string value for an attribute key based on a logic executed in a handler that receives file as input
#ifdef FILE_CONTENT_HANDLER

#if defined(gaia)

FILE_CONTENT_HANDLER("hasIdpConfigured", "/opt/CPSamlPortal/phpincs/spPortal/idpPolicy.xml", checkIDP)

FILE_CONTENT_HANDLER(
    "cpProductIntegrationMgmtParentObjectUid",
    (getenv("FWDIR") ? string(getenv("FWDIR")) : "") + "/database/myself_objects.C",
    getMgmtParentObjUid
)
FILE_CONTENT_HANDLER(
    "cpProductIntegrationMgmtParentObjectName",
    (getenv("FWDIR") ? string(getenv("FWDIR")) : "") + "/database/myself_objects.C",
    getMgmtParentObjName
)
FILE_CONTENT_HANDLER(
    "cpProductIntegrationMgmtObjectName",
    (getenv("FWDIR") ? string(getenv("FWDIR")) : "") + "/database/myown.C",
    getMgmtObjName
)
#endif //gaia

#if defined(alpine)
FILE_CONTENT_HANDLER("alpine_tag", "/usr/share/build/cp-alpine-tag", getCPAlpineTag)
#endif // alpine
#if defined(gaia) || defined(smb)
FILE_CONTENT_HANDLER("os_release", "/etc/cp-release", getOsRelease)
FILE_CONTENT_HANDLER(
    "cpProductIntegrationMgmtObjectUid",
    (getenv("FWDIR") ? string(getenv("FWDIR")) : "") + "/database/myown.C",
    getMgmtObjUid
)
FILE_CONTENT_HANDLER(
    "IP Address",
    (getenv("FWDIR") ? string(getenv("FWDIR")) : "") + "/database/myself_objects.C",
    getGWIPAddress
)
FILE_CONTENT_HANDLER(
    "Hardware",
    (getenv("FWDIR") ? string(getenv("FWDIR")) : "") + "/database/myself_objects.C",
    getGWHardware
)
FILE_CONTENT_HANDLER(
    "Application Control",
    (getenv("FWDIR") ? string(getenv("FWDIR")) : "") + "/database/myself_objects.C",
    getGWApplicationControlBlade
)
FILE_CONTENT_HANDLER(
    "URL Filtering",
    (getenv("FWDIR") ? string(getenv("FWDIR")) : "") + "/database/myself_objects.C",
    getGWURLFilteringBlade
)
FILE_CONTENT_HANDLER(
    "IPSec VPN",
    (getenv("FWDIR") ? string(getenv("FWDIR")) : "") + "/database/myself_objects.C",
    getGWIPSecVPNBlade
)
FILE_CONTENT_HANDLER(
    "Version",
    (getenv("FWDIR") ? string(getenv("FWDIR")) : "") + "/database/myself_objects.C",
    getGWVersion
)

#else // !(gaia || smb)
FILE_CONTENT_HANDLER("os_release", "/etc/os-release", getOsRelease)
#endif // gaia || smb

#endif // FILE_CONTENT_HANDLER
